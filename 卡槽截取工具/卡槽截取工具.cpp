#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include "..\\track.h"
HDC hDCMap;
MapType(&map) = *MallocMap(hDCMap);
const int customX = 10, customY = 20, customWidth = 25, customHeight = 15;//识别范围
const int bagX = 382, bagWidth = 49, bagHeight = 57;
int bagY = 0;
const int bagY1 = 179, bagY2 = 407;//确定bagY的范围
float ratio;//显示比例
HCURSOR CrossCursor;//十字光标
HWND hWnd;
//绘制靶形光标
void PaintTargetCursor(int cx, int cy)
{
  circle(cx, cy, 8);
  circle(cx, cy, 4);
  line(cx - 8, cy, cx + 8, cy);
  line(cx, cy - 8, cx, cy + 8);
}
int GrabHall()
{
  HWND hWndPointed;
  char ClassName[256];
  SetSystemCursor(CrossCursor, (DWORD)IDC_ARROW);//将箭头和十字光标替换
  while (GetAsyncKeyState(VK_LBUTTON))//等待鼠标左键松开
    Sleep(10);
  SetSystemCursor(CrossCursor, (DWORD)IDC_ARROW);//光标恢复正常

  hWndPointed = MousePoint();//记录抓取的窗口句柄
  GetClassName(hWndPointed, ClassName, 256);//获取窗口类名
  if (strcmp(ClassName, "NativeWindowClass") != 0 && strcmp(ClassName, "WebPluginView") != 0)//如果不是游戏窗口
  {
    MessageBox(hWnd, "抓取位置不对，请看【截图说明.bmp】。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return 0;
  }

  MapShot(hWndPointed, map, hDCMap);

  //分析bagY
  int x = bagX + customX;
  for (int y = bagY1; y < bagY2; y++)
    if (map[y][x] == 0x001f41 && map[y + 1][x] == 0x1a4263 && map[y + 2][x] == 0x1a4263)
    {
      bagY = y;
      break;
    }
  if (bagY == 0)
  {
    MessageBox(hWnd, "未识别到卡片，请进入选卡界面再截图。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return 0;
  }

  //绘图
  ColorToBitmap(map, "TempShot.bmp");
  IMAGE image;
  loadimage(&image, "TempShot.bmp", int(950 * ratio), int(596 * ratio));
  remove("TempShot.bmp");
  solidrectangle(0, 0, 300, 70);
  Resize(NULL, 950 * DPI / 96, 596 * DPI / 96);
  BeginBatchDraw();
  setaspectratio(1, 1);
  putimage(0, 0, &image);
  setaspectratio(ratio, ratio);
  EndBatchDraw();

  //标注卡片位置
  for (int row = 0; row <= 4; row++)
    line(bagX, bagY + row * bagHeight, bagX + 11 * bagWidth, bagY + row * bagHeight);
  for (int column = 0; column <= 11; column++)
    line(bagX + column * bagWidth, bagY, bagX + column * bagWidth, bagY + 4 * bagHeight);
  return 1;
}
//保存整张防御卡截图
int SaveCustom(int row, int column)
{
  int x = bagX + column * bagWidth;
  int y = bagY + row * bagHeight;
  char path[MAX_PATH];
  for (int i = 0; i < 100; i++)
  {
    sprintf_s(path, "%d.bmp", i);
    if (_access(path, 0))
    {
      ColorToBitmap(map, path, x, y, bagWidth, bagHeight);
      return i;
    }
  }
  return -1;
}
void main()
{
  int ScreenWidth1 = GetSystemMetrics(SM_CXSCREEN);
  SetProcessDPIAware();//设置进程DPI感知
  int ScreenWidth2 = GetSystemMetrics(SM_CXSCREEN);
  DPI = 96 * ScreenWidth2 / ScreenWidth1;

  CrossCursor = LoadCursor(NULL, IDC_CROSS);//加载十字光标资源
  ratio = (float)(DPI / 96.0);
  initgraph((int)(300 * ratio), (int)(70 * ratio));
  setaspectratio(ratio, ratio);

  hWnd = GetHWnd();
  RECT rect;
  GetWindowRect(hWnd, &rect);
  SetWindowPos(hWnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_NOSIZE);//置顶显示

  setfillcolor(0);
  setbkmode(TRANSPARENT);     //透明字体
  LOGFONT f;
  gettextstyle(&f);						//获取当前字体设置
  f.lfHeight = 20;						//设置字体高度
  f.lfWeight = 800;						//设置字体厚度
  strcpy_s(f.lfFaceName, "等线");		//设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
  f.lfQuality = ANTIALIASED_QUALITY;		//设置输出效果为抗锯齿
  settextstyle(&f);						//设置字体样式

  outtextxy(50, 10, "进入任意关卡选卡界面，");
  outtextxy(50, 40, "将图标拖至游戏画面内");
  PaintTargetCursor(25, 35);

  ExMessage msg;
  int SleepTime = 0;
  while (1)
  {
    if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN) //鼠标点击
    {
      int x = (int)(msg.x * 96 / DPI);
      int y = (int)(msg.y * 96 / DPI);
      if (x < 48)
        if (GrabHall())
          break;
    }
    SleepTime++;
    if (SleepTime % 10 == zero)
      Sleep(1);
  }

  //响应点击，根据点击位置保存截图
  int row = -1, column = -1;
  char message[100] = {};
  while (1)
  {
    if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN) //鼠标点击
    {
      int x = (int)(msg.x * 96 / DPI);
      int y = (int)(msg.y * 96 / DPI);
      row = (y - bagY) / bagHeight;
      column = (x - bagX) / bagWidth;
      if (x >= bagX && y >= bagY && row < 4 && column < 11)
      {
        int customCode = SaveCustom(row, column);
        if (customCode >= 0)
        {
          sprintf_s(message, "截图已保存至【%d.bmp】。", customCode);
          MessageBox(hWnd, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        }
      }
    }
    SleepTime++;
    if (SleepTime % 10 == zero)
      Sleep(1);
  }
  ClickPause();
}