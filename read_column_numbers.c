#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLS 20
#define MAX_INPUT 1000

int read_column_number( int columns[], int max );
void rearrange( char *output, char const *input, int n_columns, int const columns[] );

int main( void ) {
	int n_columns;
	int columns[MAX_COLS];
	char input[MAX_INPUT];
	char output[MAX_INPUT];

	n_columns = read_column_number( columns, MAX_COLS);

	while( gets( input ) != NULL) {
		printf( "Original input: %s\n", input );
		rearrange( output, input, n_columns, columns );
		printf( "Rearrange line: %s\n", output );

	}

	return 0;
}

int read_column_number( int columns[], int max ) {
	int num = 0;
	int ch;

	while( num < max && scanf( "%d", &columns[num] ) == 1 && columns[num] >= 0) 
		num++;

	if( num % 2 != 0 ) {
		puts( "Last column number is not paired." );
		exit( -1 );
	}

	while( (ch = getchar()) != EOF && ch != '\n' );

	return num;
}

void rearrange( char * output, char const *input, int n_columns, int const columns[] ) {
	int col;
	int output_col;
	int len;

	len = strlen( input );
	output_col = 0;

	for ( col = 0; col < n_columns; col += 2) {
		int nchars = columns[col + 1] - columns[col] + 1;
		if (columns[col] >= len || output_col == MAX_INPUT -1 ){
			break;
		}
		strncpy( output + output_col, input + columns[col], nchars );
		output_col += nchars;
						
	}

	output[output_col] = '\0';
}
