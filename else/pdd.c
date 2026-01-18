#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <mntent.h>

// Структура для хранения информации о диске
typedef struct {
    char device[256];
    char mount_point[256];
    char filesystem[64];
    unsigned long long total_size;
    unsigned long long free_size;
    unsigned long long available_size;
    unsigned long long used_size;
} DiskInfo;

// Функция для проверки, является ли устройство блочным
/*int is_block_device(const char *device) {
    struct stat st;
    if (stat(device, &st) == 0) {
        return S_ISBLK(st.st_mode);
    }
    return 0;
}*/

// Функция для получения размера блочного устройства
unsigned long long get_block_device_size(const char *device) {
    FILE *fp;
    char path[512];
    unsigned long long size = 0;
    
    // Попробовать прочитать размер из sysfs
    snprintf(path, sizeof(path), "/sys/block/%s/size", device);
    fp = fopen(path, "r");
    if (fp) {
        if (fscanf(fp, "%llu", &size) == 1) {
            fclose(fp);
            return size * 512; // Размер в секторах по 512 байт
        }
        fclose(fp);
    }
    
    return 0;
}

// Функция для получения списка всех блочных устройств
void get_block_devices(DiskInfo **disks, int *count) {
    DIR *dir;
    struct dirent *entry;
    char path[512];
    
    dir = opendir("/sys/block");
    if (!dir) {
        perror("Не удалось открыть /sys/block");
        return;
    }
    
    *count = 0;
    *disks = NULL;
    
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаем . и .. и виртуальные устройства
        if (strcmp(entry->d_name, ".") == 0 || 
            strcmp(entry->d_name, "..") == 0 ||
            strncmp(entry->d_name, "loop", 4) == 0 ||
            strncmp(entry->d_name, "ram", 3) == 0) {
            continue;
        }
        
        // Пропускаем разделы (они содержат / в имени)
        if (strchr(entry->d_name, '/') != NULL) {
            continue;
        }
        
        // Увеличиваем массив
        *disks = realloc(*disks, (*count + 1) * sizeof(DiskInfo));
        if (!*disks) {
            perror("Ошибка выделения памяти");
            break;
        }
        
        DiskInfo *disk = &(*disks)[*count];
        memset(disk, 0, sizeof(DiskInfo));
        
        // Заполняем информацию об устройстве
        snprintf(disk->device, sizeof(disk->device), "/dev/%s", entry->d_name);
        strcpy(disk->mount_point, "Не смонтирован");
        strcpy(disk->filesystem, "raw");
        
        // Получаем размер устройства
        
        disk->total_size = get_block_device_size(entry->d_name);
        disk->free_size = 0;
        disk->available_size = 0;
        disk->used_size = disk->total_size;
        
        (*count)++;
    }
    
    closedir(dir);
}

// Функция для получения информации о смонтированных файловых системах
void get_mounted_filesystems(DiskInfo **disks, int *count) {
    FILE *fp;
    struct mntent *mnt;
    
    fp = setmntent("/proc/mounts", "r");
    if (!fp) {
        perror("Не удалось открыть /proc/mounts");
        return;
    }
    
    while ((mnt = getmntent(fp)) != NULL) {
        // Пропускаем виртуальные файловые системы
        if (strncmp(mnt->mnt_fsname, "/dev/", 5) != 0) {
            continue;
        }
        
        // Пропускаем CD/DVD и другие специальные устройства
        if (strstr(mnt->mnt_fsname, "/dev/sr") != NULL ||
            strstr(mnt->mnt_fsname, "/dev/fd") != NULL) {
            continue;
        }
        
        // Получаем статистику файловой системы
        struct statvfs fs_info;
        if (statvfs(mnt->mnt_dir, &fs_info) == 0) {
            // Увеличиваем массив
            *disks = realloc(*disks, (*count + 1) * sizeof(DiskInfo));
            if (!*disks) {
                perror("Ошибка выделения памяти");
                break;
            }
            
            DiskInfo *disk = &(*disks)[*count];
            memset(disk, 0, sizeof(DiskInfo));
            
            // Заполняем информацию
            strncpy(disk->device, mnt->mnt_fsname, sizeof(disk->device) - 1);
            strncpy(disk->mount_point, mnt->mnt_dir, sizeof(disk->mount_point) - 1);
            strncpy(disk->filesystem, mnt->mnt_type, sizeof(disk->filesystem) - 1);
            
            unsigned long long block_size = fs_info.f_frsize;
            disk->total_size = fs_info.f_blocks * block_size;
            disk->free_size = fs_info.f_bfree * block_size;
            disk->available_size = fs_info.f_bavail * block_size;
            disk->used_size = disk->total_size - disk->free_size;
            
            (*count)++;
        }
    }
    
    endmntent(fp);
}

// Функция для форматированного вывода размера
void print_size(unsigned long long bytes) {
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit_index = 0;
    double size = bytes;

    while (size >= 1024.0 && unit_index < 4) {
        size /= 1024.0;
        unit_index++;
    }

    printf("%8.2f %-2s", size, units[unit_index]);
}

// Функция для вывода информации о дисках
void print_disk_info(DiskInfo *disks, int count, int show_all) {
    printf("\n%-15s %-12s %-20s %12s %12s %12s %6s\n", 
           "Устройство", "Тип ФС", "Точка монтирования", 
           "Общий", "Использовано", "Свободно", "Исп.%");
    printf("--------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        DiskInfo *disk = &disks[i];
        
        // Пропускаем несмонтированные, если не показан флаг --all
        if (!show_all && strcmp(disk->mount_point, "Не смонтирован") == 0) {
            continue;
        }
        
        printf("%-15s %-12s %-20s ", 
               disk->device, 
               disk->filesystem, 
               disk->mount_point);
        
        if (disk->total_size > 0) {
            print_size(disk->total_size);
            printf(" ");
            print_size(disk->used_size);
            printf(" ");
            print_size(disk->free_size);
            
            // Процент использования
            if (disk->total_size > 0) {
                double used_percent = (double)disk->used_size / disk->total_size * 100;
                printf(" %5.1f%%", used_percent);
            } else {
                printf("   N/A");
            }
        } else {
            printf("%12s %12s %12s %6s", "N/A", "N/A", "N/A", "N/A");
        }
        printf("\n");
    }
}

// Функция для вывода детальной информации о конкретном диске
void print_disk_detail(DiskInfo *disk) {
    printf("\n=== Детальная информация о диске ===\n");
    printf("Устройство:        %s\n", disk->device);
    printf("Файловая система:  %s\n", disk->filesystem);
    printf("Точка монтирования: %s\n", disk->mount_point);
    printf("----------------------------------------\n");
    
    if (disk->total_size > 0) {
        printf("Общий размер:      ");
        print_size(disk->total_size);
        printf("\n");
        
        printf("Использовано:      ");
        print_size(disk->used_size);
        printf("\n");
        
        printf("Свободно:          ");
        print_size(disk->free_size);
        printf("\n");
        
        printf("Доступно:          ");
        print_size(disk->available_size);
        printf("\n");
        
        double used_percent = (double)disk->used_size / disk->total_size * 100;
        double free_percent = (double)disk->free_size / disk->total_size * 100;
        
        printf("Использовано:      %.1f%%\n", used_percent);
        printf("Свободно:          %.1f%%\n", free_percent);
    } else {
        printf("Размер: N/A\n");
    }
    printf("========================================\n");
}

int main(int argc, char *argv[]) {
    DiskInfo *disks = NULL;
    int disk_count = 0;
    int show_all = 0;
    char *specific_device = NULL;
    
    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--all") == 0 || strcmp(argv[i], "-a") == 0) {
            show_all = 1;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Использование: %s [опции] [устройство]\n", argv[0]);
            printf("Опции:\n");
            printf("  -a, --all      Показать все устройства (включая несмонтированные)\n");
            printf("  -h, --help     Показать эту справку\n");
            printf("\nПримеры:\n");
            printf("  %s              # Показать смонтированные диски\n", argv[0]);
            printf("  %s -a           # Показать все диски\n", argv[0]);
            printf("  %s /dev/sda     # Информация о конкретном устройстве\n", argv[0]);
            return 0;
        } else {
            specific_device = argv[i];
        }
    }
    
    // Получаем информацию о дисках
    get_mounted_filesystems(&disks, &disk_count);
    
    if (show_all) {
        get_block_devices(&disks, &disk_count);
    }
    
    if (specific_device) {
        // Поиск конкретного устройства
        int found = 0;
        for (int i = 0; i < disk_count; i++) {
            if (strcmp(disks[i].device, specific_device) == 0) {
                print_disk_detail(&disks[i]);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Устройство '%s' не найдено\n", specific_device);
        }
    } else {
        // Вывод списка всех дисков
        print_disk_info(disks, disk_count, show_all);
    }
    
    // Освобождаем память
    free(disks);
    
    return 0;
}