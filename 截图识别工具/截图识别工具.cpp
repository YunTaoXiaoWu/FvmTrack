#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include "..\\track.h"
HCURSOR CrossCursor;//十字光标
const int WndWidth = 300, WndHeight = 60;//窗口尺寸
const int mapLeftCut = 140;//地图左侧截去的宽度
bool isExited;//是否点击了关闭
HWND hWndTool;//本程序窗口
HWND hWndGame;//游戏窗口
int clickMessage;//由WM_LBUTTONDOWN或WM_RBUTTONDOWN生成的点击消息
int mode;//0=未抓取句柄；1=识别模式；2=截图模式
int isTemplateCatched;//模板是否已截取
int isBackgroundCatched;//背景是否已截取
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
HDC hDCMap, hDCBackground;//地图DC
COLORREF(&map)[596][950] = *MallocMap(hDCMap);//地图数组
COLORREF(&background)[596][950] = *MallocMap(hDCBackground);//转置背景
COLORREF erasedColor[templateWidth * templateHeight * 27];
int erasedColorNum;
COLORREF recordedColor[templateWidth * templateHeight * 27];
int recordedColorNum;
int offsetX, offsetY;//识别区域的位置偏移
int order = 1, slotOffsetX, slotOffsetY;//卡槽编号；卡槽模板位置偏移
int realGridWidth = gridWidth, realGridHeight = gridHeight;//识别区域的宽度和高度
int requiredSimilarity = 200;//临界相似度（满分1000）
char *programName;
const int settingsFont = 18, similarityFont = 16;
const int maxItemNum = 200;
char itemList[maxItemNum][maxPath];
char itemName[10];//当前物件名称（不要超过4个字）
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
      maxSimilarity[row][column] >= requiredSimilarity ? settextcolor(RGB(255, 0, 0)) : settextcolor(RGB(255, 255, 255));
      outtextxy(cutGridX + offsetX + (column - 1) * gridWidth,
        gridY + offsetY + (row - 1) * gridHeight, maxSimilarity[row][column]);
    }
  SetFontSize(settingsFont);
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
    settextcolor(RGB(255, 255, 255));
    if (mode == 1)
      CenterView("无背景", backgroundX + 1 + backgroundWidth / 2, backgroundY + 1 + backgroundHeight / 2);
    else if (mode == 2)
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
      settextcolor(RGB(255, 0, 0));
    else
      settextcolor(WHITE);
    CenterView(modeString[i], modeX + modeWidth / 2, y);
  }
  //设置按钮和数值
  int value[8] = { offsetX, offsetY, realGridWidth, realGridHeight, requiredSimilarity,
    order, slotOffsetX, slotOffsetY };
  for (int i = 0; i < 8; i++)
  {
    int y = settingsY + settingsHeight * (2 * i + 1) / 2;
    if (i >= 5)
      y += backgroundHeight + 1;
    PrintHorizontalAdjustButton(settingsX + settingsWidth / 2, y, settingsWidth / 2);
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
//显示地图画面
void ViewMap()
{
  ColorToWindow(map, 0, 0, mapLeftCut, 0);
}
//绘制识别模式
void RepaintRecognition()
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
  //绘图
  BeginBatchDraw();
  ViewMap();//绘制地图
  ViewMapGrid();//绘制地图格子
  ViewSimilarity();//显示相似度数值
  ViewMode();//显示模式按钮
  ViewTemplate();//显示模板
  ViewBackground();//显示背景
  ViewDebugInfo();//显示调试信息
  EndBatchDraw();
}
//绘制截图模式
void RepaintShot()
{
  BeginBatchDraw();
  ViewMap();//绘制地图
  ViewMapGrid();//绘制地图格子
  ViewMode();//显示模式按钮
  ViewTemplate();//绘制模板
  ViewBackground();//显示背景
  ViewDebugInfo();//显示调试信息
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
    isTemplateCatched = 1;
  RecordTemplate();//记录新模板颜色
  memset(similarity, 0, sizeof(similarity));
}
int GetArea(int x, int y)
{
  if (mode == 0) //抓取句柄前：点击抓取图标为1
    if (x < 48)
      return 1;
  //背景截取
  if (x >= backgroundX && x < backgroundX + backgroundWidth + 1
    && y >= backgroundY && y < backgroundY + backgroundHeight + 1)
    return 99;
  //设置按钮（本体20+x；左调30，右调40）
  //if (x >= settingsX && x < settingsX + settingsWidth && y >= modeY && y < modeY + 7 * modeHeight)
  if (x >= modeX && x < settingsX + settingsWidth
    && y >= modeY && y < slotSettingsY + 3 * modeHeight)
  {
    //判断点击位置在背景上面还是下面
    if (y >= backgroundY)
      y -= backgroundHeight + 1;
    int row = (y - modeY) / modeHeight;
    if (x < modeX + modeWidth)
      return 10 + row;
    if (x - settingsX < 20)
      return 30 + row;
    if (settingsX + settingsWidth - x < 20)
      return 40 + row;
    return 20 + row;
  }
  if (mode == 2) //截图模式
  {
    //点击地图格子（保存截图）
    if (x >= cutGridX && y >= gridY && x < cutGridX + 10 * gridWidth && y < gridY + 7 * gridHeight)
    {
      int row = (y - gridY) / gridHeight;
      int column = (x - cutGridX) / gridWidth;
      return 100 + row * 10 + column;
    }
  }
  return 0;
}
//响应五大参数的调节，调节成功返回true
bool AdjustParam()
{
  return Adjust(offsetX, minOffsetX, maxOffsetX, 1, 32, 42, 0) ||
    Adjust(offsetY, minOffsetY, maxOffsetY, 1, 33, 43, 0) ||
    Adjust(realGridWidth, minGridWidth, maxGridWidth, 1, 34, 44, 0) ||
    Adjust(realGridHeight, minGridHeight, maxGridHeight, 1, 35, 45, 0) ||
    Adjust(requiredSimilarity, minRequiredSimilarity, maxRequiredSimilarity, 10, 36, 46, 0);
}
bool AdjustSlotParam()
{
  return Adjust(order, 1, 21, 1, 37, 47, 0) ||
    Adjust(slotOffsetX, -20, 20, 1, 38, 48, 0) ||
    Adjust(slotOffsetY, -10, 10, 1, 39, 49, 0);
}
//填写物件路径
void GetItemPath(char(&path)[maxPath])
{
  char param[100] = {};
  strcpy_s(path, itemName);
  sprintf_s(param, "[%d]", requiredSimilarity);
  strcat_s(path, param);
  if (!(offsetX == 0 && offsetY == 0 && realGridWidth == gridWidth && realGridHeight == gridHeight))
  {
    sprintf_s(param, "(%d,%d,%d,%d)", offsetX, offsetY, realGridWidth, realGridHeight);
    strcat_s(path, param);
  }
  strcat_s(path, ".bmp");
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
void UpdateTemplate()
{
  //更新当前模板
  for (int y = 0; y < templateHeight; y++)
    for (int x = 0; x < templateWidth; x++)
      imageTemplate[y][x] = slotOffsetY + y < 0 ? 0 :
      fullTemplate[slotOffsetY + y][20 + slotOffsetX + x];
  UpdateTemplateColor();
}
void AdjustSlotParamAndSave()
{
  if (AdjustSlotParam())
  {
    UpdateTemplate();
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
  UpdateTemplate();

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
  if (area == 17)
  {
    if (CatchSlotTemplate(path))
    {
      sprintf_s(message, "卡片截图已保存至\n%s", path);
      PopMessageDPI(hWndTool, message);
    }
    else
      PopMessageDPI(hWndTool, "卡片截取失败。");
  }
}
//识别模式（mode==1）
void EditRecognition()
{
  int sleepTimes = 0;
  RepaintRecognition();
  DWORD lastCheckTick = GetTickCount();//上次绘图时刻
  while (1)
  {
    if (clickMessage) //检测到鼠标按下
    {
      int buttons = clickMessage / 100000000;
      area = clickMessage % 100000000;
      clickMessage = 0;
      if (buttons == 0) //无键消息
      {
        if (area == 1) //拖拽更换模板
          ChangeTemplate(newTemplatePath);
      }
      else if (buttons == 1)//如果按下的是左键
      {
        if (area == 11) //切换到截图模式
        {
          mode = 2;
          return;
        }
        AdjustParamAndRename();//调节参数，调节成功则重命名文件
        AdjustSlotParamAndSave();//调节卡槽参数，调节成功则重写模板
        TryCatchSlotTemplate();//截取卡片模板
      }
      else if (buttons == 2)
      {
        if (area == 99) //截取背景
        {
          remove("背景.bmp");
          isBackgroundCatched = 0;
        }
      }
    }
    sleepTimes++;
    if (sleepTimes % 10 == zero)
    {
      sleepTimes = 0;
      Sleep(1);
      DWORD checkTick = GetTickCount();
      if (checkTick - lastCheckTick >= duringTime / frame)
      {
        lastCheckTick = checkTick;
        RepaintRecognition();
      }
    }
    if (isExited)
      exit(0);
  }
}
//将当前地图指定位置设置为新模板
void SetTemplate(int x0, int y0)
{
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
  int itemNum = GetFileList("*.bmp", itemList, maxItemNum);//图像数量
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
  SetTemplate(x0, y0);//更新模板
  ColorToBitmap(imageTemplate, path);//保存模板文件
  return validCode;
}
//保存背景截图
int SaveBackground()
{
  ColorToBitmap(map, "背景.bmp");//保存背景文件
  BitBlt(hDCBackground, 0, 0, gameWidth, gameHeight, hDCMap, 0, 0, SRCCOPY);//更换当前背景
  isBackgroundCatched = 1;
  return 1;
}
//截图模式（mode==2）
void EditShot()
{
  char message[100] = {};
  char path[maxPath] = {};
  int sleepTimes = 0;
  RepaintShot();
  while (1)
  {
    if (clickMessage) //检测到鼠标按下
    {
      int buttons = clickMessage / 100000000;
      area = clickMessage % 100000000;
      clickMessage = 0;
      if (buttons == 0) //无键消息
      {
        if (area == 1) //拖拽更换模板
          ChangeTemplate(newTemplatePath);
      }
      else if (buttons == 1)//如果按下的是左键
      {
        if (area == 10) //切换到识别模式
        {
          mode = 1;
          return;
        }
        AdjustParamAndRename();//调节参数，调节成功则重命名文件
        AdjustSlotParamAndSave();//调节卡槽参数，调节成功则重写模板
        TryCatchSlotTemplate();//截取卡片模板
        if (area == 99) //截取背景
        {
          SaveBackground();
          PopMessageDPI(hWndTool, "背景已保存至【背景.bmp】。");
        }
        if (area / 100 == 1)//点击格子：保存截图
        {
          int row = area % 100 / 10;
          int column = area % 10;
          int itemCode = SaveNewItem(row, column, path);
          if (itemCode >= 0)
          {
            sprintf_s(message, "截图已保存至\n%s", path);
            PopMessageDPI(hWndTool, message);
          }
        }
      }
      else if (buttons == 2)
      {
        if (area == 99) //截取背景
        {
          remove("背景.bmp");
          isBackgroundCatched = 0;
        }
      }
    }
    sleepTimes++;
    if (sleepTimes % 10 == zero)
      Sleep(1);
    if (sleepTimes >= 100)
    {
      sleepTimes = 0;
      RepaintShot();//显示调试信息
    }
  }
}
//绘制靶形光标
void PaintTargetCursor(int cx, int cy)
{
  circle(cx, cy, 8);
  circle(cx, cy, 4);
  line(cx - 8, cy, cx + 8, cy);
  line(cx, cy - 8, cx, cy + 8);
}
//抓取游戏窗口句柄，抓取成功则记录句柄
int GrabHall()
{
  SetSystemCursor(CrossCursor, (DWORD)IDC_ARROW);//将箭头和十字光标替换
  while (GetAsyncKeyState(VK_LBUTTON))//等待鼠标左键松开
    Sleep(10);
  SetSystemCursor(CrossCursor, (DWORD)IDC_ARROW);//光标恢复正常

  char ClassName[256];
  hWndGame = MousePoint();//记录抓取的窗口句柄
  GetClassName(hWndGame, ClassName, 256);//获取窗口类名
  if (strcmp(ClassName, "NativeWindowClass") != 0 && strcmp(ClassName, "WebPluginView") != 0)//如果不是游戏窗口
  {
    PopMessageDPI(hWndTool, "抓取位置不对，请看【截图说明.bmp】。");
    return 0;
  }
  return 1;
}
//自定义窗口过程
LRESULT CALLBACK MyWindowProc(HWND hWndTool, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CLOSE:
    if (mode == 1) //识图模式强制关闭可能让游戏画面卡死，所以只记录关闭
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
      clickMessage = 1;//非键消息1：载入模板newTemplate
    else
      PopMessageDPI(hWndTool, "模板截图必须是60x64的bmp位图。");
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
void GetAccountHandle()
{
  //抓取句柄并记录到hWndGame
  int sleepTimes = 0;
  while (true)
  {
    if (clickMessage) //鼠标点击
    {
      int buttons = clickMessage / 100000000;
      area = clickMessage % 100000000;
      clickMessage = 0;
      if (buttons == 1 && area == 1)
        if (GrabHall())
          break;
    }
    sleepTimes++;
    if (sleepTimes % 10 == zero)
      Sleep(1);
  }
}
int main(int argc, char *argv[])
{
  if (argc == 3)
  {
    SetProcessDPIAware();
    PopMessage((HWND)atoi(argv[1]), argv[2]);
    return 0;
  }
  //DPI = SetDPIAware();
  programName = argv[0];
  //载入背景
  if (BitmapToColor("背景.bmp", background))
  {
    isBackgroundCatched = 1;
    //识图位点最高字节设为0
    for (int y = 0; y < 596; y++)
      for (int x = 0; x < 950; x++)
        background[y][x] &= 0x00ffffff;
  }

  CrossCursor = LoadCursor(NULL, IDC_CROSS);//加载十字光标资源
  hWndTool = initgraph(WndWidth, WndHeight);
  //setaspectratio(1.25, 1.25);
  //SetWindowPos(hWndTool, HWND_TOPMOST, 50, 50, 0, 0, SWP_NOSIZE);//置顶显示
  SetWindowLongPtr(hWndTool, GWLP_WNDPROC, (LONG_PTR)MyWindowProc); // 设置自定义窗口过程
  EnableDragDropForHighIntegrity(hWndTool);
  DragAcceptFiles(hWndTool, TRUE);//允许窗口接收文件

  LOGFONT f;
  gettextstyle(&f);						//获取当前字体设置
  f.lfHeight = 20;						//设置字体高度
  f.lfWeight = 800;						//设置字体厚度
  strcpy_s(f.lfFaceName, "等线");		//设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
  f.lfQuality = ANTIALIASED_QUALITY;//设置输出效果为抗锯齿
  settextstyle(&f);						//设置字体样式

  outtextxy(50, 20, "请将图标拖至游戏画面内");
  PaintTargetCursor(25, 30);

  GetAccountHandle();
  //hWndGame = (HWND)328964;

  solidrectangle(0, 0, WndWidth, WndHeight);
  Resize(NULL, 950 - mapLeftCut, 596);
  setfillcolor(RGB(30, 30, 30));
  mode = 1;//进入识别模式

  while (1)
    if (mode == 1)
      EditRecognition();
    else if (mode == 2)
      EditShot();
  closegraph();
  return 0;
}