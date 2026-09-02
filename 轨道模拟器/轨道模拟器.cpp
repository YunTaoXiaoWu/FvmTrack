#ifndef _DEBUG
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")
#endif
#include "resource.h"
#include "..\\track.h"

const char versionString[] = "轨道模拟器%s by -云涛晓雾-";
const int maxStar = 21;
int mode; // 模式：0=制卡，1=强卡
// 制卡界面
COLORREF makeBase[gameHeight][gameWidth];
COLORREF make[gameHeight][gameWidth];
// 强卡界面
COLORREF upgradeBase[gameHeight][gameWidth];
COLORREF upgrade[gameHeight][gameWidth];

HWND hWndSimulator; // 模拟器
int sleepTime;

// 0-12级四叶草倍率（单位：0.1）
const int cloverRate[] = { 10, 12, 14, 17, 20, 24, 27, 30, 32, 36, 40, 42, 42 };
// 0-16级VIP加成（单位：%）
const int vipBonus[] = { 0, 0, 0, 0, 3, 4, 5, 7, 8, 9, 11, 13, 15, 17, 19, 22, 25 };
// 0-6级合成屋加成（单位：%）
const int guildBonus[7] = { 0, 1, 3, 5, 8, 12, 16 };

double rates[maxStar + 1][3][3]; // 0-20星主卡使用n,n-1,n-2星好/中/差卡强化的成功率
int goldMake[maxStar + 1]; // 0-20星卡制作的金币消耗
int goldUpgrade[maxStar + 1]; // 0-20星主卡强化的金币消耗
int tokenInsurance[maxStar + 1]; // 0-20星主卡强化的保险金

// 读取强化参数（成功率、金币消耗、保险金）
void LoadUpgradeParam()
{
  const char path[] = "附加程序\\文本\\强化参数.txt";
  FILE *f;
  if (fopen_s(&f, path, "r"))
    ReportMissingFile(path);
  char s[1000];
  fgets(f, s);
  for (int star = 0; star <= maxStar; star++)
  {
    fgets(f, s);
    int tempStar = 0;
    sscanf_s(s,
      "%d\t"
      "%lf\t%lf\t%lf\t"
      "%lf\t%lf\t%lf\t"
      "%lf\t%lf\t%lf\t"
      "%d\t%d\t%d",
      &tempStar,
      &rates[star][0][0], &rates[star][1][0], &rates[star][2][0],
      &rates[star][0][1], &rates[star][1][1], &rates[star][2][1],
      &rates[star][0][2], &rates[star][1][2], &rates[star][2][2],
      &goldMake[star], &goldUpgrade[star], &tokenInsurance[star]);
    // 文档中的成功率没有百分号，除以100才是实际成功率
    for (int order = 0; order < 3; order++)
      for (int quality = 0; quality < 3; quality++)
        rates[star][order][quality] /= 100;
  }
  fclose(f);
}

int gold; // 金币
int token; // 点券
int vip; // VIP
int guild; // 公会等级
double base[maxStar + 1]; // 保底奖池

void LoadGold()
{
  const char path[] = "用户参数\\文本\\模拟器-金币.txt";
  if (!FileExist(path))
    return;

  FILE *f;
  fopen_s(&f, path, "r");
  char s[1000];
  fgets(f, s);
  gold = atoi(s);
  fgets(f, s);
  token = atoi(s);
  fgets(f, s);
  vip = atoi(s);
  fgets(f, s);
  guild = atoi(s);
  fclose(f);
}

void SaveGold()
{
  const char path[] = "用户参数\\文本\\模拟器-金币.txt";
  if (!FileExist(path))
    return;

  FILE *f;
  fopen_s(&f, path, "w");
  fprintf(f, "%d\n", gold);
  fprintf(f, "%d\n", token);
  fprintf(f, "%d\n", vip);
  fprintf(f, "%d\n", guild);
  fclose(f);
}

const int upgradeX = 559, upgradeY = 91; // 强化界面
const int makeX = 559, makeY = 319; // 强化界面
const int synthesisPropX = 33, synthesisPropY = 526; // 合成道具栏位置
const int recipeSlotX = 265, recipeSlotY = 340; // 配方槽位置
const int spiceSlotX = 158, spiceSlotY = 372; // 香料槽位置
const int cloverSlotX = 159, cloverSlotY = 370; // 四叶草槽位置

const int propWidth = 49, propHeight = 49; // 道具
const int cardWidth = 49, cardHeight = 57; // 卡片
const int infoWidth = 230, infoHeight = 336; // 卡片信息
COLORREF emptyProp[propHeight][propWidth];
COLORREF emptyCard[cardHeight][cardWidth];
// 主卡和副卡1~3的位置
const POINT deskLoc[4] = { { 264, 320 }, { 264, 249 }, { 208, 320 }, { 320, 320 } };

// 翻页按钮
const int pageX = 525, pageY[2] = { 532, 554 }, pageWidth = 17, pageHeight = 20;
COLORREF page[2][pageHeight][pageWidth];
int cloverPage;

// 根据卡片位置确定信息显示位置
void GetInfoLocation(int cardX, int cardY, int &infoX, int &infoY)
{
  // 左右：右侧有空间就显示右边，否则显示左边
  if (cardX + cardWidth + infoWidth < gameWidth)
    infoX = cardX + cardWidth;
  else
    infoX = cardX - infoWidth;
  // 上下：下面有空间就显示下面，上面有空间就显示下面，否则置顶显示
  if (cardY + cardHeight + infoHeight < gameHeight)
    infoY = cardY + cardHeight;
  else if (cardY - infoHeight >= 0)
    infoY = cardY - infoHeight;
  else
    infoY = 0;
}

// 强化材料
struct Supply
{
  char name[20]; // 名称
  int quality; // 品质：0=好，1=中，2=差
  COLORREF recipe[propHeight][propWidth]; // 配方图片
  COLORREF side[cardHeight][cardWidth]; // 副卡图片
  COLORREF info[infoHeight][infoWidth]; // 卡片信息
};
const int maxSupplyNum = 100; // 强化用卡上限
char supplyList[maxSupplyNum][maxPath]; // 强化用卡目录
Supply supply[maxSupplyNum]; // 强化用卡
int supplyNum; // 强化用卡数量

COLORREF starImage[maxStar + 1][cardHeight][cardWidth]; // 星级标志
const int jewelX = 158, jewelY = 25, jewelWidth = 55, jewelHeight = 55;
COLORREF jewel[maxStar + 1][jewelHeight][jewelWidth]; // 星级宝石

// 防御卡
struct Card
{
  int code; // 编号
  int star; // 星级
  int x, y; // 左上角位置
  // 检查鼠标pt是否悬停在本卡上，是则记录newHoverCard
  void CheckHover(POINT pt, Card *&newHoverCard)
  {
    if (pt.x >= x && pt.x < x + cardWidth && pt.y >= y && pt.y < y + cardHeight)
      newHoverCard = this;
  }
  // 显示卡片信息
  void ShowInfo(COLORREF(&dest)[gameHeight][gameWidth])
  {
    int infoX = 0, infoY = 0;
    GetInfoLocation(x, y, infoX, infoY);
    // 显示信息面板
    CopyMap(dest, infoX, infoY, infoWidth, infoHeight, supply[code].info);
    // 显示卡片星级
    TransCopyMap(dest, infoX + jewelX, infoY + jewelY, jewelWidth, jewelHeight,
      jewel[star]);
  }
};
const int maxCardNum = 56;
Card card[maxCardNum]; // 防御卡背包
int cardNum;

// 配方
struct Recipe
{
  int code; // 编号
  int quantity; // 数量
};
const int maxRecipeNum = 28;
Recipe recipe[maxRecipeNum];
int recipeNum;

// 四叶草
struct Clover
{
  int level; // 等级
  int quantity; // 数量
};
const int maxCloverNum = 11;
Clover clover[maxCloverNum];
int cloverNum;

// 香料
struct Spice
{
  int level; // 等级
  int quantity; // 数量
};
const int maxSpiceNum = 10;
Spice spice[maxSpiceNum];
int spiceNum;

// 所有参数
struct Param
{
  Card card[maxCardNum];
  int cardNum;
  Clover clover[maxCloverNum];
  int cloverNum;
  Spice spice[maxSpiceNum];
  int spiceNum;
  Recipe recipe[maxRecipeNum];
  int recipeNum;
  int gold;
  int token;
  int vip;
  int guild;
};
Param param; // 所有参数

Card deskCard[4]; // 工作台上的4张卡
int deskCardOrder[4]; // 工作台上4张卡的编号
Clover deskClover; // 工作台上的四叶草
int deskCloverOrder; // 工作台上的四叶草编号
Recipe deskRecipe; // 工作台上的配方
int deskRecipeOrder; // 工作台上的配方编号
Spice deskSpice; // 工作台上的香料
int deskSpiceOrder; // 工作台上的香料编号

// 更新卡片位置
void UpdateCardLocation()
{
  // 更新背包卡片位置
  for (int i = 0; i < cardNum; i++)
  {
    int row = i / 7;
    int column = i % 7;
    if (mode == 0)
    {
      card[i].x = makeX + column * cardWidth;
      card[i].y = makeY + row * cardHeight;
    }
    else if (mode == 1)
    {
      card[i].x = upgradeX + column * cardWidth;
      card[i].y = upgradeY + row * cardHeight;
    }
  }
  // 更新工作台卡片位置
  for (int i = 0; i < 4; i++)
  {
    deskCard[i].x = deskLoc[i].x;
    deskCard[i].y = deskLoc[i].y;
  }
}

// 载入防御卡背包
void LoadCard()
{
  cardNum = 0;
  const char path[] = "用户参数\\文本\\模拟器-防御卡.txt";
  if (!FileExist(path))
    return;

  FILE *f;
  fopen_s(&f, path, "r");
  char s[1000];
  fgets(f, s);
  while (strlen(s) > 0)
  {
    // 每行格式：编号\t星级
    sscanf_s(s, "%d\t%d", &card[cardNum].code, &card[cardNum].star);
    cardNum++;
    fgets(f, s);
  }
  fclose(f);
}

// 保存防御卡背包
void SaveCard()
{
  const char path[] = "用户参数\\文本\\模拟器-防御卡.txt";
  if (!FileExist(path))
    return;

  FILE *f;
  fopen_s(&f, path, "w");
  for (int i = 0; i < cardNum; i++)
    fprintf(f, "%d\t%d\n", card[i].code, card[i].star);
  fclose(f);
}

// 1-11级四叶草的图像
COLORREF cloverImage[12][propHeight][propWidth];
// 1-10级香料的图像
COLORREF spiceImage[11][propHeight][propWidth];

// 载入香料图片
void LoadSpiceImage()
{
  char path[maxPath];
  for (int level = 0; level < 11; level++)
  {
    sprintf_s(path, "附加程序\\图片\\合成\\香料\\%d.png", level);
    BitmapToColor(path, spiceImage[level]);
  }
}

// 载入四叶草图片
void LoadCloverImage()
{
  char path[maxPath];
  for (int level = 0; level < 12; level++)
  {
    sprintf_s(path, "附加程序\\图片\\合成\\四叶草\\%d.png", level);
    BitmapToColor(path, cloverImage[level]);
  }
}

// 载入四叶草背包
void LoadClover()
{
  cloverNum = 0;
  const char path[] = "用户参数\\文本\\模拟器-四叶草.txt";
  if (!FileExist(path))
    return;

  FILE *f;
  fopen_s(&f, path, "r");
  char s[1000];
  fgets(f, s);
  while (strlen(s) > 0)
  {
    // 每行格式：等级\t数量
    sscanf_s(s, "%d\t%d", &clover[cloverNum].level, &clover[cloverNum].quantity);
    cloverNum++;
    fgets(f, s);
  }
  fclose(f);
}

// 保存四叶草背包
void SaveClover()
{
  const char path[] = "用户参数\\文本\\模拟器-四叶草.txt";
  if (!FileExist(path))
    return;

  FILE *f;
  fopen_s(&f, path, "w");
  for (int i = 0; i < cloverNum; i++)
    fprintf(f, "%d\t%d\n", clover[i].level, clover[i].quantity);
  fclose(f);
}

// 载入配方背包
void LoadRecipe()
{
  recipeNum = 0;
  const char path[] = "用户参数\\文本\\模拟器-配方.txt";
  if (!FileExist(path))
    return;

  FILE *f;
  fopen_s(&f, path, "r");
  char s[1000];
  fgets(f, s);
  while (strlen(s) > 0)
  {
    // 每行格式：等级\t数量
    sscanf_s(s, "%d\t%d", &recipe[recipeNum].code, &recipe[recipeNum].quantity);
    recipeNum++;
    fgets(f, s);
  }
  fclose(f);
}

// 保存配方背包
void SaveRecipe()
{
  const char path[] = "用户参数\\文本\\模拟器-配方.txt";
  if (!FileExist(path))
    return;

  FILE *f;
  fopen_s(&f, path, "w");
  for (int i = 0; i < recipeNum; i++)
    fprintf(f, "%d\t%d\n", recipe[i].code, recipe[i].quantity);
  fclose(f);
}

// 载入香料背包
void LoadSpice()
{
  spiceNum = 0;
  const char path[] = "用户参数\\文本\\模拟器-香料.txt";
  if (!FileExist(path))
    return;

  FILE *f;
  fopen_s(&f, path, "r");
  char s[1000];
  fgets(f, s);
  while (strlen(s) > 0)
  {
    // 每行格式：等级\t数量
    sscanf_s(s, "%d\t%d", &spice[spiceNum].level, &spice[spiceNum].quantity);
    spiceNum++;
    fgets(f, s);
  }
  fclose(f);
}

// 保存香料背包
void SaveSpice()
{
  const char path[] = "用户参数\\文本\\模拟器-香料.txt";
  if (!FileExist(path))
    return;

  FILE *f;
  fopen_s(&f, path, "w");
  for (int i = 0; i < spiceNum; i++)
    fprintf(f, "%d\t%d\n", spice[i].level, spice[i].quantity);
  fclose(f);
}

void LoadParam0()
{
  LoadCard();
  LoadClover();
  LoadSpice();
  LoadRecipe();
  LoadGold();
}

void LoadParam()
{
  const char path[] = "用户参数\\文本\\模拟器.txt";
  FILE *f;
  fopen_s(&f, path, "rb");
  fread(&param, sizeof(param), 1, f);
  fclose(f);

  memcpy(card, param.card, sizeof(card));
  cardNum = param.cardNum;
  memcpy(clover, param.clover, sizeof(clover));
  cloverNum = param.cloverNum;
  memcpy(spice, param.spice, sizeof(spice));
  spiceNum = param.spiceNum;
  memcpy(recipe, param.recipe, sizeof(recipe));
  recipeNum = param.recipeNum;
  gold = param.gold;
  token = param.token;
  vip = param.vip;
  guild = param.guild;
}

void SaveParam0()
{
  SaveCard();
  SaveClover();
  SaveSpice();
  SaveRecipe();
  SaveGold();
}

void SaveParam()
{
  memcpy(param.card, card, sizeof(card));
  param.cardNum = cardNum;
  memcpy(param.clover, clover, sizeof(clover));
  param.cloverNum = cloverNum;
  memcpy(param.spice, spice, sizeof(spice));
  param.spiceNum = spiceNum;
  memcpy(param.recipe, recipe, sizeof(recipe));
  param.recipeNum = recipeNum;
  param.gold = gold;
  param.token = token;
  param.vip = vip;
  param.guild = guild;

  const char path[] = "用户参数\\文本\\模拟器.txt";
  FILE *f;
  fopen_s(&f, path, "wb");
  fwrite(&param, sizeof(param), 1, f);
  fclose(f);
}

// 强化用卡文件名排序比较函数（按字典序）
int CompareInnate(const void *vpName1, const void *vpName2)
{
  // 两个强化用卡文件名（A0巧克力面包 A1全能糖球）
  char(&name1)[260] = *(char(*)[260])vpName1;
  char(&name2)[260] = *(char(*)[260])vpName2;
  int type1 = (name1[0] - 'A') * 2 + name1[1] - '0';
  int type2 = (name2[0] - 'A') * 2 + name2[1] - '0';
  if (type1 != type2)
    return type1 - type2;
  int len1 = (int)strlen(name1);
  int len2 = (int)strlen(name2);
  if (len1 != len2)
    return len1 - len2;
  return strcmp(name1, name2);
}

// 载入强化材料
void LoadSupply()
{
  const char recipeFolder[] = "附加程序\\图片\\合成\\配方";
  const char sideFolder[] = "附加程序\\图片\\合成\\卡片";
  const char infoFolder[] = "附加程序\\图片\\合成\\卡片信息";
  if (!FileExist(recipeFolder) || !FileExist(sideFolder))
    return;

  char searchPath[maxPath] = {};
  // 查找内置卡片
  sprintf_s(searchPath, "%s\\*.png", sideFolder);
  int innateNum = GetFileList(searchPath, supplyList, maxSupplyNum);
  // 对内置卡片排序
  qsort(supplyList, innateNum, sizeof(supplyList[0]), CompareInnate);

  supplyNum = 0;
  char recipePath[maxPath], sidePath[maxPath], infoPath[maxPath];
  for (int i = 0; i < innateNum && supplyNum < maxSupplyNum; i++)
  {
    sprintf_s(recipePath, "%s\\%s", recipeFolder, supplyList[i]);
    sprintf_s(sidePath, "%s\\%s", sideFolder, supplyList[i]);
    sprintf_s(infoPath, "%s\\%s", infoFolder, supplyList[i]);
    if (FileExist(recipePath) && FileExist(sidePath)) // 如果文件存在
    {
      supply[supplyNum].quality = supplyList[i][0] - 'A'; // 品质
      supplyList[i][strlen(supplyList[i]) - 4] = 0;
      strcpy_s(supply[supplyNum].name, supplyList[i] + 2); // 名称
      BitmapToColor(recipePath, supply[supplyNum].recipe); // 配方图像
      BitmapToColor(sidePath, supply[supplyNum].side); // 副卡图像
      BitmapToColor(infoPath, supply[supplyNum].info); // 副卡图像
      supplyNum++;
    }
  }
}

// 显示一张卡片
void ViewCard(COLORREF(&dest)[gameHeight][gameWidth], int x0, int y0,
  int code, int star)
{
  const int shrink = 2; // 裁剪宽度
  const int width = cardWidth - 2 * shrink;
  const int height = cardHeight - 2 * shrink;
  // 显示卡片
  if (code == -1)
    CopyMap(dest, x0 + shrink, y0 + shrink, width, height,
      emptyCard, shrink, shrink);
  else
    CopyMap(dest, x0 + shrink, y0 + shrink, width, height,
      supply[code].side, shrink, shrink);
  // 显示星级
  for (int y = 0; y < cardHeight; y++)
    for (int x = 0; x < cardWidth; x++)
      if (starImage[star][y][x])
        dest[y0 + y][x0 + x] = starImage[star][y][x];
}

// 道具数量个位数位置
const int propDigitX = 38, propDigitY = 34, propDigitWidth = 6, propDigitHeight = 8;
const int propDigitDistance = propDigitWidth + 1;
COLORREF propDigit[10][propDigitHeight][propDigitWidth];

// 载入物品数字
void LoadPropDigit()
{
  char path[maxPath];
  for (int num = 0; num < 10; num++)
  {
    sprintf_s(path, "附加程序\\数字\\物品数量\\%d.png", num);
    BitmapToColor(path, propDigit[num]);
    for (int y = 0; y < propDigitHeight; y++)// 只保留白色，其他颜色涂黑
      for (int x = 0; x < propDigitWidth; x++)
        if (propDigit[num][y][x] != 0xffffff)
          propDigit[num][y][x] = 0;
  }
}

// 将整数num分解得到各位数字
int GetDigits(int num, int(&digits)[9])
{
  memset(digits, 0, sizeof(digits));
  if (num == 0) // 0视为一位数
    return 1;
  int digitsNum = 0;
  while (num > 0)
  {
    digits[digitsNum++] = num % 10;
    num = num / 10;
  }
  return digitsNum;
}

// 在图片dest指定位置写入物品数量quantity
template <int width, int height>
void WritePropQuantity(COLORREF(&dest)[height][width], int x0, int y0,
  int row, int column, int quantity)
{
  int totalLootX = x0 + column * propWidth;
  int totalLootY = y0 + row * propHeight;
  int digits[9] = {};
  // 把数量保存到截图中
  int digitsNum = GetDigits(quantity, digits);
  int quantityWidth = digitsNum * propDigitDistance + 1;
  const int quantityHeight = propDigitHeight + 2;
  int quantityTop = totalLootY + propDigitY - 1; // 数量上方Y
  int quantityRight = totalLootX + propDigitX + propDigitWidth; // 数量右侧X
  // 把数量区涂黑
  for (int y = quantityTop; y < quantityTop + quantityHeight; y++)
    for (int x = quantityRight - quantityWidth + 1; x <= quantityRight; x++)
      dest[y][x] = 0;
  // 填写各位数字
  for (int i = 0; i < digitsNum; i++)
  {
    int digitX = quantityRight - (i + 1) * propDigitDistance + 1;
    int digitY = quantityTop + 1;
    CopyMap(dest, digitX, digitY,
      propDigitWidth, propDigitHeight, propDigit[digits[i]], 0, 0);
  }
}

// 显示一个道具
void ViewProp(COLORREF(&dest)[gameHeight][gameWidth], int x0, int y0,
  COLORREF(&color)[propHeight][propWidth], int quantity)
{
  const int shrink = 2; // 裁剪宽度
  const int width = propWidth - 2 * shrink;
  const int height = propHeight - 2 * shrink;
  if (quantity == 0)
    CopyMap(dest, x0 + shrink, y0 + shrink, width, height,
      emptyProp, shrink, shrink);
  else
  {
    // 显示道具
    CopyMap(dest, x0 + shrink, y0 + shrink, width, height,
      color, shrink, shrink);
    // 显示数量
    if (quantity >= 2)
      WritePropQuantity(dest, x0, y0, 0, 0, quantity);
  }
}

// 制作按钮是否可用
bool IsMakeAvailable()
{
  if (deskRecipe.code == -1)
    return false;
  // 检验金币是否充足
  int star = deskSpice.level;
  int goldCost = goldMake[star];
  if (gold < goldCost)
    return false;
  return true;
}

// 强化按钮是否可用
bool IsUpgradeAvailable()
{
  bool sideExist = false;
  for (int i = 1; i <= 3; i++)
    if (deskCard[i].code >= 0)
      sideExist = true;
  if (deskCard[0].code == -1 || !sideExist)
    return false;
  // 不能强化21星卡
  if (deskCard[0].star == 21)
    return false;
  // 检验金币是否充足
  int star = deskCard[0].star;
  int goldCost = goldUpgrade[star];
  if (gold < goldCost)
    return false;
  return true;
}

// 向下翻页是否可用
bool IsPageDownAvailable()
{
  return cloverNum > 10 && cloverPage == 0;
}

// 向上翻页是否可用
bool IsPageUpAvailable()
{
  return cloverNum > 10 && cloverPage > 0;
}

// 计算当前面板的基础成功率
double GetBasicRate()
{
  // 1. 强化按钮未亮起显示0%
  if (!IsUpgradeAvailable())
    return 0;

  // 2. 计算主卡星级
  int main = deskCard[0].star;

  // 3. 对3张副卡星级从大到小排序
  Card side[4] = {};
  for (int i = 0; i < 4; i++)
  {
    side[i] = deskCard[i];
    if (deskCard[i].code == -1)
      side[i].star = -1;
  }
  for (int i = 1; i <= 3; i++)
    for (int j = i + 1; j <= 3; j++)
      if (side[i].star < side[j].star)
        Swap(&side[i], &side[j]);

  // 5. 计算副卡成功率
  double successRate = 0;
  double sideRate[4] = {};
  // 倒上成功率
  const double reverseRate[3] = { 0.88, 0.80, 0.72 };
  for (int i = 1; i <= 3; i++)
    if (side[i].star >= 0)
    {
      // 副卡质量
      int quality = supply[side[i].code].quality;
      // 倒上2星：100%
      if (side[i].star - main >= 2)
        sideRate[i] = 1;
      // 倒上1星
      else if (side[i].star - main == 1)
      {
        // 主卡3星及以上：88% 80% 72%
        if (main >= 3)
          sideRate[i] = reverseRate[quality];
        // 主卡2星及以下：100%
        else
          sideRate[i] = 1;
      }
      // 跨3星：0%
      else if (main - side[i].star >= 3)
        sideRate[i] = 0;
      // 跨0-2星
      else
        sideRate[i] = rates[main][main - side[i].star][quality];

      if (i == 1)
        successRate += sideRate[i];
      else
        successRate += sideRate[i] / 3;
    }

  // 考虑四叶草加成
  int clover = deskClover.level;
  successRate = successRate * cloverRate[clover] / 10;
  // 超过100%视为100%
  if (successRate >= 1)
    successRate = 1;
  return successRate;
}

// 计算加成
int GetBonus()
{
  return vipBonus[vip] + guildBonus[guild];
}

// 计算成功率
double GetRate()
{
  double rate = GetBasicRate() * (100 + GetBonus()) / 100;
  if (rate > 1)
    rate = 1;
  return rate;
}

// 成功率数字位置
const int rateX = 358, rateY = 464, rateWidth = 7, rateHeight = 10;
const int goldRequiredX = 214; // 金币需求数字位置
COLORREF rateDigit[13][rateHeight][rateWidth]; // 成功率数字图像
const COLORREF rateColor = 0xf5d389; // 成功率数字颜色
const COLORREF insuranceColor = 0xffc75f; // 保险金数字颜色

// 载入成功率数字
void LoadRateDigit()
{
  const char folder[] = "附加程序\\数字\\成功率";
  if (!FileExist(folder))
    ReportMissingFile(folder);

  char path[maxPath] = {};
  for (int i = 0; i < 13; i++)
  {
    sprintf_s(path, "%s\\%d.png", folder, i);
    BitmapToColor(path, rateDigit[i]);
  }
}

// 显示成功率字符
void ViewRateChar(COLORREF(&dest)[gameHeight][gameWidth],
  char rateChar, int order, int rateX)
{
  int digit = 0;
  if (rateChar == '%')
    digit = 10;
  else if (rateChar == '+')
    digit = 11;
  else if (rateChar == '.')
    digit = 12;
  else
    digit = rateChar - '0';
  for (int y = 0; y < rateHeight; y++)
    for (int x = 0; x < rateWidth; x++)
      if (rateDigit[digit][y][x] == 0xffffff)
        dest[rateY + y][rateX + order * rateWidth + x] = rateColor;
}

// 显示成功率字符串
void ViewRateString(COLORREF(&dest)[gameHeight][gameWidth],
  const char *rateString, int rateX)
{
  int length = (int)strlen(rateString);
  for (int i = 0; i < length; i++)
    ViewRateChar(dest, rateString[i], i, rateX);
}

// 显示成功率
void ViewRate(COLORREF(&dest)[gameHeight][gameWidth])
{
  char rateString[20] = "100%";
  if (mode == 1)
  {
    double basicRate = GetBasicRate();
    int bonus = GetBonus();
    if (bonus == 0 || basicRate == 0)
      sprintf_s(rateString, "%d%%", (int)(basicRate * 100));
    else
      sprintf_s(rateString, "%d%%+%.2lf%%", (int)(basicRate * 100),
        basicRate * bonus);
  }
  ViewRateString(dest, rateString, rateX);
}

// 显示需要的金币
void ViewGold(COLORREF(&dest)[gameHeight][gameWidth])
{
  char goldString[10] = "0";
  int goldRequired = 0;
  // 制卡金币
  if (mode == 0)
    goldRequired = deskRecipe.code == -1 ? 0 : goldMake[deskSpice.level];
  else if (mode == 1)
    goldRequired = deskCard[0].code == -1 ? 0 : goldUpgrade[deskCard[0].star];
  sprintf_s(goldString, "%d", goldRequired);
  ViewRateString(dest, goldString, goldRequiredX);
}

// 保险金数字
const int insuranceDigitX = 392, insuranceDigitY = 393;
const int insuranceDigitWidth = 5, insuranceDigitHeight = 8;
const int insuranceDigitDistance = 6;
COLORREF insuranceDigit[10][insuranceDigitHeight][insuranceDigitWidth];

// 载入保险金数字
void LoadInsuranceDigit()
{
  char path[maxPath];
  for (int num = 0; num < 10; num++)
  {
    sprintf_s(path, "附加程序\\数字\\列表房间号\\%d.png", num);
    BitmapToColor(path, insuranceDigit[num]);
  }
}

// 显示保险金
void ViewInsurance()
{
  int insuranceCost = 0;
  if (deskCard[0].code != -1)
    insuranceCost = tokenInsurance[deskCard[0].star];
  char insuranceString[20] = {};
  sprintf_s(insuranceString, "%d", insuranceCost);

  // 确定保险金显示位置
  int length = strlen(insuranceString);
  int left = insuranceDigitX - length * insuranceDigitDistance / 2;
  for (int i = 0; i < length; i++)
  {
    int charX = left + i * insuranceDigitDistance;
    int digit = insuranceString[i] - '0';
    for (int y = 0; y < insuranceDigitHeight; y++)
      for (int x = 0; x < insuranceDigitWidth; x++)
        if (insuranceDigit[digit][y][x] == 0xffffff)
          upgrade[insuranceDigitY + y][charX + x] = insuranceColor;
  }
}

const int greenWordWidth = 320, greenWordHeight = 62;
COLORREF greenWord[4][greenWordHeight][greenWordWidth]; // 绿字
const int greenWordX[4] = { 197, 154, 154, 231 }; // 绿字X
const int greenWordY0 = 178; // 绿字初始Y
QWORD greenWordT0; // 绿字初始时间
int greenWordType; // -1=无，0=失败，1=成功，2=制卡

// 显示绿字
void ViewGreenWord(COLORREF(&dest)[gameHeight][gameWidth])
{
  if (greenWordType == -1)
    return;
  // 计算绿字高度
  QWORD greenWordT = GetTickCount64();
  double ratio = (greenWordT - greenWordT0) / 4000.0;
  if (ratio >= 1)
    return;
  int greenWordY = greenWordY0 - (int)(50 * (1 - Sqr(1 - ratio)));
  // 显示绿字
  TransCopyMap(dest, greenWordX[greenWordType], greenWordY,
    greenWordWidth, greenWordHeight, greenWord[greenWordType]);
}

// 金币
const int goldX = 857, goldY = 556, goldWidth = 11, goldHeight = 14;
// 数字距离，每多一位数的偏移
const int goldDistance = 12, goldOffset = 6;
// 点券
const int tokenX = 709;
COLORREF goldDigit[10][goldHeight][goldWidth];

// 载入金币数字
void LoadGoldDigit()
{
  char path[maxPath];
  for (int num = 0; num < 10; num++)
  {
    sprintf_s(path, "附加程序\\数字\\金币\\%d.png", num);
    BitmapToColor(path, goldDigit[num]);
  }
}

// 显示金币/点券
void ViewGold(COLORREF(&dest)[gameHeight][gameWidth], int type)
{
  char str[20] = {};
  int length = 0;
  int realX = 0;
  if (type == 0)
  {
    sprintf_s(str, "%d", gold);
    length = (int)strlen(str);
    realX = goldX - length * goldOffset;
  }
  else
  {
    sprintf_s(str, "%d", token);
    length = (int)strlen(str);
    realX = tokenX - length * goldOffset;
  }
  for (int i = 0; i < length; i++)
    CopyMap(dest, realX + i * goldDistance, goldY,
      goldWidth, goldHeight, goldDigit[str[i] - '0']);
}

const int recipeX = 559, recipeY = 90; // 配方背包

Card *hoverCard = nullptr;      // 当前悬停的卡片
QWORD hoverStartTime = 0;       // 开始悬停时间
bool showCardInfo = false;      // 是否显示信息

// 保险金
const int insuranceX = 369, insuranceY = 404, insuranceWidth = 46, insuranceHeight = 42;
COLORREF insurance[insuranceHeight][insuranceWidth];
bool insuranceUsed; // 保险金是否使用

// 强化按钮
const int upgradeButtonX = 245, upgradeButtonY = 415,
upgradeButtonWidth = 81, upgradeButtonHeight = 43;
COLORREF upgradeButton[2][upgradeButtonHeight][upgradeButtonWidth];
bool isHoverUpgrade;
bool isPressUpgrade;

// 制作按钮
const int makeButtonX = 244, makeButtonY = 407,
makeButtonWidth = 81, makeButtonHeight = 41;
COLORREF makeButton[2][makeButtonHeight][makeButtonWidth];
bool isHoverMake;
bool isPressMake;

// 显示制卡界面
void ViewMake()
{
  CopyMap(make, makeBase);

  // 显示防御卡背包
  for (int i = 0; i < cardNum && i < 28; i++)
  {
    int row = i / 7;
    int column = i % 7;
    int left = makeX + column * cardWidth;
    int top = makeY + row * cardHeight;
    if (card[i].code >= 0)
      ViewCard(make, left, top, card[i].code, card[i].star);
  }

  // 显示配方
  for (int i = 0; i < recipeNum && i < 28; i++)
  {
    int row = i / 7;
    int column = i % 7;
    int left = recipeX + column * propWidth;
    int top = recipeY + row * propHeight;
    if (recipe[i].quantity > 0)
      ViewProp(make, left, top, supply[recipe[i].code].recipe,
        recipe[i].quantity);
  }

  // 显示配方槽
  if (deskRecipe.code >= 0)
    ViewProp(make, recipeSlotX, recipeSlotY, supply[deskRecipe.code].recipe, 1);

  // 显示香料
  for (int i = 0; i < spiceNum; i++)
    if (spice[i].quantity > 0)
      ViewProp(make, synthesisPropX + i * propWidth, synthesisPropY,
        spiceImage[spice[i].level], spice[i].quantity);

  // 显示香料槽
  if (deskSpice.level > 0)
    ViewProp(make, spiceSlotX, spiceSlotY, spiceImage[deskSpice.level], 5);

  // 显示制作按钮
  if (IsMakeAvailable())
  {
    if (isPressMake)
      CopyMap(make, makeButtonX, makeButtonY,
        makeButtonWidth, makeButtonHeight, makeButton[0]); // 按下（暗）
    else if (isHoverMake)
      CopyMap(make, makeButtonX, makeButtonY,
        makeButtonWidth, makeButtonHeight, makeButton[1]); // 悬停（亮）
    else
      CopyMap(make, makeButtonX, makeButtonY,
        makeButtonWidth, makeButtonHeight, makeButton[0]); // 默认
  }

  // 显示成功率和金币
  ViewRate(make);
  ViewGold(make);

  // 显示绿字
  ViewGreenWord(make);

  // 显示金币
  ViewGold(make, 0);
  ViewGold(make, 1);

  // 显示卡片信息
  if (showCardInfo && hoverCard && hoverCard->code != -1)
    hoverCard->ShowInfo(make);

  ColorToWindow(make);
}

// 显示VIP和公会等级
void PrintBonus()
{
  char bonusLevel[100] = {};
  sprintf_s(bonusLevel, "VIP%d + %d级合成屋", vip, guild);
  outtextxy(30, 30, bonusLevel);
}

// 显示强卡界面
void ViewUpgrade()
{
  CopyMap(upgrade, upgradeBase);

  // 显示防御卡背包
  for (int i = 0; i < cardNum; i++)
  {
    int row = i / 7;
    int column = i % 7;
    int left = upgradeX + column * cardWidth;
    int top = upgradeY + row * cardHeight;
    if (card[i].code >= 0)
      ViewCard(upgrade, left, top, card[i].code, card[i].star);
  }

  // 显示工作台
  for (int i = 0; i < 4; i++)
    if (deskCard[i].code >= 0)
      ViewCard(upgrade, deskLoc[i].x, deskLoc[i].y, deskCard[i].code, deskCard[i].star);

  // 显示四叶草
  for (int i = 0; i < 10; i++)
  {
    int order = i + cloverPage;
    if (order < cloverNum && clover[order].level > 0)
      ViewProp(upgrade, synthesisPropX + i * propWidth, synthesisPropY,
        cloverImage[clover[order].level], clover[order].quantity);
  }

  // 显示四叶草槽
  if (deskClover.level > 0)
    ViewProp(upgrade, cloverSlotX, cloverSlotY, cloverImage[deskClover.level], 1);

  // 显示强化按钮
  if (IsUpgradeAvailable())
  {
    if (isPressUpgrade)
      CopyMap(upgrade, upgradeButtonX, upgradeButtonY,
        upgradeButtonWidth, upgradeButtonHeight, upgradeButton[0]); // 按下（暗）
    else if (isHoverUpgrade)
      CopyMap(upgrade, upgradeButtonX, upgradeButtonY,
        upgradeButtonWidth, upgradeButtonHeight, upgradeButton[1]); // 悬停（亮）
    else
      CopyMap(upgrade, upgradeButtonX, upgradeButtonY,
        upgradeButtonWidth, upgradeButtonHeight, upgradeButton[0]); // 默认
  }

  // 显示成功率和金币
  ViewRate(upgrade);
  ViewGold(upgrade);

  // 显示绿字
  ViewGreenWord(upgrade);

  // 显示金币
  ViewGold(upgrade, 0);
  ViewGold(upgrade, 1);

  // 显示保险金打钩
  if (insuranceUsed)
    CopyMap(upgrade, insuranceX, insuranceY, insuranceWidth,
      insuranceHeight, insurance);

  // 显示保险金数字
  ViewInsurance();

  // 显示翻页按钮
  if (IsPageUpAvailable())
    CopyMap(upgrade, pageX, pageY[0], pageWidth, pageHeight, page[0]);
  if (IsPageDownAvailable())
    CopyMap(upgrade, pageX, pageY[1], pageWidth, pageHeight, page[1]);

  // 显示卡片信息
  if (showCardInfo && hoverCard && hoverCard->code != -1)
    hoverCard->ShowInfo(upgrade);

  ColorToWindow(upgrade);

  PrintBonus();
}
int lastClickMessage;
// 模拟器窗口过程函数
LRESULT CALLBACK SimulatorProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CLOSE:  // 关闭软件
  {
    ShowWindow(hWndSimulator, SW_HIDE);
    if (mode == 0)
    {
      // 卸香料
      if (deskSpice.level > 0)
      {
        spice[deskSpiceOrder].quantity += 5;
        deskSpice.level = 0;
        deskSpiceOrder = -1;
      }

      // 卸配方
      if (deskRecipe.code >= 0)
      {
        recipe[deskRecipeOrder].quantity++;
        deskRecipe.code = -1;
        deskRecipeOrder = -1;
      }
    }
    else if (mode == 1)
    {
      // 卸卡
      for (int i = 0; i < 4; i++)
        if (deskCard[i].code >= 0)
        {
          card[deskCardOrder[i]] = deskCard[i];
          deskCard[i].code = -1;
          deskCardOrder[i] = -1;
        }

      // 卸四叶草
      if (deskClover.level > 0)
      {
        clover[deskCloverOrder].quantity++;
        deskClover.level = 0;
        deskCloverOrder = -1;
      }
    }

    SaveParam();
    exit(0);
  }
  break;
  case WM_LBUTTONDOWN:
  {
    // 获取鼠标的x和y坐标
    int xPos = LOWORD(lParam);
    int yPos = HIWORD(lParam);
    // 传入自定义消息
    lastClickMessage = 100000000 + xPos * 10000 + yPos;
  }
  break;
  case WM_RBUTTONDOWN:
  {
    // 获取鼠标的x和y坐标
    int xPos = LOWORD(lParam);
    int yPos = HIWORD(lParam);
    // 传入自定义消息
    lastClickMessage = 200000000 + xPos * 10000 + yPos;
  }
  break;
  case WM_LBUTTONUP:
  {
    int xPos = LOWORD(lParam);
    int yPos = HIWORD(lParam);
    lastClickMessage = 300000000 + xPos * 10000 + yPos;
  }
  break;
  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
    // 处理其他消息...
  }
  // 调用默认窗口过程
  return 0;
}

// 标签位置
const RECT modeButton[7] = {
  { 87, 227, 127, 289 },
  { 87, 290, 127, 352 },
  { 87, 353, 127, 417 },
  { 87, 218, 127, 480 },
  { 442, 231, 482, 313 },
  { 442, 314, 482, 394 },
  { 442, 395, 482, 477 }
};

// 请求提交中
const int requestX = 340, requestY = 227, requestWidth = 267, requestHeight = 142;
COLORREF request[requestHeight][requestWidth];

// 删除第order张防御卡
void DeleteCard(int order)
{
  if (deskCardOrder[0] > order)
    deskCardOrder[0]--;
  for (int i = order; i < cardNum - 1; i++)
    card[i] = card[i + 1];
  card[cardNum - 1].code = -1;
  cardNum--;
}

// 删除第order个四叶草
void DeleteClover(int order)
{
  for (int i = order; i < cloverNum - 1; i++)
    clover[i] = clover[i + 1];
  clover[cloverNum - 1].level = 0;
  cloverNum--;
}

// 删除第order个香料
void DeleteSpice(int order)
{
  for (int i = order; i < spiceNum - 1; i++)
    spice[i] = spice[i + 1];
  spice[spiceNum - 1].level = 0;
  spiceNum--;
}

// 删除第order个配方
void DeleteRecipe(int order)
{
  for (int i = order; i < recipeNum - 1; i++)
    recipe[i] = recipe[i + 1];
  recipe[recipeNum - 1].code = -1;
  recipeNum--;
}

// 执行制作
void Make()
{
  if (cardNum == maxCardNum)
    return;

  // 0. 扣除金币
  gold -= goldMake[deskSpice.level];

  // 1. 新增防御卡
  card[cardNum].code = deskRecipe.code;
  card[cardNum].star = deskSpice.level;
  cardNum++;

  // 2. 如果有香料，扣除香料
  if (deskSpice.level > 0)
  {
    // 如果香料数量不少于5，直接从背包里扣
    if (spice[deskSpiceOrder].quantity >= 5)
      spice[deskSpiceOrder].quantity -= 5;
    // 否则清空台上的香料
    else
    {
      // 如果香料数量为0，删除香料
      if (spice[deskSpiceOrder].quantity == 0)
        DeleteSpice(deskSpiceOrder);
      deskSpice.level = 0;
      deskSpiceOrder = -1;
    }
  }

  // 3. 扣除配方
  // 如果配方数量不少于1，直接从背包里扣
  if (recipe[deskRecipeOrder].quantity >= 1)
    recipe[deskRecipeOrder].quantity--;
  else
  {
    // 如果配方数量为0，删除配方
    if (recipe[deskRecipeOrder].quantity == 0)
      DeleteRecipe(deskRecipeOrder);
    deskRecipe.code = -1;
    deskRecipeOrder = -1;
  }
}

// 执行强化
int Upgrade()
{
  // 0. 扣除金币和保险金
  gold -= goldUpgrade[deskCard[0].star];
  if (insuranceUsed)
    token -= tokenInsurance[deskCard[0].star];

  // 1. 生成强化结果
  bool success = false;
  double successRate = GetRate();
  // 保险有保底
  if (insuranceUsed)
  {
    // 计算累计投入概率
    double baseRate = base[deskCard[0].star] + successRate;
    // 累计投入概率大于1.35，下次必定成功
    if (baseRate > 1.35)
      success = true;
    // 否则用随机数决定
    else
    {
      double random = (double)rand() / (RAND_MAX + 1); // 0-1之间的随机数
      success = successRate > random;
    }
    // 成功则清空保底
    if (success)
      base[deskCard[0].star] = 0;
    // 失败则奖池累积
    else
      base[deskCard[0].star] += successRate;
  }
  // 不保险看运气
  else
  {
    double random = (double)rand() / (RAND_MAX + 1); // 0-1之间的随机数
    success = successRate > random;
  }

  // 2. 扣除副卡
  if (success || !insuranceUsed)
  {
    // 2.1 副卡按序号从大到小排序
    for (int i = 1; i <= 3; i++)
      for (int j = i + 1; j <= 3; j++)
        if (deskCardOrder[i] < deskCardOrder[j])
          Swap(&deskCardOrder[i], &deskCardOrder[j]);
    // 2.2 从防御卡背包删除副卡
    for (int i = 1; i <= 3; i++)
      if (deskCardOrder[i] != -1)
        DeleteCard(deskCardOrder[i]);
    // 2.3 清空工作台副卡
    for (int i = 1; i <= 3; i++)
    {
      deskCard[i].code = -1;
      deskCardOrder[i] = -1;
    }
  }

  // 3. 扣除四叶草
  if (deskClover.level > 0)
  {
    if (clover[deskCloverOrder].quantity == 0)
      DeleteClover(deskCloverOrder);
    deskClover.level = 0;
    deskCloverOrder = -1;
  }

  // 4. 变更主卡星级
  if (success)
    deskCard[0].star++;
  else if (deskCard[0].star >= 6 && !insuranceUsed)
    deskCard[0].star--;

  cloverPage = 0;

  return success ? 1 : 0;
}

// 编辑卡片制作
void EditMake()
{
  int xPos, yPos, buttons; // 鼠标按下
  ViewMake();
  while (true)// 循环检测鼠标事件
  {
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWndSimulator, &pt);

    Card *newHoverCard = nullptr; // 新的悬停卡片
    UpdateCardLocation(); // 更新卡片位置
    // 检测悬停卡片
    for (int i = 0; i < cardNum && i < 28; i++)
      card[i].CheckHover(pt, newHoverCard);

    // 如果换了一张卡
    if (newHoverCard != hoverCard)
    {
      hoverCard = newHoverCard;
      hoverStartTime = GetTickCount64();
      showCardInfo = false;
    }
    else
    {
      // 同一张卡，判断时间
      if (hoverCard && GetTickCount64() - hoverStartTime >= 500)
        showCardInfo = true;
    }

    bool inButton =
      pt.x >= makeButtonX && pt.x <= makeButtonX + makeButtonWidth &&
      pt.y >= makeButtonY && pt.y <= makeButtonY + makeButtonHeight;

    if (lastClickMessage > zero) // 检测到鼠标按下
    {
      buttons = lastClickMessage / 100000000;
      xPos = lastClickMessage % 100000000 / 10000;
      yPos = lastClickMessage % 10000;
      lastClickMessage = 0;

      if (buttons == 1)
      {
        // 按下制作按钮
        if (IsMakeAvailable() && inButton)
        {
          isPressMake = true;
          ViewMake(); // 立即刷新显示“按下效果”
          continue;
        }

        // 点击标签
        for (int i = 0; i < 7; i++)
          if (xPos >= modeButton[i].left && xPos <= modeButton[i].right &&
            yPos >= modeButton[i].top && yPos <= modeButton[i].bottom)
          {
            if (i >= 2)
              PopMessage(hWndSimulator, "该功能暂未开放。");
            // 切换到强卡界面
            else if (i == 1)
            {
              cloverPage = 0;

              // 卸香料
              if (deskSpice.level > 0)
              {
                spice[deskSpiceOrder].quantity += 5;
                deskSpice.level = 0;
                deskSpiceOrder = -1;
              }

              // 卸配方
              if (deskRecipe.code >= 0)
              {
                recipe[deskRecipeOrder].quantity++;
                deskRecipe.code = -1;
                deskRecipeOrder = -1;
              }

              mode = 1;
              return;
            }
            break;
          }

        // 点击配方
        if (xPos >= recipeX && xPos < recipeX + 7 * propWidth &&
          yPos >= recipeY && yPos < recipeY + 4 * propHeight)
        {
          int row = (yPos - recipeY) / propHeight;
          int column = (xPos - recipeX) / propWidth;
          int order = row * 7 + column;
          if (order < recipeNum && recipe[order].code >= 0)
          {
            // 如果台上有配方，下配方
            if (deskRecipe.code >= 0)
            {
              recipe[deskRecipeOrder].quantity++;
              deskRecipe.code = -1;
              deskRecipeOrder = -1;
            }
            // 上配方
            recipe[order].quantity--;
            deskRecipe.code = recipe[order].code;
            deskRecipe.quantity = 1;
            deskRecipeOrder = order;
          }
        }

        // 点击配方槽
        if (xPos >= recipeSlotX && xPos < recipeSlotX + propWidth &&
          yPos >= recipeSlotY && yPos < recipeSlotY + propHeight)
          // 如果台上有配方，下配方
          if (deskRecipe.code >= 0)
          {
            recipe[deskRecipeOrder].quantity++;
            deskRecipe.code = -1;
            deskRecipeOrder = -1;
          }

        // 点击香料
        if (xPos >= synthesisPropX && xPos < synthesisPropX + 10 * propWidth &&
          yPos >= synthesisPropY && yPos < synthesisPropY + propHeight)
        {
          int order = (xPos - synthesisPropX) / propWidth;
          if (order < spiceNum && spice[order].quantity >= 5)
          {
            // 如果台上有香料，下香料
            if (deskSpice.level > 0)
            {
              spice[deskSpiceOrder].quantity += 5;
              deskSpice.level = 0;
              deskSpiceOrder = -1;
            }
            // 上香料
            spice[order].quantity -= 5;
            deskSpiceOrder = order;
            deskSpice.level = spice[order].level;
            deskSpice.quantity = 5;
          }
        }

        // 点击香料槽
        if (xPos >= spiceSlotX && xPos < spiceSlotX + propWidth &&
          yPos >= spiceSlotY && yPos < spiceSlotY + propHeight)
        {
          // 如果台上有香料，下香料
          if (deskSpice.level > 0)
          {
            spice[deskSpiceOrder].quantity += 5;
            deskSpice.level = 0;
            deskSpiceOrder = -1;
          }
        }
      }
      // 鼠标松开
      else if (buttons == 3)
      {
        if (isPressMake && inButton)
        {
          // 显示“请求提交中”
          TransCopyMap(make, requestX, requestY, requestWidth, requestHeight,
            request);
          ColorToWindow(make);
          Sleep(300);
          // 执行制作
          Make();
          greenWordT0 = GetTickCount64();
          greenWordType = 2;
        }
        isPressMake = false;
      }
      //SaveParameter();
      ViewMake();
    }
    // 如果制作按钮亮度变化，也要重绘
    else if (inButton != isHoverMake)
    {
      isHoverMake = inButton;
      ViewMake();
    }
    // 绿字存在时要不断重绘
    else if (GetTickCount64() - greenWordT0 < 4100 && sleepTime % 10 == 0)
      ViewMake();

    sleepTime++;
    if (sleepTime % 10 == zero)
      Sleep(1);
    if (sleepTime >= 100)
    {
      ViewMake();
      sleepTime = 0;
    }
  }
}

// 加成设置对话框过程函数
INT_PTR CALLBACK BonusProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    // 对话框位置居中
    CenterDialog(hDlg);

    // VIP
    HWND hCombo = GetDlgItem(hDlg, idVip);
    char level[10] = {};
    for (int i = 0; i <= 16; i++)
    {
      sprintf_s(level, "%d", i);
      ComboBox_AddString(hCombo, level); // 添加组合框选项
    }
    ComboBox_SetCurSel(hCombo, vip); // 设置默认选中

    // 公会
    hCombo = GetDlgItem(hDlg, idGuild);
    for (int i = 0; i <= 6; i++)
    {
      sprintf_s(level, "%d", i);
      ComboBox_AddString(hCombo, level); // 添加组合框选项
    }
    ComboBox_SetCurSel(hCombo, guild); // 设置默认选中
  }
  return TRUE;
  case WM_COMMAND:
  {
    int id = LOWORD(wParam);
    if (id == IDOK)
    {
      // VIP
      HWND hCombo = GetDlgItem(hDlg, idVip);
      vip = ComboBox_GetCurSel(hCombo);

      // 公会
      hCombo = GetDlgItem(hDlg, idGuild);
      guild = ComboBox_GetCurSel(hCombo);

      EndDialog(hDlg, id);
      return TRUE;
    }
    else if (id == IDCANCEL)
    {
      EndDialog(hDlg, id);
      return TRUE;
    }
  }
  break;
  }
  return FALSE;
}

int EditBonus()
{
  return (int)DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_BONUS),
    hWndSimulator, BonusProc, 0);
}

// 编辑卡片强化
void EditUpgrade()
{
  int xPos, yPos, buttons; // 鼠标按下
  ViewUpgrade();
  while (true)// 循环检测鼠标事件
  {
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hWndSimulator, &pt);

    Card *newHoverCard = nullptr; // 新的悬停卡片
    UpdateCardLocation(); // 更新卡片位置
    // 检测悬停卡片
    for (int i = 0; i < cardNum; i++)
      card[i].CheckHover(pt, newHoverCard);
    for (int i = 0; i < 4; i++)
      deskCard[i].CheckHover(pt, newHoverCard);

    // 如果换了一张卡
    if (newHoverCard != hoverCard)
    {
      hoverCard = newHoverCard;
      hoverStartTime = GetTickCount64();
      showCardInfo = false;
    }
    else
    {
      // 同一张卡，判断时间
      if (hoverCard && GetTickCount64() - hoverStartTime >= 500)
        showCardInfo = true;
    }

    bool inButton =
      pt.x >= upgradeButtonX && pt.x <= upgradeButtonX + upgradeButtonWidth &&
      pt.y >= upgradeButtonY && pt.y <= upgradeButtonY + upgradeButtonHeight;

    if (lastClickMessage > zero) // 检测到鼠标按下
    {
      buttons = lastClickMessage / 100000000;
      xPos = lastClickMessage % 100000000 / 10000;
      yPos = lastClickMessage % 10000;
      lastClickMessage = 0;

      if (buttons == 1)
      {
        // 按下强化按钮
        if (IsUpgradeAvailable() && inButton)
        {
          isPressUpgrade = true;
          ViewUpgrade(); // 立即刷新显示“按下效果”
          continue;
        }

        // 调整VIP公会等级
        if (xPos < 190 && yPos < 60)
          EditBonus();

        // 点击标签
        for (int i = 0; i < 7; i++)
          if (xPos >= modeButton[i].left && xPos <= modeButton[i].right &&
            yPos >= modeButton[i].top && yPos <= modeButton[i].bottom)
          {
            if (i >= 2)
              PopMessage(hWndSimulator, "该功能暂未开放。");
            // 切换到制卡界面
            else if (i == 0)
            {
              // 卸卡
              for (int i = 0; i < 4; i++)
                if (deskCard[i].code >= 0)
                {
                  card[deskCardOrder[i]] = deskCard[i];
                  deskCard[i].code = -1;
                  deskCardOrder[i] = -1;
                }

              // 卸四叶草
              if (deskClover.level > 0)
              {
                clover[deskCloverOrder].quantity++;
                deskClover.level = 0;
                deskCloverOrder = -1;
              }

              mode = 0;
              return;
            }
            break;
          }

        // 点击卡片
        if (xPos >= upgradeX && xPos < upgradeX + 7 * cardWidth &&
          yPos >= upgradeY && yPos < upgradeY + 8 * cardHeight)
        {
          int row = (yPos - upgradeY) / cardHeight;
          int column = (xPos - upgradeX) / cardWidth;
          int order = row * 7 + column;
          // 如果选中了一张不在台上的卡
          if (order < cardNum && card[order].code >= 0)
          {
            // 2. 如果台上没有主卡，上主卡
            if (deskCard[0].code == -1)
            {
              deskCardOrder[0] = order;
              deskCard[0] = card[order];
              card[order].code = -1;
            }
            // 3. 如果台上有主卡，无副卡1，上副卡1
            else if (deskCard[0].code >= 0 && deskCard[1].code == -1)
            {
              deskCardOrder[1] = order;
              deskCard[1] = card[order];
              card[order].code = -1;
            }
            // 4. 如果台上有主卡、副卡1，无副卡2，上副卡2
            else if (deskCard[0].code >= 0 && deskCard[1].code >= 0 && deskCard[2].code == -1)
            {
              deskCardOrder[2] = order;
              deskCard[2] = card[order];
              card[order].code = -1;
            }
            // 5. 如果台上有主卡、副卡1、副卡2，无副卡3，上副卡3
            else if (deskCard[0].code >= 0 && deskCard[1].code >= 0 && deskCard[2].code >= 0 &&
              deskCard[3].code == -1)
            {
              deskCardOrder[3] = order;
              deskCard[3] = card[order];
              card[order].code = -1;
            }
            // 6. 如果三张卡都有，什么也不做
          }
        }

        // 点击工作台（卸卡）
        for (int i = 0; i < 4; i++)
          if (xPos >= deskLoc[i].x && xPos < deskLoc[i].x + cardWidth &&
            yPos >= deskLoc[i].y && yPos < deskLoc[i].y + cardHeight)
            if (deskCard[i].code >= 0)
            {
              card[deskCardOrder[i]] = deskCard[i];
              deskCard[i].code = -1;
              deskCardOrder[i] = -1;
            }

        // 点击四叶草道具栏
        if (xPos >= synthesisPropX && xPos < synthesisPropX + 10 * propWidth &&
          yPos >= synthesisPropY && yPos < synthesisPropY + propHeight)
        {
          int order = (xPos - synthesisPropX) / propWidth + cloverPage;
          if (order < cloverNum && clover[order].level > 0)
          {
            // 如果台上有四叶草，下四叶草
            if (deskClover.level > 0)
            {
              clover[deskCloverOrder].quantity++;
              deskClover.level = 0;
              deskCloverOrder = -1;
            }
            // 上四叶草
            clover[order].quantity--;
            deskCloverOrder = order;
            deskClover.level = clover[order].level;
            deskClover.quantity = 1;
          }
        }

        // 点击四叶草槽
        if (xPos >= cloverSlotX && xPos < cloverSlotX + propWidth &&
          yPos >= cloverSlotY && yPos < cloverSlotY + propHeight)
        {
          // 如果台上有四叶草，下四叶草
          if (deskClover.level > 0)
          {
            clover[deskCloverOrder].quantity++;
            deskClover.level = 0;
            deskCloverOrder = -1;
          }
        }

        // 点击四叶草翻页
        if (IsPageUpAvailable())
          if (xPos >= pageX && xPos < pageX + pageWidth &&
            yPos >= pageY[0] && yPos < pageY[0] + pageHeight)
            cloverPage--;
        if (IsPageDownAvailable())
          if (xPos >= pageX && xPos < pageX + pageWidth &&
            yPos >= pageY[1] && yPos < pageY[1] + pageHeight)
            cloverPage++;

        // 点击保险金
        if (xPos >= insuranceX && xPos < insuranceX + insuranceWidth &&
          yPos >= insuranceY && yPos < insuranceY + insuranceHeight)
          insuranceUsed = !insuranceUsed;
      }
      // 鼠标松开
      else if (buttons == 3)
      {
        if (isPressUpgrade && inButton)
        {
          // 如果勾选了保险金且保险金不够
          if (insuranceUsed && token < tokenInsurance[deskCard[0].star])
          {
            // 显示绿字
            greenWordT0 = GetTickCount64();
            greenWordType = 3;
          }
          else
          {
            // 显示“请求提交中”
            TransCopyMap(upgrade, requestX, requestY, requestWidth, requestHeight,
              request);
            ColorToWindow(upgrade);
            // 显示VIP和公会等级
            PrintBonus();
            Sleep(300);
            // 执行强化
            bool success = Upgrade();
            // 显示绿字
            greenWordT0 = GetTickCount64();
            if (success)
              greenWordType = 1;
            else
              greenWordType = 0;
          }
        }
        isPressUpgrade = false;
      }

      //SaveParameter();
      ViewUpgrade();
    }
    // 如果强化按钮亮度变化，也要重绘
    else if (inButton != isHoverUpgrade)
    {
      isHoverUpgrade = inButton;
      ViewUpgrade();
    }
    // 绿字存在时要不断重绘
    else if (GetTickCount64() - greenWordT0 < 4100 && sleepTime % 10 == 0)
      ViewUpgrade();

    sleepTime++;
    if (sleepTime % 10 == zero)
      Sleep(1);
    if (sleepTime >= 100)
    {
      sleepTime = 0;
      ViewUpgrade();
    }
  }
}

// 模拟器初始化
void InitSimulator()
{
  char path[maxPath];
  srand(GetTickCount());

  // 1. 图片素材
  // 载入制卡界面和强卡界面图片
  BitmapToColor("附加程序\\图片\\合成\\控件\\制卡界面.png", makeBase);
  BitmapToColor("附加程序\\图片\\合成\\控件\\强卡界面.png", upgradeBase);
  // 载入制作按钮
  BitmapToColor("附加程序\\图片\\合成\\控件\\制作按钮.png", makeButton[0]);
  BitmapToColor("附加程序\\图片\\合成\\控件\\制作按钮_亮.png", makeButton[1]);
  // 载入强化按钮
  BitmapToColor("附加程序\\图片\\合成\\控件\\强化按钮.png", upgradeButton[0]);
  BitmapToColor("附加程序\\图片\\合成\\控件\\强化按钮_亮.png", upgradeButton[1]);
  // 载入请求提交中
  BitmapToColor("附加程序\\图片\\合成\\控件\\请求提交中.png", request);
  // 载入保险金
  BitmapToColor("附加程序\\图片\\合成\\控件\\保险金.png", insurance);
  // 载入绿字
  BitmapToColor("附加程序\\图片\\合成\\控件\\失败绿字.png", greenWord[0]);
  BitmapToColor("附加程序\\图片\\合成\\控件\\成功绿字.png", greenWord[1]);
  BitmapToColor("附加程序\\图片\\合成\\控件\\制作绿字.png", greenWord[2]);
  BitmapToColor("附加程序\\图片\\合成\\控件\\点券不足绿字.png", greenWord[3]);
  // 载入翻页按钮
  BitmapToColor("附加程序\\图片\\合成\\控件\\向上翻页.png", page[0]);
  BitmapToColor("附加程序\\图片\\合成\\控件\\向下翻页.png", page[1]);
  // 载入空卡片、空物品
  BitmapToColor("附加程序\\图片\\合成\\空卡片.png", emptyCard);
  BitmapToColor("附加程序\\图片\\合成\\空物品.png", emptyProp);
  // 载入星级宝石
  for (int star = 0; star <= maxStar; star++)
  {
    sprintf_s(path, "附加程序\\图片\\合成\\星级\\%d.png", star);
    BitmapToColor(path, starImage[star]);
    sprintf_s(path, "附加程序\\图片\\合成\\大星级\\%d.png", star);
    BitmapToColor(path, jewel[star]);
  }
  // 载入强化材料
  LoadSupply();
  // 载入保险金数字
  LoadInsuranceDigit();
  // 载入金币数字
  LoadGoldDigit();
  // 载入道具数字
  LoadPropDigit();
  // 载入成功率数字
  LoadRateDigit();
  // 载入四叶草图片
  LoadCloverImage();
  // 载入香料图片
  LoadSpiceImage();

  // 2. 程序参数
  LoadUpgradeParam();

  // 3. 用户数据
  LoadParam();

  // 4. 初始化数据
  // 工作台上的卡、四叶草、配方、香料清空
  for (int i = 0; i < 4; i++)
  {
    deskCard[i].code = -1;
    deskCardOrder[i] = -1;
  }
  deskClover.level = 0;
  deskCloverOrder = -1;
  deskRecipe.code = -1;
  deskRecipeOrder = -1;
  deskSpice.level = 0;
  deskSpiceOrder = -1;
  // 无绿字
  greenWordType = -1;
}

int main()
{
  // 初始化模拟器
  InitSimulator();

  // 打开窗口
  hWndSimulator = initgraph(gameWidth, gameHeight);

  // 设置自定义窗口过程
  SetWindowLongPtr(hWndSimulator, GWLP_WNDPROC, (LONG_PTR)SimulatorProc);

  char wndTitle[maxPath] = {};
  sprintf_s(wndTitle, versionString, version); // 设置窗口标题

  CenterShow(hWndSimulator);
  SetForegroundWindow(hWndSimulator);
  SetWindowText(hWndSimulator, wndTitle);

  HINSTANCE hInstance = GetModuleHandle(NULL);
  HICON hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_MAIN), IMAGE_ICON,
    GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
  SendMessage(hWndSimulator, WM_SETICON, ICON_BIG, (LPARAM)hIcon); // 设置图标
  SendMessage(hWndSimulator, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

  // 字体设置
  setbkmode(TRANSPARENT);     //透明字体
  LOGFONT f;
  gettextstyle(&f);						//获取当前字体设置
  f.lfHeight = 20;						//设置字体高度
  f.lfWeight = 1000;						//设置字体厚度
  strcpy_s(f.lfFaceName, "等线");		//设置字体为等线
  f.lfQuality = ANTIALIASED_QUALITY;//设置输出效果为抗锯齿
  settextstyle(&f);						//设置字体样式

  // 编辑循环
  while (true)
    if (mode == 0)
      EditMake();
    else if (mode == 1)
      EditUpgrade();
  closegraph();
  return 0;
}