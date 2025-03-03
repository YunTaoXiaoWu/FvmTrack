#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include "..\\track.h"
#include <gdiplus.h>
using namespace Gdiplus;
HCURSOR CrossCursor;//十字光标
const int mapLeftCut = 140;//地图左侧截去的宽度
const int wndWidth = 950 - mapLeftCut, wndHeight = 596;//窗口尺寸
int wndWidthDPI, wndHeightDPI;//按DPI缩放后的窗口尺寸
float ratio;//窗口缩放比例
const COLORREF red = RGB(255, 64, 32), yellow = RGB(255, 255, 0),
white = RGB(255, 255, 255), black = RGB(30, 30, 30);
const int QQ_HEAD = 0, TRASH = 1, CUSTOM = 2, IMAGES = 3;//QQ头像 可删物品 自定卡槽 自定图像
const int RECOGNITION = 1, SHOT = 2;//识别和截图

//自定卡槽标签参数
const int customX = 10, customY = 20, customWidth = 25, customHeight = 15;//卡槽识别范围
const int bagX = 382, bagWidth = 49, bagHeight = 57;
int bagY;
const int bagY1 = 179, bagY2 = 407;//确定bagY的范围
//可删物品标签参数
const int propX = 468, propWidth = 49, propHeight = 49;//道具背包
int propY;
const int propY1 = 89, propY2 = 89 + propHeight + 10;//确定bagY的范围
//教程图片参数
const int tagX = 0, tagY = 0, tagWidth = 100, tagHeight = 36;//标签栏
const int tutorWidth = wndWidth, tutorHeight = wndHeight - tagHeight;//确定bagY的范围
COLORREF tutor[4][tutorHeight][tutorWidth];
const char *tagsName[5] = { "QQ头像", "可删物品", "自定卡槽", "自定图像" };

int tag;//0=QQ头像 1=可删物品 2=自定卡槽 3=自定图像
int mode;//1=识别模式；2=截图模式
bool isCardTemplate;//当前模板是否为卡片（截图卡片成功时设为true，截图物件或更换模板成功时设为false）
bool isExited;//是否点击了关闭
HWND hWndTool;//本程序窗口
HWND hWndGame;//游戏窗口
int clickMessage;//由WM_LBUTTONDOWN或WM_RBUTTONDOWN生成的点击消息
bool isTemplateCatched;//模板是否已截取
bool isBackgroundCatched;//背景是否已截取
char newTemplatePath[MAX_PATH];//拖拽的模板文件
byte colorExist[16777216 / 8];//每种颜色是否存在
const int gridX = 304, gridY = 98;//用于补阵识图的1行1列横、纵坐标，地图格子宽度、高度
const int cutGridX = gridX - mapLeftCut;//去掉左侧画面之后的GridX
const int templateX = 24, templateY = 374, templateWidth = 60, templateHeight = 64;//卡片模板（用于补阵）
const int modeX = 5, modeY = 100, modeWidth = 75, modeHeight = 33;//切换模式
const int settingsX = modeX + modeWidth, settingsWidth = templateWidth + 1, settingsHeight = modeHeight;
const int settingsY = modeY + 2 * modeHeight;
const int backgroundX = modeX, backgroundY = modeY + 7 * modeHeight;
const int backgroundWidth = 9 * gridWidth / 4, backgroundHeight = 7 * gridHeight / 4;
const int slotSettingsX = settingsX;
const int slotSettingsY = settingsY + 5 * settingsHeight + backgroundHeight + 1;
const int moveToTemplateX = templateX + 28, moveToTemplateY = templateY + 42;//放置模板时鼠标移动到的位置
//完整卡片模板尺寸
const int fullTemplateX = templateX - 20, fullTemplateY = templateY,
fullTemplateWidth = 120, fullTemplateHeight = 74;
COLORREF imageTemplate[templateHeight][templateWidth];//当前识别图像
COLORREF fullTemplate[fullTemplateHeight][fullTemplateWidth];//完整卡片模板
HDC hDCMap;//地图DC
HBITMAP hBmpMap;//地图Bitmap
HDC hDCBackground;//背景DC
COLORREF(&map)[596][950] = *MallocMap(&hDCMap, &hBmpMap);//地图
COLORREF(&background)[596][950] = *MallocMap(&hDCBackground);//背景
COLORREF erasedColor[templateWidth * templateHeight * 27];
int erasedColorNum;
COLORREF recordedColor[templateWidth * templateHeight * 27];
int recordedColorNum;
int offsetX, offsetY;//识别区域的位置偏移
int order = 1, slotOffsetX, slotOffsetY;//卡槽编号；卡槽模板位置偏移
int realGridWidth = gridWidth, realGridHeight = gridHeight;//识别区域的宽度和高度
int requiredSimilarity = 200;//临界相似度（满分1000）
char *programName;
const int defaultFont = 18, similarityFont = 16;
const int maxItemNum = 200;
char itemList[maxItemNum][maxPath];
char itemName[10];//当前物件名称（不要超过4个字）

//窗口截图缩放核心类
class WindowScaler
{
private:
  ULONG_PTR gdiplusToken;
  Graphics graphics;//绑定绘图窗口，用于绘制缩放后的图像
public:
  //初始化类，绑定窗口hWnd并申请所需要的内存
  explicit WindowScaler(HDC hDCImage) : graphics(hDCImage)
  {
    //初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    //初始化graphics设置
    graphics.SetInterpolationMode(InterpolationModeBilinear);
    graphics.SetPixelOffsetMode(PixelOffsetModeHalf);
  }
  ~WindowScaler()
  {
    //注销GDI+
    GdiplusShutdown(gdiplusToken);
  }

  //绘制缩放后的图像
  void PaintZoomedImage()
  {
    Bitmap srcBitmap(hBmpMap, nullptr);
    graphics.DrawImage(&srcBitmap, Rect(0, 0, wndWidthDPI, wndHeightDPI),
      mapLeftCut, 0, wndWidth, wndHeight, UnitPixel);
  }
};
WindowScaler *pScaler;
//绘制map
void ViewMap()
{
  pScaler->PaintZoomedImage();
}

//载入教程图片
void LoadTutor()
{
  char path[maxPath];
  for (int i = 0; i < 4; i++)
  {
    sprintf_s(path, "截图教程\\%s.png", tagsName[i]);
    BitmapToColor(path, tutor[i]);
  }
}

const int trashWidth = 49, trashHeight = 49;
const int lootCoreX = 5, lootCoreY = 5, lootCoreWidth = 36, lootCoreHeight = 28;
//可删物品类型（保存完整截图用于显示，但只有中间区域用于比较）
struct TrashType
{
  char name[50];//可删物品名称
  COLORREF image[trashWidth][trashHeight];//可删物品图像
};
const int maxTrashNum = 200;//可删物品上限
char trashList[maxTrashNum][maxPath];//可删物品目录
TrashType trash[maxTrashNum];//可删物品
int trashNum;//可删物品数量
//载入可删物品
void LoadTrash()
{
  if (!FileExist("可删物品"))
    return;
  int filesNum = GetFileList("可删物品\\*.png", trashList, maxTrashNum);//查找所有png文件
  trashNum = 0;
  char path[maxPath] = {};
  for (int i = 0; i < filesNum && trashNum < maxTrashNum; i++)
  {
    sprintf_s(path, "可删物品\\%s", trashList[i]);
    if (FileExist(path)) //如果可以打开
    {
      BitmapToColor(path, trash[trashNum].image);//读取卡片图像到image
      trashList[i][strlen(trashList[i]) - 4] = 0;//删除".png"
      strcpy_s(trash[trashNum].name, trashList[i]);//记录名称
      trashNum++;
    }
  }
}
//自定卡槽类型
struct CustomType
{
  char name[50];//卡槽名称
  COLORREF image[customHeight][customWidth];//卡槽图像
};
const int maxCustomNum = 500;//自定卡槽上限
char customList[maxCustomNum][maxPath];//自定卡槽目录
CustomType custom[maxCustomNum];//自定卡槽
int customNum;//自定卡槽数量
//载入自定卡槽
void LoadCustom()
{
  if (!FileExist("自定卡槽"))
    return;
  int filesNum = GetFileList("自定卡槽\\*.png", customList, maxCustomNum);//查找所有png文件
  customNum = 0;
  COLORREF bag[bagHeight][bagWidth] = {};//背包卡
  char path[maxPath] = {};
  for (int i = 0; i < filesNum && customNum < maxCustomNum; i++)
  {
    sprintf_s(path, "自定卡槽\\%s", customList[i]);
    if (FileExist(path)) //如果可以打开
    {
      BitmapToColor(path, bag);//读取卡片图像到bag
      for (int x = 0; x < customWidth; x++)//bag提取到image
        for (int y = 0; y < customHeight; y++)
          custom[customNum].image[y][x] = bag[customY + y][customX + x];
      customList[i][strlen(customList[i]) - 4] = 0;//删除".png"
      strcpy_s(custom[customNum].name, customList[i]);//记录名称
      customNum++;
    }
  }
}
//判断row行column列的物品是否已存在，存在则记录编号并返回true
bool FindTrash(int row, int column, int *pCode = nullptr)
{
  int x = propX + column * propWidth;
  int y = propY + row * propHeight;
  for (int code = 0; code < trashNum; code++)
    if (IsBitmapEqual(map, trash[code].image, lootCoreWidth, lootCoreHeight,
      x + lootCoreX, y + lootCoreY, lootCoreX, lootCoreY))
    {
      if (pCode)
        *pCode = code;
      return true;
    }
  return false;
}
//判断row行column列的卡槽是否已存在，存在则记录编号并返回true
bool FindCustom(int row, int column, int *pCode = nullptr)
{
  int x = bagX + column * bagWidth + customX;
  int y = bagY + row * bagHeight + customY;
  for (int code = 0; code < customNum; code++)
  {
    if (IsBitmapEqual(map, custom[code].image, customWidth, customHeight, x, y))
    {
      if (pCode)
        *pCode = code;
      return true;
    }
  }
  return false;
}

//调用本进程弹出高清对话框
void PopMessageDPI(HWND hWnd, const char *message)
{
  char cmd[1000];
  sprintf_s(cmd, "\"%s\" %d \"%s\"", programName, (int)hWnd, message);
  Execute(cmd);
}
void RecordColor(COLORREF color)
{
  colorExist[color >> 3] |= 1 << (color & 0x00000007);
}
void EraseColor(COLORREF color)
{
  colorExist[color >> 3] &= ~(1 << (color & 0x00000007));
}
int IsColorExist(COLORREF color)
{
  return colorExist[color >> 3] & 1 << (color & 0x00000007);
}
//记录模板颜色
void RecordTemplate()
{
  recordedColorNum = 0;
  for (int x = 0; x < templateWidth; x++)
    for (int y = 0; y < templateHeight; y++)
      if (!IsColorExist(imageTemplate[y][x]))
      {
        RecordColor(imageTemplate[y][x]);
        recordedColor[recordedColorNum++] = imageTemplate[y][x];
      }
}
//清除模板颜色
void EraseTemplate()
{
  for (int i = 0; i < recordedColorNum; i++)
    EraseColor(recordedColor[i]);
  recordedColorNum = 0;
}
//在地图(x0,y0)位置截取实物与模板卡进行比色，返回颜色匹配的像素数量
int GetSimilarityXY(int x0, int y0)
{
  int similarity = 0;//重合的颜色数量
  for (int y = 0; y < realGridHeight; y++)
    for (int x = 0; x < realGridWidth; x++)
      if (IsColorExist(map[y0 + y][x0 + x]))
      {
        similarity++;
        EraseColor(map[y0 + y][x0 + x]);
        erasedColor[erasedColorNum++] = map[y0 + y][x0 + x];
      }

  for (int i = 0; i < erasedColorNum; i++) //恢复被删除的记录
    RecordColor(erasedColor[i]);
  erasedColorNum = 0;

  return similarity;
}
//从row行column列截取实物与模板卡进行比色，返回颜色匹配的像素数量
int GetSimilarity(int row, int column)
{
  if (isTemplateCatched && recordedColorNum >= 10)
    return GetSimilarityXY(gridX + offsetX + (column - 1) * gridWidth,
      gridY + offsetY + (row - 1) * gridHeight);
  else
    return 0;
}
const int frame = 15;//统计帧数
const int duringTime = 1000;//统计时间
int globalMaxSimilarity;//全局最大相似度
int maxSimilarity[8][11];//某格1秒内的检测到的最大相似度
int similarity[frame][8][11];//7行10列BufferSize次检测的相似度
void ViewSimilarity()
{
  SetFontSize(similarityFont);
  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 10; column++)
    {
      maxSimilarity[row][column] >= requiredSimilarity ? settextcolor(red) : settextcolor(white);
      outtextxy(cutGridX + offsetX + (column - 1) * gridWidth,
        gridY + offsetY + (row - 1) * gridHeight, maxSimilarity[row][column]);
    }
  SetFontSize(defaultFont);
}
//选择最大值
void selectMax(int &oldMax, int value)
{
  if (value > oldMax)
    oldMax = value;
}
void ViewDebugInfo()
{
#ifdef _DEBUG
  char message[100];
  sprintf_s(message, "区域%d", area);
  settextcolor(white);
  outtextxy(5, 570, message);//显示当前点击区域（调试用）
#endif
}
//显示模板
void ViewTemplate()
{
  ColorToWindow(imageTemplate, modeX + modeWidth + 1, modeY + 1);//绘制模板
}
//显示背景
void ViewBackground()
{
  if (isBackgroundCatched)
  {
    SetStretchBltMode(GetImageHDC(nullptr), HALFTONE);
    StretchBlt(GetImageHDC(nullptr), backgroundX + 1, backgroundY + 1, backgroundWidth, backgroundHeight,
      hDCBackground, gridX, gridY, 9 * gridWidth, 7 * gridHeight, SRCCOPY);
  }
  else
  {
    settextcolor(white);
    if (mode == RECOGNITION)
      CenterView("无背景", backgroundX + 1 + backgroundWidth / 2, backgroundY + 1 + backgroundHeight / 2);
    else if (mode == SHOT)
      CenterView("点此截取背景", backgroundX + 1 + backgroundWidth / 2, backgroundY + 1 + backgroundHeight / 2);
  }
}
//显示模块切换按钮
void ViewMode()
{
  //涂黑
  solidrectangle(modeX - 5, modeY - 5, modeX + modeWidth + templateWidth + 6,
    modeY + modeHeight * 10 + backgroundHeight + 6);
  //模块切换
  char *modeString[] = { "识别", "截图", "X偏移", "Y偏移", "宽度", "高度", "标准",
    "截卡槽", "卡片X", "卡片Y" };
  for (int i = 0; i < 10; i++)
  {
    int y = modeY + (2 * i + 1) * modeHeight / 2;
    if (i >= 7)
      y += backgroundHeight + 1;
    if (mode == i + 1)
      settextcolor(red);
    else
      settextcolor(white);
    CenterView(modeString[i], modeX + modeWidth / 2, y);
  }
  //设置按钮和数值
  int value[8] = { offsetX, offsetY, realGridWidth, realGridHeight, requiredSimilarity,
    order, slotOffsetX, slotOffsetY };
  settextcolor(white);
  for (int i = 0; i < 8; i++)
  {
    int y = settingsY + settingsHeight * (2 * i + 1) / 2;
    if (i >= 5)
      y += backgroundHeight + 1;
    PaintHorizontalAdjust(settingsX + settingsWidth / 2, y, settingsWidth / 2);
    CenterView(value[i], settingsX + settingsWidth / 2, y);
  }
  //矩形框绘制
  PaintGrid(7, 1, modeX, modeY, modeWidth, modeHeight);//模式+参数名
  PaintGrid(5, 1, settingsX, settingsY, settingsWidth, settingsHeight);//参数
  PaintGrid(1, 1, settingsX, modeY, settingsWidth, templateHeight + 2);//模板
  PaintGrid(1, 1, backgroundX, backgroundY, backgroundWidth + 1, backgroundHeight + 1);//背景
  PaintGrid(3, 1, modeX, slotSettingsY, modeWidth, modeHeight);//卡片参数名
  PaintGrid(3, 1, settingsX, slotSettingsY, settingsWidth, settingsHeight);//卡片参数
}
//显示地图格子
void ViewMapGrid()
{
  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 10; column++)
      PaintGrid(1, 1, cutGridX + offsetX + (column - 1) * gridWidth, gridY + offsetY + (row - 1) * gridHeight,
        realGridWidth, realGridHeight);
}
const int cursorCX = 500, cursorCY = tagHeight / 2, cursorRadium = 12;//抓取光标
//绘制标签
void ViewTags()
{
  solidrectangle(0, 0, wndWidth, tagHeight + 1);

  SetFontSize(20);
  setlinecolor(yellow);
  settextcolor(yellow);
  PaintTargetCursor(cursorCX, cursorCY);//绘制抓取图标
  char handleString[100];
  if (hWndGame == 0)
    strcpy_s(handleString, "← 拖动至游戏窗口内获取句柄");
  else
    sprintf_s(handleString, "窗口句柄：%d", (int)hWndGame);
  outtextxy(cursorCX + 20, 8, handleString);

  setlinecolor(white);
  settextcolor(white);
  PaintGrid(1, 4, tagX, tagY, tagWidth, tagHeight);//绘制标签格子
  for (int i = 0; i < 4; i++)
  {
    if (tag == i)
      settextcolor(red);
    else
      settextcolor(white);
    CenterView(tagsName[i], (2 * i + 1) * tagWidth / 2, tagHeight / 2);
  }
  SetFontSize(defaultFont);
}
//截图1帧并计算各格相似度
void ShotAndGetSimilarity()
{
  //窗口可见：截取地图，直到左上角不为黑色，但最多截取10次
  if (IsGameWindowVisible(hWndGame))
    MapShot(hWndGame, map, hDCMap); //截取地图（最多截5次防黑）
  else //窗口不可见：直接设为全黑
    memset(map, 0, sizeof(map));

  //将最近BufferSize-1次相似度右移一格
  for (int i = frame - 1; i >= 1; i--)
    for (int row = 1; row <= 7; row++)
      for (int column = 1; column <= 10; column++)
        similarity[i][row][column] = similarity[i - 1][row][column];

  //获取相似度
  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 10; column++)
      similarity[0][row][column] = GetSimilarity(row, column);//获取该格卡片与模板卡的相似度

  //求取每格的最大相似度和全局最大相似度
  globalMaxSimilarity = 0;
  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 10; column++)
    {
      maxSimilarity[row][column] = 0;
      for (int i = 0; i < frame; i++)
        selectMax(maxSimilarity[row][column], similarity[i][row][column]);
      if (recordedColorNum > 0)
        maxSimilarity[row][column] = maxSimilarity[row][column] * 1000 / recordedColorNum;
      selectMax(globalMaxSimilarity, maxSimilarity[row][column]);
    }
}
//绘制道具框线
void ViewPropGrid()
{
  if (hWndGame == nullptr)
    return;

  //标注存在的可删道具
  SetFontSize(14);
  settextcolor(red);
  setfillcolor(white);
  int code = 0;
  for (int row = 0; row < 7; row++)
    for (int column = 0; column < 9; column++)
      if (FindTrash(row, column, &code))
      {
        int x = propX + column * propWidth + propWidth / 2 - mapLeftCut;
        int y = propY + (row + 1) * propHeight - 7;
        solidrectangle(x - propWidth / 2 + 1, y - 7, x + propWidth / 2 - 1, y + 7);
        CenterView(trash[code].name, x, y);
      }
  SetFontSize(defaultFont);
  setfillcolor(black);

  //标注道具位置
  for (int row = 0; row <= 7; row++)
    line(propX - mapLeftCut, propY + row * propHeight,
      propX - mapLeftCut + 9 * propWidth, propY + row * propHeight);
  for (int column = 0; column <= 9; column++)
    line(propX - mapLeftCut + column * propWidth, propY,
      propX - mapLeftCut + column * propWidth, propY + 7 * propHeight);
}
//从字符串str中移除所有字符ch
void removeChar(char *str, char ch)
{
  int length = strlen(str);
  int i = 0, j = 0;//原字符串、新字符串指针
  while (i <= length)
  {
    while (str[i] == ch)
      i++;
    str[j++] = str[i++];
  }
}
//绘制卡片框线，标注存在的卡片
void ViewSlotGrid()
{
  if (hWndGame == nullptr)
    return;

  //标注存在的卡片
  SetFontSize(14);
  settextcolor(white);
  setfillcolor(black);
  //settextcolor(red);
  //setfillcolor(white);
  int code = 0;
  char name[50];
  for (int row = 0; row < 4; row++)
    for (int column = 0; column < 11; column++)
      if (FindCustom(row, column, &code))
      {
        int x = bagX + column * bagWidth + bagWidth / 2 - mapLeftCut;
        int y = bagY + (row + 1) * bagHeight - 9;
        solidrectangle(x - bagWidth / 2 + 1, y - 8, x + bagWidth / 2 - 1, y + 8);
        //移除名称中的下划线再显示，避免名称过长
        strcpy_s(name, custom[code].name);
        removeChar(name, '_');
        CenterView(name, x, y);
      }
  SetFontSize(defaultFont);
  //setfillcolor(black);

  //绘制卡片框线
  for (int row = 0; row <= 4; row++)
    line(bagX - mapLeftCut, bagY + row * bagHeight,
      bagX - mapLeftCut + 11 * bagWidth, bagY + row * bagHeight);
  for (int column = 0; column <= 11; column++)
    line(bagX - mapLeftCut + column * bagWidth, bagY,
      bagX - mapLeftCut + column * bagWidth, bagY + 4 * bagHeight);
}
const char *tutorString[4][2] = { {
    "截取空间服登录游戏所需的头像。",
    "将右上角图标拖动至快捷登录界面即可。"
  }, {
    "截取可删除的物品和可分解的宝石。",
    "将右上角图标拖动至背包，单击物品截图，右键删除截图。"
  }, {
    "截取需要自动携带的防御卡。",
    "将右上角图标拖动至选卡界面，单击卡片截图，右键删除截图。"
  }, {
    "截取需要在战斗中识别的老鼠、BOSS或物件。",
    "将右上角图标拖动至游戏窗口内，按使用手册操作。"
  }
};
void ViewTutor()
{
  SetFontSize(24);
  ColorToWindow(tutor[tag], 0, tagHeight);//显示教程图片
  settextcolor(red);
  outtextxy(36, tagHeight + 40 + 0 * 40, tutorString[tag][0]);
  settextcolor(RGB(255, 255, 255));
  outtextxy(36, tagHeight + 40 + 1 * 40, tutorString[tag][1]);
  SetFontSize(defaultFont);
}
//界面绘制函数
void Repaint()
{
  if (tag == IMAGES)
    setbkmode(OPAQUE);     //不透明字体
  else
    setbkmode(TRANSPARENT);//透明字体

  BeginBatchDraw();
  solidrectangle(0, 0, wndWidth, wndHeight);
  //计划：没有句柄时绘制教程，抓取后绘制内容
  if (hWndGame == nullptr)
    ViewTutor();
  else
  {
    if (tag == QQ_HEAD)
    {
      ViewMap();//绘制地图
    }
    else if (tag == TRASH) //可删物品标签
    {
      ViewMap();//绘制地图
      ViewPropGrid();//绘制卡片框线
    }
    else if (tag == CUSTOM) //自定卡槽标签
    {
      ViewMap();//绘制地图
      ViewSlotGrid();//绘制卡片框线
    }
    else if (tag == IMAGES) //自定图像标签
    {
      ViewMap();//绘制地图
      ViewMapGrid();//绘制地图格子
      if (mode == RECOGNITION)
      {
        ShotAndGetSimilarity();//截图并计算相似度
        ViewSimilarity();//显示相似度
      }
      ViewMode();//显示模式按钮
      ViewTemplate();//绘制模板
      ViewBackground();//显示背景
    }
  }
  ViewDebugInfo();//显示调试信息
  ViewTags();//显示标签（最后的覆盖）
  EndBatchDraw();
}
//更换当前模板
void ChangeTemplate(const char *itemFullPath)
{
  //提取短名称
  char shortPath[maxPath] = {};
  char *slash = strrchr((char *)itemFullPath, '\\');
  if (slash)
    strcpy_s(shortPath, slash + 1);
  else
    strcpy_s(shortPath, itemFullPath);
  //如果名称不合格，进行提示
  if (!IsItemPathLegal(shortPath, itemName, &requiredSimilarity,
    &offsetX, &offsetY, &realGridWidth, &realGridHeight))
  {
    PopMessageDPI(hWndTool, "截图名称不合格。");
    return;
  }
  EraseTemplate();//清除原模板颜色
  if (BitmapToColor(itemFullPath, imageTemplate))
  {
    isTemplateCatched = 1;
    isCardTemplate = false;
  }
  RecordTemplate();//记录新模板颜色
  memset(similarity, 0, sizeof(similarity));
}
int GetArea(int originX, int originY)
{
  int x = (int)(originX / ratio + 0.5);
  int y = (int)(originY / ratio + 0.5);
  if (x < 0 || y < 0 || x >= wndWidth || y >= wndHeight)
    return 0;
  //tag切换：10-13
  if (x < 4 * tagWidth && y < tagHeight)
    return 10 + x / tagWidth;
  //标签抓取19
  if (x >= cursorCX - cursorRadium && x <= cursorCX + cursorRadium
    && y >= cursorCY - cursorRadium && y <= cursorCY + cursorRadium)
    return 19;

  if (hWndGame == nullptr) //未抓取句柄，返回0
    return 0;

  if (tag == TRASH)
  {
    x += mapLeftCut;
    //道具背包区域：10000+
    if (x >= propX && x < propX + 9 * propWidth &&
      y >= propY && y < propY + 7 * propHeight)
    {
      int row = (y - propY) / propHeight;
      int column = (x - propX) / propWidth;
      return 10000 + row * 100 + column;
    }
  }
  else if (tag == CUSTOM)
  {
    x += mapLeftCut;
    //选卡区域：10000+
    if (x >= bagX && x < bagX + 11 * bagWidth &&
      y >= bagY && y < bagY + 4 * bagHeight)
    {
      int row = (y - bagY) / bagHeight;
      int column = (x - bagX) / bagWidth;
      return 10000 + row * 100 + column;
    }
  }
  else if (tag == IMAGES)//自定图像
  {
    //背景截取99
    if (x >= backgroundX && x < backgroundX + backgroundWidth + 1
      && y >= backgroundY && y < backgroundY + backgroundHeight + 1)
      return 99;
    //选项（标题20+，数值30+；左调40+，右调50+）
    if (x >= modeX && x < settingsX + settingsWidth
      && y >= modeY && y < slotSettingsY + 3 * modeHeight)
    {
      //判断点击位置在背景上面还是下面
      if (y >= backgroundY)
        y -= backgroundHeight + 1;
      int row = (y - modeY) / modeHeight;
      if (x < modeX + modeWidth)
        return 20 + row;
      if (x - settingsX < 20)
        return 40 + row;
      if (settingsX + settingsWidth - x < 20)
        return 50 + row;
      return 30 + row;
    }
    if (mode == 2) //截图模式
    {
      //点击地图格子（保存截图）：100+
      if (x >= cutGridX && y >= gridY && x < cutGridX + 10 * gridWidth && y < gridY + 7 * gridHeight)
      {
        int row = (y - gridY) / gridHeight;
        int column = (x - cutGridX) / gridWidth;
        return 100 + row * 10 + column;
      }
    }
  }
  return 0;
}
//响应五大参数的调节，调节成功返回true
bool AdjustParam()
{
  return Adjust(offsetX, minOffsetX, maxOffsetX, 1, 42, 52, 0) ||
    Adjust(offsetY, minOffsetY, maxOffsetY, 1, 43, 53, 0) ||
    Adjust(realGridWidth, minGridWidth, maxGridWidth, 1, 44, 54, 0) ||
    Adjust(realGridHeight, minGridHeight, maxGridHeight, 1, 45, 55, 0) ||
    Adjust(requiredSimilarity, minRequiredSimilarity, maxRequiredSimilarity, 10, 46, 56, 0);
}
bool AdjustSlotParam()
{
  return Adjust(order, 1, 21, 1, 47, 57, 0) ||
    Adjust(slotOffsetX, -20, 20, 1, 48, 58, 0) ||
    Adjust(slotOffsetY, -10, 10, 1, 49, 59, 0);
}
//填写物件路径
void GetItemPath(char(&path)[maxPath])
{
  char param[100] = {};
  sprintf_s(path, "自定图像\\%s", itemName);
  sprintf_s(param, "[%d]", requiredSimilarity);
  strcat_s(path, param);
  if (!(offsetX == 0 && offsetY == 0 && realGridWidth == gridWidth && realGridHeight == gridHeight))
  {
    sprintf_s(param, "(%d,%d,%d,%d)", offsetX, offsetY, realGridWidth, realGridHeight);
    strcat_s(path, param);
  }
  strcat_s(path, ".png");
}
//响应五大参数的调节，调节成功则重命名截图
void AdjustParamAndRename()
{
  char oldPath[maxPath];
  GetItemPath(oldPath);
  if (AdjustParam())
  {
    char newPath[maxPath];
    GetItemPath(newPath);
    //重命名模板，如果无法重命名，则重新保存一份模板
    if (rename(oldPath, newPath) != 0)
    {
      remove(oldPath);
      ColorToBitmap(imageTemplate, newPath);
    }
  }
}
//更新模板颜色记录
void UpdateTemplateColor()
{
  EraseTemplate();//清除原模板颜色记录
  RecordTemplate();//记录新模板颜色
  isTemplateCatched = 1;
  memset(similarity, 0, sizeof(similarity));
}
//（修改卡片XY偏移时）从完整模板中更新当前模板
void UpdateCardTemplate()
{
  if (isCardTemplate) //若当前模板为卡片，更新当前模板
  {
    for (int y = 0; y < templateHeight; y++)
      for (int x = 0; x < templateWidth; x++)
        imageTemplate[y][x] = slotOffsetY + y < 0 ? 0 :
        fullTemplate[slotOffsetY + y][20 + slotOffsetX + x];
    UpdateTemplateColor();
  }
}
void AdjustSlotParamAndSave()
{
  if (AdjustSlotParam())
  {
    UpdateCardTemplate();
    char path[maxPath];
    GetItemPath(path);
    ColorToBitmap(imageTemplate, path);
  }
}
const int SlotX1 = 228, SlotY1 = 11, SlotWidth = 53, SlotHeight = 68;//卡槽1横坐标、上部纵坐标，卡片宽度、高度
//拾起卡槽
void PickSlot()
{
  if (order <= 14)
    LeftClickDPI(hWndGame, SlotX1 + (order - 1) * SlotWidth, SlotY1);//卡槽1-14
  else
    LeftClickDPI(hWndGame, SlotX1 + 13 * SlotWidth, SlotY1 + (order - 14) * SlotHeight);//卡槽15-21
}
//截取order卡槽的完整模板
int CatchSlotTemplate(char(&path)[maxPath])
{
  if (!isBackgroundCatched)
    return 0;
  if (!IsGameWindowVisible(hWndGame))
    return 0;
  PickSlot();
  MouseMoveDPI(hWndGame, moveToTemplateX, moveToTemplateY);//鼠标移动到模板位置
  Sleep(30);//等待30毫秒
  int counter = 0;
  int isShotSuccessful = 0;
  //若画面没有变化，每20ms重新截取
  do
  {
    counter++;
    if (counter > 3)
      return 0;
    Sleep(20);
    MapShot(hWndGame, map, hDCMap);//截取地图
  } while (IsBitmapEqual(background, map, fullTemplateWidth, fullTemplateHeight,
    fullTemplateX, fullTemplateY, fullTemplateX, fullTemplateY));//当完整模板区域与背景不同时退出
  LeftClickDPI(hWndGame, gridX - gridWidth, gridY + 3 * gridHeight);//释放卡槽
  //获取完整模板，扣除背景区域
  for (int y = 0; y < fullTemplateHeight; y++)
    for (int x = 0; x < fullTemplateWidth; x++)
      fullTemplate[y][x] = map[fullTemplateY + y][fullTemplateX + x] == background[fullTemplateY + y][fullTemplateX + x] ?
      0 : map[fullTemplateY + y][fullTemplateX + x];

  //重设参数并更新当前模板
  slotOffsetX = 0;
  slotOffsetY = 0;
  offsetX = 0;
  offsetY = 0;
  realGridWidth = slotTemplateWidth;
  realGridHeight = slotTemplateHeight;
  requiredSimilarity = 40;
  isCardTemplate = true;
  UpdateCardTemplate();

  //保存模板文件
  sprintf_s(itemName, "卡槽%d", order);
  GetItemPath(path);
  ColorToBitmap(imageTemplate, path);
  return 1;
}
void TryCatchSlotTemplate()
{
  char path[maxPath];
  char message[100];
  if (area == 27)
  {
    if (CatchSlotTemplate(path))
    {
      sprintf_s(message, "卡片截图已保存至\n【%s】", path);
      PopMessageDPI(hWndTool, message);
    }
    else //卡片截取失败
    {
      if (isBackgroundCatched)
        PopMessageDPI(hWndTool, "卡片截取失败。");
      else
        PopMessageDPI(hWndTool, "卡片截取失败，请先截取背景。");
    }
  }
}
//将当前地图指定位置设置为新模板
void SetImageTemplate(int x0, int y0)
{
  isCardTemplate = false;
  memset(imageTemplate, 0, sizeof(imageTemplate));//清空原模板
  //拷贝颜色数据
  for (int x = 0; x < realGridWidth; x++)
    for (int y = 0; y < realGridHeight; y++)
      imageTemplate[y][x] = map[y0 + y][x0 + x];
  //扣除背景
  if (isBackgroundCatched)
    for (int x = 0; x < templateWidth; x++)
      for (int y = 0; y < templateHeight; y++)
        if (imageTemplate[y][x] == background[y0 + y][x0 + x])
          imageTemplate[y][x] = 0;
  UpdateTemplateColor();
}
//保存新模板截图
int SaveNewItem(int row, int column, char(&path)[maxPath])
{
  //寻找可用的文件名
  int itemNum = GetFileList("自定图像\\*.png", itemList, maxItemNum);//图像数量
  bool isCodeUsed[maxItemNum] = {};
  int code = 0;
  //记录已被使用的编号
  for (int i = 0; i < itemNum; i++)
    if (sscanf_s(itemList[i], "%d", &code) == 1 && code >= 0 && code < maxItemNum)
      isCodeUsed[code] = true;
  int validCode = maxItemNum;
  for (int i = 0; i < maxItemNum; i++)
    if (!isCodeUsed[i])
    {
      validCode = i;
      break;
    }
  //保存文件
  sprintf_s(itemName, "%d", validCode);
  GetItemPath(path);
  int x0 = gridX + offsetX + column * gridWidth;
  int y0 = gridY + offsetY + row * gridHeight;
  SetImageTemplate(x0, y0);//更新模板
  ColorToBitmap(imageTemplate, path);//保存模板文件
  return validCode;
}
//保存背景截图
int SaveBackground()
{
  ColorToBitmap(map, "自定图像\\背景.png");//保存背景文件
  BitBlt(hDCBackground, 0, 0, gameWidth, gameHeight, hDCMap, 0, 0, SRCCOPY);//更换当前背景
  isBackgroundCatched = 1;
  return 1;
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
void ShotQQHead(HWND hWndHall)
{
  int hallWidth, hallHeight;
  COLORREF *hallShot = nullptr;//大厅截图一级指针
  //截取大厅图像（HallShot函数自动分配内存）
  if (!HallShot(hWndHall, hallShot, &hallWidth, &hallHeight, 1))
  {
    PopMessageDPI(hWndTool, "大厅截图失败。");
    return;
  }
  //截图成功，则构造二级指针
  COLORREF *pHallShot[4320] = {};//高度不超过8K分辨率4320
  for (int i = 0; i < hallHeight; i++)
    pHallShot[i] = hallShot + i * hallWidth;
  GetWindowSize(hWndHall, &hallWidth, &hallHeight);//获得大厅尺寸

  int headAreaLength = 0;//头像区域累积长度（一行全白色则累积中断）
  int headX = 0, headY = 0;//头像右上角尺寸
  int isHeadFound = 0;//头像是否找到
  for (int y = 120; y < hallHeight && isHeadFound == 0; y++) //从高度120开始从上到下扫描
  {
    for (int x = hallWidth - 20; x >= hallWidth / 2; x--) //从右往左扫，最多扫到半屏
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
      else if (x == hallWidth / 2) //整行白色：非头像区
      {
        if (headAreaLength == 87)//如果到达非头像区时累积长度达到87，说明找到头像位置了
          isHeadFound = 1;//记录头像已找到，退出循环
        headAreaLength = 0; //否则清零继续
      }
  }
  if (isHeadFound == 0) //找不到头像区，返回0
  {
    PopMessageDPI(hWndTool, "找不到头像，可能是页面缩放不对。\n请重新打开登录窗口再试。");
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
    PopMessageDPI(hWndTool, "没有找到头像，请先登录QQ。");
    return;
  }
  headNum--; //去掉二维码的头像个数
  char path[MAX_PATH];
  for (int i = 0; i < headNum; i++) //保存前headNum - 1个头像，从左到右命名为1P,2P,...
  {
    sprintf_s(path, "QQ头像截图\\%dP.png", headNum - i);
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
      BitmapToColor("QQ头像截图\\默认头像.png", newheadShot[order]);
    else
    {
      char path[MAX_PATH];
      sprintf_s(path, "QQ头像截图\\%dP.png", order);
      BitmapToColor(path, newheadShot[order]);
    }
  }
  FILE *f;
  fopen_s(&f, "QQ头像截图\\头像区分度.txt", "w");
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
  PopMessageDPI(hWndTool, "截图已保存至【QQ头像截图】文件夹。\n"
    "请将1P/2P头像分别命名为1P.png/2P.png。");
}
bool IsCustomTop(int y)
{
  int x = bagX + customX;
  return map[y][x] == 0x001f41 && map[y + 1][x] == 0x1a4263 && map[y + 2][x] == 0x1a4263;
}
bool IsPropTop(int y)
{
  const COLORREF topColor[2] = { 0x14263d, 0x001f41 };//格顶标准颜色
  const int topCenterX = 492;//格顶中心X
  COLORREF topCenterColor = map[y][topCenterX]; //格顶中心颜色
  //如果格顶中心颜色都不对，说明不是格顶
  if (topCenterColor != topColor[0] && topCenterColor != topColor[1])
    return false;
  //如果中心颜色对，那么周围10格都要是这个颜色，才判断为格顶
  for (int x = topCenterX - 10; x < topCenterX + 10; x++)
    if (map[y][x] != topCenterColor)
      return false;
  return true;
}
//获取道具或选卡界面顶部纵坐标（在y1~y2范围内查找）
int GetTop(int y1, int y2, bool IsTop(int))
{
  int topY = 0;
  for (int y = y1; y < y2; y++)
    if (IsTop(y))
    {
      topY = y;
      break;
    }
  return topY;
}
//抓取游戏窗口句柄，抓取成功则记录句柄
int GrabHandle()
{
  SetSystemCursor(CrossCursor, (DWORD)IDC_ARROW);//将箭头和十字光标替换
  while (GetAsyncKeyState(VK_LBUTTON))//等待鼠标左键松开
    Sleep(10);
  SetSystemCursor(CrossCursor, (DWORD)IDC_ARROW);//光标恢复正常

  char ClassName[256];
  HWND hWndPointed = MousePoint();//记录抓取的窗口句柄
  GetClassName(hWndPointed, ClassName, 256);//获取窗口类名

  HWND hWndGame0, hWndHall0;//抓取的游戏窗口和大厅窗口
  //微端窗口或大厅窗口
  if (strcmp(ClassName, "ApolloRuntimeContentWindow") == 0 || strcmp(ClassName, "DUIWindow") == 0)
  {
    hWndGame0 = GetActiveGameWindow(hWndPointed);//获取游戏窗口
    hWndHall0 = hWndPointed; //大厅窗口就是抓取的窗口
  }
  //游戏窗口
  else if (strcmp(ClassName, "NativeWindowClass") == 0 || strcmp(ClassName, "WebPluginView") == 0)
  {
    hWndGame0 = hWndPointed; //游戏窗口就是抓取的窗口
    hWndHall0 = GetHallWindow(hWndPointed); //获取大厅窗口
  }
  //选服窗口
  else if (strcmp(ClassName, "Chrome_RenderWidgetHostHWND") == 0)
  {
    hWndGame0 = GetGameWindowFromServer(hWndPointed); //寻找游戏窗口
    hWndHall0 = GetHallWindow(hWndPointed); //寻找大厅窗口
  }
  //都不是
  else
  {
    PopMessageDPI(hWndTool, "抓取位置不对，请按说明操作。");
    return 0;
  }
  if (tag == QQ_HEAD) //QQ头像标签：直接截图大厅，截图成功则完工，否则提示失败
  {
    ShotQQHead(hWndHall0);
    return 0;
  }
  else //其他三种情况都要求抓到游戏画面
  {
    if (!hWndGame0)
    {
      PopMessageDPI(hWndTool, "未找到游戏窗口。");
      return 0;
    }
    //如果抓到了游戏窗口，自定图像标签直接通过，自定卡槽标签则额外要求在选卡界面
    if (tag == TRASH)
    {
      MapShot(hWndGame0, map, hDCMap); //截取地图（最多截5次防黑）
      propY = GetTop(propY1, propY2, IsPropTop);
      if (propY == 0)
      {
        hWndGame = 0;//清空句柄
        PopMessageDPI(hWndTool, "未识别到物品，请进入道具背包再截图。");
        return 0;
      }
      propY++;//propY自增1
      while (propY >= 88 + propHeight) //把propY修正到[88,88+49)范围内
        propY -= propHeight;
    }
    else if (tag == CUSTOM)
    {
      MapShot(hWndGame0, map, hDCMap); //截取地图（最多截5次防黑）
      bagY = GetTop(bagY1, bagY2, IsCustomTop);
      if (bagY == 0)
      {
        hWndGame = 0;//清空句柄
        PopMessageDPI(hWndTool, "未识别到卡片，请进入选卡界面再截图。");
        return 0;
      }
    }
  }
  hWndGame = hWndGame0;
  return 1;
}
//重置数据
void ResetData()
{
  hWndGame = nullptr;//清空句柄
  memset(map, 0, sizeof(map));//清空地图
}
//保存防御卡截图
int SaveCustom(int row, int column)
{
  if (FindCustom(row, column)) //已有截图的卡不予保存
    return -1;
  int x = bagX + column * bagWidth;
  int y = bagY + row * bagHeight;
  char path[MAX_PATH];
  for (int i = 0; i < 999; i++)
  {
    sprintf_s(path, "自定卡槽\\%d.png", i);
    if (!FileExist(path))
    {
      ColorToBitmap(map, path, x, y, bagWidth, bagHeight);
      CopyMap(custom[customNum].image, map, x + customX, y + customY);
      sprintf_s(custom[customNum].name, "%d", i);
      customNum++;
      return i;
    }
  }
  return -1;
}
//删除防御卡截图
void DeleteCustom(int row, int column)
{
  int code = 0;
  if (!FindCustom(row, column, &code)) //没有截图的卡无法删除
    return;
  char path[MAX_PATH];
  sprintf_s(path, "自定卡槽\\%s.png", custom[code].name);
  remove(path);
  //如果要删的不是最后一张，把最后一张移动到被删除的位置
  if (code != customNum - 1)
  {
    strcpy_s(custom[code].name, custom[customNum - 1].name);
    CopyMap(custom[code].image, custom[customNum - 1].image, 0, 0);
  }
  customNum--;
}
//保存可删物品截图
int SaveTrash(int row, int column)
{
  if (FindTrash(row, column)) //已有截图的道具不予保存
    return -1;
  int x = propX + column * propWidth;
  int y = propY + row * propHeight;
  char path[MAX_PATH];
  for (int i = 0; i < 999; i++)
  {
    sprintf_s(path, "可删物品\\%d.png", i);
    if (!FileExist(path))
    {
      ColorToBitmap(map, path, x, y, propWidth, propHeight);//保存道具截图
      CopyMap(trash[trashNum].image, map, x, y);//记录可删物品图像和名称
      sprintf_s(trash[trashNum].name, "%d", i);
      trashNum++;
      return i;
    }
  }
  return -1;
}
//删除可删物品截图
void DeleteTrash(int row, int column)
{
  int code = 0;
  if (!FindTrash(row, column, &code)) //没有截图的物品无法删除
    return;
  char path[MAX_PATH];
  sprintf_s(path, "可删物品\\%s.png", trash[code].name);
  remove(path);
  //如果要删的不是最后一张，把最后一张移动到被删除的位置
  if (code != trashNum - 1)
  {
    strcpy_s(trash[code].name, trash[trashNum - 1].name);
    CopyMap(trash[code].image, trash[trashNum - 1].image, 0, 0);
  }
  trashNum--;
}
//点击响应函数
void Edit()
{
  char message[100] = {};//用于填写MessageBox
  char path[maxPath] = {};//文件路径
  int sleepTimes = 0;
  Repaint();
  DWORD lastCheckTick = GetTickCount();//上次绘图时刻
  while (true)
  {
    if (clickMessage) //检测到鼠标按下
    {
      int buttons = clickMessage / 100000000;
      area = clickMessage % 100000000;
      clickMessage = 0;

      if (buttons == 1)
      {
        //标签切换
        if (area >= 10 && area <= 13)
        {
          int newTag = area - 10;
          if (newTag != tag)
          {
            ResetData();//重置数据
            tag = newTag;
            if (tag == IMAGES)
              mode = RECOGNITION;//识别模式
            return;
          }
        }
        else if (area == 19)//抓取句柄
          GrabHandle();
      }

      if (tag == TRASH) //可删物品标签
      {
        if (area / 10000 == 1)
        {
          int row = area % 10000 / 100;
          int column = area % 100;
          if (buttons == 1)
            SaveTrash(row, column);
          else if (buttons == 2)
            DeleteTrash(row, column);
        }
      }
      else if (tag == CUSTOM) //自定卡槽标签
      {
        if (area / 10000 == 1)
        {
          int row = area % 10000 / 100;
          int column = area % 100;
          if (buttons == 1)
            SaveCustom(row, column);
          else if (buttons == 2)
            DeleteCustom(row, column);
        }
      }
      else if (tag == IMAGES) //自定图像标签
      {
        //两种模式共同的点击响应
        if (buttons == 0) //无键消息
        {
          if (area == 97) //拖拽更换模板
            ChangeTemplate(newTemplatePath);
        }
        else if (buttons == 1)//如果按下的是左键
        {
          AdjustParamAndRename();//调节参数，调节成功则重命名文件
          AdjustSlotParamAndSave();//调节卡槽参数，调节成功则重写模板
          TryCatchSlotTemplate();//截取卡片模板
        }
        else if (buttons == 2)
        {
          if (area == 99) //截取背景
          {
            remove("自定图像\\背景.png");
            isBackgroundCatched = 0;
          }
        }

        if (mode == RECOGNITION) //识别模式
        {
          if (buttons == 1)//如果按下的是左键
          {
            if (area == 21) //切换到截图模式
            {
              mode = SHOT;
              return;
            }
          }
        }
        else if (mode == SHOT) //截图模式
        {
          if (buttons == 1)//如果按下的是左键
          {
            if (area == 20) //切换到识别模式
            {
              mode = RECOGNITION;
              return;
            }
            if (area == 99) //截取背景
            {
              SaveBackground();
              PopMessageDPI(hWndTool, "背景已保存至【自定图像\\背景.png】。");
            }
            if (area / 100 == 1)//点击格子：保存截图
            {
              int row = area % 100 / 10;
              int column = area % 10;
              int itemCode = SaveNewItem(row, column, path);
              if (itemCode >= 0)
              {
                sprintf_s(message, "截图已保存至\n【%s】", path);
                PopMessageDPI(hWndTool, message);
              }
            }
          }
        }
      }
      if (tag != IMAGES)
        Repaint();
    }
    sleepTimes++;
    if (sleepTimes % 10 == zero) //每10次响应等待1ms
    {
      sleepTimes = 0;
      Sleep(1);
      //自定图像标签按15帧刷新，其他模式只在点击时刷新
      DWORD checkTick = GetTickCount();
      if (checkTick - lastCheckTick >= duringTime / frame)
      {
        lastCheckTick = checkTick;
        if (tag == IMAGES)
          Repaint();
      }
    }
    if (isExited)
      exit(0);
  }
}
//自定义窗口过程
LRESULT CALLBACK MyWindowProc(HWND hWndTool, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CLOSE:
    if (mode == RECOGNITION) //识图模式强制关闭可能让游戏画面卡死，所以只记录关闭
      isExited = true;
    else
      exit(0);
    break;
  case WM_DROPFILES:
  {
    HDROP hDrop = (HDROP)wParam;
    int width = 0, height = 0;
    DragQueryFileA(hDrop, 0, newTemplatePath, sizeof(newTemplatePath));
    DragFinish(hDrop);
    if (GetBitmapRect(newTemplatePath, &width, &height) && width == 60 && height == 64)
      clickMessage = 97;//非键消息97：载入模板newTemplate
    else
      PopMessageDPI(hWndTool, "模板截图必须是60x64的png位图。");
  }
  break;
  case WM_LBUTTONDOWN:
  {
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    int area = GetArea(x, y);
    clickMessage = 100000000 + area;
  }
  break;
  case WM_RBUTTONDOWN:
  {
    int x = LOWORD(lParam);
    int y = HIWORD(lParam);
    int area = GetArea(x, y);
    clickMessage = 200000000 + area;
  }
  break;
  default:
    return DefWindowProc(hWndTool, uMsg, wParam, lParam);
    // 处理其他消息...
  }
  return 0;
}
int main(int argc, char *argv[])
{
  DPI = SetDPIAware();
  if (argc == 3)
  {
    PopMessage((HWND)atoi(argv[1]), argv[2]);
    return 0;
  }
  programName = argv[0];
  LoadTutor();
  LoadTrash();
  LoadCustom();
  //载入背景
  if (BitmapToColor("自定图像\\背景.png", background))
  {
    isBackgroundCatched = 1;
    //识图位点最高字节设为0
    for (int y = 0; y < 596; y++)
      for (int x = 0; x < 950; x++)
        background[y][x] &= 0x00ffffff;
  }
  CrossCursor = LoadCursor(NULL, IDC_CROSS);//加载十字光标资源
  wndWidthDPI = wndWidth * DPI / 96;
  wndHeightDPI = wndHeight * DPI / 96;
  hWndTool = initgraph(wndWidthDPI, wndHeightDPI);//创建绘图窗口
  pScaler = new WindowScaler(GetImageHDC());//绑定绘图窗口DC
  ratio = (float)DPI / 96;//DPI缩放比例
  setaspectratio(ratio, ratio);
  SetWindowLongPtr(hWndTool, GWLP_WNDPROC, (LONG_PTR)MyWindowProc); // 设置自定义窗口过程
  EnableDragDropForHighIntegrity(hWndTool);
  DragAcceptFiles(hWndTool, TRUE);//允许窗口接收文件

  LOGFONT f;
  gettextstyle(&f);						//获取当前字体设置
  f.lfHeight = defaultFont;		//设置字体高度
  f.lfWeight = 1000;				  //设置字体厚度
  strcpy_s(f.lfFaceName, "等线");		//设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
  f.lfQuality = ANTIALIASED_QUALITY;//设置输出效果为抗锯齿
  settextstyle(&f);						//设置字体样式

  setfillcolor(black);
  tag = QQ_HEAD;//自定图像标签

  while (1)
    Edit();
  closegraph();
  return 0;
}