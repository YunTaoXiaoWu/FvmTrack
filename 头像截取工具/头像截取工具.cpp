#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include "..\\track.h"
HCURSOR CrossCursor;//十字光标
//绘制靶形光标
void PaintTargetCursor(int cx, int cy)
{
  circle(cx, cy, 8);
  circle(cx, cy, 4);
  line(cx - 8, cy, cx + 8, cy);
  line(cx, cy - 8, cx, cy + 8);
}
//计算头像head0和head1的颜色方差
int headDifference(COLORREF head0[87][87], COLORREF head1[87][87])
{
  const int headWidth = 87, headHeight = 87;
  int diff = 0;
  for (int x = 0; x < headWidth; x++)
    for (int y = 0; y < headHeight; y++)
    {
      int diffR = GetRValue(head0[x][y]) - GetRValue(head1[x][y]);
      int diffG = GetGValue(head0[x][y]) - GetGValue(head1[x][y]);
      int diffB = GetBValue(head0[x][y]) - GetBValue(head1[x][y]);
      diff += diffR * diffR + diffG * diffG + diffB * diffB;
    }
  return diff;
}
void GrabHall()
{
  char ClassName[256];
  SetSystemCursor(CrossCursor, (DWORD)IDC_ARROW);//将箭头和十字光标替换
  while (GetAsyncKeyState(VK_LBUTTON))//等待鼠标左键松开
    Sleep(10);
  SetSystemCursor(CrossCursor, (DWORD)IDC_ARROW);//光标恢复正常

  HWND hWndPointed = MousePoint();//记录抓取的窗口句柄
  GetClassName(hWndPointed, ClassName, 256);//获取窗口类名
  if (strcmp(ClassName, "DUIWindow") != 0)//如果不是大厅窗口
  {
    MessageBox(NULL, "抓取位置不对，请看【截图说明.bmp】", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }

  int WindowWidth, WindowHeight;
  COLORREF *hallShot = nullptr;
  COLORREF *pHallShot[4320] = {};//高度不超过8K分辨率4320
  if (!HallShot(hWndPointed, hallShot, &WindowWidth, &WindowHeight, 1))//截取大厅图像
  {
    MessageBox(NULL, "截图失败", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }

  //截图成功，则构造二级指针
  for (int i = 0; i < WindowHeight; i++)
    pHallShot[i] = hallShot + i * WindowWidth;
  GetWindowSize(hWndPointed, &WindowWidth, &WindowHeight);//获得大厅尺寸

  int headAreaLength = 0;//头像区域累积长度（一行全白色则累积中断）
  int headX = 0, headY = 0;//头像右上角尺寸
  int isHeadFound = 0;//头像是否找到
  for (int y = 120; y < WindowHeight && isHeadFound == 0; y++) //从高度120开始从上到下扫描
  {
    for (int x = WindowWidth - 20; x >= WindowWidth / 2; x--) //从右往左扫，最多扫到半屏
      if (pHallShot[y][x] != 0xffffff) //非白色区位头像区
      {
        if (headAreaLength == 0) //记录首次到达头像区的坐标
        {
          headX = x;
          headY = y;
        }
        headAreaLength++;
        break;
      }
      else if (x == WindowWidth / 2) //整行白色：非头像区
      {
        if (headAreaLength == 87)//如果到达非头像区时累积长度达到87，说明找到头像位置了
          isHeadFound = 1;//记录头像已找到，退出循环
        headAreaLength = 0; //否则清零继续
      }
  }
  if (isHeadFound == 0) //找不到头像区，返回0
  {
    MessageBox(NULL, "找不到头像，可能是页面缩放不对。\n请重新打开登录窗口再试", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }

  int headNum = 0;//头像数量
  POINT head[10] = {};//每个头像的位置
  for (int i = 0; i < sizeof(head) / sizeof(head[0]); i++) //填写头像位置高度
    head[i].y = headY;
  for (int x = headX; x > 20; x--) //从右往左扫描
    for (int y = headY; y < headY + 87; y++) //在头像纵坐标区域扫描
      if (pHallShot[y][x] != 0xffffff) //非白色区域为头像区，累积长度+1
      {
        headAreaLength++;
        break;
      }
      else if (y == headY + 86) //全白为非头像区
      {
        if (headAreaLength == 87)//如果遇到全白列时累积长度达到87，说明已经遍历完一个头像
        {
          head[headNum].x = x + 1;//记录头像左上角坐标
          head[headNum].y = headY;//记录头像左上角坐标
          headNum++;
        }
        headAreaLength = 0; //头像区长度清零
      }

  if (headNum <= 1) //找不到头像区，返回0
  {
    MessageBox(NULL, "没有找到头像，请先登录QQ", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }
  headNum--; //去掉二维码的头像个数
  char path[MAX_PATH];
  for (int i = 0; i < headNum; i++) //保存前headNum - 1个头像，从左到右命名为1P,2P,...
  {
    sprintf_s(path, "%dP.bmp", headNum - i);
    COLORREF tempHead[87][87];
    for (int y = 0; y < 87; y++)
      for (int x = 0; x < 87; x++)
        tempHead[y][x] = pHallShot[head[i].y + y][head[i].x + x];
    ColorToBitmap(tempHead, path);
  }
  //截图后分析相似度
  COLORREF newheadShot[10][87][87];
  for (int order = 0; order <= headNum; order++)
  {
    if (order == 0)
      BitmapToColor("默认头像.bmp", newheadShot[order]);
    else
    {
      char path[MAX_PATH];
      sprintf_s(path, "%dP.bmp", order);
      BitmapToColor(path, newheadShot[order]);
    }
  }
  FILE *f;
  fopen_s(&f, "头像区分度.txt", "w");
  fprintf(f, "\t默认");
  for (int i = 1; i <= headNum; i++)
    fprintf(f, "\t%dP", i);
  fprintf(f, "\n");
  for (int i = 0; i <= headNum; i++)
  {
    if (i == 0)
      fprintf(f, "默认");
    else
      fprintf(f, "%dP", i);
    for (int j = 0; j <= headNum; j++)
    {
      int diff = headDifference(newheadShot[i], newheadShot[j]);
      fprintf(f, "\t%d", diff / 10000);
    }
    fprintf(f, "\n");
  }
  fprintf(f, "任意两个头像之间的区分度都要大于1000，否则可能识别错误。");
  fclose(f);
  MessageBox(NULL, "截取成功，请将1P/2P头像\n分别命名为1P.bmp/2P.bmp", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
  exit(0);
}
void main()
{
  int ScreenWidth1 = GetSystemMetrics(SM_CXSCREEN);
  SetProcessDPIAware();//设置进程DPI感知
  int ScreenWidth2 = GetSystemMetrics(SM_CXSCREEN);
  DPI = 96 * ScreenWidth2 / ScreenWidth1;

  CrossCursor = LoadCursor(NULL, IDC_CROSS);//加载十字光标资源
  float ratio = (float)(DPI / 96.0);
  initgraph((int)(300 * ratio), (int)(70 * ratio));
  setaspectratio(ratio, ratio);

  HWND hWnd = GetHWnd();
  RECT rect;
  GetWindowRect(hWnd, &rect);
  SetWindowPos(hWnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_NOSIZE);//移动到左上角

  setbkmode(TRANSPARENT);     //透明字体
  LOGFONT f;
  gettextstyle(&f);						//获取当前字体设置
  f.lfHeight = 20;						//设置字体高度
  f.lfWeight = 800;						//设置字体厚度
  strcpy_s(f.lfFaceName, "等线");		//设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
  f.lfQuality = ANTIALIASED_QUALITY;		//设置输出效果为抗锯齿
  settextstyle(&f);						//设置字体样式

  outtextxy(50, 10, "进入空间服选头像登录界面");
  outtextxy(50, 40, "将图标拖到标题栏完成截图");
  PaintTargetCursor(25, 35);

  ExMessage msg;
  int SleepTime = 0;
  while (1)
  {
    if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN) //鼠标点击
    {
      int x = (int)(msg.x * 96 / DPI);
      int y = (int)(msg.y * 96 / DPI);
      if (x < 40)
        GrabHall();
    }
    SleepTime++;
    if (SleepTime % 10 == zero)
      Sleep(1);
  }
  ClickPause();
}