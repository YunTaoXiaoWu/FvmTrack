//轨道编辑器.cpp|轨道执行器.cpp|头像截取.cpp|卡槽截取.cpp|截图识别.cpp共用头文件
//1. 窗口、颜色数组、图片文件的相互拷贝
//2. 字符串的分割、截断
//3. 文件搜索和目录创建
//4. 窗口创建及相关绘图操作
//5. 大厅、flash窗口及选服窗口查找
#pragma once
#include <io.h>       //使用_access函数
#include <direct.h>   //获取和修改工作目录
#include <time.h>     //获取系统时间
#include <Windows.h>  //Windows API
#include <atlimage.h> //保存截图（CImage类）
#include <graphics.h> //ExsyX库
#include <windowsx.h> //组合框控件操作宏

const char *const version = "v4.3";//版本号
const int zero = 0;//整数的0
const int maxPath = 260;//最大文件路径长度
const int gameWidth = 950, gameHeight = 596;//游戏窗口尺寸
const int gridWidth = 60, gridHeight = 64;//地图格子尺寸
const int slotTemplateWidth = gridWidth - 5, slotTemplateHeight = gridHeight - 18;//卡片模板宽高（截去一些距离）
const int maxPlantTimes = 480;//同一张卡片的最大放置次数
const int maxLineSize = maxPlantTimes * 9 + 100;//轨道文件一行的最大长度

//卡片、图像识别参数范围
const int minOffsetX = -10, maxOffsetX = 10;
const int minOffsetY = -10, maxOffsetY = 10;
const int minGridWidth = 40, maxGridWidth = 60;
const int minGridHeight = 44, maxGridHeight = 64;
const int minRequiredSimilarity = 10, maxRequiredSimilarity = 500;

int DPI = 96; //DPI缩放，用于校正点击位置
int area;//记录最近一次点击的区域

//从0xbbggrr格式的颜色中提取RGB分量
#define bgrRValue(rgb)      (LOBYTE((rgb)>>16))
#define bgrGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define bgrBValue(rgb)      (LOBYTE(rgb))

//a的平方
int Sqr(int a)
{
  return a * a;
}
//获取字体大小
int GetFontSize()
{
  LOGFONT font;
  gettextstyle(&font);
  return font.lfHeight;
}
//设置字体大小
void SetFontSize(int size)
{
  LOGFONT font;
  gettextstyle(&font);
  font.lfHeight = size;
  settextstyle(&font);
}
//给定中心坐标(x,y)显示文字s
void CenterView(const char *s, int x, int y)
{
  outtextxy(x - textwidth(s) / 2, y - textheight(s) / 2, s);
}
//给定中心坐标(x,y)显示数字a
void CenterView(int a, int x, int y)
{
  char str[20];
  sprintf_s(str, "%d", a);
  CenterView(str, x, y);
}
//把UTF8原位转成ANSI
void Utf8ToAnsi(char *Utf8Str)
{
  /*UTF-8转UTF-16*/
  int wideSize = MultiByteToWideChar(CP_UTF8, 0, Utf8Str, -1, NULL, 0);//计算宽字符串长度
  WCHAR *wideStr = (WCHAR *)malloc(wideSize * sizeof(WCHAR));//分配宽字符数组
  MultiByteToWideChar(CP_UTF8, 0, Utf8Str, -1, wideStr, wideSize);//UTF-8转UTF-16

  /*UTF-16转ANSI*/
  int ansiSize = WideCharToMultiByte(CP_ACP, 0, wideStr, -1, NULL, 0, NULL, NULL);//计算需要的ANSI字符串大小
  char *ansiStr = (char *)malloc(ansiSize * sizeof(char));//分配ANSI字符串数组
  WideCharToMultiByte(CP_ACP, 0, wideStr, -1, ansiStr, ansiSize, NULL, NULL);//UTF-16转ANSI

  strcpy_s(Utf8Str, strlen(ansiStr) + 1, ansiStr);//输出ANSI字符串
  free(wideStr);//释放内存
  free(ansiStr);//释放内存
}
//判断字符串是不是UTF8格式（如果符合UTF8编码但不可转换为ANSI，依然返回“否”）
template<size_t size>
bool IsUtf8(const char(&str)[size])
{
  //UTF首字节记录该字符用几字节表示：110xx=2字节，1110xx=3字节，...，1111110x=6字节
  //后续每个字节以10开头

  size_t length = strlen(str);//字符串长度
  int nBytes = 0;//UTF8可用1-6个字节编码,ASCII用一个字节
  unsigned char ch = 0;
  bool isAllAscii = true;//是否全部为0-127号ASCII
  for (size_t i = 0; i < length; i++)
  {
    ch = str[i];//拷贝第i个字符串
    if (ch & 0x80)//如果某字节最高位是1，说明不全是0-127号ASCII
      isAllAscii = false;
    if (nBytes == 0)//如果是UTF的首字节
    {
      if (ch & 0x80)//如果UTF首字节最高位是1，说明这是一个多字节字符
      {
        while (ch & 0x80)//左移1位，继续判断最高位是不是1，直到不是为止
        {
          ch = ch << 1;
          nBytes++;//nBytes得到总字节数，10xx=1,110xx=2,1110xx=3
        }
        if (nBytes < 2 || nBytes > 6)//总字节数必须是2-6，否则说明不是UTF8
          return false;
        nBytes--;//nBytes转化为后续字节数
      }
    }
    else //如果是UTF的后续字节
    {
      if ((ch & 0xc0) != 0x80)//后续字节必须以10开头，否则说明不是UTF8
        return false;
      nBytes--;
    }
  }

  if (isAllAscii)//全是ascii，视为ANSI格式
    return false;
  if (nBytes != 0)//不符合UTF-8编码，视为ANSI格式
    return false;

  //符合UTF-8编码也可能是ANSI格式，试试能不能转化为ANSI
  size_t len = strlen(str);
  char *str2 = (char *)malloc(len + 1);//开辟空间
  strcpy_s(str2, len + 1, str);//将str复制到str2
  //将str2中的'?'改为'0'
  for (size_t i = 0; i < len; i++)
    if (str2[i] == '?')
      str2[i] = '0';
  Utf8ToAnsi(str2);//str2转换为ANSI

  //如果转换后出现了'?'，说明不能转换为ANSI，判断为ANSI格式；没有出现'?'则判断为UTF-8格式
  bool result = strchr(str2, '?') == 0;
  free(str2);
  return result;
}
//从文件f中读取一行ANSI字符串s（UTF8自动转成ANSI），换行符不记入s，到达文件末尾会读取空串
template <size_t size>
void fgets(FILE *f, char(&s)[size])
{
  s[0] = 0;//初始值设为空串
  fgets(s, size, f);//从文件中读取1000个字符，如果读取失败，则不改变s（即s仍为空串）
  size_t n = strlen(s);
  if (s[n - 1] == '\n') //删除换行符
    s[n - 1] = 0;
  if (IsUtf8(s)) //如果是UTF8，转化为ANSI
    Utf8ToAnsi(s);
}
//向文件f中写入字符串s并添加换行符
void fputs(FILE *f, const char *s)
{
  fputs(s, f);
  fputs("\n", f);
}
//点击鼠标继续
void ClickPause()
{
  ExMessage m;
  while (!(peekmessage(&m, EX_MOUSE) && m.message == WM_LBUTTONDOWN))
    Sleep(1);
}
//创建多级目录
void CreatePath(const char *path)
{
  char multiPath[maxPath];//多级目录
  strcpy_s(multiPath, path);
  char *slash = strchr(multiPath, '\\');
  while (slash)
  {
    *slash = 0;//斜杠改成0
    CreateDirectoryA(multiPath, nullptr);
    *slash = '\\';
    slash = strchr(slash + 1, '\\');
  }
  CreateDirectoryA(multiPath, nullptr);
}
//判断字符串是不是自然数
bool IsNature(const char *s)
{
  return strlen(s) == strspn(s, "0123456789");
}
//字符串转自然数，不是自然数返回-1
int StrToNat(char *s)
{
  return IsNature(s) ? atoi(s) : -1;
}
//从time_t变量获得“yyyy/mm/dd hh:mm:ss”格式的时间字符串
template<size_t size>
void GetTimeString(char(&timeString)[size], time_t time)
{
  tm local;//本地时间
  localtime_s(&local, &time);//time_t转化为本地时间
  sprintf_s(timeString, "%d/%02d/%02d %02d:%02d:%02d",
    1900 + local.tm_year, 1 + local.tm_mon, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
}
//秒数（second）转时分秒hh:mm:ss（timeString）
template<size_t size>
bool SecondToHms(char(&timeString)[size], int second)
{
  if (second < 0 || second >= 86400)
    return false;
  sprintf_s(timeString, "%02d:%02d:%02d", second / 3600, second % 3600 / 60, second % 60);
  return true;
}
//时分秒hh:mm:ss（timeString）转秒数（second），格式必须严格匹配，转换失败返回-1
int HmsToSecond_Strict(const char *timeString)
{
  //要求长度为8位，第2/5位为冒号
  if (strlen(timeString) != 8 || timeString[2] != ':' || timeString[5] != ':')
    return -1;
  char tempTimeString[9];
  strcpy_s(tempTimeString, timeString);
  tempTimeString[2] = tempTimeString[5] = 0;
  int hh = StrToNat(tempTimeString);
  int mm = StrToNat(tempTimeString + 3);
  int ss = StrToNat(tempTimeString + 6);
  if (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59 && ss >= 0 && ss <= 59)
    return hh * 3600 + mm * 60 + ss;
  return -1;
}
//时分秒hh:mm:ss（timeString）转秒数（second），可以没有ss，转换失败返回-1
int HmsToSecond(const char *timeString)
{
  int len = strlen(timeString);
  //如果长度为0，或者首尾有冒号，或者有连续的冒号，都不行
  if (len == 0 || timeString[0] == ':' || timeString[len - 1] == ':' || strstr(timeString, "::"))
    return -1;
  //至此timeString只能是a:b:c:d格式，冒号数量为0-n
  int colonsNum = 0;//冒号数量
  char tempTimeString[9];
  strcpy_s(tempTimeString, timeString);//拷贝timeString
  char *colons = strchr(tempTimeString, ':');//冒号的位置
  char *dataLoc[3] = { tempTimeString };//数据位置，第一个设为开头
  while (colons) //如果能搜到
  {
    colons[0] = 0;//冒号位置设为空串
    colonsNum++;
    if (colonsNum > 2) //超过两个冒号，不合格
      return -1;
    dataLoc[colonsNum] = colons + 1;
    colons = strchr(dataLoc[colonsNum], ':');//搜索下一个冒号
  }
  if (colonsNum == 0) //没有冒号也不合格
    return -1;
  else if (colonsNum == 1) //1个冒号，判定为时分格式
  {
    int hh = StrToNat(dataLoc[0]);
    int mm = StrToNat(dataLoc[1]);
    if (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59)
      return hh * 3600 + mm * 60;
  }
  else if (colonsNum == 2)//2个冒号，判定为时分秒格式
  {
    int hh = StrToNat(dataLoc[0]);
    int mm = StrToNat(dataLoc[1]);
    int ss = StrToNat(dataLoc[2]);
    if (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59 && ss >= 0 && ss <= 59)
      return hh * 3600 + mm * 60 + ss;
  }
  return -1;
}
//从time_t获得“yyyy.mm.dd-hh.mm.ss”字符串，用于文件夹命名
template<size_t size>
void GetTimeStringForFolder(char(&s)[size], time_t time)
{
  tm local;
  localtime_s(&local, &time);//time_t转化为本地时间
  sprintf_s(s, "%d.%02d.%02d-%02d.%02d.%02d",
    1900 + local.tm_year, 1 + local.tm_mon, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
}
//比较两个文档是否相同。其中一个文档不存在视为不相同
bool IsFileEqual(const char *file1, const char *file2)
{
  FILE *f1, *f2;
  size_t size1, size2;
  unsigned char buffer1[1024], buffer2[1024];
  fopen_s(&f1, file1, "rb+");
  fopen_s(&f2, file2, "rb+");
  bool isTheSame = false;
  if (!f1)
    goto end;
  if (!f2)
    goto end;
  while (true)
  {
    //每次读取1024字节
    size1 = fread(buffer1, 1, 1024, f1);
    size2 = fread(buffer2, 1, 1024, f2);
    if (size1 != size2)
      goto end;
    if (size1 < 1)
      break;
    for (size_t i = 0; i < size1; i++)
      if (buffer1[i] != buffer2[i])
        goto end;
  }
  isTheSame = true;
end:
  if (f1)
    fclose(f1);
  if (f2)
    fclose(f2);
  return isTheSame;
}
//将多个连续的空格或Tab原位改为一个Tab
template<size_t size>
void RemoveBlank(char(&source)[size])
{
  char dest[size] = {};
  int nDest = 0;//dest字符串当前位置
  bool isBlankBegin = false;//是否已进入source的空格区
  //遍历source字符串
  for (int nSource = 0; nSource < (int)strlen(source); nSource++)
  {
    //对于source中的空格或tab
    if (source[nSource] == ' ' || source[nSource] == '\t')
    {
      //遇到首个空格，往dest中增加一个'\t'
      if (!isBlankBegin)
      {
        dest[nDest++] = '\t';
        isBlankBegin = true;
      }
      //后续空格不予处理
    }
    else //对于source中的非空格内容，照搬到dest
    {
      dest[nDest++] = source[nSource];
      isBlankBegin = false;
    }
  }
  strcpy_s(source, dest);//将dest拷贝到source
}
//将color数组从(x0,y0)开始、宽高为picWidth,picHeight的区域保存为24位bmp文件path
template <size_t width, size_t height>
void ColorToBitmap(COLORREF(&color)[height][width], const char *path
  , int x0 = 0, int y0 = 0, int picWidth = 0, int picHeight = 0)
{
  FILE *f;
  fopen_s(&f, path, "wb");
  fprintf(f, "BM");
  if (picWidth == 0)
    picWidth = width;
  if (picHeight == 0)
    picHeight = height;
  int size = (picWidth * 3 + picWidth % 4) * picHeight + 54;
  for (int i = 1; i <= 4; i++)
  {
    fprintf(f, "%c", size % 256);
    size = size / 256;
  }
  fprintf(f, "%c%c%c%c", 0, 0, 0, 0);
  fprintf(f, "%c%c%c%c", 54, 0, 0, 0);
  fprintf(f, "%c%c%c%c", 40, 0, 0, 0);
  size = picWidth;
  for (int i = 1; i <= 4; i++)
  {
    fprintf(f, "%c", size % 256);
    size = size / 256;
  }
  size = picHeight;
  for (int i = 1; i <= 4; i++)
  {
    fprintf(f, "%c", size % 256);
    size = size / 256;
  }
  fprintf(f, "%c%c%c%c", 1, 0, 24, 0);
  fprintf(f, "%c%c%c%c", 0, 0, 0, 0);
  fprintf(f, "%c%c%c%c", 0, 0, 0, 0);
  fprintf(f, "%c%c%c%c", 196, 14, 0, 0);
  fprintf(f, "%c%c%c%c", 196, 14, 0, 0);
  fprintf(f, "%c%c%c%c", 0, 0, 0, 0);
  fprintf(f, "%c%c%c%c", 0, 0, 0, 0);

  for (int y = picHeight - 1; y >= 0; y--)
  {
    for (int x = 0; x < picWidth; x++)
    {
      fprintf(f, "%c", bgrBValue(color[y0 + y][x0 + x]));
      fprintf(f, "%c", bgrGValue(color[y0 + y][x0 + x]));
      fprintf(f, "%c", bgrRValue(color[y0 + y][x0 + x]));
    }
    for (int i = 1; i <= picWidth % 4; i++)
      fprintf(f, "%c", 0);
  }
  fclose(f);
}
//获得窗口尺寸（大厅、选服窗口要缩放，游戏窗口固定950*596）
bool GetWindowSize(HWND hWnd, int *pWidth, int *pHeight)
{
  if (!IsWindow(hWnd))
    return false;
  RECT WindowRect;
  GetWindowRect(hWnd, &WindowRect);//获取窗口尺寸
  *pWidth = (WindowRect.right - WindowRect.left) * 96 / DPI;//获取窗口尺寸的宽高
  *pHeight = (WindowRect.bottom - WindowRect.top) * 96 / DPI;
  char ClassName[256];
  GetClassNameA(hWnd, ClassName, 256);//获取窗口类名
  //游戏窗口尺寸固定为950x596
  if (strcmp(ClassName, "NativeWindowClass") == 0 || strcmp(ClassName, "WebPluginView") == 0)
  {
    *pWidth = gameWidth;
    *pHeight = gameHeight;
  }
  return true;
}
//将窗口hWnd从(x0,y0)开始cx*cy的区域保存为图片文件path
bool WindowToBitmap(HWND hWnd, const char *path, int x0 = 0, int y0 = 0, int cx = MAXINT, int cy = MAXINT, int times = 1)
{
  if (!IsWindowVisible(hWnd))
    return false;
  RECT ScreenRect, DesktopRect;
  GetWindowRect(hWnd, &ScreenRect);//获取窗口尺寸
  GetWindowRect(GetDesktopWindow(), &DesktopRect);//获取桌面尺寸
  int width = (ScreenRect.right - ScreenRect.left) * 96 / DPI;//获取窗口尺寸的宽高
  int height = (ScreenRect.bottom - ScreenRect.top) * 96 / DPI;
  char ClassName[256];
  GetClassName(hWnd, ClassName, 256);//获取窗口类名
  if (strcmp(ClassName, "NativeWindowClass") == 0 || strcmp(ClassName, "WebPluginView") == 0)
  {
    width = 950;
    height = 596;
  }
  HDC hScreenDC = GetDC(hWnd);
  HDC hMemDC = CreateCompatibleDC(hScreenDC);//创建内存DC
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);//创建位图
  SelectObject(hMemDC, hBitmap);//将位图选择到内存DC
  if (ScreenRect.left >= 0 && ScreenRect.right <= DesktopRect.right &&
    ScreenRect.top >= 0 && ScreenRect.bottom <= DesktopRect.bottom)//如果屏幕完全在桌面内
    BitBlt(hMemDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);//将屏幕内容复制到内存DC
  else
  {
    for (int i = 0; i < times; i++)
      PrintWindow(hWnd, hMemDC, NULL);//将窗口内容复制到内存DC
    InvalidateRect(hWnd, NULL, false);//重画
  }

  CImage image;
  if (x0 == 0 && y0 == 0 && cx == MAXINT && cy == MAXINT)
  {
    image.Attach(hBitmap);
    image.Save(path);
  }
  else
  {
    HDC hMemDC2 = CreateCompatibleDC(hScreenDC);//创建内存DC
    HBITMAP hBitmap2 = CreateCompatibleBitmap(hScreenDC, cx, cy);//创建位图
    SelectObject(hMemDC2, hBitmap2);//将位图选择到内存DC
    BitBlt(hMemDC2, 0, 0, cx, cy, hMemDC, x0, y0, SRCCOPY);
    image.Attach(hBitmap2);
    image.Save(path);
    DeleteObject(hBitmap2);
    DeleteDC(hMemDC2);
  }

  DeleteObject(hBitmap);
  DeleteDC(hMemDC);
  ReleaseDC(NULL, hScreenDC);
  return true;
}
//获取bmp尺寸
int GetBitmapRect(char *path, int *pWidth = NULL, int *pHeight = NULL)
{
  if (_access(path, 0) != 0)
    return 0;
  HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (!hBitmap)
    return 0;
  BITMAP bmp = {};
  GetObject(hBitmap, sizeof(BITMAP), &bmp);

  if (pWidth)
    *pWidth = bmp.bmWidth;
  if (pHeight)
    *pHeight = bmp.bmHeight;

  DeleteObject(hBitmap);
  return 1;
}
//将图片path读入数组color，记录图片尺寸
template <size_t width, size_t height>
bool BitmapToColor(const char *path, COLORREF(&color)[height][width], int *pBmpWidth = nullptr, int *pBmpHeight = nullptr)
{
  if (_access(path, 0) != 0)
    return false;
  HBITMAP hBitmap = (HBITMAP)LoadImageA(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (!hBitmap)
    return false;
  BITMAP bmp = {};
  GetObject(hBitmap, sizeof(BITMAP), &bmp);//获取bmp尺寸
  if (pBmpWidth)
    *pBmpWidth = bmp.bmWidth;
  if (pBmpHeight)
    *pBmpHeight = bmp.bmHeight;
  if (bmp.bmWidth > width || bmp.bmHeight > height) //图片尺寸超过数组尺寸，返回0
    return false;

  HDC hScreenDC = GetDC(NULL);
  HDC hMemDC = CreateCompatibleDC(hScreenDC);
  SelectObject(hMemDC, hBitmap);

  BITMAPINFO bmi = { 0 };
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//设置BITMAPINFO结构
  bmi.bmiHeader.biWidth = bmp.bmWidth;
  bmi.bmiHeader.biHeight = -bmp.bmHeight;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = bmp.bmBitsPixel; //色深

  GetDIBits(hMemDC, hBitmap, 0, bmp.bmHeight, color, &bmi, DIB_RGB_COLORS);//获取位图的像素数据
  if (bmp.bmWidth < width)//如果图片宽度小于数组宽度，需要进行移动并删除最高字节
  {
    for (int y = bmp.bmHeight - 1; y >= 0; y--)
      for (int x = bmp.bmWidth - 1; x >= 0; x--)
      {
        int order = y * bmp.bmWidth + x;//计算图像的y行x列是第几个像素
        //找到这个像素在连续数组中的位置，移动至数组的y行x列
        color[y][x] = color[order / width][order % width] & 0x00ffffff;
      }
  }
  else //否则直接删除最高字节
  {
    for (int y = 0; y < height; y++)
      for (int x = 0; x < width; x++)
        color[y][x] &= 0x00ffffff;
  }

  DeleteObject(hBitmap);
  DeleteDC(hMemDC);
  ReleaseDC(NULL, hScreenDC);
  return true;
}
//在当前绘图窗口(xView,yView )位置显示数组color从(x0,y0)开始的部分
template <size_t width, size_t height>
void ColorToWindow(COLORREF(&color)[height][width], int xView = 0, int yView = 0,
  int x0 = 0, int y0 = 0)
{
  int imageWidth = width - x0, imageHeight = height - y0;
  IMAGE image(imageWidth, imageHeight);
  DWORD *grid = GetImageBuffer(&image);
  for (int y = 0; y < imageHeight; y++)
    for (int x = 0; x < imageWidth; x++)
      grid[y * imageWidth + x] = color[y0 + y][x0 + x];
  putimage(xView, yView, &image);
}
//获取鼠标指向的窗口句柄
HWND MousePoint()
{
  POINT pNow;
  GetCursorPos(&pNow);//获取鼠标当前位置
  return WindowFromPoint(pNow);//获取鼠标所在窗口的句柄
}
//弹窗报告最近一次错误
void ReportLastError()
{
  char message[1000];
  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), message, sizeof(message), NULL);
  MessageBox(NULL, message, "", MB_ICONINFORMATION | MB_SYSTEMMODAL);
}
//在窗口hWnd内点击，返回是否成功
BOOL LeftClick(HWND hWnd, int x, int y)
{
  BOOL result = PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
  PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
  return result;
}
//在窗口hWnd内点击，返回是否成功
BOOL RightClick(HWND hWnd, int x, int y)
{
  BOOL result = PostMessage(hWnd, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(x, y));
  PostMessage(hWnd, WM_RBUTTONUP, 0, MAKELPARAM(x, y));
  return result;
}
//在窗口hWnd内移动，返回是否成功
BOOL MouseMove(HWND hWnd, int x, int y)
{
  return PostMessage(hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(x, y));
}
//在窗口hWnd内点击，位置进行DPI换算，返回是否成功
BOOL LeftClickDPI(HWND hWnd, int x, int y)
{
  int x1 = int(double(x) * DPI / 96 + 0.5);
  int y1 = int(double(y) * DPI / 96 + 0.5);
  return LeftClick(hWnd, x1, y1);
}
//在窗口hWnd内移动，位置进行DPI换算，返回是否成功
BOOL MouseMoveDPI(HWND hWnd, int x, int y)
{
  int x1 = int(double(x) * DPI / 96 + 0.5);
  int y1 = int(double(y) * DPI / 96 + 0.5);
  return MouseMove(hWnd, x1, y1);
}
//后台模拟按键
BOOL PressKey(HWND hWnd, int KeyCode)
{
  return PostMessage(hWnd, WM_KEYDOWN, KeyCode, 0);//按键
}
//判断字符src是不是汉字首字节，pIsChineseFirst指示上一个字符是不是汉字首字节
void JudgeChineseFirst(char source, bool *pIsChineseFirst)
{
  if (*pIsChineseFirst)
    *pIsChineseFirst = false;
  else if (source < 0)//如果是汉字首字节
    *pIsChineseFirst = true;
}
//str[n]是否为汉字的第1字节
bool IsChineseFirst(const char *str, int n)
{
  if (n > (int)strlen(str))
    return false;
  bool isChineseFirst = false;
  for (int i = 0; i <= n; i++)
    JudgeChineseFirst(str[i], &isChineseFirst);
  return isChineseFirst;
}
//str[n]是否为汉字的第2字节
int IsChineseSecond(const char *str, int n)
{
  if (n > (int)strlen(str))
    return false;
  bool isChineseFirst = false;
  for (int i = 0; i <= n - 1; i++)
    JudgeChineseFirst(str[i], &isChineseFirst);
  return isChineseFirst;
}
//将字符串source按场宽fieldWidth截断并添加省略号（...），写入dest
template<size_t size>
void TruncateStringWithEllipsis(char(&dest)[size], char *source, int fieldWidth)
{
  if (textwidth(source) <= fieldWidth)//如果字符串长度不超过场宽，则无需截尾
  {
    strcpy_s(dest, source);
    return;
  }
  int length = strlen(source);
  bool isChineseFirst = false;//是不是汉字的第1个字节
  int last = 0;//上次截取的位置
  int tempSize = strlen(source) + 4;
  char *temp = (char *)malloc(tempSize);
  //用于获取字符串宽度的临时字符串
  for (int i = 0; i < length; i++)
  {
    JudgeChineseFirst(source[i], &isChineseFirst);
    if (isChineseFirst)
      continue;
    strncpy_s(temp, tempSize, source, i + 1);
    temp[i + 1] = 0;//截取前i+1个字符
    strcat_s(temp, tempSize, "...");//字符后面加3个点
    if (textwidth(temp) > fieldWidth)//如果字符串宽度超过场宽了，从这里截断
    {
      strncpy_s(dest, source, last);
      dest[last] = 0;
      strcat_s(dest, "...");//字符后面加3个点
      free(temp);
      return;
    }
    last = i + 1;//如果还没超过场宽，记录本次指针位置
  }
}
//将字符串source按场宽fieldWidth截断，写入dest
template<size_t size>
void TruncateString(char(&dest)[size], char *source, int fieldWidth)
{
  if (textwidth(source) <= fieldWidth)//如果字符串长度不超过场宽，则无需分割
  {
    strcpy_s(dest, source);
    return;
  }
  bool isChineseFirst = false;//是不是汉字的首字节
  char tempString[size + 2] = {};//用于获取字符串宽度的临时字符串
  strcpy_s(dest, "");

  for (size_t i = 0; i < strlen(source); i++) //遍历Src的每个字节
  {
    JudgeChineseFirst(source[i], &isChineseFirst);
    if (isChineseFirst)
      continue;
    strncpy_s(tempString, source, i + 1);//将Src的前i+1个字符写入tempString
    if (textwidth(tempString) > fieldWidth)//如果字符串宽度超过场宽了，从这里截断
      return;
    strcpy_s(dest, tempString);//如果还没超过场宽，将tempString写入dest
  }
}
//将字符串source按长度length截断，写入dest
template<size_t size>
void TruncateStringInLength(char(&dest)[size], char *source, int length)
{
  if ((int)strlen(source) <= length)//如果字符串长度不超过场宽，则无需分割
  {
    strcpy_s(dest, source);
    return;
  }
  bool isChineseFirst = false;//是不是汉字的首字节
  char tempString[size + 2] = {};//用于获取字符串宽度的临时字符串
  strcpy_s(dest, "");

  for (size_t i = 0; i < strlen(source); i++) //遍历Src的每个字节
  {
    JudgeChineseFirst(source[i], &isChineseFirst);
    if (isChineseFirst)
      continue;
    strncpy_s(tempString, source, i + 1);//将Src的前i+1个字符写入tempString
    if ((int)strlen(tempString) > length)//如果字符串宽度超过场宽了，从这里截断
      return;
    strcpy_s(dest, tempString);//如果还没超过场宽，将tempString写入dest
  }
}
//将字符串source按场宽fieldWidth截断，分两行写入dest1和dest2
template<size_t size1, size_t size2>
void TruncateString(const char *source, char(&dest1)[size1], char(&dest2)[size2], int fieldWidth)
{
  if (textwidth(source) <= fieldWidth)//如果字符串长度不超过场宽，则无需分割
  {
    strcpy_s(dest1, source);
    strcpy_s(dest2, "");
    return;
  }
  int length = strlen(source);
  bool isChineseFirst = false;//是不是汉字的第1个字节
  int last = 0;//上次截取的位置
  int tempSize = strlen(source) + 1;
  char temp[size1 + size2 + 10] = {};
  //用于获取字符串宽度的临时字符串
  for (int i = 0; i < length; i++)
  {
    JudgeChineseFirst(source[i], &isChineseFirst);
    if (isChineseFirst)
      continue;
    strncpy_s(temp, tempSize, source, i + 1);
    temp[i + 1] = 0;//截取前i+1个字符
    if (textwidth(temp) > fieldWidth)//如果字符串宽度超过场宽了，从这里截断
    {
      strncpy_s(dest1, source, last);
      dest1[last] = 0;
      strcpy_s(dest2, source + last);
      return;
    }
    last = i + 1;//如果还没超过场宽，记录本次指针位置
  }
}
//给定中心点坐标(x,y)和半边长画矩形
void CenterRectangle(int x, int y, int dx, int dy)
{
  line(x - dx, y - dy, x - dx, y + dy);
  line(x + dx, y - dy, x + dx, y + dy);
  line(x - dx, y - dy, x + dx, y - dy);
  line(x - dx, y + dy, x + dx, y + dy);
}
//位图map从(x0,y0)开始、color1从(x1,y1)开始截取cx*cy区域比较，相等返回1，不相等返回0
template <size_t width0, size_t height0, size_t width1, size_t height1>
bool IsBitmapEqual(COLORREF(&color0)[height0][width0], COLORREF(&color1)[height1][width1],
  int cx, int cy, int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0, COLORREF specificColor = 0)
{
  if (specificColor) //只需要指定颜色匹配
  {
    for (int y = 0; y < cy; y++)
      for (int x = 0; x < cx; x++)
        if ((color0[y0 + y][x0 + x] == specificColor) != (color1[y1 + y][x1 + x] == specificColor))
          return false;
  }
  else //全色匹配
  {
    for (int y = 0; y < cy; y++)
      for (int x = 0; x < cx; x++)
        if (color0[y0 + y][x0 + x] != color1[y1 + y][x1 + x])
          return false;
  }
  return true;
}
//位图color0从(x0,y0)开始的cx*cy区域是否全部满足颜色要求requirement
int IsBitmapFit(bool requirement(COLORREF, int), COLORREF *(&color0)[4320], int platform, int cx, int cy, int x0 = 0, int y0 = 0, int step = 1)
{
  for (int x = cx - 1; x >= 0; x -= step)
    for (int y = 0; y < cy; y += step)
      if (!requirement(color0[y0 + y][x0 + x], platform))
        return 0;
  return 1;
}
//检索路径符合表达式pathExpression（例如"图片\\*.bmp"）的所有文件（夹），返回文件数量
int GetFileList(char *pathExpression, char(*fileList)[maxPath], int maxFileNum)
{
  _finddata_t fileinfo;
  int handle = _findfirst(pathExpression, &fileinfo);// 使用_findfirst函数查找文件夹中的第一个文件
  if (handle == -1)
    return 0;

  int order = 0;
  do // 遍历文件夹中的文件
  {
    if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
      continue;
    strcpy_s(fileList[order], fileinfo.name);
    order++;
  } while (_findnext(handle, &fileinfo) == 0 && order < maxFileNum);

  // 关闭文件搜索句柄
  _findclose(handle);
  return order;
}
//删除指定目录direct下具有指定后缀名extension的文件
void ClearDirect(char *direct, char *extension)
{
  char direct1[maxPath];//以斜杠结尾的目录
  strcpy_s(direct1, direct);
  if (direct[strlen(direct) - 1] != '\\') //结尾不是斜杠，就加一个斜杠
    strcat_s(direct1, "\\");
  char FilePath[maxPath];//文件路径
  char expression[maxPath];//检索表达式
  sprintf_s(expression, "%s*%s", direct1, extension);

  int handle;
  _finddata_t fileinfo;
  handle = _findfirst(expression, &fileinfo);// 使用_findfirst函数查找文件夹中的第一个文件
  if (handle == -1L)
    return;

  do // 遍历文件夹中的文件
  {
    if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
      continue;
    sprintf_s(FilePath, "%s%s", direct1, fileinfo.name);
    remove(FilePath);
  } while (_findnext(handle, &fileinfo) == 0);

  // 关闭文件搜索句柄
  _findclose(handle);
}
//判断Ansi和Unicode是否相等
bool IsAnsiAndUnicodeEqual(const char *ansiStr, const wchar_t *unicodeStr)
{
  //先把Unicode转Ansi，然后比较
  int ansiSize = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, NULL, 0, NULL, NULL);//计算ANSI编码文本所需的缓冲区大小
  char *ansiText = (char *)malloc(ansiSize);//分配ANSI编码文本的缓冲区
  WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, ansiText, ansiSize, NULL, NULL);//将UTF-16编码的文本转换为ANSI编码
  bool result = strcmp(ansiText, ansiStr) == 0;
  free(ansiText);
  return result;
}
//若剪贴板中缺少ANSI或Unicode中的一种，补全另一种
void CompleteClipboard()
{
  if (!OpenClipboard(NULL))//打开剪贴板，打不开就结束
    return;
  if (GetClipboardData(CF_TEXT) != NULL)//检查剪贴板中是否有文本，如果有
  {
    HGLOBAL ansiClipBuffer = GetClipboardData(CF_TEXT);//获取剪贴板中CF_TEXT文本的句柄
    HGLOBAL unicodeClipBuffer = GetClipboardData(CF_UNICODETEXT);//获取剪贴板中CF_UNICODETEXT文本的句柄
    char *ansiBuffer = (char *)GlobalLock(ansiClipBuffer);//锁定剪贴板Ansi文本的内存
    wchar_t *unicodeBuffer = (wchar_t *)GlobalLock(unicodeClipBuffer);//锁定剪贴板Ansi文本的内存
    if (IsAnsiAndUnicodeEqual(ansiBuffer, unicodeBuffer)) //如果两者相等，说明两者都有，直接退出
    {
      GlobalUnlock(ansiClipBuffer);//解锁剪贴板Ansi内存块
      GlobalUnlock(unicodeClipBuffer);//解锁剪贴板Unicode内存块
      CloseClipboard();//关闭剪贴板
      return;
    }
    //运行到这里，说明Ansi和Unicode不相等，必然缺少其中一种
    if (strchr(ansiBuffer, '?') != NULL)//如果AnsiBuffer中有'?'，说明剪贴板中缺少CF_TEXT文本，需补齐CF_TEXT文本
    {
      GlobalUnlock(ansiClipBuffer);//解锁剪贴板Ansi内存块
      int AnsiSize = WideCharToMultiByte(CP_ACP, 0, unicodeBuffer, -1, NULL, 0, NULL, NULL);//计算ANSI编码文本所需的缓冲区大小
      char *AnsiText = (char *)malloc(AnsiSize * sizeof(char));//分配ANSI编码文本的缓冲区
      WideCharToMultiByte(CP_ACP, 0, unicodeBuffer, -1, AnsiText, AnsiSize, NULL, NULL);//将UTF-16编码的文本转换为ANSI编码
      GlobalUnlock(unicodeBuffer);//解锁剪贴板Unicode内存块
      //Unicode文本已转换为CF_TEXT文本存入AnsiText，接下来把AnsiText写入剪贴板
      HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, (AnsiSize + 1) * sizeof(char));//分配用于储存写入剪贴板内容的空间
      char *buffer = (char *)GlobalLock(hMem);//锁定全局内存对象并获取指向其数据的指针
      strcpy_s(buffer, AnsiSize + 1, AnsiText);
      GlobalUnlock(hMem);
      SetClipboardData(CF_TEXT, hMem);//向剪贴板添加ANSI文本
      CloseClipboard();//关闭剪贴板
      free(AnsiText);//释放内存并解锁剪贴板数据
    }
    else//如果AnsiBuffer中没有'?'，说明剪贴板中缺少CF_UNICODETEXT文本，需补齐CF_UNICODETEXT文本
    {
      GlobalUnlock(unicodeBuffer);//解锁剪贴板Unicode内存块
      int UnicodeSize = MultiByteToWideChar(CP_ACP, 0, ansiBuffer, -1, NULL, 0);
      wchar_t *UnicodeText = (wchar_t *)malloc(UnicodeSize * sizeof(wchar_t));
      MultiByteToWideChar(CP_ACP, 0, ansiBuffer, -1, UnicodeText, UnicodeSize);//Ansi转换为Unicode
      GlobalUnlock(ansiClipBuffer);//解锁剪贴板Ansi内存块
      //Ansi文本已转换为Unicode文本存入UnicodeText，接下来把UnicodeText写入剪贴板
      HGLOBAL hMem = GlobalAlloc(GMEM_DDESHARE, (UnicodeSize + 1) * sizeof(wchar_t));
      wchar_t *wbuffer = (wchar_t *)GlobalLock(hMem);
      wcscpy_s(wbuffer, UnicodeSize + 1, UnicodeText);
      GlobalUnlock(hMem);
      SetClipboardData(CF_UNICODETEXT, hMem);
      CloseClipboard();//关闭剪贴板
      free(UnicodeText);//释放内存并解锁剪贴板数据
    }
  }
  else //如果连文本都没有，直接关闭剪贴板，结束函数
  {
    CloseClipboard();
    return;
  }
}
//获取剪贴板文本
template<size_t size>
bool GetClipboardString(char(&dest)[size])
{
  CompleteClipboard();
  if (!OpenClipboard(NULL))//打开剪贴板，打不开就结束
    return false;
  HGLOBAL ansiClipBuffer = GetClipboardData(CF_TEXT);//获取剪贴板中CF_TEXT文本的句柄
  if (!ansiClipBuffer) //如果剪贴板中没有文本，关闭剪贴板
  {
    CloseClipboard();
    return false;
  }
  char *ansiBuffer = (char *)GlobalLock(ansiClipBuffer);//锁定剪贴板Ansi文本的内存
  strcpy_s(dest, ansiBuffer);
  GlobalUnlock(ansiClipBuffer);//解锁剪贴板Ansi内存块
  CloseClipboard();//关闭剪贴板
  return true;
}
//指定中心和半径，绘制水平调节按钮
void PrintHorizontalAdjustButton(int centerX, int centerY, int radium)
{
  LOGFONT font;
  int size = GetFontSize();
  SetFontSize(16);
  gettextstyle(&font);
  font.lfOrientation = 900;
  settextstyle(&font);

  CenterView("▲", centerX - radium - 4, centerY + 8);
  CenterView("▼", centerX + radium - 20, centerY + 8);

  font.lfOrientation = 0;
  settextstyle(&font);
  SetFontSize(size);
}
//指定矩形区域，绘制水平调节按钮
void PrintHorizontalAdjustButton(int x, int y, int width, int height)
{
  PrintHorizontalAdjustButton(x + width / 2, y + height / 2, width / 2);
}
//资源文件输出到磁盘
void OutputRes(char *ResName, char *ResClass, char *OutputPath)
{
  HMODULE hModule = GetModuleHandle(NULL);//获取模块句柄
  HRSRC hResource = FindResourceA(hModule, ResName, ResClass); //查找资源
  HGLOBAL hGlobal = LoadResource(hModule, hResource);//加载资源
  void *pResource = LockResource(hGlobal);//锁定资源
  DWORD resourceSize = SizeofResource(hModule, hResource);//获取资源大小
  FILE *file;
  fopen_s(&file, OutputPath, "wb");//写入到磁盘
  fwrite(pResource, 1, resourceSize, file);//写入资源数据到文件
  fclose(file);//关闭文件
  FreeResource(hGlobal);
}
//将整数num转化为字符串再输出
void outtextxy(int x, int y, int num)
{
  char str[20];
  sprintf_s(str, "%d", num);
  outtextxy(x, y, str);
}
//复制文件
void Copy(const char *sourcePath, const char *destinationPath)
{
  CopyFile(sourcePath, destinationPath, FALSE);
}
//交换两个整数
void Swap(int *pA, int *pB)
{
  int temp = *pA;
  *pA = *pB;
  *pB = temp;
}
//运行程序（返回-1=执行失败，0=执行正常，正数=执行出错）
int Execute(const char *exePath, bool hide = false, bool wait = false)
{
  STARTUPINFO si = {};
  PROCESS_INFORMATION pi = {};
  si.cb = sizeof(si);
  if (hide)
    si.wShowWindow = SW_HIDE;//隐藏窗口
  else
    si.wShowWindow = SW_SHOWNORMAL;//显示窗口
  si.dwFlags = STARTF_USESHOWWINDOW;
  if (!CreateProcess(NULL, (char *)exePath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))//创建进程
    return -1;
  DWORD exitCode = 0;
  if (wait)
  {
    WaitForSingleObject(pi.hProcess, INFINITE);
    char exitString[100] = {};
    if (!GetExitCodeProcess(pi.hProcess, &exitCode))
      exitCode = 0;
  }
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  return exitCode;
}
//将sourceDir目录下所有文件复制到destDir目录（目录不存在则创建）
int CopyDirect(const char *sourceDir, const char *destDir)
{
  if (_access(destDir, 0))
    CreatePath(destDir);
  char sourceDir1[maxPath] = {};
  strcpy_s(sourceDir1, sourceDir);
  if (sourceDir[strlen(sourceDir) - 1] != '\\')
    strcat_s(sourceDir1, "\\"); //结尾不是斜杠，就加一个斜杠

  char destDir1[maxPath] = {};
  strcpy_s(destDir1, destDir);
  if (destDir[strlen(destDir) - 1] != '\\')
    strcat_s(destDir1, "\\"); //结尾不是斜杠，就加一个斜杠

  char expression[maxPath] = {};
  sprintf_s(expression, "%s*", sourceDir1);

  // 查找目录下的所有文件
  WIN32_FIND_DATA findData;
  HANDLE hFind = FindFirstFile(expression, &findData);
  if (hFind == INVALID_HANDLE_VALUE)
    return 0;

  char sourcePath[maxPath];
  char destinationPath[maxPath];
  do
  {
    sprintf_s(sourcePath, "%s%s", sourceDir1, findData.cFileName);
    sprintf_s(destinationPath, "%s%s", destDir1, findData.cFileName);
    CopyFile(sourcePath, destinationPath, FALSE);
  } while (FindNextFile(hFind, &findData));

  FindClose(hFind);
  return 1;
}
//寻找目录direct下按名称排序最靠后的文件夹，将名称写入lastFolder
int GetLastFolder(char *direct, char(&lastFolder)[maxPath])
{
  char expression[maxPath];//检索表达式
  strcpy_s(expression, direct);
  if (direct[strlen(direct) - 1] != '\\')
    strcat_s(expression, "\\*"); //结尾不是斜杠，就加一个斜杠
  else
    strcat_s(expression, "*");

  WIN32_FIND_DATA findData;
  HANDLE hFind = FindFirstFile(expression, &findData); //查找第一个文件
  if (hFind == INVALID_HANDLE_VALUE)
    return 0;

  strcpy_s(lastFolder, "");
  do
  {
    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // 检查是否是文件夹
      if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
        strcpy_s(lastFolder, findData.cFileName); // 跳过"."和".."这两个特殊目录
  } while (FindNextFile(hFind, &findData));
  FindClose(hFind);

  if (strlen(lastFolder) > 0)
    return 1;
  return 0;
}
//找到一个小号子窗口下唯一的选服窗口
HWND GetServerWindow(HWND hWndChild)
{
  HWND hWnd1 = FindWindowEx(hWndChild, NULL, "CefBrowserWindow", NULL); //一级子窗口
  if (hWnd1 == NULL)
    return NULL;
  HWND hWnd2 = FindWindowEx(hWnd1, NULL, "Chrome_WidgetWin_0", NULL); //二级子窗口
  if (hWnd2 == NULL)
    return NULL;
  return FindWindowEx(hWnd2, NULL, "Chrome_RenderWidgetHostHWND", NULL); //三级子窗口
}
//找到游戏大厅当前显示的选服窗口，找不到返回NULL
HWND GetActiveServerWindow(HWND hWndHall)
{
  char className[256];
  GetClassName(hWndHall, className, sizeof(className));
  if (strcmp(className, "ApolloRuntimeContentWindow") == 0)
    return hWndHall; //微端的选服窗口就是微端窗口
  if (strcmp(className, "DUIWindow") != 0)
    return NULL; //不是微端也不是大厅，返回NULL
  //判断为游戏大厅窗口
  HWND hWndChild = NULL, hWndServer;
  while (true)
  {
    hWndChild = FindWindowEx(hWndHall, hWndChild, "TabContentWnd", NULL); //查找一个小号窗口
    if (hWndChild == NULL)
      return NULL;//没有小号窗口了，还没找到活跃窗口，就返回NULL
    hWndServer = GetServerWindow(hWndChild);
    if (hWndServer != NULL)
      return hWndServer;
  }
}
//找到一个小号子窗口下唯一的游戏窗口
HWND GetGameWindow(HWND hWndChild)
{
  HWND hWnd2 = FindWindowEx(hWndChild, NULL, "CefBrowserWindow", NULL); //二级子窗口
  if (hWnd2 == NULL)
    return NULL;
  HWND hWnd3 = FindWindowEx(hWnd2, NULL, "Chrome_WidgetWin_0", NULL); //三级子窗口
  if (hWnd3 == NULL)
    return NULL;
  HWND hWnd4 = FindWindowEx(hWnd3, NULL, "WrapperNativeWindowClass", NULL); //四级子窗口
  if (hWnd4 == NULL)
    return NULL;
  return FindWindowEx(hWnd4, NULL, "NativeWindowClass", NULL); //五级子窗口
}
//找到游戏大厅当前显示的游戏窗口，找不到返回NULL
HWND GetActiveGameWindow(HWND hWndHall)
{
  HWND hWndChild = FindWindowEx(hWndHall, NULL, "WebPluginView", NULL); //按微端查找小号窗口
  if (hWndChild != NULL)
    return hWndChild;//找到了直接返回游戏窗口
  while (1)
  {
    hWndChild = FindWindowEx(hWndHall, hWndChild, "TabContentWnd", NULL); //查找一个小号窗口
    if (hWndChild == NULL)
      return NULL;//没有小号窗口了，还没找到活跃窗口，就返回NULL
    HWND hWndGame = GetGameWindow(hWndChild);
    if (hWndGame != NULL)
      return hWndGame;
  }
}
//根据副大厅找主大厅
HWND GetMainHallWindow(HWND hHallWnd)
{
  HWND hWndMain;
  char className[256], title[256];
  GetClassName(hHallWnd, className, 256);//获取窗口类名
  if (strcmp(className, "DUIWindow") != zero)//如果参数不是大厅句柄，返回NULL
    return NULL;
  /*到这里说明hHallWnd是大厅了，直接获取标题*/
  GetWindowText(hHallWnd, title, 256);//获取窗口标题
  char *SubTitle;
  char *str = strstr(title, " | ");//从标题中寻找" | "
  while (1)
  {
    if (str == NULL)//找不到" | "，说明现在的大厅就是主大厅
      return hHallWnd;
    SubTitle = str + 3;//找到了则去掉" | "前面的内容（可能是小号名称），再次查找
    hWndMain = FindWindow("DUIWindow", SubTitle);
    if (hWndMain != NULL)//如果去掉一层或几层后能找到大厅，找到的就是主大厅
      return hWndMain;
    str = strstr(SubTitle, " | ");//没找到就继续寻找" | "，继续去掉
  }
}
//找到游戏窗口或选服窗口hWnd所属的大厅窗口
HWND GetHallWindow(HWND hWnd)
{
  HWND hWndParent = GetParent(hWnd);//获得父窗口
  if (hWndParent == NULL)
    return NULL;//没有父窗口，返回NULL
  char className[256];
  GetClassName(hWndParent, className, 256);//获取窗口类名
  if (strcmp(className, "ApolloRuntimeContentWindow") == zero)//如果是微端窗口，直接返回
    return hWndParent;
  if (strcmp(className, "WrapperNativeWindowClass") == zero)//如果是大厅游戏窗口的上级窗口，向上找四层
  {
    hWndParent = GetParent(hWndParent);//找到Chrome_WidgetWin_0层
    hWndParent = GetParent(hWndParent);//找到CefBrowserWindow层
    hWndParent = GetParent(hWndParent);//找到TabContentWnd层
    return GetParent(hWndParent);//找到DUIWindow层（大厅）并返回
  }
  if (strcmp(className, "Chrome_WidgetWin_0") == zero)//如果是大厅选服窗口的上级窗口，向上找三层
  {
    hWndParent = GetParent(hWndParent);//找到CefBrowserWindow层
    hWndParent = GetParent(hWndParent);//找到TabContentWnd层
    return GetParent(hWndParent);//找到DUIWindow层（大厅）并返回
  }
  return NULL;
}
//游戏窗口hWnd是否能识别图像
bool IsGameWindowVisible(HWND hWnd)
{
  return IsWindowVisible(hWnd) && !IsIconic(GetHallWindow(hWnd));//窗口可见且大厅没有最小化
}
//从选服窗口获取游戏窗口
HWND GetGameWindowFromServer(HWND hWndServer)
{
  HWND hWnd3 = GetParent(hWndServer);//获取三级子窗口Chrome_WidgetWin_0
  if (hWnd3 == NULL)
    return NULL;
  HWND hWnd4 = FindWindowEx(hWnd3, NULL, "WrapperNativeWindowClass", NULL); //四级子窗口
  if (hWnd4 == NULL)
    return NULL;
  return FindWindowEx(hWnd4, NULL, "NativeWindowClass", NULL); //五级子窗口
}
//多次调用PrintWindow直到左上角不是黑色，成功返回1，一直是黑色返回0
bool MultiPrintWindow(HWND hWnd, HDC hMemDC, int times)
{
  int counter = 0;
  while (true)
  {
    if (counter >= times || !IsGameWindowVisible(hWnd)) //如果截图times次还没成功，或窗口不可见
      return false;//截图失败
    Sleep(1);
    PrintWindow(hWnd, hMemDC, NULL);//将窗口内容复制到内存DC
    counter++;
    if (GetPixel(hMemDC, 0, 0)) //如果左上角不是黑色
      return true;//截图成功
  }
  return false;
}
//对游戏窗口的一部分进行截图
template <size_t width, size_t height>
int RegionalMapShot(HWND hWnd, COLORREF(&color)[height][width], int x0 = 0, int y0 = 0)
{
  if (!IsGameWindowVisible(hWnd))
    return 0;

  HDC hScreenDC = GetDC(hWnd);
  HDC hMemDC = CreateCompatibleDC(hScreenDC);
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, gameWidth, gameHeight);//创建位图
  SelectObject(hMemDC, hBitmap);

  int result = MultiPrintWindow(hWnd, hMemDC, 5);//截图5次直到成功，记录结果
  InvalidateRect(hWnd, NULL, false);//重画

  if (result == 1)
  {
    HDC hMemDC2 = CreateCompatibleDC(hScreenDC);//创建内存DC
    HBITMAP hBitmap2 = CreateCompatibleBitmap(hScreenDC, width, height);//创建位图
    SelectObject(hMemDC2, hBitmap2);//将位图选择到内存DC
    BitBlt(hMemDC2, 0, 0, width, height, hMemDC, x0, y0, SRCCOPY);//将窗口截图hMemDC复制到区域截图hMemDC2

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//设置BITMAPINFO结构
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -int(height);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // 32位色深
    GetDIBits(hMemDC2, hBitmap2, 0, height, color, &bmi, DIB_RGB_COLORS);//获取位图的像素数据
    for (int y = 0; y < height; y++)
      for (int x = 0; x < width; x++)
        color[y][x] &= 0x00ffffff;

    DeleteObject(hBitmap2);
    DeleteDC(hMemDC2);
  }

  DeleteObject(hBitmap);
  DeleteDC(hMemDC);
  ReleaseDC(NULL, hScreenDC);
  return result;
}
//对大厅进行截图，存入一维数组hall并返回，记录大厅宽高
int HallShot(HWND hWnd, COLORREF *&hall, int *pWidth, int *pHeight, int times)
{
  if (!IsWindowVisible(hWnd))
    return 0;

  int width = 0, height = 0;
  GetWindowSize(hWnd, &width, &height);//获取窗口宽高
  if (pWidth)
    *pWidth = width;
  if (pHeight)
    *pHeight = height;
  if (hall)//如果pHall内存还没释放，则释放内存
    free(hall);
  hall = (COLORREF *)malloc(width * height * sizeof(COLORREF));//分配内存
  if (!hall) //分配失败则返回0
    return 0;

  HDC hScreenDC = GetDC(hWnd);
  HDC hMemDC = CreateCompatibleDC(hScreenDC);
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);//创建位图
  SelectObject(hMemDC, hBitmap);

  for (int i = 0; i < times; i++)
    PrintWindow(hWnd, nullptr, NULL);//多次截图促进画面更新
  Sleep(1);
  PrintWindow(hWnd, hMemDC, NULL);//将窗口内容复制到内存DC
  InvalidateRect(hWnd, NULL, false);//重画

  BITMAPINFO bmi = { 0 };
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//设置BITMAPINFO结构
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32; // 32位色深
  GetDIBits(hMemDC, hBitmap, 0, height, hall, &bmi, DIB_RGB_COLORS);//获取位图的像素数据
  for (int i = 0; i < width * height; i++)
    hall[i] &= 0x00ffffff;

  DeleteObject(hBitmap);
  DeleteDC(hMemDC);
  ReleaseDC(NULL, hScreenDC);
  return 1;
}
// 允许WM_DROPFILES消息通过UAC过滤器
void EnableDragDropForHighIntegrity(HWND hwnd)
{
  ChangeWindowMessageFilterEx(hwnd, WM_DROPFILES, MSGFLT_ALLOW, nullptr);
  ChangeWindowMessageFilterEx(hwnd, WM_COPYDATA, MSGFLT_ALLOW, nullptr);
  ChangeWindowMessageFilterEx(hwnd, 0x0049 /* WM_COPYGLOBALDATA */, MSGFLT_ALLOW, nullptr);
}
//绘制row行column列的栅格
void PaintGrid(int row, int column, int x, int y, int width, int height)
{
  int i, j;
  for (i = 0; i <= row; i++)
    line(x, y + i * height, x + column * width, y + i * height);
  for (j = 0; j <= column; j++)
    line(x + j * width, y, x + j * width, y + row * height);
}
//弹出消息框
void PopMessage(HWND hWnd, const char *content, const char *title = "提示")
{
  MessageBox(hWnd, content, title, MB_ICONINFORMATION | MB_SYSTEMMODAL);
}
//将图像source从(x0,y0)开始的区域拷贝到dest
template <size_t destWidth, size_t destHeight, size_t sourceWidth, size_t sourceHeight>
void CopyMap(COLORREF(&dest)[destHeight][destWidth], COLORREF(&source)[sourceHeight][sourceWidth],
  int x0, int y0)
{
  for (int y = 0; y < destHeight; y++)
    for (int x = 0; x < destWidth; x++)
      dest[y][x] = source[y0 + y][x0 + x];
}
const int maxWave = 24;
const int maxSmallWave = 12;
const int maxTotalWave = 24 * 12;
//从字符串source("--第%d.%d波--")中读取大小波，格式不符返回0，波次溢出返回-1，正常读取返回1
int ScanWave(const char *source, int *pWave, int *pSmallWave)
{
  *pWave = *pSmallWave = 0;
  int result = sscanf_s(source, "--第%d.%d波--", pWave, pSmallWave);
  if (result < 1) //读不到数据，返回0
    return 0;
  //读到波次且波次在范围内，返回1
  if (*pWave >= 0 && *pWave < maxWave && *pSmallWave >= 0 && *pSmallWave < maxSmallWave)
    return 1;
  //波次不在范围内，返回-1
  return -1;
}
//将totalWave转化成"wave.smallWave"格式的字符串
template <size_t size>
void GetWaveString(char(&dest)[size], int totalWave)
{
  int wave = totalWave / maxSmallWave;
  int smallWave = totalWave % maxSmallWave;
  if (smallWave == 0)
    sprintf_s(dest, "%d", wave);
  else
    sprintf_s(dest, "%d.%d", wave, smallWave);
}
//字符串s是否为"--第x波--"格式
bool IsWaveTitle(const char *s)
{
  return strstr(s, "--第") == s && strstr(s, "波--");
}
//大小波是否均在范围内
bool IsWaveInRange(int wave, int smallWave)
{
  return wave >= 0 && wave < maxWave && smallWave >= 0 && smallWave < maxSmallWave;
}
//波次字符串m.n是否符合格式
bool IsWaveLegal(const char *waveString)
{
  if (waveString[0] == 0)
    return true;
  int wave = 0, smallWave = 0;
  char tail[10] = {};
  int waveResult = sscanf_s(waveString, "%d%s", &wave, tail, sizeof(tail));
  if (waveResult == 1 && IsWaveInRange(wave, smallWave))
    return true;
  wave = 0;
  smallWave = 0;
  int smallWaveResult = sscanf_s(waveString, "%d.%d%s", &wave, &smallWave, tail, sizeof(tail));
  if (smallWaveResult == 2 && IsWaveInRange(wave, smallWave))
    return true;
  return false;
}
char *const waveErrorString = "【波次】格式为m或m.n。\n大波m范围0~23，小波n范围0~11。";
//字符串分割，但是不跳过空串
char *NewStrTok(char *source, const char *delim, char **context)
{
  if (source)
  {
    char *delimLoc = strstr(source, delim);//寻找分隔符
    if (delimLoc)//如果存在分隔符，*context指向分隔符后第一个字符，分隔符处设为0
    {
      *context = delimLoc + strlen(delim);
      delimLoc[0] = 0;
    }
    else //如果不存在分隔符，*context设为nullptr，表示不能再分割了
      *context = nullptr;
    return source;
  }
  if (*context)
    return NewStrTok(*context, delim, context);
  return nullptr;
}
//设置进程DPI感知，返回DPI值
int SetDPIAware()
{
  int screenWidthBeforeAware = GetSystemMetrics(SM_CXSCREEN);
  SetProcessDPIAware();//设置进程DPI感知
  int screenWidthAfterAware = GetSystemMetrics(SM_CXSCREEN);
  return int(96 * double(screenWidthAfterAware) / screenWidthBeforeAware + 0.5);//获取DPI值
}
//596x950地图类型
typedef COLORREF MapType[gameHeight][gameWidth];
//申请Map[596][950]的内存空间，返回颜色数据地址和HDC hMemDC
MapType *MallocMap(HDC &hMemDC)
{
  void *pvMap; // pointer to DIB section pixels
  BITMAPINFO bmi = {};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//设置BITMAPINFO结构
  bmi.bmiHeader.biWidth = gameWidth;
  bmi.bmiHeader.biHeight = -gameHeight;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32; // 32位色深

  hMemDC = CreateCompatibleDC(GetDC(NULL));
  HBITMAP hBitmap = CreateDIBSection(hMemDC, &bmi, DIB_RGB_COLORS, &pvMap, NULL, 0);
  SelectObject(hMemDC, hBitmap);
  return (MapType *)pvMap;
}
// 注册窗口类
void InitClass(const char *className, WNDPROC WindowProc)
{
  WNDCLASS wndClass = {};
  wndClass.lpfnWndProc = WindowProc;
  wndClass.hInstance = GetModuleHandle(NULL);
  wndClass.lpszClassName = className;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  RegisterClass(&wndClass);
}
HBRUSH hBrushBlack;//黑色画刷
// 窗口创建信息
struct WindowInfo
{
  int cx, cy;
  char className[maxPath];
  char windowText[maxPath];
  HWND hWnd;//窗口
  HDC hDC;//设备环境
  HDC hDCMem;//内存（用于双缓冲绘图）
  HDC hDCUsed;//当前使用的DC
  float ratio;//缩放比例
  bool isCreated;//是否完成创建
  bool isExist;//是否存在
  void initSettings(HDC hDC, HFONT hFont, HPEN hPen) const
  {
    SelectObject(hDC, hFont);//设置字体
    SelectObject(hDC, hPen);//设置画笔
    SetTextColor(hDC, RGB(255, 255, 255)); //设置字体颜色
    SetBkColor(hDC, RGB(16, 16, 16));//设置背景颜色

    SetGraphicsMode(hDC, GM_ADVANCED);

    // 创建变换矩阵
    XFORM xForm = {};
    xForm.eM11 = ratio;  // X 轴缩放
    xForm.eM12 = 0;               // 无扭曲
    xForm.eM21 = 0;               // 无扭曲
    xForm.eM22 = ratio;  // Y 轴缩放
    xForm.eDx = 0;                // 无平移
    xForm.eDy = 0;                // 无平移

    // 设置变换矩阵
    SetWorldTransform(hDC, &xForm);
  }
  //初始化绘图设置
  void initSettings(HFONT hFont, HPEN hPen)
  {
    //创建内存DC并选择位图
    hDCMem = CreateCompatibleDC(hDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, cx, cy);
    SelectObject(hDCMem, hBitmap);
    //初始化字体、画笔、变换矩阵等
    initSettings(hDC, hFont, hPen);
    initSettings(hDCMem, hFont, hPen);
    hDCUsed = hDC;
  }
  //修改字体大小
  void settextsize(int size) const
  {
    HFONT hFontOld = (HFONT)GetCurrentObject(hDC, OBJ_FONT); //获取旧字体
    LOGFONT logFont = {};
    GetObject(hFontOld, sizeof(LOGFONT), &logFont); // 获取字体属性
    logFont.lfHeight = -size; // 设置新的字体高度（40像素）
    HFONT hFont = CreateFontIndirect(&logFont);// 创建新的字体
    SelectObject(hDC, hFont); // 选择新字体
    SelectObject(hDCMem, hFont); // 选择新字体
    DeleteObject(hFontOld); //删除旧字体
  }
  //设置字体颜色
  void settextcolor(COLORREF color) const
  {
    SetTextColor(hDC, color);
    SetTextColor(hDCMem, color);
  }
  //设置背景颜色
  void SetBackgroundColor(COLORREF color) const
  {
    SetBkColor(hDC, color);
    SetBkColor(hDCMem, color);
  }
  void BeginBatchDraw()
  {
    hDCUsed = hDCMem;
  }
  void FlushBatchDraw() const
  {
    BitBlt(hDC, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);
  }
  void EndBatchDraw()
  {
    BitBlt(hDC, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);
    hDCUsed = hDC;
  }
  //输出文字
  void outtextxy(const char *text, int x, int y) const
  {
    TextOutA(hDCUsed, x, y, text, strlen(text));
  }
  //输出数字
  void outtextxy(int num, int x, int y) const
  {
    char text[20];
    sprintf_s(text, "%d", num);
    TextOutA(hDCUsed, x, y, text, strlen(text));
  }
  //清空窗口
  void ClearWindow() const
  {
    RECT rect;
    GetClientRect(hWnd, &rect);
    FillRect(hDCUsed, &rect, hBrushBlack);
  }
  void line(int x1, int y1, int x2, int y2) const
  {
    MoveToEx(hDCUsed, x1, y1, nullptr);
    LineTo(hDCUsed, x2, y2);
  }
};
// 创建透明窗口并进行消息循环
DWORD __stdcall TransparentWindowThread(void *param)
{
  WindowInfo &wndInfo = *(WindowInfo *)param;
  wndInfo.hWnd = CreateWindowExA(
    WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,  // 扩展窗口风格
    wndInfo.className,                      // 窗口类名
    wndInfo.windowText,                     // 窗口标题
    WS_POPUP,//WS_OVERLAPPEDWINDOW,            // 窗口风格
    0, 0, wndInfo.cx, wndInfo.cy,// 窗口位置和尺寸
    NULL,       // 父窗口
    NULL,       // 菜单
    GetModuleHandle(NULL),  // 实例句柄
    NULL        // 额外的创建参数
  );
  if (wndInfo.hWnd == NULL)
  {
    wndInfo.isCreated = true;
    return 0;
  }
  wndInfo.hDC = GetDC(wndInfo.hWnd);
  SetLayeredWindowAttributes(wndInfo.hWnd, 0, 255, LWA_COLORKEY);
  ShowWindow(wndInfo.hWnd, SW_SHOW);
  wndInfo.isCreated = true;
  MSG msg = {};
  while (GetMessageA(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }
  return 0;
}
//创建透明窗口
HWND InitTransparentWindow(WindowInfo &wndInfo)
{
  CreateThread(NULL, 0, TransparentWindowThread, &wndInfo, 0, NULL);//启动任务线程
  while (wndInfo.isCreated == false)
    Sleep(1);
  return wndInfo.hWnd;
}
//将窗口hWnd截图到map数组，需要传入map和hDCMap
int MapShot(HWND hWnd, MapType(&map), HDC(&hDCMap))
{
  if (!IsGameWindowVisible(hWnd))
    return 0;
  int result = MultiPrintWindow(hWnd, hDCMap, 5);//截图5次直到成功，记录结果
  InvalidateRect(hWnd, NULL, false);//重画
  for (int y = 0; y < gameHeight; y++)
    for (int x = 0; x < gameWidth; x++)
      map[y][x] &= 0x00ffffff;
  return result;
}
//调节变量Variable（下限Minimum，上限Maximum，下调区域TurnDownArea，上调区域TurnUpArea）isCycle：是否循环
//Variable数值改变返回1
int Adjust(int &Variable, int Minimum, int Maximum, int step, int TurnDownArea, int TurnUpArea, int isCycle = 0)
{
  int oldValue = Variable;//记录原值
  if (area == TurnDownArea) //下调
  {
    if (Variable > Minimum + step)
      Variable -= step;
    else
    {
      if (isCycle == 1 && Variable == Minimum)
        Variable = Maximum;
      else
        Variable = Minimum;
    }
  }
  if (area == TurnUpArea) //上调
  {
    if (Variable < Maximum - step)
      Variable += step;
    else
    {
      if (isCycle == 1 && Variable == Maximum)
        Variable = Minimum;
      else
        Variable = Maximum;
    }
  }
  if (oldValue != Variable) //数值改变返回1
    return 1;
  return 0;
}
//截图文件名是否合格，是则更新参数
bool IsItemPathLegal(const char *itemPath, char(&itemName)[10], int *pRequiredSimilarity = nullptr,
  int *pOffsetX = nullptr, int *pOffsetY = nullptr, int *pWidth = nullptr, int *pHeight = nullptr)
{
  //拷贝图片路径
  char path[maxPath] = {};
  strcpy_s(path, itemPath);

  //预设默认数值
  int tempSimilarity = 200;
  int tempOffsetX = 0, tempOffsetY = 0, tempGridWidth = gridWidth, tempGridHeight = gridHeight;

  char *middleBra = strchr(path, '[');//中括号
  if (middleBra) //如果有中括号，按格式读取
  {
    //读取的项数
    int num = sscanf_s(middleBra, "[%d](%d,%d,%d,%d)",
      &tempSimilarity, &tempOffsetX, &tempOffsetY, &tempGridWidth, &tempGridHeight);
    if (num != 1 && num != 5) //只能读取1/5项
      return false;
    if (!(tempSimilarity >= minRequiredSimilarity && tempSimilarity <= maxRequiredSimilarity))
      return false;
    if (!(tempOffsetX >= minOffsetX && tempOffsetX <= maxOffsetX))
      return false;
    if (!(tempOffsetY >= minOffsetY && tempOffsetY <= maxOffsetY))
      return false;
    if (!(tempGridWidth >= minGridWidth && tempGridWidth <= maxGridWidth))
      return false;
    if (!(tempGridHeight >= minGridHeight && tempGridHeight <= maxGridHeight))
      return false;
    middleBra[0] = 0;
  }
  else //没有中括号，把最右边的.bmp删除
  {
    char *dot = strrchr(path, '.');
    if (dot)
      dot[0] = 0;
  }
  if (!(strlen(path) > 0 && strlen(path) <= 8)) //名称长度必须是1-8字符
    return false;

  //如果合格，记录名称和参数
  strcpy_s(itemName, path);
  if (pRequiredSimilarity)
    *pRequiredSimilarity = tempSimilarity;
  if (pOffsetX)
    *pOffsetX = tempOffsetX;
  if (pOffsetY)
    *pOffsetY = tempOffsetY;
  if (pWidth)
    *pWidth = tempGridWidth;
  if (pHeight)
    *pHeight = tempGridHeight;
  return true;
}
//在hashTable中记录颜色color存在
void RecordColor(byte(&hashTable)[1 << 21], COLORREF color)
{
  hashTable[color >> 3] |= 1 << (color & 0x00000007);
}
//在hashTable中判断颜色color是否存在
int IsColorExist(byte(&hashTable)[1 << 21], COLORREF color)
{
  return hashTable[color >> 3] & 1 << (color & 0x00000007);
}
//跳到文件f以str开头的行下一行开头，将这行内容写入dest。如果没找到str这一行，返回0
int fskip(FILE *f, char *str, char *dest = NULL)
{
  char s[maxLineSize];
  do //一直读取，直到找到目标行
  {
    if (feof(f)) //遇到文件结束返回0
      return 0;
    fgets(f, s);
  } while (strstr(s, str) != s);
  if (dest)
    strcpy_s(dest, sizeof(s), s);
  return 1;//成功跳到目标行返回1
}
//获取工作目录（带斜杠结尾）
void GetWorkPath(char(&workPath)[maxPath])
{
  _getcwd(workPath, sizeof(workPath));
  if (workPath[strlen(workPath) - 1] != '\\')
    strcat_s(workPath, "\\");//加一个斜杠结尾
}
//将完整路径转化为相对路径
void GetShortPath(const char *fullPath, char(&shortPath)[maxPath])
{
  char workPath[maxPath] = {};//当前工作目录（结尾带斜杠）
  GetWorkPath(workPath);
  //在完整路径中搜索工作目录
  if (strstr(fullPath, workPath))//如果找到了，说明轨道文件在工作目录下
    strcpy_s(shortPath, fullPath + strlen(workPath));
  else
    strcpy_s(shortPath, fullPath);
}
//从路径获取文件名
void GetFileName(const char *path, char(&fileName)[maxPath])
{
  const char *slash = strrchr(path, '\\');
  if (slash)
    strcpy_s(fileName, slash + 1);
  else
    strcpy_s(fileName, path);
}
//path扩展名是不是.txt
bool IsTxtFile(const char *path)
{
  int length = strlen(path);
  if (length < 4)
    return false;
  return strcmp(path + length - 4, ".txt") == 0;
}
//检查文件path类型：0=无效文件；1=轨道；2=任务列表
int CheckFileType(const char *path)
{
  int fileType = 0;//文件类型：0=无效文件；1=轨道；2=任务列表
  if (!IsTxtFile(path)) //不是txt视为无效文件
    return 0;
  FILE *f;
  if (fopen_s(&f, path, "r")) //打不开视为无效文件
    return 0;
  char s[100];
  fgets(f, s);
  fclose(f);
  if (strstr(s, "人物1位置=") || strstr(s, "轨道版本号="))
    fileType = 1; //第一行是人物1位置或轨道版本号，判定为轨道
  if (strstr(s, "列表版本号="))
    fileType = 2; //第一行是列表版本号，判定为列表
  //不是列表版本号，那就读6个数据
  int checked = 0, type = 0, level = 0, maxGames = 0, deck0 = 0, deck1 = 0;
  if (sscanf_s(s, "%d\t%d\t%d\t%d\t%d\t%d",
    &checked, &type, &level, &maxGames, &deck0, &deck1) == 6)
    fileType = 2;
  return fileType;
}