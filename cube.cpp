#include <cmath>
#include <cstring>
#include <iostream>

float A = 0, B = 0, C = 0;
float x, y, z;

float cubeWidth = 10;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = ' ';
int distanceFromCam = 60;

float incrementSpeed = 0.6f;
float ooz;
int xp, yp;
int K1 = 40;

int idx;

float calculateX(int i, int j, int k) {
  return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
         j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
  return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
         j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
         i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
  return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, char ch) {
  x = calculateX(cubeX, cubeY, cubeZ);
  y = calculateY(cubeX, cubeY, cubeZ);
  z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

  ooz = 1 / z;

  xp = static_cast<int>(width / 2 + K1 * ooz * x * 2);
  yp = static_cast<int>(height / 2 + K1 * ooz * y);

  idx = xp + yp * width;

  if (idx >= 0 && idx < width * height) {
    if (ooz > zBuffer[idx]) {
      zBuffer[idx] = ooz;
      buffer[idx] = ch;
    }
  }
}

int main() {
  std::cout << "\x1b[2J";

  while (true) {
    std::memset(buffer, backgroundASCIICode, width * height);
    std::memset(zBuffer, 0, sizeof(zBuffer));

    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth;
           cubeY += incrementSpeed) {
        calculateForSurface(cubeX, cubeY, -cubeWidth, '#');
        calculateForSurface(cubeWidth, cubeY, cubeX, '$');
        calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
        calculateForSurface(-cubeX, cubeY, cubeWidth, '@');
        calculateForSurface(cubeX, -cubeWidth, -cubeY, '=');
        calculateForSurface(cubeX, cubeWidth, cubeY, '+');
      }
    }

    std::cout << "\x1b[H"; // Reset cursor to top-left
    for (int k = 0; k < width * height; k++) {
      if (k % width)
        std::cout << buffer[k];
      else
        std::cout << std::endl;
    }

    A += 0.5f;
    B += 0.5f;
  }

  return 0;
}