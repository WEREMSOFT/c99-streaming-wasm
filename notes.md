#define MSF_GIF_IMPL
#include <msf_gif.h>

Image img = GetScreenData();
int centisecondsPerFrame = 5, bitDepth = 15;
MsfGifState gifState = { 0 };
msf_gif_begin(&gifState, "example.gif", img.width, img.height);
msf_gif_frame(&gifState, img.data, bitDepth, centisecondsPerFrame, img.width * 4, false); //frame 1
msf_gif_end(&gifState);

position startpos moves d2d4 d7d5 b1c3 c7c6 e2e3 b8d7 f2f4 e7e6 g1f3 g8f6 h2h3 f8b4 a2a3 b4c3 b2c3 d8a5 c1d2 a5a4 d1b1 f6e4 f1d3 e4d2 e1d2 c6c5 f3g5 c5c4 d3e2 a4c6 b1b4 a7a5 b4b2 a8a6 a1b1 g5h7



Use time() and localtime() to get the time:

#include <stdio.h>
#include <time.h>

int main()
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

main() {
   // Calculate the time taken by take_enter()
   clock_t t;
   t = clock();
   printf("Timer starts\n");
   take_enter();
   printf("Timer ends \n");
   t = clock() - t;
   double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
   printf("The program took %f seconds to execute", time_taken);
}