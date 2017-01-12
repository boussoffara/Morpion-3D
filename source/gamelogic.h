/*
 *      __  __    ___    ___   ___   ___    ___    _  _
 *     |  \/  |  / _ \  | _ \ | _ \ |_ _|  / _ \  | \| |
 *     | |\/| | | (_) | |   / |  _/  | |  | (_) | | .` |
 *     |_|  |_|  \___/  |_|_\ |_|   |___|  \___/  |_|\_|
 *
 *     @Author: Alaeddine.Boussoffara
       @File:  GameLogic.h
 */

extern int usleep (__useconds_t __useconds);
extern void print_ascii_art(int max_x,int max_y);
extern void displaycollapse(int ** collapse,int size);
extern int remove_piece(char *** board,int ** summits,int i, int j);
extern int add_piece(int size,char *** board,int ** summits,int i, int j,char cell);
extern void error_handler(int error,int size);
extern int ** collapse_prob(int ** summits,int size);
extern int check_result(int choice,char *** board,int ** summits,int size);
extern void celebrate(int result);
