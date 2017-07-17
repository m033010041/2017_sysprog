# 2017 暑期系統軟體課程台北場(課前測驗題)

###### tags: `OS` `system prog`
---
題目詳細內容：[課前測驗題](https://hackmd.io/s/B1wb8CyHb)

程式內容(Github)：[請點我](https://github.com/m033010041/2017_sysprog)


上班之餘，看到了 jserv 的軟體課程，花了一點時間看看。以下是我寫程式途中所的開發筆記與成果

### Question 01

以下是題目給予的程式碼：

```clike=    
#include <stdint.h>
uint32_t func(uint32_t x) {
    uint32_t n = x;
    n = ((n & 0xffff0000) >> 16) | ((n & 0x0000ffff) << 16);
    n = ((n & 0xff00ff00) >>  8) | ((n & 0x00ff00ff) <<  8);
    n = ((n & 0xf0f0f0f0) >>  4) | ((n & 0x0f0f0f0f) <<  4);
    n = ((n & 0xcccccccc) >>  2) | ((n & 0x33333333) <<  2);
    n = ((n & 0xaaaaaaaa) >>  1) | ((n & 0x55555555) <<  1);
    return n;
}
```

#### 我的解答：

仔細觀察後可以發現這個 function 會將數值 x 的 bit 從大範圍至小範圍進行交換，這種算是有效率的 bit reverse algorithm

bit reverse algorithm 最常用在快速傅立葉轉換 (FFT) 中，裡面有一個重要的步驟，就是透過 butterfly 網路進行頻率降取樣 (decimation-in-frequency) 的時候，需要變換輸入點的編號 (order)

![8 point FFT](https://i.imgur.com/KFqpUCV.gif)

以下是透過 for/while 所改寫的 bit reverse function

```clike=
#include <stdint.h>
uint32_t func1(uint32_t x) {
    uint32_t r = x & 1;
    uint32_t shift = sizeof(x) * 8 - 1;
    
    for(x = x >> 1; x; x>>=1)
    {   
        r = r << 1;
	r = r | (x & 1); 
	shift--;
    }   
    r <<= shift;
    return r;
    }
```

這個方式就是先觀察 x 是否為 0 ，若不為 0 就進行右移，存入 r 中後進行左移，當 x = 0 時看還剩下多少需要左移的數量一次移完，這種寫法的好處是我 16 bit 的做法只要把 input/output 的宣告改掉即可。

```clike=
#include <stdint.h>
uint16_t func2(uint16_t x) {
    uint16_t r = x & 1;
    uint16_t shift = sizeof(x) * 8 - 1;
    
    for(x = x >> 1; x; x>>=1)
    {   
        r = r << 1;
	r = r | (x & 1); 
	shift--;
    }   
    r <<= shift;
    return r;
}
```

#### 參考資料

[快速傅立葉轉換：bit reverse](https://zh.wikipedia.org/wiki/%E5%BF%AB%E9%80%9F%E5%82%85%E9%87%8C%E5%8F%B6%E5%8F%98%E6%8D%A2#.E7.AE.97.E6.B3.95.E5.AE.9E.E7.8E.B0)
[fast bit reversal algorithm](http://www.idi.ntnu.no/~elster/pubs/elster-bit-rev-1989.pdf)



### Question 02

乘法器實作，其實看的範例的乘法器，那個 half_add()，似乎是全加器欸？

因為 `return half_add(sum, carry);` 補進去之後就變成了 ripple-carry adder 了

剛開始我的想法是，給定一個被乘數 a 和乘數 b，表示 a 會 ***被加 b 次*** ，由這個簡單的想法我可以把 code 寫成

```clike=
uint32_t mul32(uint32_t a, uint32_t b)
{
    uint32_t result=0, i;
    uint32_t j=0;
    for(i=b; b; b>>=1)
    {   
        if(b&1)  //if bit i is 1
	result = half_add(result, (a<<j));
	j++;
    }   
    return result;
}
```

但這樣似乎不符合 recurive 的規定，因此修改了程式

```clike=
uint32_t mul32_recursive(uint32_t a, uint32_t b)
{
    if(b == 0)
	return 0;
    else
	return half_add(((b&1)?a:0), (mul32_recursive(a, b>>1)<< 1));


### Question 05

討論以下程式碼

```clike=
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>                                                           
#include <unistd.h>

int main() {
    for (int i = 0; i < 3; i++) {
        fork();
	printf("-");
    }
    wait(NULL); wait(NULL); wait(NULL);
    return 0;
    }
```

如果將這個程式丟進 linux 執行後我們可以得到輸出的數值為 24，這個到底是什麼原因呢？理論上，fork() 執行 N 次，所產生的 process 數目應為 2^N^ - 1 個，因此此程式執行完後總共有 8 個 process (包含 main 程序)。

接下來我們討論 printf 的處理方式，printf 主要會 call 
			     ```clike
`fprintf(stdout, "...");`
			     ```
但是在 [這篇文章](http://fanli7.net/a/bianchengyuyan/_NET/20120831/215063.html) 中提到，fork()執行時會先把資料丟到 stdout 的緩衝區，但是不會立即的觸發輸出，這個可能就是為什麼會輸出 24 的原因了

理論上所顯示的數目為 14 個，由下圖的分析可得知

![](https://i.imgur.com/rpUw9JB.png)

因為 fork() 之後子程序會保留當下父程序的狀態，因此可以獲得 14 個 "-" 符號，但是由於不會立即觸發 stdout 緩衝區，因此在 fork() 的當下其實 stdout 緩衝區的內容也一併被複製到子程序了，如下圖

![](https://i.imgur.com/936j2Od.png)

因此輸出時會得到 24 個 "-" 符號

#### 參考資料

[linux中fork（）函數詳解（原創！！實例講解）（轉載）](http://fanli7.net/a/bianchengyuyan/_NET/20120831/215063.html)

### Release Note

三四題之後會在此文章補上

comment
>This is Gugeegee.[name=劉安庭][time=Sun, Jul 16, 2017 6:23 PM][color=#def29d]
>GitHub: https://github.com/m033010041/2017_sysprog

