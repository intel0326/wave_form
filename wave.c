/*
gcc -o wave wave.c
./wave ImpulseResponse.raw ImpulseResponse_wave.dat

出力
ImpulseResponse_wave.datが出力
これをgnuplotで波形表示

gnuplot
 plot "ImpulseResponse_wave.dat" w line linecolor rgb "#F80606"
*/

/*
注意

音ファイルのbit数に合わせて、short型、double型を変更するのを忘れずに
70行付近のfprintf内の型にも注意
*/

#include<stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>

#define bit 16 //ビット数
#define sample_L 0.0208333 //標本間隔(tミリ秒)

int main(int argc, char *argv[]){
  /* argv[0]はプログラム名 */
  /* argv[1]は第一引数名で入力するファイル名 */
  /* argv[2]は第二引数名で出力するファイル名 */

  FILE *fp;
  FILE *fp2;
  int i;
  int sample;
  //標本数を保存する変数
  double time = 0.0;
  //時刻を保存する変数
  short *wave;
  //動的メモリ確保に用いられる変数
  struct stat filesize; 
  //stat関数を使用する際にファイルの状態を保存するために使われる構造体
  int byte = 0;
  //バイトの計算用変数
  
  //バイトの計算
  byte = bit/8;



  //バイナリ形式で読み込み
  if((fp = fopen( argv[1], "rb" ))==NULL)
    {
      printf("ファイルを開くことができません\n");
      exit(1);
    }
  if((fp2 = fopen( argv[2], "w" ))==NULL)
    {
      printf("ファイルを開くことができません\n");
      exit(1);
    }
   
  stat(argv[1], &filesize);
  //ファイルの状態をstatによって構造体に保存
  //http://www.c-lang.net/stat/index.html
  sample = filesize.st_size / byte;
  //printf("size=%d\n", sample*byte);
  //2バイトで1標本なので「.st_size」を使い標本数を求める
  wave = (short *)malloc(filesize.st_size);
  //標本数よりも多く配列を確保
  fread( wave, byte, filesize.st_size, fp );

  //標本数分出力
  for(i=0; i<sample; i++)
    {
      fprintf(fp2, "%f\t%d\n", time, wave[i]);
      time = time + sample_L;
    }

  free(wave);

  fclose(fp);
  fclose(fp2);
  
  return 0;
}
