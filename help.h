#ifndef HELP_H_INCLUDED
#define HELP_H_INCLUDED

#define KB_SIZE 1
#define MB_SIZE 2
#define GB_SIZE 3
#define TB_SIZE 4

typedef unsigned char byte;

#define VERSION "pdd 1.0.0 (x86_64)"
#define INVERT "\e[7m"
#define BOLD "\e[1m"
#define DEFAULT "\e[0m"
#define COMMANDS "Available commands:\n 1.quit/exit\n 2.help\n"
#define STARTMESSAGE "Welcome to P.D.D. (Professional Disk Diagnostic), type 'help' for further instructions, or 'quit' to exit the application.\n "
#define LOGO " ██████╗ ██████╗ ██████╗ \n ██╔══██╗██╔══██╗██╔══██╗ \n ██████╔╝██║  ██║██║  ██║\n ██╔═══╝ ██║  ██║██║  ██║\n ██║     ██████╔╝██████╔╝\n ╚═╝     ╚═════╝ ╚═════╝ \n "
void get_program_path(char** p);
void parse_cli(char* input);
void parse_args(int argc, char* argv[]);
void print_help();
void printMessage();
void disk_info_print();
typedef struct{
    char * colorWhite;
    char * colorBlack;
    char * colorBlue;
    char * colorRed;
    char * colorYellow;
}Colors;

#endif