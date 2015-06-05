#ifndef TEXT2D_HPP
#define TEXT2D_HPP

void initText2D(const char * texturePath);
int printText2D(const char * text, int x, int y, int size);
void drawText(int nVertices);
void cleanupText2D();

#endif