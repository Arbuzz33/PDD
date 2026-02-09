#ifndef HELP_H_INCLUDED
#define HELP_H_INCLUDED


#define KB_SIZE 1
#define MB_SIZE 2
#define GB_SIZE 3
#define TB_SIZE 4

#define VERSION "pdd 1.0.0 (x86_64)"
#define INVERT "\e[7m"
#define BOLD "\e[1m"
#define DEFAULT "\e[0m"
#define STARTMESSAGE "Welcome to P.D.D. (Professional Disk Diagnostic), type 'help' for further instructions, or 'quit' to exit the application.\n "
#define LOGO " ██████╗ ██████╗ ██████╗ \n ██╔══██╗██╔══██╗██╔══██╗ \n ██████╔╝██║  ██║██║  ██║\n ██╔═══╝ ██║  ██║██║  ██║\n ██║     ██████╔╝██████╔╝\n ╚═╝     ╚═════╝ ╚═════╝ \n "
typedef unsigned char byte;

typedef struct{
    char * colorWhite;
    char * colorBlack;
    char * colorBlue;
    char * colorRed;
    char * colorYellow;
} Colors;

void get_program_path(char** p);                       //gets program path in variable
void parse_args(int argc, char* argv[]);               //parses program args
void print_help();                                     //prints the help page

int alphabet_compare(const void *a, const void *b);    //sorting in alphabet order
int realphabet_compare(const void *a, const void *b);  //sorting in reversed alphabet order
int less_compare(const void *a, const void *b);        //from lesser to greater
int greater_compare(const void *a, const void *b);     //from greater to lesser


#endif