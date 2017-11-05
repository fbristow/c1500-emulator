#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define MEM_SIZE 8

#define LINE_SIZE 80
#define MAX_INSTR 20


enum InstructionType
{
    SAVE,
    STORE,
    GETA,
    GETB,
    ADD,
    SUB,
    MULT,
    GETINPUT,
    OUTPUT,
    JUMP,
    JUMPIFZERO,
    UNRECOGNIZED
};

struct Instruction
{
    int operand1;
    int operand2;
    enum InstructionType type;
};

struct ALU
{
    int a, b, out;
};

struct CPU
{
    struct ALU alu;
    int memory[ MEM_SIZE ];
    struct Instruction instructions[ MAX_INSTR ];
    unsigned int current_instruction;
    unsigned int total_instructions;
};

static struct CPU cpu = {
    .current_instruction = 0,
    .total_instructions = 0
};

enum InstructionType decode_instruction( char const * const instruction )
{
    enum InstructionType type = UNRECOGNIZED;

    if ( strcmp( instruction, "SAVE" ) == 0 ) {
        type = SAVE;
    } else if ( strcmp( instruction, "STORE" ) == 0 ) {
        type = STORE;
    } else if ( strcmp( instruction, "GETA" ) == 0 ) {
        type = GETA;
    } else if ( strcmp( instruction, "GETB" ) == 0 ) {
        type = GETB;
    } else if ( strcmp( instruction, "ADD" ) == 0 ) {
        type = ADD;
    } else if ( strcmp( instruction, "SUB" ) == 0 ) {
        type = SUB;
    } else if ( strcmp( instruction, "MULT" ) == 0 ) {
        type = MULT;
    } else if ( strcmp( instruction, "GETINPUT" ) == 0 ) {
        type = GETINPUT;
    } else if ( strcmp( instruction, "OUTPUT" ) == 0 ) {
        type = OUTPUT;
    } else if ( strcmp( instruction, "JUMP" ) == 0 ) {
        type = JUMP;
    } else if ( strcmp( instruction, "JUMPIFZERO" ) == 0 ) {
        type = JUMPIFZERO;
    }

    assert( type != UNRECOGNIZED );

    return type;
}

void load_instructions( char const * const filename )
{
#define DELIMITERS ", "
    char line[ LINE_SIZE ];
    char *operand;
    FILE *f = fopen( filename, "r" );
    assert( f != NULL );

    if ( f )
    {
        unsigned int current_instruction = 0;
        struct Instruction *instruction = &cpu.instructions[ current_instruction ];
        
        // load all instructions into memory
        while( fgets( line, LINE_SIZE, f ) != NULL )
        {
            // chuck the newline(s)
            line[ strcspn( line, "\r\n" ) ] = '\0';

            // get the actual op
            strtok( line, DELIMITERS );
            instruction->type = decode_instruction( line );

            // get operand1
            operand = strtok( NULL, DELIMITERS );
            if ( operand != NULL ) 
            {
                instruction->operand1 = atoi( operand );
                // get operand2
                operand = strtok( NULL, DELIMITERS );
                if ( operand != NULL ) 
                {
                    instruction->operand2 = atoi( operand );
                }
            }


            current_instruction++;
            assert( current_instruction < MAX_INSTR );
            instruction = &cpu.instructions[ current_instruction ];
        }

        fclose( f );
        cpu.total_instructions = current_instruction;
    }
    else
    {
        printf( "Unable to open %s [%d]\n", filename, ferror( f ) );
    }
}

void check_state()
{
    assert( cpu.total_instructions > 0 );
    assert( cpu.current_instruction < cpu.total_instructions );

    // instructions are consecutive (we don't have addresses
    // in program-space that are missing instructions)
    for( int i = 0; i < cpu.total_instructions; i++ )
    {
        assert( cpu.instructions[ i ].type != UNRECOGNIZED );
    }
}

void init_cpu()
{
    struct Instruction default_instruction = {
        .type = UNRECOGNIZED,
        .operand1 = 0,
        .operand2 = 0
    };

    for ( int i = 0; i < MEM_SIZE; i++ )
    {
        cpu.memory[ i ] = 0;
    }

    for ( int i = 0; i < MAX_INSTR; i++ )
    {
        cpu.instructions[ i ] =  default_instruction;
    }
}

int main( int argc, char *argv[] )
{
    init_cpu();
    load_instructions( argv[ 1 ] );
    check_state();

    bool executing = true;
    struct Instruction *instruction;

    while( executing )
    {
        check_state();

        instruction = &cpu.instructions[ cpu.current_instruction++ ];
        switch( instruction->type )
        {
            case SAVE:
                cpu.memory[ instruction->operand1 ] = cpu.alu.out;
                break;
            case STORE:
                cpu.memory[ instruction->operand2 ] = instruction->operand1;
                break;
            case GETA:
                cpu.alu.a = cpu.memory[ instruction->operand1 ];
                break;
            case GETB:
                cpu.alu.b = cpu.memory[ instruction->operand1 ];
                break;
            case ADD:
                cpu.alu.out = cpu.alu.a + cpu.alu.b;
                break;
            case SUB:
                cpu.alu.out = cpu.alu.a - cpu.alu.b;
                break;
            case MULT:
                cpu.alu.out = cpu.alu.a * cpu.alu.b;
                break;
            case GETINPUT:
                scanf( "%d", &cpu.memory[ instruction->operand1 ] );
                break;
            case OUTPUT:
                printf( "%d\n", cpu.memory[ instruction->operand1 ] );
                break;
            case JUMP:
                cpu.current_instruction = instruction->operand1;
                break;
            default: // JUMPIFZERO
                if ( cpu.alu.out == 0 ) {
                    cpu.current_instruction = instruction->operand1;
                }
        }

        if ( cpu.current_instruction == cpu.total_instructions ) {
            executing = false;
        }

    }

    return EXIT_SUCCESS;
}
