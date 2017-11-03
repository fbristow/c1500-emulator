#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MEM_SIZE 8

#define LINE_SIZE 80
#define MAX_INSTR 20

static int memory[ MEM_SIZE ];
static char instructions[ MAX_INSTR ][ LINE_SIZE ];
typedef struct ALU
{
    int a, b, out;
} Alu;

int main( int argc, char *argv[] )
{
    int current_instruction = 0;
    int total_instructions = 0;
    FILE *f = fopen( argv[ 1 ], "r" );
    while( fgets( instructions[ current_instruction ], LINE_SIZE, f ) != NULL )
    { 
        instructions[ current_instruction ][ strlen( instructions[ current_instruction ] ) - 1] = 0;
        current_instruction++;
    } // load all of our instructions into memory.
    fclose( f );
    total_instructions = current_instruction;

    bool executing = true;
    Alu alu;
    current_instruction = 0;

    while( executing )
    {
        char full_instruction[ LINE_SIZE ];
        char *instruction, *o1, *o2;
        int operand1, operand2;
        strncpy( full_instruction, instructions[ current_instruction++ ], LINE_SIZE );
        instruction = strtok( full_instruction, " " );
        o1 = strtok( NULL, ", " );
        if ( o1 != NULL ) 
        {
            operand1 = atoi( o1 );
            o2 = strtok( NULL, ", " );
            if ( o2 != NULL ) 
            {
                operand2 = atoi( o2 );
            }
        }
        
        if ( strcmp( instruction, "SAVE" ) == 0 ) {
            memory[ operand1 ] = alu.out;
        } else if ( strcmp( instruction, "STORE" ) == 0 ) {
            memory[ operand2 ] = operand1;
        } else if ( strcmp( instruction, "GETA" ) == 0 ) {
            alu.a = memory[ operand1 ];
        } else if ( strcmp( instruction, "GETB" ) == 0 ) {
            alu.b = memory[ operand1 ];
        } else if ( strcmp( instruction, "ADD" ) == 0 ) {
            alu.out = alu.a + alu.b;
        } else if ( strcmp( instruction, "SUB" ) == 0 ) {
            alu.out = alu.a - alu.b;
        } else if ( strcmp( instruction, "MULT" ) == 0 ) {
            alu.out = alu.a * alu.b;
        } else if ( strcmp( instruction, "GETINPUT" ) == 0 ) {
            scanf( "%d", &memory[ operand1 ] );
        } else if ( strcmp( instruction, "OUTPUT" ) == 0 ) {
            printf( "%d\n", memory[ operand1 ] );
        } else if ( strcmp( instruction, "JUMP" ) == 0 ) {
            current_instruction = operand1;
        } else if ( strcmp( instruction, "JUMPIFZERO" ) == 0 ) {
            if ( alu.out == 0 ) {
                current_instruction = operand1;
            }
        } else {
            printf( "Unrecognized instruction %s\n", instruction );
        }

        if ( current_instruction == total_instructions ) {
            executing = false;
        }
    }

/*    printf( "Contents of memory after execution:\n" );
    for ( int i = 0; i < MEM_SIZE; i++ )
    {
        printf( "%d\n", memory[ i ] );
    }
*/

    return EXIT_SUCCESS;
}
