//   ██████╗██████╗  █████╗ ████████╗ ██████╗
//  ██╔════╝██╔══██╗██╔══██╗╚══██╔══╝██╔════╝
//  ██║     ██████╔╝███████║   ██║   ██║     
//  ██║     ██╔══██╗██╔══██║   ██║   ██║     
//  ╚██████╗██║  ██║██║  ██║   ██║   ╚██████╗
//   ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝    ╚═════╝
#include <ncurses.h> // Import ncurses: It's main_library of this tools, Don't remove it lol.
#include <unistd.h>  // Import unistd:  It's use for checking_uid_user
#include <locale.h>  // Import locale:  It's use for enable unicode_char support.
#include <stdlib.h>  // Import stdlib:  It's use for run system_command and serveral macros like NULL.
#include <string.h>  // Import string:  It's use for strcmp/Comparing string.
#include <stdbool.h> // Import stdbool: it's use for boolean type and values
#include <stdio.h>   // Import stdio:   It's use for printing text, idk just include it.

#define ARRAY_LIMIT 8 // Define a macro

bool check_(const char package[]); // Declare function
void compile_(const char *package); // Declare function
void install_(const char package[], bool reverse_mode); // Declare function
void create_box_inside_(WINDOW *target_window, const char box_title[], const int start_title_y, const int start_title_x); // Declare function

struct object_size { // Define a structure
    unsigned int minimum_size_of_screen_x, minimum_size_of_screen_y; // Declare structure_member
    unsigned int maximum_size_of_screen_x, maximum_size_of_screen_y; // Declare structure_member 
};

int main() // Main function, this program begins executing at the beginning of 'main'
{
    setlocale(LC_ALL, "en_US.UTF-8"); // Set locale of this tools
    initscr();         // Initialize the window
    cbreak();          // If it enable it will read ctrl+c as an exit
    raw();             // Change keypresses to raw
    noecho();          // Don't display any keypresses 
    curs_set(FALSE);   // Display cursor (TRUE/FALSE) 
    if(has_colors())   // Check if terminal has color capabilities
    {  
        start_color(); // Initialize color_support
    }
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Define COLOR_PAIR(1)

    FILE *read_command; // Declare type of variable
    char output_command[45], output_command_1[40], *command; // Declare type of variable
    bool internet_connection_status, reverse_mode_status = false, root_status; // Declare type of variable
    int width_main_window, height_main_window, start_x_main_window, start_y_main_window; // Declare type of variable
    int width_sub_window, height_sub_window, start_x_sub_window, start_y_sub_window; // Declare type of variable
    
    struct object_size terminal_size, main_window_size; // Declare terminal_size,main_window_size  of type object_size
    getmaxyx(stdscr, terminal_size.maximum_size_of_screen_y, terminal_size.maximum_size_of_screen_x); // Get information about maximum_size of terminal screen
    getbegyx(stdscr, terminal_size.minimum_size_of_screen_y, terminal_size.minimum_size_of_screen_x); // Get information about minimum_size of terminal screen

    height_main_window   = terminal_size.maximum_size_of_screen_y-4;  // Set height of main_window
    width_main_window    = terminal_size.maximum_size_of_screen_x-4;  // Set width of main_window
    start_y_main_window  = terminal_size.minimum_size_of_screen_y+2;  // Set start_point(heigth) of main_window
    start_x_main_window  = terminal_size.minimum_size_of_screen_x+2;  // Set start_point(width) of main_window
    WINDOW * main_window = newwin(height_main_window, width_main_window, start_y_main_window, start_x_main_window);  // Create main_window(WINDOW)
    getmaxyx(main_window, main_window_size.maximum_size_of_screen_y, main_window_size.maximum_size_of_screen_x); // Get information about maximum_size of main_window(WINDOW) screen
    getbegyx(main_window, main_window_size.minimum_size_of_screen_y, main_window_size.minimum_size_of_screen_x); // Get information about minimum_size of main_window(WINDOW) screen
    keypad(main_window, true); // Enables the readings of function keys like F1,F2,ARROW_KEYS etc.
    
    height_sub_window   = main_window_size.maximum_size_of_screen_y-8; // Set height of sub_window
    width_sub_window    = main_window_size.maximum_size_of_screen_x-8; // Set width of sub_window
    start_y_sub_window  = main_window_size.minimum_size_of_screen_y+4; // Set start_point(heigth) of sub_window
    start_x_sub_window  = main_window_size.minimum_size_of_screen_x+4; // Set start_point(width) of sub_window
    WINDOW * sub_window = subwin(main_window ,height_sub_window, width_sub_window, start_y_sub_window, start_x_sub_window); // Create sub_window(WINDOW)
    keypad(sub_window, true); // Enables the readings of function keys like F1,F2,ARROW_KEYS etc.
    
    const char *main_menu_options[] = { // Declare type of list
        "Compile/Recompile dwm,st and dmenu",
        "Tools Installer",
        "Help",
        "Exit"
    }, *main_menu_indicator[] = {
        "\ufb3a ",
        "\ue20f ",
        "\uf7d6 ",
        "\uf705 ",
    }, *tools_installer_menu_options[] = { // Declare type of list
        "Hacking Tools",
        "Other Tools"
    }, *tools_installer_menu_hacking_tools_options[ARRAY_LIMIT] = { // Declare type of list
        "aircrack-ng",
        "arpfox",
        "burpsuite",
        "metasploit",
        "sqlmap",
        "nmap",
        "wireshark",
        "wpscan"
    }, *tools_installer_menu_hacking_tools_indicator[ARRAY_LIMIT] = { // Declare type of list
        "\uf023 ",
        "\uf007 ",
        "\uf007 ",
        "\uf023 ",
        "\uf023 ",
        "\uf023 ",
        "\uf023 ",
        "\uf023 "
    }, *box_title[] = { // Declare type of list
        "Craccken Tools Menu",
        "Must run as root!!",
        "Must have internet_connection!!",
        "Tools Installer",
        "Compile",
        "Help"
    };

    unsigned short int hacking_tools_installed[ARRAY_LIMIT]; // Declare type of list
    int main_menu_input, tools_installer_menu_input, tools_installer_menu_hacking_tools_input; // Declare type of variable
    int main_menu_highlight = 0, tools_installer_menu_highlight = 0, tools_installer_menu_hacking_tools_highlight = 0; // Declare type of variable & Set highlight to 0

    //TODO: Create feature that could install yay and run system command from user input
    char ab[80];
    snprintf(ab, sizeof ab,"%d", 8);
    printf("%s", ab);

    read_command = popen(command="ping -c1 8.8.8.8 2> /dev/null", "r"); // Run & read command from shell.
    fscanf(read_command, "%s", output_command_1); // Input the value from read_command to output_command
    if(strcmp(output_command_1,"") == 1){ // If output_command doesn't return any output then:
        internet_connection_status = false;   // Set variable internet_connection_status to 0 (FALSE)
    } else {
        internet_connection_status = true;   // Set variable internet_connection_status to 1 (TRUE)
    }
    if(!getuid()){ // Check uid user if 0 then it's root user
        root_status = true; // Set variable root_status to 1 (TRUE)
    } else {
        root_status = false; // Set variable root_status to 0 (FALSE)
    }
    while(1) // Create an infinite-loop
    {
        create_box_inside_(main_window, box_title[0], 1, width_main_window/2-10); // Call function that create box inside main_window
        if (root_status == true){ // Check if run as root or not
            wattron(main_window, A_BLINK);  // Set string_attributes(main_window) on
            mvwprintw(main_window, height_main_window-2, 1, " \uf8c2 "); // Print string
            wattroff(main_window, A_BLINK); // Set string_attributes(main_window) off
        } else {
            wattron(main_window, A_BLINK);  // Set string_attributes(main_window) on
            mvwprintw(main_window, height_main_window-2, 1, " \uf8c3 "); // Print string
            wattroff(main_window, A_BLINK); // Set string_attributes(main_window) off
        }
        if(reverse_mode_status == true) // Check if reverse_mode is on
        {
            mvwprintw(main_window, height_main_window-2, 4, "\uf04a "); // Print string
        }
        if(internet_connection_status == false){ // If output_command doesn't return any output then:
            mvwprintw(main_window, height_main_window-3, 1, " \ufaa9 "); // Print string
        } else {
            mvwprintw(main_window, height_main_window-3, 1, " \ufaa8 "); // Print string
        }
        for(int line = 0; line < 4; line++) // Create for loop line_down
        {
            if(line == main_menu_highlight) // Check if line same as highlighted_number
            {
                wattron(main_window, A_BOLD); // Set string_attributes(main_window) on
                mvwprintw(main_window, line+3, 1, "> "); // Print string
            }
            mvwprintw(main_window, line+3, 3, main_menu_options[line]); // Print options to main_window
            mvwprintw(main_window, line+3, width_main_window-3, main_menu_indicator[line]); // Print indicator to main_window
            wattroff(main_window, A_BOLD); // Set string_attributes(main_window) off
        }
        main_menu_input = wgetch(main_window); // Get input_char from user
        if(main_menu_input == 259){ // If up_key pressed then:
            main_menu_highlight--; // Decrement highlight, it means move highlight up 1 line
            if(main_menu_highlight == -1) // Check if highlight not out of options_line
            {
                main_menu_highlight = 0; // Set highlight to line 0
            }
        } else if(main_menu_input == 258){ // If down_key pressed then:
            main_menu_highlight++; // Increment highlight, it means move highlight down 1 line
            if(main_menu_highlight == 4) // Check if highlight not out of options_line
            {
                main_menu_highlight = 3; // Set highlight to line 2
            }
        } else if(main_menu_input == 107){ // If k_key pressed then:
            main_menu_highlight--; // Decrement highlight, it means move highlight up 1 line
            if(main_menu_highlight == -1) // Check if highlight not out of options_line
            {
                main_menu_highlight = 0; // Set highlight to line 0
            }
        } else if(main_menu_input == 106){ // If j_key pressed then:
            main_menu_highlight++; // Increment highlight, it means move highlight down 1 line
            if(main_menu_highlight == 4) // Check if highlight not out of options_line
            {
                main_menu_highlight = 3; // Set highlight to line 2
            }
        } else if(main_menu_input == 114){ // If r_key pressed then:
            if(reverse_mode_status == false){ // Check if reverse_mode is off
                reverse_mode_status = true; // Set reverse_mode to on
            } else {
                reverse_mode_status = false; // Set reverse_mode to off
            }
        } else if(main_menu_input == 10){ // If enter_key pressed then:
            if(main_menu_highlight == 0){
                if(!getuid()){ // Check if run as root or not
                    create_box_inside_(sub_window, box_title[4], 1, width_sub_window/2-5); // Call function that create box inside sub_window
                    wattron(sub_window, COLOR_PAIR(1)); // Set string_attributes(sub_window) on
                    compile_("st"); // Call function that compile st
                    mvwprintw(sub_window, 2, 2, "+ Compile st"); // Print string
                    compile_("dwm"); // Call function that compile dwm 
                    mvwprintw(sub_window, 3, 2, "+ Compile Dwm"); // Print string
                    compile_("dmenu"); // Call function that compile dmenu
                    mvwprintw(sub_window, 4, 2, "+ Compile Dmenu"); // Print string
                    wattroff(sub_window, COLOR_PAIR(1)); // Set string_attributes(sub_window) off
                    read_command = popen(command="grep 'Craccken' /usr/lib/os-release 2> /dev/null", "r"); // Run & read command from shell.
                    fscanf(read_command, "%s", output_command); // Input the value from read_command to output_command
                    wgetch(sub_window); // Get input_char from user and wait if user doesn't input_char anything.
                } else {
                    create_box_inside_(sub_window, box_title[1], height_sub_window/2, width_sub_window/2-9); // Call function that create box inside sub_window
                    wgetch(sub_window); // Get input_char from user and wait if user doesn't input_char anything.
                }
            } else if(main_menu_highlight == 1){
                if(internet_connection_status == true){ // Check if internet_connection available
                    while(1) // Create an infinite-loop
                    {
                        if(reverse_mode_status == true){ // Check if reverse_mode is on
                            wclear(sub_window); // Clear sub_window
                            box(sub_window, 0, 0); // Create box inside sub_window
                            wattron(sub_window, A_ITALIC);
                            mvwprintw(sub_window, 1, width_sub_window/2-8, "Tools Installer"); // Print string
                            wattroff(sub_window, A_ITALIC);
                            mvwprintw(sub_window, height_sub_window-2, 2, "\uf04a "); // Print string
                        } else {
                            create_box_inside_(sub_window, box_title[3], 1, width_sub_window/2-8); // Call function that create box inside sub_window
                        }
                        for(int line = 0; line < 2; line++) // Create for loop line_down
                        {
                            if(line == tools_installer_menu_highlight) // Check if line same as highlighted_number
                            { 
                                wattron(sub_window, A_BOLD); // Set string_attributes(sub_window) on
                                mvwprintw(sub_window, line+3, 1, ">"); // Print string
                            }
                            mvwprintw(sub_window, line+3, 3, tools_installer_menu_options[line]); // Print options to tools_installer_menu
                            wattroff(sub_window, A_BOLD); // Set string_attributes(sub_window) off
                        }
                        tools_installer_menu_input = wgetch(sub_window); // Get input_char from user
                        if(tools_installer_menu_input == 259){ // If up_key pressed then:
                            tools_installer_menu_highlight--; // Decrement highlight, it means move highlight up 1 line 
                            if(tools_installer_menu_highlight == -1) // Check if highlight not out of options_line
                            {
                                tools_installer_menu_highlight = 0; // Set highlight to line 0
                            }
                        } else if(tools_installer_menu_input == 258){ // If down_key pressed then:
                            tools_installer_menu_highlight++; // Increment highlight, it means move highlight down 1 line
                            if(tools_installer_menu_highlight == 2) // Check if highlight not out of options_line
                            {
                                tools_installer_menu_highlight = 1; // Set highlight to line 2
                            }
                        } else if(tools_installer_menu_input == 107){ // If k_key pressed then:
                            tools_installer_menu_highlight--; // Decrement highlight, it means move highlight up 1 line 
                            if(tools_installer_menu_highlight == -1) // Check if highlight not out of options_line
                            {
                                tools_installer_menu_highlight = 0; // Set highlight to line 0
                            }
                        } else if(tools_installer_menu_input == 106){ // If j_key pressed then:
                            tools_installer_menu_highlight++; // Increment highlight, it means move highlight down 1 line
                            if(tools_installer_menu_highlight == 2) // Check if highlight not out of options_line
                            {
                                tools_installer_menu_highlight = 1; // Set highlight to line 2
                            }
                        } else if(tools_installer_menu_input == 113){ // If q_key pressed then:
                            break; // Break the loop
                        } else if(tools_installer_menu_input == 10){ // If enter_key pressed then: 
                            if(tools_installer_menu_highlight == 0){
                                for(int index_of_array = 0; index_of_array < ARRAY_LIMIT; index_of_array++) // Create a loop to conveniently call check_() function each tools_installer_menu_hacking_tools_options
                                {
                                    hacking_tools_installed[index_of_array] = check_(tools_installer_menu_hacking_tools_options[index_of_array]); // Call a check_() function and then return value of that function saved to hacking_tools_installed(list)
                                }
                                while(1) // Create an infinite-loop
                                {
                                    if(reverse_mode_status == true){ // Check if reverse_mode is on
                                        wclear(sub_window); // Clear sub_window
                                        box(sub_window, 0, 0); // Create box inside sub_window
                                        wattron(sub_window, A_ITALIC);  // Set string_attributes(sub_window) on
                                        mvwprintw(sub_window, 0, width_sub_window/2-8, "| Tools Installer |"); // Print string
                                        wattroff(sub_window, A_ITALIC); // Set string_attributes(sub_window) off
                                        mvwprintw(sub_window, height_sub_window-2, 2, "\uf04a "); // Print string
                                    } else {
                                        create_box_inside_(sub_window, box_title[3], 1, width_sub_window/2-8); // Call function that create box inside sub_window
                                    }
                                    wattron(sub_window, A_ITALIC | A_UNDERLINE);  // Set string_attributes(sub_window) on
                                    mvwprintw(sub_window, 2, 2, "Hacking_Tools"); // Print string
                                    wattroff(sub_window, A_ITALIC | A_UNDERLINE); // Set string_attributes(sub_window) off
                                    for(int line = 0; line < ARRAY_LIMIT; line++) // Create for loop line_down
                                    {
                                        if(line == tools_installer_menu_hacking_tools_highlight) // Check if line same as highlighted_number
                                        { 
                                            wattron(sub_window, A_BOLD); // Set string_attributes(sub_window) on
                                            mvwprintw(sub_window, line+4, 1, ">"); // Print string
                                        }
                                        if(hacking_tools_installed[line] == 1){ // If in hacking_tools_installed array value is true/1 then:
                                            mvwprintw(sub_window, line+4, width_sub_window-5, "\uf62e "); // Print string 
                                        } else {
                                            mvwprintw(sub_window, line+4, width_sub_window-5, "\uf62f "); // Print string 
                                        }
                                        mvwprintw(sub_window, line+4, width_sub_window-3, tools_installer_menu_hacking_tools_indicator[line]); // Print indicator to tools_installer_menu
                                        mvwprintw(sub_window, line+4, 3, tools_installer_menu_hacking_tools_options[line]); // Print options to tools_installer_menu
                                        wattroff(sub_window, A_BOLD); // Set string_attributes(sub_window) off
                                    }
                                    tools_installer_menu_hacking_tools_input = wgetch(sub_window); // Get input_char from user
                                    if(tools_installer_menu_hacking_tools_input == 259){ // If up_key pressed then:
                                        tools_installer_menu_hacking_tools_highlight--; // Decrement highlight, it means move highlight up 1 line 
                                        if(tools_installer_menu_hacking_tools_highlight == -1) // Check if highlight not out of options_line
                                        {
                                            tools_installer_menu_hacking_tools_highlight = 0; // Set highlight to line 0
                                        }
                                    } else if(tools_installer_menu_hacking_tools_input == 258){ // If down_key pressed then:
                                        tools_installer_menu_hacking_tools_highlight++; // Increment highlight, it means move highlight down 1 line
                                        if(tools_installer_menu_hacking_tools_highlight == ARRAY_LIMIT) // Check if highlight not out of options_line
                                        {
                                            tools_installer_menu_hacking_tools_highlight = ARRAY_LIMIT-1; // Set highlight to line 6
                                        }
                                    } else if(tools_installer_menu_hacking_tools_input == 107){ // If k_key pressed then:
                                        tools_installer_menu_hacking_tools_highlight--; // Decrement highlight, it means move highlight up 1 line 
                                        if(tools_installer_menu_hacking_tools_highlight == -1) // Check if highlight not out of options_line
                                        {
                                            tools_installer_menu_hacking_tools_highlight = 0; // Set highlight to line 0
                                        }
                                    } else if(tools_installer_menu_hacking_tools_input == 106){ // If j_key pressed then:
                                        tools_installer_menu_hacking_tools_highlight++; // Increment highlight, it means move highlight down 1 line
                                        if(tools_installer_menu_hacking_tools_highlight == ARRAY_LIMIT) // Check if highlight not out of options_line
                                        {
                                            tools_installer_menu_hacking_tools_highlight = ARRAY_LIMIT-1; // Set highlight to line 6
                                        }
                                    } else if(tools_installer_menu_hacking_tools_input == 113) { // If q_key pressed then:
                                        break; // Break the loop
                                    } else if(tools_installer_menu_hacking_tools_input == 10){ // If enter_key pressed then:
                                        endwin();                   // Restore to normal terminal
                                        system(command="clear");    // Run system command that clear the terminal
                                        install_(tools_installer_menu_hacking_tools_options[tools_installer_menu_hacking_tools_highlight],reverse_mode_status); // Call function that install_package or remove_package based on reverse_mode_status
                                    }
                                }
                            } 
                        }
                    }
                } else {
                    create_box_inside_(sub_window, box_title[2], height_sub_window/2, width_sub_window/2-15); // Call function that create box inside sub_window
                    wgetch(sub_window); // Get input_char from user and wait if user doesn't input_char anything.
                }
            } else if(main_menu_highlight == 2){ // If highlight same as option index 2 then:
                create_box_inside_(sub_window, box_title[5], 1, width_sub_window/2-3); // Call function that create box inside sub_window
                mvwprintw(sub_window, 2, 2, "j/down-key : Move highlight down");
                mvwprintw(sub_window, 3, 2, "k/up-key   : Move highlight up");
                mvwprintw(sub_window, 4, 2, "r          : Toggle reverse_mode");
                mvwprintw(sub_window, 5, 2, "q          : Quit sub_window");
                wgetch(sub_window); // Get input_char from user and wait if user doesn't input_char anything.
            } else if(main_menu_highlight == 3){ // If highlight same as option index 3 then:
                break; // Break the loop
            }
        }
    }
    pclose(read_command); // Close a stream that opened by a popen() call and wait for the command to terminate.
    endwin(); // Restore to normal terminal
    return 0;
}

void compile_(const char *package) // Define function
{
    char *command; // Declare type of variable
    if(strcmp(package,"st") == 0){ // Check if string is same as the parameter
        system(command="[[ -d /usr/local/src/st/ ]] && cd /usr/local/src/st && make clean install >> /var/log/cratc.log 2>1&"); // Run system_command that compile st.
    } else if(strcmp(package,"dwm") == 0){ // Check if string is same as the parameter
        system(command="[[ -d /usr/local/src/dwm/ ]] && cd /usr/local/src/dwm && make clean install >> /var/log/cratc.log 2>1&"); // Run system_command that compile dwm.   
    } else if(strcmp(package,"dmenu") == 0){ // Check if string is same as the parameter
        system(command="[[ -d /usr/local/src/dmenu/ ]] && cd /usr/local/src/dmenu && make clean install >> /var/log/cratc.log 2>1&"); // Run system_command that compile dmenu.
    }
}

void install_(const char package[], bool reverse_mode) // Define function
{
    char *command; // Declare type of variable
    if(getuid() == 0){ // Check if root user
        if(!strcmp(package,"metasploit")){       // Check if string is same as the parameter
            if(reverse_mode == true){ // Check if reverse_mode is on
                system(command="/bin/zsh -c \"pacman -Rsv --noconfirm metasploit\""); // Run system command that remove metasploit 
            } else {
                system(command="/bin/zsh -c \"pacman -Sy --needed --noconfirm metasploit\""); // Run system command that install metasploit
            }
        } else if(!strcmp(package,"nmap")){      // Check if string is same as the parameter
            if(reverse_mode == true){ // Check if reverse_mode is on
                system(command="/bin/zsh -c \"pacman -Rsv --noconfirm nmap\""); // Run system command that remove nmap
            } else {
                system(command="/bin/zsh -c \"pacman -Sy --needed --noconfirm nmap\""); // Run system command that install nmap
            }
        } else if(!strcmp(package,"sqlmap")){    // Check if string is same as the parameter
            if(reverse_mode == true){ // Check if reverse_mode is on
                system(command="/bin/zsh -c \"pacman -Rsv --noconfirm sqlmap\""); // Run system command that remove sqlmap
            } else {
                system(command="/bin/zsh -c \"pacman -Sy --needed --noconfirm sqlmap\""); // Run system command that install sqlmap
            }
        } else if(!strcmp(package,"wpscan")){    // Check if string is same as the parameter
            if(reverse_mode == true){ // Check if reverse_mode is on
                system(command="/bin/zsh -c \"pacman -Rsv --noconfirm wpscan\""); // Run system command that remove wpscan
            } else {
                system(command="/bin/zsh -c \"pacman -Sy --needed --noconfirm wpscan\""); // Run system command that install wpscan
            }
        } else if(!strcmp(package,"wireshark")){ // Check if string is same as the parameter
            if(reverse_mode == true){ // Check if reverse_mode is on
                system(command="/bin/zsh -c \"pacman -Rsv --noconfirm wireshark-qt\""); // Run system command that remove wireshark
            } else {
                system(command="/bin/zsh -c \"pacman -Sy --needed --noconfirm wireshark-qt\""); // Run system command that install wireshark-qt
            }
        } else if(!strcmp(package,"aircrack-ng")){ // Check if string is same as the parameter
            if(reverse_mode == true){ // Check if reverse_mode is on
                system(command="/bin/zsh -c \"pacman -Rsv --noconfirm aircrack-ng\""); // Run system command that remove aircrack-ng
            } else {
                system(command="/bin/zsh -c \"pacman -Sy --needed --noconfirm aircrack-ng\""); // Run system command that install aircrack-ng
            }
        } else if(!strcmp(package,"nikto")){ // Check if string is same as the parameter
            if(reverse_mode == true){ // Check if reverse_mode is on
                system(command="/bin/zsh -c \"pacman -Rsv --noconfirm nikto\""); // Run system command that remove nikto
            } else {
                system(command="/bin/zsh -c \"pacman -Sy --needed --noconfirm nikto\""); // Run system command that install nikto
            }
        }
    } else {
        if(access("/usr/bin/yay",F_OK) == 0){
            if(!strcmp(package,"arpfox")){    // Check if string is same as the parameter
                if(reverse_mode == true){ // Check if reverse_mode is on
                    system(command="/bin/zsh -c \"yay -Rsv --noconfirm arpfox-bin\""); // Run system command that remove arpfox
                    system(command="/bin/zsh -c \"yay -Sy --needed arpfox-bin\""); // Run system command that install arpfox
                }
            } else if(!strcmp(package,"burpsuite")){    // Check if string is same as the parameter
                if(reverse_mode == true){ // Check if reverse_mode is on
                    system(command="/bin/zsh -c \"yay -Rsv --noconfirm burpsuite\""); // Run system command that remove burpsuite
                } else {
                    system(command="/bin/zsh -c \"yay -Sy --needed burpsuite\""); // Run system command that install burpsuite
                    system(command="/bin/zsh -c \"archlinux-java set java-11-openjdk\""); // Fix "an unknown error has occured" The default java installed in AUR package is 13 but burpsuite officially only support 11.
                }
            }
        } else {
            system(command="/bin/zsh -c \"echo 'YOU MUST INSTALL,YAY!!'; sleep 3\"");
        }
    }
}

bool check_(const char package[]) // Define function
{
    if(!strcmp(package,"arpfox")){ // Check if string is same as the parameter
        if(access("/usr/bin/arpfox",F_OK) == 0){ // Check if arpfox is available
            return true; // Return true(1)
        } else {
            return false; // Return false(0)
        }
    } else if(!strcmp(package,"burpsuite")){ // Check if string is same as the parameter
        if(access("/usr/bin/burpsuite",F_OK) == 0){ // Check if burpsuite is available
            return true; // Return true(1)
        } else {
            return false; // Return false(0)
        }
    } else if(!strcmp(package,"metasploit")){ // Check if string is same as the parameter
        if(access("/usr/bin/metasploit",F_OK) == 0){ // Check if metasploit is available
            return true; // Return true(1)
        } else {
            return false; // Return false(0)
        }
    } else if(!strcmp(package,"nmap")){ // Check if string is same as the parameter
        if(access("/usr/bin/nmap",F_OK) == 0){ // Check if nmap is available.
            return true; // Return true(1)
        } else {
            return false; // Return false(0)
        }
    } else if(!strcmp(package,"sqlmap")){ // Check if string is same as the parameter
        if(access("/usr/bin/sqlmap",F_OK) == 0){ // Check if sqlmap is available
            return true; // Return true(1)
        } else {
            return false; // Return false(0)
        }
    } else if(!strcmp(package,"wireshark")){ // Check if string is same as the parameter
        if(access("/usr/bin/wireshark",F_OK) == 0){ // Check if wireshark is available
            return true; // Return true(1)
        } else {
            return false; // Return false(0)
        }
    } else if(!strcmp(package,"wpscan")){ // Check if string is same as the parameter
        if(access("/usr/bin/wpscan",F_OK) == 0){ // Check if wpscan is available
            return true; // Return true(1)
        } else {
            return false; // Return false(0)
        }
    } else if(!strcmp(package,"aircrack-ng")){ // Check if string is same as the parameter
        if(access("/usr/bin/aircrack-ng",F_OK) == 0){ // Check if aircrack-ng is available
            return true; // Return true(1)
        } else {
            return false; // Return false(0)
        }
    } else {
        return 0;
    }
}

void create_box_inside_(WINDOW *target_window, const char box_title[], const int start_title_y, const int start_title_x) // Define function
{
    wclear(target_window); // Clear target_window
    box(target_window, 0, 0); // Create box inside target_window
    mvwprintw(target_window, start_title_y, start_title_x, box_title); // Print string
}
