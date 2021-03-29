#ifndef TEXTED_PRINT_H
#define TEXTED_PRINT_H

// Load a file into RAM
char* load(char* Filename);

// Print the whole file
void ed_print(LineBuffer_s* linebuff, int LineNum);

// Print file permissions
int ed_print_permissions(const char* Filename);

#endif // TEXTED_PRINT_H
