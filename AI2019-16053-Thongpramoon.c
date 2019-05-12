/* sd16053 トンプラムーン　ポンサシット */
/* 人工知能（４）*/

#include "stdio.h"

/* function prototypes */

void init_board();  /* initialize the game board */
void print_board(); /* display the 3x3 board */
void print_xo();    /* print the x or o mark */
void human();       /* get the position to mark */
void computer(int turn);  /* a computer's move */
int  find_max(int *x, int *y, int turn);  /* find the max position */
int  find_min(int *x, int *y, int turn);  /* find the min position */
int judge(); /* judge who wins */

/* global variable */

int board[3][3];  /* 3x3 game board */
/*  1: o (computer) */
/* -1: x (human) */
/*  0: empty position */

/* ------------ main program --------------- */

void main () {
  int i,j;
  int turn = 1; /* the number of moves, starting from 1 */
  int result;

  init_board();

  print_board();

  while ( (result=judge())==0 ){ 
    if (turn % 2==1) human();  /* human's turn */
    else computer(turn);       /* computer's turn */
   
    print_board(); 
    turn++;
    if( turn>9 ) { break; } /* no more empty position */
  }
  if ( result==-1) printf( "x won!\n" );
  if ( result==1) printf( "o won!\n" );
  if ( result==0) printf( "Draw!\n" );
}

/* -------------- functions --------------*/

/* clear the board */
void init_board() {
  int i,j;

  for( i=0 ; i<3 ; i++ ) 
    for( j=0 ; j<3 ; j++ ) 
      board[i][j]=0;
}

/* print " ", "o" , or "x", 
   if the argument is 0, 1, or -1, respectively */
void print_xo( int xo ) {
  switch (xo) { 
           case 0: printf( " " ); 
              break;
           case 1: printf( "o" ); 
              break;
           case -1: printf( "x" ); 
              break;
  }
}

/* display the 3x3 board */
void print_board() {
   int i,j;

   printf( "\ni\\j  (1)     (2)     (3)   \n" );
   printf( "---------------------------\n" );
   for ( i=0; i<3 ; i++ ) {
        printf( "(%d)   ",i+1 );
        print_xo( board[i][0] );
        for ( j=1; j<3 ; j++ ) {
           printf( "   |   " );
           print_xo( board[i][j] );
        }
        printf( "   \n" );
        printf( "---------------------------\n" );
    }
}

/* get a pair of numbers from a human */
void human () {
  int x,y;
  
  do { 
    printf( "Input row and column (i, j): " );

    if ( scanf( "%d %d", &x, &y) ==2 ) {
       x--; y--;
    } else {
      fflush(stdin);
      printf("*** Input a pair of numbers 1-3\n\n");
      continue;
    }
  } while ((x<0)||(x>3)||(y<0)||(y>3)||(board[x][y]!=0));

  board[x][y] = -1;
}

/* judge who wins */
/*   Return value: 1 if "o" wins; -1 if "x" wins; 0 otherwise */
int judge() {
  int i,j;
  int count;

  /* horizontal check */
  for( i=0; i<3 ; i++ ) {
      count = 0;
      for( j=0; j<3 ; j++ ) count += board[i][j];
      if ( count==3 ){  return(1); }
      if ( count==-3 ){ return(-1); }
  }

  /* vertical check */
  for( j=0; j<3 ; j++ ) {
      count = 0;
      for( i=0; i<3 ; i++ ) count += board[i][j];
      if ( count==3 ){ return(1); }
      if ( count==-3 ){ return(-1); }
  }

  /* diagonal check */
  count = 0;
  for( i=0; i<3 ; i++ ) count += board[i][i];
  if ( count==3 ){ return(1); }
  if ( count==-3 ){ return(-1); }

  count = 0;
  for( i=0; i<3 ; i++ ) count += board[i][2-i];
  if ( count==3 ){ return(1); }
  if ( count==-3 ){ return(-1); }

  return(0); 
}

/* ------- write your code below ------ */

/* find the best position using the min-max approach */
void computer (int turn) {
  int x,y;
  
  find_max(&x,&y,turn);
  board[x][y] = 1;
}

/* Maximization phase */
/*   Input: trun */
/*   Output: the max position (x,y) */
int find_max( int *x, int *y, int turn ) {
  int max_x,max_y;

  int min_x,min_y;
  int i,j;
  int value;
  int max=-999;

  value = judge();
  if ((turn>9)&&(value==0)) return(0); /* draw */
  if (value==1) return(1);              /* o won */
  if (value==-1) return(-1);            /* x won */

  /* find a position with the maximum value */  
  for(i=0; i<3; i++) {
    for(j=0; j<3; j++) {
      if(board[i][j] == 0) {
        board[i][j] = 1; /* temporally, mark "o" at (i,j) */
        value = find_min(&min_x, &min_y, turn+1);
        if (value > max) {
          max = value;
          max_x = i;
          max_y = j;
         }
        board[i][j]=0; /* clear "o" at (i,j) */
      }
    }
  }
  *x = max_x;  /* the best row */
  *y = max_y;  /* the best column */
  return max;
}

/* Minimization phase */
/*   Input: trun */
/*   Output: the min position (x,y) */
int find_min( int *x, int *y, int turn ){
  int min_x,min_y;
  int max_x,max_y;
  int i,j;
  int value;
  int min=999;

  value = judge();
  if ((turn>9)&&(value==0)) return(0); /* draw */
  if (value==1) return(1);              /* o won */
  if (value==-1) return(-1);            /* x won */

  /* find a position with the minimum value */  
  for(i=0; i<3; i++) {
    for(j=0; j<3; j++) {
      if(board[i][j] == 0) {
        board[i][j] = -1; /* temporally, mark "x" at (i,j) */
        value = find_max(&max_x, &max_y, turn+1);
        if (value < min) {
          min =value;
          min_x = i;
          min_y = j;
         }
        board[i][j]=0; /* clear "x" at (i,j) */
      }
    }
  }
  *x = min_x;
  *y = min_y;
  return min;
}


