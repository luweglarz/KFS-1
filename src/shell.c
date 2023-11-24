#include "shell.h"
#include "vga.h"
#include "klib.h"
#include "builtin.h"
#include "kscreen.h"

kprompt     prompt;
kbuiltin    builtin[BUILTINS_SIZE] = {
    {"reboot\0", &reboot}, {"halt\0", &halt}
};

void catch_entry(char c){
    prompt.buffer[prompt.size] = c;
    kputchar(prompt.buffer[prompt.size] , LIGHT_GRAY_COLOR, 1);
    prompt.size++;
}

void remove_entry(){
    prompt.buffer[prompt.size] = '\0';
    prompt.size--;
    *vga_area_head = 0;
    move_cursor(ARROW_LEFT);
}

void exec_cmd(){
    int i = 0;

    while (i < BUILTINS_SIZE){
        if (kstrncmp(prompt.buffer, builtin[i].name, kstrlen(builtin[i].name)) == 0 
            && kstrlen(builtin[i].name) == kstrlen(prompt.buffer))
            builtin[i].builtin_func();
        i++;
    }
    vga_area_head = (uint16_t*)VGA_JMP_LINE;
    init_shell();
}

void init_shell(){
    kmemset(prompt.buffer, '\0', sizeof(prompt.buffer));
    prompt.size = 0;
    kprintf("kshell>", GREEN_COLOR, 0);
}