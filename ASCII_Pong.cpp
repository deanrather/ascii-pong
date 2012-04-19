//ASCII PONG!!!
//By Dean Rather.

//Library's to include.
#include <stdlib.h> // C++'s Standard Template Library (all the standard functions)
#include <iostream> // cin and cout
#include <ctime> // timer-related functions
#include <string> // string-related functions

using namespace std; // refine scope to std namespace

//Function Declerations
inline int TIMER(); // returns miliseconds (10 in a second)
void drawArena(); // draws items to buffer and displays screen
void moveBall(); // calculates collisions/bounces ball, and mods ball coords
inline int wrap(int degrees); // makes numbers between 0 and 360
void miss(int player); // called when a player misses the ball
void aiPaddle(); // alters paddle coords

//Global vars

//Time-relates vars
const int ORIG_TIME_SECS = time(0); // seed time
const int ORIG_TIME_MS = clock(); // seed time
int speed = 1; // speed/10

//arena related vars
const int ARENA_HEIGHT = 20; // Height of arena
const int ARENA_WIDTH = 50; // Width of arena
char arena[ARENA_HEIGHT][ARENA_WIDTH]; // the 2D character array that is the arena

//ball related vars
int BALL_Y = ARENA_HEIGHT/2; // ball's Y starts in middle of arena
int BALL_X = ARENA_WIDTH/2; //  ball's X begins in middle of arena
int BALL_DEGREES = 45; // ball starts facing 45 degrees (note - degrees are upside down)
const char BALL = '?'; // graphics for the ball.

//paddle related vars
const int PADDLE_WIDTH = 1; // Width of paddles
const int PADDLE_HEIGHT = 4; // Height of paddles
int PADDLE1_Y = (ARENA_HEIGHT/2) - (PADDLE_HEIGHT/2); // start paddle in the middle
int PADDLE2_Y = (ARENA_HEIGHT/2) - (PADDLE_HEIGHT/2);
const char PADDLE[PADDLE_HEIGHT] = {'#',
                                    '|',
                                    '|',
                                    '#'};// graphics for paddle
            
//layout related vars
string leftMargin(15,' '); // fifteen spaces make the left margin

//Angle related vars
int angles[8]; // an array of 8 different angles (un-initialized)
const int directions[8][2]  = {{2,0},
                               {1,1},
                               {0,2},
                               {-1,1},
                               {-2,0},
                               {-1,-1},
                               {0,-2},
                               {1,-1}};
                               //These are how many paces in what direction
                              //I want the ball to move corresponding to
                             //what angle the ball is facing. (the andgles array)
                            
//player-related vars
int hitLast = 1; // the last player to hit the ball
string message = " "; // a message I might want to display


int main ()
{  
    //Setting Up
    int temp_time = TIMER(); // sets temp time to the current time (in ms)
    bool gameOver = false; // the game is not over... yet.
    for (int i = 0; i < 8; i++) // innitialising the angles array
        angles[i] = 45 * i; //angles[0] is 0, angles[1] is 45, [2] is 90, etc. 
    
    while (!gameOver) // while the game isn't over, loop this
    {
        // Screen refreshes every x/10'th of a second.
        if (TIMER() == temp_time+speed)
        {
             //The Actual Loop!!!
             
             message = " "; // clear the message var
             
             aiPaddle(); // move the paddles
             moveBall(); // move the ball
             
             drawArena(); // draw the arena
             
             
             temp_time = TIMER(); //update the temp timer
             
             //End The Actual Loop!!
         }
    }
    

    //Game Over Info.
    string quit; // makes a string
    cout << "Now Quitting, ok?\n"; // lets the player know were quitting
    cin >> quit; //waits for user input
    return 0; // ends the game regardless (the point of this was so it wouldn't
             // just close without waiting for the users input.)
}

inline int TIMER() //inlining the function copys it to it's reference on compilation
{                  //this speeds it up at runtime because it doesn't need to jump code
    int secs = time(0) - ORIG_TIME_SECS; // how many seconds the game has passed
    int ms = (clock() - ORIG_TIME_MS)/100; // this int is how many ms have passed
    return ms; //10 ms in a second.

}

void moveBall()
{
    //calculate wall collisions.
    if (BALL_Y >= ARENA_HEIGHT-2 || BALL_Y <= 1) // if the ball is at the very top
                                                 //or very bottom, then:
       BALL_DEGREES = wrap(180 - BALL_DEGREES); //alter it's angle
    
    //calculate colisions with left paddle
    if (BALL_X <= 1 && hitLast == 2) // if the paddle is on the left wall, and
                                     //player 2 hit it last;
        if (BALL_Y < PADDLE1_Y ||               // if the ball isn't hitting the
            BALL_Y > PADDLE1_Y + PADDLE_HEIGHT) //paddle
            miss(1); //call the miss function, and tell it who missed.
        else // otherwise
        {
            BALL_DEGREES = wrap(0-BALL_DEGREES); //alter the ball's angle
            hitLast = 1; // and change who hit it last
        }
            
    //calculate colisions with right paddle
    if (BALL_X >= ARENA_WIDTH - 3 && hitLast == 1)
        if (BALL_Y < PADDLE2_Y || BALL_Y > PADDLE2_Y + PADDLE_HEIGHT)
            miss(2);
        else
        {
            BALL_DEGREES = wrap(0-BALL_DEGREES);
            hitLast = 2;
        }
            
     //compare ball's angle to the arrays of viable angles, and their corresponding
    //coordinate alterations.
    for (int i = 0; i<8; i++) //loop 8 times (0 - 7)
        if (angles[i] == BALL_DEGREES) // if the ball's angle corresponds to the angle
                                       // stored in andgles[i]
        {
            BALL_Y += directions[i][0]; // alter the ball's Y by the corresponding value
            BALL_X += directions[i][1]; // and the ball's X.
            return; // exit the function early once this loop's succeeded
        }
}


inline int wrap(int degrees)
{
    //filter all numbers to be from 0 to 359.
    while (degrees < 0) // add 360 till it untill it's in range
        degrees += 360;
    while (degrees > 359) // or minus 360 from it till it's in range
        degrees -= 360;
    return degrees; // return the result.
    
}

void drawArena()
{
    //initiate arena to all blank
    for (int y=0; y<ARENA_HEIGHT; y++) // for every arena Y position
        for (int x=0; x<ARENA_WIDTH; x++) // and for every arena X position
             arena[y][x] = ' '; // make that position a blank
    
    //paste top and bottom bars onto arena
    for (int x = 0; x < ARENA_WIDTH; x++) // for every arena x position
    {
        arena[0][x] = '-'; //make the top row dashes 
        arena[ARENA_HEIGHT-1][x] = '-'; // and the bottom row dashes
    }
             
    //paste ball position into arena array
    arena[BALL_Y][BALL_X] = BALL; // put the BALL char into the arena
                                  //at the ball's X and Y coord.
    
    //paste paddle positions into arena array
    for (int i = 0; i<PADDLE_HEIGHT; i++) // for ever piece of paddle
    {
        arena[PADDLE1_Y+i][0] = PADDLE[i]; // put the left paddle in it's place
        arena[PADDLE2_Y+i][ARENA_WIDTH-1] = PADDLE[i]; // and the right paddle too
    }
    
    //clear screen.
    system("cls");
        
    //display arena array
    for (int y=0; y<ARENA_HEIGHT; y++) // for every arena Y position
    {    
        //cout << leftMargin;
        for (int x=0; x<ARENA_WIDTH; x++)  //and every arena X position
        {
             cout << arena[y][x]; // print the arena's X Y position to screen
        }
        
        switch (y) // with reference to the line
        {
            case 1: // if it's line 1, print this message. etc.
                cout << "   Ball Angle: " << BALL_DEGREES;
                break;
            case 2:
                cout << "   Ball Y/X: " << BALL_Y << "/" << BALL_X;
                break;
            case 3:
                cout << "   HitLast: " << hitLast;
                break;
            case 4:
                cout << "   Time: " << TIMER();
                break;
            case 5:
                cout << "    " <<  message;
                break;
                
        }
        
        cout << endl; // after each line is read, start a new one
    }
}

void miss(int player) // if a player misses
{
    if (player == 1) // and it's player one
        while(true)
            cout << "Player Two Scores!!\n"; // player two scores!
            
    if (player == 2)
        while(true)
            cout << "Player One Scores!!\n";
}//note: this function should never occur with this AI

void aiPaddle() // alters paddle coords
{
    {
       
       if (hitLast == 2) // if player 2 hit the ball,
          if (PADDLE1_Y < BALL_Y-2) // and if the paddle is too low
              PADDLE1_Y ++; // increase it's height.
          else if (PADDLE1_Y > BALL_Y-2) // but if it's too high
              PADDLE1_Y --; // decrease it's height!
       
       if (hitLast == 1) //same
          if (PADDLE2_Y < BALL_Y-2)
              PADDLE2_Y ++;
          else if (PADDLE2_Y > BALL_Y-2)
              PADDLE2_Y --;
   }
}
