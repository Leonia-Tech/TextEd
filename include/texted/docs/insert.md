# INSERT.H DOCUMENTATION 

## INSERT FUNCTIONS 

``` C
char* insert();
``` 
insert mode

## ------------------------------------------------------

 ``` C 
 int getInsertArgs(char* args);
 ```
insert 

## ------------------------------------------------------

``` C 
char* strins(char* out, char* in, char ch);
``` 
Insert "in" into "out" before "ch".

## AAD LINE FUNCTION 

``` C 
int addLine(char*** LineBuffer, size_t* Lines, char* NewLine, int Position);
``` 
Adds a new line before "Position"

## DELETE FUCTION 

``` C
int delLine(char*** LineBuffer, size_t* Lines, int Del);
``` 
Delete the "Del" line
