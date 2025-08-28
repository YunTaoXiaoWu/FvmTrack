#ifndef _DEBUG
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")
#endif
#include "resource.h"
#include "..\\track.h"

const char versionString[] = "轨道执行器%s by -云涛晓雾- 所属文件夹：%s";
bool developerMode;//开发者模式
HANDLE hMutexUserImage; //读写用户截图时的互斥锁
HANDLE hMutexLoot; //读写战利品时的互斥锁
HANDLE hMutexProp; //读写道具时的互斥锁
HANDLE hMutexSaveParameter; //保存执行器参数和列表时的互斥锁
HANDLE hMutexLoadList; //保存执行器参数和列表时的互斥锁
HANDLE hMutexSaveList; //保存执行器参数和列表时的互斥锁
HANDLE hMutexTrack; //读取轨道文件的互斥锁
HANDLE hMutexTray; //读取承载方案的互斥锁
bool isLoadFinished;//载入资源线程是否完成
bool isUpdateFinished;//下载update.txt线程是否完成
bool isLoadAndUpdateFinished;//载入资源和下载update.txt是否均完成
//1. 软件位置坐标
const int wndWidth = 1368, wndHeight = 745;//窗口宽度和高度
const int tipHeight = 27;
const int dialogWidth = 488, dialogHeight = 161;//输入框尺寸
const int playerX = 610, playerY = 5, playerDistance = 275;//1P/2P的服务器和二级密码
const int titleX = 28, titleY = 30, titleWidth = 90, titleHeight = 40;//左侧选项位置
const int parameterX = titleX + titleWidth, parameterY = 30, parameterWidth = 222, parameterHeight = 40;//参数表信息显示
const int advanceX = parameterX + parameterWidth + 2, advanceY = 30;
const int advanceTitleNum = 10;//任务列表选项数量
//任务列表各列宽度
const int advanceWidth[advanceTitleNum] = { 68, 60, 96, 50, 44, 44, parameterWidth - 4, 44, parameterWidth - 4, 44 };
//任务列表各列高度
const int advanceHeight[17] = { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 41, 41, 40, 40, 40, 40 };
const int modeX = wndWidth - 123, modeY = 30, modeWidth = 95, modeHeight = 40;//模式切换
const int zoomX = modeX, zoomY = 0, zoomWidth = modeWidth, zoomHeight = modeY;//缩放按钮
const int settingsX = modeX, settingsY = 30 + 6 * modeHeight, settingsWidth = 95, settingsHeight = 40;//右侧按钮
const int upperSettingsY = settingsY - 7 * settingsHeight / 2;//右侧按钮上区纵坐标
const int lockX = parameterX + 5 * parameterWidth - 65, lockY = 5, lockWidth = 20, lockHeight = 20;//任务列表锁定
//任务列表选项标识符
const int ID = 0, TYPE = 1, LEVEL = 2, GAMES = 3, KEY = 4, HOST = 5,
TRACK_0 = 6, DECK_0 = 7, TRACK_1 = 8, DECK_1 = 9, CHECK = 10;
//识别任务标识符：0当前，1公会任务，2情侣任务，3大赛任务
const int ADVANCE = 0, GUILD = 1, LOVER = 2, CONTEST = 3, CAMP = 4, DAILY = 5;

//2. 游戏内位置坐标
const int guildX = 136, guildWidth = 140, guildHeight = 18;//公会任务名称位置
const int vipX = 27, vipY = 94, vipWidth = 38, vipHeight = 38, vipDistance = 178;//vip框
const int frontX = 860, frontY = 46, frontWidth = 1, frontHeight = 64;//前线（用于检测出怪）
const int lightNameX = 280, lightNameY = 188, lightNameWidth = 83, lightNameHeight = 12;//好友列表细角色名
const int heavyNameX = 108, heavyNameY = 38, heavyNameWidth = 90, heavyNameHeight = 12;//左上角粗角色名
const int templateX = 24, templateY = 374, templateWidth = 60, templateHeight = 64;//卡片模板（用于补阵）
//卡片宽模板（用于生成需要偏移的模板）
const int wideTemplateX = templateX - 20, wideTemplateY = templateY, wideTemplateWidth = 120, wideTemplateHeight = 74;
const int objectX = 304, objectY = 98;//地图1行1列识图位置
const int moveToTemplateX = templateX + 28, moveToTemplateY = templateY + 42;//放置模板时鼠标移动到的位置
const int startX = 875, startY = 453;//“开始”按钮
const int slotX1 = 228, slotY1 = 11, slotWidth = 53, slotHeight = 68;//卡槽1位置，卡槽宽度、高度
const int gridX = 302, gridY = 105;//地图1行1列
const int innateGridX = 299, innateGridY = 110; //初始地图格子判别位置
const int skillX = 22, skillY = 208, skillHeight = 44; //技能宝石
const POINT proceed = { 471, 349 };//继续作战
const POINT claim = { 568, 349 };//领取奖励
const POINT flip[5] = { { 577, 267 }, { 724, 267 }, { 871, 267 }, { 577, 467 }, { 724, 467 } };//5张牌的翻牌位置
const POINT finish = { 710, 495 };//翻牌“完成”
const int deckX = 424, deckY = 121, deckWidth = 83;//卡组1，卡组宽度
const POINT expNoMoreTip = { 430, 332 };//经验满不再提示
const int bagConfirmX = 430, bagConfirmY = 358;//背包满、经验满确认
const int bagCancelX = 528, bagCancelY = 358;//背包满取消
const POINT retreat = { 915, 560 };//后退（退出房间）
const POINT worldMap = { 862, 41 };//世界地图按钮
const POINT blueBox = { 358, 316 };//“正在登陆服务器”蓝框消失判定位置

//矩形区域类型(x,y,width,height)
struct MyRect
{
  int x, y, width, height;
};
const int customScrollX = 930, customScrollY = 196, customScrollHeight = 196;
const int lootScrollX = 708, lootScrollY = 472, lootScrollHeight = 64;
//战利品格子
const int lootGridX = 209, lootWidth = 49, lootHeight = 49;
//战利品识别区
const int lootCoreX = 5, lootCoreY = 5, lootCoreWidth = 36, lootCoreHeight = 28;
const COLORREF scrollBlankColor = 0x054971;//滚动条空白处颜色

//3. 高级任务常数
const int basicTypeNum = 6; //基础类型数量（悬赏/勇士/魔塔/跨服/假期/公会）
const int islandTypeNum = 11; //岛屿类型数量（美味/火山/遗迹/浮空/海底/星际+营地/沙漠/雪山/雷城/奇境）
const int operationTypeNum = 2;//操作类型数量
const int maxTypeNum = basicTypeNum + islandTypeNum + operationTypeNum;//高级任务类型数量
const int maxAdvancePage = 12;//任务列表最大页数
const int advanceNumPerPage = 16;//任务列表每页任务数
const int maxAdvanceNum = advanceNumPerPage * maxAdvancePage;//任务列表容量
const char bounty[4][20] = { "美味", "火山", "浮空", "星际" };//悬赏关卡名
const int challengeNum = 23;//勇士关卡数
const char challenge[challengeNum][13] = { //勇士关卡名
  "洞君", "阿诺", "冰渣", "轰隆隆", "法老", "保罗", "玛丽", "皮特", "使者", "男爵", "朱莉", "中尉",
  "杰克", "金刚", "小明", "Baby", "美队", "钢铁侠", "绿巨人", "蜘蛛侠", "列车", "水母", "鲨鱼" };
const char crossServer[6][20] = { "古堡", "天空", "炼狱", "水火", "巫毒", "冰封" };//跨服关卡名
const char instance[5][20] = { "假期", "月光", "堕落", "死亡", "巅峰" };//副本关卡
const char missionName[3][20] = { "公会", "情侣", "大赛" };//识别任务类型
const char fullMissionName[3][20] = { "公会任务", "情侣任务", "美食大赛" };//识别任务全名
const char special[8][20] = { "签到", "施肥", "清包", "双经卡", "双爆卡", "买魔塔", "发任务", "检查" };//特殊任务名称
const char control[5][20] = { "定时", "刷新", "退服", "关机", "循环" };//控制任务名称
const char serverName[7][20] = { //服务器名称
  "最近登录", "3366_1服", "3366_2服", "3366_3服",
  "3366_4服", "3366_5服", "3366_6服" };
//任务类型名称
char typeName[maxTypeNum][7] = { "悬赏", "勇士", "魔塔", "跨服", "副本", "任务",
"", "", "", "", "", "", "", "", "", "", "", "特殊", "控制" };
int levelNum[maxTypeNum] = { //每种类型的关卡数量
  4, challengeNum, 3005, 48, 5, 3,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 5 };
int defaultLevel[maxTypeNum] = { //每种类型的默认关卡
  0, 17, 1151, 35, 0, 0,
  7, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const int maxIslandLevelNum = 20;//岛屿类型最大关卡数量
char islandLevel[islandTypeNum][maxIslandLevelNum][13];//各岛屿各关卡名称
char islandPrompt[islandTypeNum][300];//各岛屿输入关卡时的提示语
POINT islandLocation[islandTypeNum];//各岛屿的位置
POINT islandLevelButton[islandTypeNum][maxIslandLevelNum];//各岛屿各关卡的按钮位置

//高级任务类型
struct Advance
{
  //任务参数
  int isSelected;//是否勾选
  int type;//任务类型：0悬赏 1勇士 2魔塔 3跨服 4假期 5公会 6-n地图
  int level;//关卡
  int maxGames;//执行局数
  int isProceed;//是否继续作战
  int host;//房主账号（0=1P 1=2P 2=同步任务）
  char track[2][maxPath];//每个轨道的路径
  int deck[2];//每个轨道的使用卡组
  int missionStyle;//识别任务标记：1=公会 2=情侣 3=大赛
  int timer;//定时时刻（以秒数储存，仅定时任务有效）
  time_t realTimer;//带日期的定时时刻（StartList）时确定
  bool dual;//假同步标记（激活时，识别类任务房主实为1P，显示为同步）
  //完成情况
  int result;//任务完成结果：0正常完成 1未领奖 2未完成
  int gamesFinished;//完成局数
};

const int maxLogLength = 100000;//高级任务日志最大长度
const int contestSlotNum = 18;//大赛任务卡数量
//大赛卡完整名称
const char contestSlotFull[contestSlotNum][16] = {
  "小火炉", "木盘子", "棉花糖", "油灯", "炸炸菇", "糖葫芦炮弹", "苏打气泡", "炭烧海星", "可乐炸弹", "换气扇",
  "双向水管", "扑克牌护罩", "清障猪", "防风草沙拉", "仙人掌刺身", "金箔甜筒", "酒杯灯", "主武器" };
//大赛卡名称
const char contestSlot[contestSlotNum][16] = {
  "小火炉", "木盘子", "棉花糖", "油灯", "炸炸菇", "糖葫芦", "气泡", "海星", "可乐", "换气扇",
  "水管", "扑克牌", "清障猪", "防风草", "仙人掌", "甜筒", "酒杯灯", "主武器" };

//各期大赛名称
const char contestName[][50] = {
  "奇境1", "奇境2", "奇境3", "沙漠1", "沙漠2", "沙漠3", "沙漠4",
  "雷雪1", "雷雪2", "雷雪3", "雷雪4", "雷雪5", "综合1", "综合2", "综合3", "综合4" };
const int tournamentNum = sizeof(contestName) / sizeof(contestName[0]);//大赛期数

//大赛类型
struct Contest
{
  char text[100];//任务描述（不会超过50字）
  int code;//编号
  int type;//类型
  int level;//关卡
  int roleNum;//人数：0=无要求，1=单人，2=双人
  int slotNum;//卡槽数限制
  bool slotBanned[contestSlotNum];//卡片i是否被禁
  bool slotPlanted[contestSlotNum];//卡片i是否要求放置

  int restTime;//静置时间
  int cardNum;//放卡数限制
  int star;//星级限制
  int product;//生产要求
  int energy;//火苗要求
  int grade;//评分要求：0=无 A A+ S SS SSS分别对应1-5，从高往低匹配
  char enemy[5][40];//击杀的老鼠，如"击杀阿诺1只"
  int enemySize;//杀敌条件数量
  int boss;//1=不打BOSS，2=打BOSS，0=均可

  int aliveStove;//存活的火炉数
  int averageStar;//平均星级限制
  int gameTime;//通关时间限制
  int bottleTimes;//放置酒瓶炸弹的次数
  int freeTimes;//免费卡放置次数
  int cost;//卡片最大耗能限制

  bool maelstrom;//有无"涡"字
  bool hyphen;//有无连字符"-"
  bool isFinished;//是否为已完成的任务
  //记录信息
  int column;//所属列数
  int size;//本列未完成任务数
  int failedTimes;//未完成次数
};

const int propX = 468, propY0 = 89, propWidth = 49, propHeight = 49;//道具背包
const int storeX = 31, storeY = 93, storeWidth = 49, storeHeight = 49;//储藏室
const int jewelX = 559, jewelY = 90, jewelWidth = 49, jewelHeight = 49;//宝石分解
COLORREF doubleCard[2][propHeight][propWidth];//双经卡 双爆卡
//可删物品类型（保存完整截图用于显示，但只有中间区域用于比较）
struct Trash
{
  char name[10];//可删物品名称
  COLORREF image[propWidth][propHeight];//可删物品图像
};
const int maxTrashNum = 500;//可删物品上限
char trashList[maxTrashNum][maxPath];//可删物品目录
Trash trash[maxTrashNum];//可删物品
int trashNum;//可删物品数量

const int customWidth = 49, customHeight = 57;//卡槽尺寸
const int customX = 10, customY = 20, customCoreWidth = 25, customCoreHeight = 15;//卡槽识别范围
const int cellX = 382, cellY1 = 179, cellY2 = 407; //选卡池X及Y的范围及背包卡尺寸
COLORREF missingCustom[customHeight][customWidth];//缺卡显示图像
//自定卡槽类型
struct Custom
{
  COLORREF image[customHeight][customWidth];//卡槽图像
  char name[10];//卡槽名称
  int priority;//优先级
};
//自定卡槽核心类型
struct CustomCore
{
  COLORREF image[customCoreHeight][customCoreWidth];//卡槽图像
  char name[10];//卡槽名称
  int priority;//优先级
};
const int maxCustomNum = 500;//自定卡槽上限
char customList[maxCustomNum][maxPath];//自定卡槽文件名列表
Custom custom[maxCustomNum];//自定卡槽
int customNum;//自定卡槽数量

//自定图像类型
struct Item
{
  COLORREF image[templateHeight][templateWidth];//物件图像
  char name[10];//物件名称
  int requiredSimilarity;//要求的相似度
  int offsetX, offsetY, width, height;//识别范围的偏移和宽高
};
const int maxItemNum = 200;//自定图像上限
char itemList[maxItemNum][maxPath];//自定图像目录
Item item[maxItemNum];//自定图像
int itemNum;//自定图像数量

//角色名图像类型
struct RoleName
{
  char name[10];//图片名称
  COLORREF image[heavyNameHeight][lightNameWidth + heavyNameWidth];//粗/细角色名组合图像
};
const int maxRoleNameNum = 100;//角色名上限
char roleNameList[maxRoleNameNum][maxPath];//角色名目录
RoleName roleName[maxRoleNameNum];//已保存的粗细角色名
int roleNameNum;//角色名数量

//用于双缓冲读取截图的临时参数
Trash tempTrash[maxTrashNum];
int tempTrashNum;
Custom tempCustom[maxCustomNum];
int tempCustomNum;
Item tempItem[maxItemNum];//自定图像
int tempItemNum;//自定图像数量
RoleName tempRoleName[maxRoleNameNum];//已保存的粗细角色名
int tempRoleNameNum;//角色名数量

//把临时图片应用到工作图片
void ApplyTempImage()
{
  trashNum = tempTrashNum;
  customNum = tempCustomNum;
  itemNum = tempItemNum;
  roleNameNum = tempRoleNameNum;
  memcpy(trash, tempTrash, sizeof(trash));
  memcpy(custom, tempCustom, sizeof(custom));
  memcpy(item, tempItem, sizeof(item));
  memcpy(roleName, tempRoleName, sizeof(roleName));
}
//自定卡槽文件名粗排序比较函数（按字典序牌，相同文件名放一起）
int CustomCompare(const void *vpName1, const void *vpName2)
{
  //两个自定卡槽文件名（小火炉.png 小火炉_1.png 小火炉_-1.png 小火炉_1.2.png）
  char(&name1)[260] = *(char(*)[260])vpName1;
  char(&name2)[260] = *(char(*)[260])vpName2;

  //先比较卡片名称有无不同
  int i = 0;
  while (true)
  {
    //遇到下划线，或者只剩下".png"4位扩展名，说明名称已经读完
    bool end1 = name1[i] == '_' || name1[i + 4] == 0;
    bool end2 = name2[i] == '_' || name2[i + 4] == 0;
    //两个名称都读完了，说明名称一致
    if (end1 && end2)
      return 0;
    //name1先读完，则name1<name2；name2先读完，则name1>name2
    if (end1 && !end2)
      return -1;
    if (!end1 && end2)
      return 1;
    //都没读完，则比较字典序
    if (name1[i] < name2[i])
      return -1;
    if (name1[i] > name2[i])
      return 1;
    i++;
  }
  return 0;
}
//载入一类用户截图，遇到错误时将报错信息写入info并返回false
template <class Shot, int maxShotNum>
bool LoadUserShot(const char *folder, char(&shotList)[maxShotNum][260],
  Shot(&shot)[maxShotNum], int &shotNum, char(&info)[1000])
{
  if (!FileExist(folder))
  {
    sprintf_s(info, "缺少依赖文件夹：\n%s", folder);
    return false;
  }

  char searchPath[maxPath] = {};
  char path[maxPath] = {};
  int filesNum = 0;

  //角色名：先删除txt和旧尺寸图片
  if (std::is_same<Shot, RoleName>::value)
  {
    //1. 第一遍：删除所有txt文件
    sprintf_s(searchPath, "%s\\*.txt", folder);
    filesNum = GetFileList(searchPath, roleNameList, maxRoleNameNum);//查找所有txt文件
    for (int i = 0; i < filesNum; i++)
    {
      sprintf_s(path, "%s\\%s", folder, roleNameList[i]);
      remove(path);
    }
    //2. 第二遍：删除旧尺寸png文件
    sprintf_s(searchPath, "%s\\*.png", folder);
    filesNum = GetFileList(searchPath, roleNameList, maxRoleNameNum);//查找所有png文件
    for (int i = 0; i < filesNum; i++)
    {
      sprintf_s(path, "%s\\%s", folder, roleNameList[i]);
      int width = 0, height = 0;
      if (GetBitmapRect(path, &width, &height))
        if (width == heavyNameWidth && height == heavyNameHeight)
          remove(path);
    }
  }
  //自定卡槽：将占位卡转化为-1优先级
  else if (std::is_same<Shot, Custom>::value)
  {
    sprintf_s(searchPath, "%s\\*.png", folder);
    filesNum = GetFileList(searchPath, customList, maxCustomNum);//查找所有png文件
    for (int i = 0; i < filesNum; i++)
    {
      sprintf_s(path, "%s\\%s", folder, customList[i]);
      if (strstr(customList[i], "_占.png"))
      {
        char nameMinus1[260] = {};//-1优先级文件名（"占"替换为"-1"）
        strcpy_s(nameMinus1, customList[i]);
        char *underline = strstr(nameMinus1, "_占.png");
        strcpy_s(underline, sizeof("_-1.png"), "_-1.png");

        char pathMinus1[260] = {};//-1优先级路径
        sprintf_s(pathMinus1, "%s\\%s", folder, nameMinus1);
        if (FileExist(pathMinus1)) //-1优先级已存在则直接删除占位卡
          remove(path);
        else //否则将占位卡重命名为-1优先级
          rename(path, pathMinus1);
      }
    }
  }

  sprintf_s(searchPath, "%s\\*.png", folder);
  filesNum = GetFileList(searchPath, shotList, maxShotNum);//查找所有png文件

  //截图数量过多时报错
  if (filesNum > maxShotNum)
  {
    sprintf_s(info, "[%s]\n截图数量不能超过%d张。", maxShotNum);
    return false;
  }

  //自定卡槽：需要对customList按照优先级从小到大排序
  if (std::is_same<Shot, Custom>::value)
  {
    //粗排序：文件名相同的放在一起
    qsort(customList, filesNum, sizeof(customList[0]), CustomCompare);

    //输出粗排序结果
    //FILE *f;
    //fopen_s(&f, "自定卡槽顺序.txt", "w");
    //for (int code = 0; code < filesNum; code++)
    //  fprintf(f, "%s\n", customList[code]);
    //fclose(f);
    //exit(0);

    //精排序：文件名相同的按优先级从小到大排序
    char name[10] = {}, lastName[10] = {};//当前截图和上一张截图的名称
    int priority[maxCustomNum] = {};//每张截图的优先度
    int firstCode[maxCustomNum + 1] = {};//每种卡片第一张截图的编号（用来确定每种卡片的编号范围）
    int slotNum = 0;//不同名称的卡片数量

    //记录每一张截图的优先度和每种卡片第一张截图的编号
    for (int code = 0; code < filesNum; code++)
    {
      //文件名不合格则报错
      if (!IsCustomPathLegal(customList[code], name, &priority[code]))
      {
        sprintf_s(path, "%s\\%s", folder, shotList[code]);
        goto fileNameIllegal;
      }
      //遇到新卡片时卡片数量+1并记录第一张截图编号
      if (strcmp(name, lastName) != 0)
      {
        firstCode[slotNum++] = code;
        strcpy_s(lastName, name);
      }
    }
    //补充记录第customCount张卡片的编号，用于确定最后一张卡片的范围
    firstCode[slotNum] = filesNum;

    //对每张卡片的编号区间按优先级从小到大排序
    for (int order = 0; order < slotNum; order++)
      for (int code1 = firstCode[order]; code1 < firstCode[order + 1]; code1++)
      {
        //1. 找出从code1开始的最小优先度
        int minPriority = priority[code1];//最小优先度
        int minCode = code1;//最小优先度编号
        for (int code2 = code1 + 1; code2 < firstCode[order + 1]; code2++)
          if (priority[code2] < minPriority)
          {
            minPriority = priority[code2];
            minCode = code2;
          }
        //2. 如果最小优先度不是code1，在priority和customList数组中交换minCode和code1
        if (minCode != code1)
        {
          Swap(&priority[minCode], &priority[code1]);
          SwapStr(customList[minCode], customList[code1]);
        }
      }

    //输出精排序结果
    //FILE *f;
    //fopen_s(&f, "自定卡槽顺序.txt", "w");
    //for (int code = 0; code < filesNum; code++)
    //  fprintf(f, "%s\n", customList[code]);
    //fclose(f);
    //exit(0);
  }

  const int shotWidth = sizeof(shot[0].image[0]) / 4;
  const int shotHeight = sizeof(shot[0].image) / sizeof(shot[0].image[0]);
  shotNum = 0;
  for (int i = 0; i < filesNum && shotNum < maxShotNum; i++)
  {
    sprintf_s(path, "%s\\%s", folder, shotList[i]);
    //检查图片尺寸
    int result = CheckBitmapSizez(path, shotWidth, shotHeight, std::is_same<Shot, Item>::value, info);
    if (result == -2) //图片尺寸不合格则返回false
      return false;
    if (result == -1) //LoadItem时跳过"背景.png"
      continue;
    BitmapToColor(path, shot[shotNum].image);//拷贝图像到image
    shotList[i][strlen(shotList[i]) - 4] = 0;//从名称中删除".png"

    //可删物品/角色名：名称太长报错
    if (std::is_same<Shot, Trash>::value || std::is_same<Shot, RoleName>::value)
    {
      if (strlen(shotList[i]) > 8)
        goto fileNameIllegal;
      strcpy_s(shot[shotNum].name, shotList[i]);//记录名称
    }
    //自定卡槽：从文件名获取参数，名称不合格则报错
    else if (std::is_same<Shot, Custom>::value)
    {
      Custom &custom = tempCustom[tempCustomNum];
      if (!IsCustomPathLegal(customList[i], custom.name, &custom.priority))
        goto fileNameIllegal;
    }
    //自定图像：从文件名获取参数，名称不合格则报错
    else if (std::is_same<Shot, Item>::value)
    {
      Item &item = tempItem[tempItemNum];
      if (!IsItemPathLegal(itemList[i], item.name, &item.requiredSimilarity,
        &item.offsetX, &item.offsetY, &item.width, &item.height))
        goto fileNameIllegal;
    }
    shotNum++;
  }
  return true;

  //文件名不合格报错
fileNameIllegal:
  sprintf_s(info, "【%s】文件名不合格，\n请删除该图片后重试。", path);
  return false;
}

//比较临时区和工作区的截图有无变化
template <class Shot, int maxShotNum>
bool IsUserImageChanged(Shot(&shot)[maxShotNum], int &shotNum,
  Shot(&tempShot)[maxShotNum], int &tempShotNum)
{
  if (shotNum != tempShotNum)
    return true;
  for (int i = 0; i < shotNum; i++)
  {
    if (strcmp(shot[i].name, tempShot[i].name) != 0)
      return true;
    if (memcmp(shot[i].image, tempShot[i].image, sizeof(shot[i].image)) != 0)
      return true;
    if (std::is_same<Shot, Custom>::value)
      if (custom[i].priority != tempCustom[i].priority)
        return true;
    if (std::is_same<Shot, Item>::value)
    {
      if (item[i].requiredSimilarity != tempItem[i].requiredSimilarity)
        return true;
      if (item[i].offsetX != tempItem[i].offsetX)
        return true;
      if (item[i].offsetY != tempItem[i].offsetY)
        return true;
      if (item[i].width != tempItem[i].width)
        return true;
      if (item[i].height != tempItem[i].height)
        return true;
    }
  }
  return false;
}

//尝试将用户截图读取到临时区，遇到错误时记录info并返回false
bool LoadUserShotToTemp(char(&info)[1000])
{
  if (!LoadUserShot("用户参数\\可删物品", trashList, tempTrash, tempTrashNum, info))
    return false;
  if (!LoadUserShot("用户参数\\自定卡槽", customList, tempCustom, tempCustomNum, info))
    return false;
  if (!LoadUserShot("用户参数\\自定图像", itemList, tempItem, tempItemNum, info))
    return false;
  if (!LoadUserShot("用户参数\\角色名", roleNameList, tempRoleName, tempRoleNameNum, info))
    return false;
  return true;
}
//读取四大用户截图。读取失败时记录报错信息info并返回false
bool UpdateUserShot(bool firstTime, char(&info)[1000])
{
  //读取用户截图到临时区
  if (!LoadUserShotToTemp(info))
    return false;

  //读取成功时，在互斥锁保护下更新工作区
  WaitForSingleObject(hMutexUserImage, INFINITE);
  if (firstTime
    || IsUserImageChanged(trash, trashNum, tempTrash, tempTrashNum)
    || IsUserImageChanged(roleName, roleNameNum, tempRoleName, tempRoleNameNum)
    || IsUserImageChanged(custom, customNum, tempCustom, tempCustomNum)
    || IsUserImageChanged(item, itemNum, tempItem, tempItemNum))
    ApplyTempImage();
  ReleaseMutex(hMutexUserImage);
  return true;
}

//某个卡槽的所有候选卡类型
struct Candidate
{
  char num;//候选自定卡个数
  char priority[20];//每个候选卡的优先级（从高到低排列）
  short code[20];//每个候选卡的编号
};

const int maxTicketNum = 100;//门票数量上限
const int maxHarmonySize = 100;//温馨礼包容量
const int dialogHarmonySize = 10;//对话框能显示的温馨礼包容量
const int starX = 4, starY = 3, starWidth = 21, starHeight = 19;//卡槽左上角星级区位置
const int maxUsedSlotNum = 36;
const int maxUsedTicketNum = 36;
//任务列表类型
struct Plot
{
  //列表信息
  char title[100];//任务标题
  char harmony[maxHarmonySize][50];//温馨礼包openid
  char harmonyLog[maxHarmonySize][100];//温馨日志
  int harmonyLogSize;//温馨日志条数
  Advance advance[maxAdvanceNum];//当前高级任务列表
  Advance guildAdvance[maxAdvanceNum];//公会任务预制列表
  Advance loverAdvance[maxAdvanceNum];//情侣任务预制列表
  Advance contestAdvance[tournamentNum][maxAdvanceNum];//各期大赛预制列表（主要内存占用）
  int advanceNum;//当前任务数量 
  int guildAdvanceNum;//公会任务数量
  int loverAdvanceNum;//情侣任务数量
  int contestAdvanceNum[tournamentNum];//各期大赛任务数量
  bool isAdvanceLocked;//当前列表是否锁定
  bool isGuildAdvanceLocked;//公会任务列表是否锁定
  bool isLoverAdvanceLocked;//情侣任务列表是否锁定
  bool isContestAdvanceLocked[tournamentNum];//各期大赛列表是否锁定
  int advancePage;//高级任务列表当前页数
  int advanceSelected;//选中的高级任务
  int isAdvanceSelectionRetained;//是否需要保留任务选中状态
  char advancePath[maxPath];//列表路径
  char password[2][15];//二级密码，最长14位
  int server[2];//1P和2P的3366服务器选项

  //执行信息
  int playingOrder;//正在执行的任务编号
  Advance *playingAdvance;//正在执行的任务指针
  int isAdvanceWindowBound;//高级任务所需窗口是否已绑定
  int isAdvanceStarted;//高级任务是否正在运行
  int isAdvanceStarted_Confirm;//高级任务是否正在运行（确认退出任务线程后才会设为0）
  int advanceInvolved[2];//1P和2P是否参与高级任务（以Param抓取为准，不受房主交换影响）
  int timesRefreshed[maxAdvanceNum];//每个任务的已刷新次数
  int isRefreshRequired;//开始本轮任务前是否需要刷新
  int zoneCounter;//已执行的高级任务数量（刷新时+1)
  bool isGuildClaimFailed;//公会任务领奖是否失败
  bool isLoverClaimFailed;//情侣任务领奖是否失败
  bool isDailyClaimFailed;//日常任务领奖是否失败
  bool isLevelInsufficient;//是否等级不足
  int randomZone[7];//0-6的随机排列，用于决定执行高级任务时换到哪个区
  bool curGuildImage[guildWidth][guildHeight];//当前读取的公会任务名01谱
  bool guildExist[maxAdvanceNum];//预制列表中第i条公会任务是否存在
  bool loverExist[maxAdvanceNum];//预制列表中第i条情侣任务是否存在
  int surfaceSize[2];//2个号的表面任务数量
  Contest surface[2][13];//2个号的表面任务（每个号最多12条，给1个容量存非战斗任务）
  Contest bestContest;//选择的表面大赛任务
  Contest requiredContest[24];//本轮应完成的大赛任务，最多24个
  int requiredSize;//本轮应完成任务数量
  Contest failedContest[24];//本轮未完成的大赛任务，最多24个
  int failedSize;//本轮未完成任务数量
  Contest totalFailedContest[102];//累计未完成的大赛任务，最多102个
  int totalFailedSize;//累计未完成任务数量
  int issue;//当前定位期数
  int step;//当前定位步数
  bool isLocated;//是否定位成功
  int contestSize[2][12];//2个号12列大赛的剩余任务数

  //任务检查信息
  bool isChecking;//是否以检查模式启动线程
  bool isCheckingSuccessful;//检查是否成功
  bool isDeepChecking;//是否以深度检查模式启动线程
  bool isDeepCheckingSuccessful;//深度检查是否成功
  bool isCountingSlot;//是否正在检查当前列表（用于统计使用的卡片）
  char usedSlot[2][maxUsedSlotNum][10];//2个号每个所需卡片的名称
  Candidate candidate[2][maxUsedSlotNum];//2个号每个所需卡片的候选卡信息
  int usedSlotNum[2];//2个号需要的卡片数量
  int usedTicketCount[2][maxTicketNum];//2个号每种门票的使用数量

  //背包检查信息
  int bestPriority[2][maxUsedSlotNum];//2个号每个所需卡片的最高优先级（不存在为-99）
  int bestStar[2][maxUsedSlotNum];//2个号每个所需卡片的最高星级（不存在为-1）
  int bestCode[2][maxUsedSlotNum];//2个号每个所需卡片的最高优先级编号（不存在为-1）
  COLORREF bestStarImage[2][maxUsedSlotNum][starHeight][starWidth];//2个号每个所需卡片的最高星级图像
  int ticketCount[2][maxTicketNum];//2个号每种门票在背包中的数量

  //日志信息
  char refreshLog[maxPath]; //刷新记录
  char contestLog[maxPath];//大赛日志
  char logQuestString[200];//日志中的任务信息：[1]星际(糖球日) 进行中 12/12
  HANDLE hMutexRefreshLog;//刷新记录互斥锁
  char logFinished[maxLogLength];//已完成任务的日志
  char output[maxLogLength / 100][100];//高级任务输出信息
  int outputNum;
  int outputPage;//当前页数
  char advanceFolderPath[maxPath];//高级任务日志文件夹

  //备份信息
  int currentBackup;//当前备份数
  int backupNum;//总备份数
  char backupFolder[maxPath];//备份文件夹名称
};

//4. 高级任务参数
const int taskNum = 5;//普通任务数量
const int listNum = 5;//高级任务数量
const int maxTaskNum = taskNum + listNum * 2;//总任务数量
int curList;//当前显示的高级任务
Plot plot[listNum];//任务列表

//5. 普通任务常数
const int maxSlotNum = 21;//最大卡槽数
const int sorbetLoc[3] = { 71, 79, 19 };//冰沙位置：左下、右下、右上依次尝试
const int maltoseRequiredSimilarity = 20;//棉麦识别要求的相似度

//窗口隐藏信息
struct HideInfo
{
  bool hidden;//是否隐藏
  bool maximized;//隐藏前是否最大化
  RECT rect;//隐藏前窗口位置
};
//任务参数类型
struct Param
{
  char title[100];//任务标题
  int type;//类型
  int level;//关卡（合称：关卡选择）
  char track[2][maxPath];//轨道
  int deck[2];//卡组
  int isProceed[2];//继续作战
  int isCollect[2];//自动收集
  int maxGames;//局数
  int gameTime;//每局用时
  int host;//房主（0=1P，1=2P）
  time_t realTimer;//带日期的定时时刻
  POINT tag[2];//小号1,2标签坐标，初始值-1,-1
  HWND hWnd[2];//如果直接抓的是游戏区域，则记录，否则为零
  bool isWindow[2];//两个游戏窗口是否有效
  char hallName[2][maxPath];//2个号的大厅标题
  char mainName[maxPath];//主大厅标题
  HideInfo hideInfo[2];//窗口隐藏信息
};
//任务状态类型
struct State
{
  int games, maxGames, wave, smallWave, progress;
  int currentTime;
  DWORD averageTime;
  int totalTime;
  int refreshLevel;//刷新等级（0正常 1重试 2跳过 3终止）
  char tip[100];//任务状态提示语
  char errorString[100];//错误信息
};
//放卡条件类型
struct ConditionType
{
  int delay;//达成条件后延迟放卡的时间
  int triggerMode;//触发模式（达成条件一次后不断放卡）
  int energy;//能量
  char wave[6];//带小数的波次
  char card[10];//卡片名称
  int cardNum;//卡片数量
  char image[10];//图像名称
  int imageNum;//图像数量
  char hunterRow[4], hunterColumn[4];//索敌行列表达式
  char follow[10];//紧跟的卡片名称
  int endNum, endTime;//终止数量和终止时间
};
const int maxFrame = 10;//最大帧数
//卡槽信息类型
struct SlotInfo
{
  char name[10];//原卡槽名称
  char alternate[10];//替代卡槽名称
  bool banned;//是否被ban
  bool used;//是否使用
  int cd;
  int level;
};
//卡槽类型
struct Slot
{
  //变阵保留信息
  char name[10];
  bool isMaltose;//是不是棉花糖/麦芽糖
  bool once;//是否为放置一次的卡
  bool used;//是否有轨道（检查所有波次）
  //需要每波重置的卡片信息
  //轨道信息
  int row[maxPlantTimes + 1], column[maxPlantTimes + 1];//第i次种植的行列
  int moment[maxPlantTimes + 1];//第i次种植的时刻
  byte isPlantedInMap[8][10];//卡片是否在i行j列种植（用于缩小补阵识图的范围）
  int maxTimes;//第i张卡的最大种植次数
  bool prior;//是否绝对优先
  bool repair;//是否补阵
  bool search;//是否索敌
  bool independent;//是否独立计时
  bool limit;//是否极限成阵
  bool retain;//是否继承
  int maxMoment;//第i张卡的最大种植时刻
  int cd;//卡片冷却（无用）
  int level;//卡片层级
  //识图信息
  short similarity[8][10][maxFrame];//第i次检测的相似度
  short maxSimilarity[8][10];//最近1秒内的最大相似度
  int colorNum;//卡片模板颜色总数
  int num;//场上卡片个数
  //条件记录信息
  int cardOrder;
  int followOrder;
  bool isSimilarityRequired;//是否需要识别相似度
  //运行时参数
  int times;//已放置次数
  int successfulTimes;//成功放置次数
  bool planted;//是否刚才已放置
  //int limitTimes;//极限成阵阶段放置次数
  DWORD plantableTick;//（条件放卡）可放卡时刻；不可放卡为0
  DWORD triggerTick;//触发时刻，未触发为0
  DWORD oldPlantTick;//原本放置时间（独立计时卡初次延时后记录）
  DWORD lastTick;//上次放置时间（只有极限卡用得到）
  DWORD startTick;//计时起点
  DWORD avaliableTick;//卡槽亮起时刻（读冷却时检查）
  int plantDuring;//放卡累计占用时间(ms)
  //放卡条件
  int delay;//达成条件后延迟放卡的时间
  int triggerMode;//触发模式（达成条件一次后不断放卡）
  int energy;//能量
  char wave[6];//带小数的波次
  char card[10];//卡片名称
  int cardNum;//卡片数量
  char image[10];//图像名称
  int imageNum;//图像数量
  char hunterRow[4], hunterColumn[4];//索敌行列表达式
  char follow[10];//紧跟的卡片名称
  int endNum, endTime;//终止数量和终止时间
};
//识别目标类型
struct Target
{
  char name[10];//物件名称
  short similarity[8][11][maxFrame];//7行10列格子每次识别的相似度
  short maxSimilarity[8][11];//7行10列格子最近1秒内的最大相似度
  short globalMaxSimilarity;//所有格子中的最大相似度
  int colorNum;//颜色总数
  int num;//图像数量
  int location[63];//图像位置
};
const int maxGamesAllowed = 9999;
const int maxTargetNum = 10;//每个账号允许的最大目标数量

//自定卡片类型
struct Card
{
  COLORREF image[templateHeight][templateWidth];//物件图像
  char name[10];//卡片名称
  int requiredSimilarity;//要求的相似度（0%）
  int offsetX, offsetY, width, height;//识别范围的偏移和宽高
};
const int maxCardNum = 200;//自定卡片上限
char cardList[maxCardNum][maxPath];//自定卡片目录
Card card[maxCardNum];//自定卡片
int cardNum;//自定卡片数量

//战利品类型（保存完整截图用于显示，但只有中间区域用于比较）
struct Loot
{
  bool isReplacedRequired;//是否需要用不绑图像替换
  char name[50];//物品名称
  COLORREF image[lootWidth][lootHeight];//物品图像
};
const int maxLootNum = 1000;
char lootList[maxLootNum][maxPath];
Loot loot[maxLootNum];
int lootNum;

//道具数量个位数位置
const int lootDigitX = 38, lootDigitY = 34, lootDigitWidth = 6, lootDigitHeight = 8;
const int lootDigitDistance = lootDigitWidth + 1;
COLORREF lootDigit[10][lootDigitHeight][lootDigitWidth];
//载入物品数字
void LoadLootDigit()
{
  char path[maxPath];
  for (int num = 0; num < 10; num++)
  {
    sprintf_s(path, "附加程序\\数字\\物品数量\\%d.png", num);
    BitmapToColor(path, lootDigit[num]);
    for (int y = 0; y < lootDigitHeight; y++)//只保留白色，其他颜色涂黑
      for (int x = 0; x < lootDigitWidth; x++)
        if (lootDigit[num][y][x] != 0xffffff)
          lootDigit[num][y][x] = 0;
  }
}
//道具背包翻页校准区
const int calibrationX = 49 * 3, calibrationY = 5, calibrationWidth = 49, calibrationHeight = 36;
//放卡优先队列
struct Queue
{
  char name[10];//卡片名称
  bool planted;//是否已放置
};
//任务运行参数类型
struct Work
{
  //不能被清空的参数（前8字节）
  HDC hMemDC;//map的内存DC
  COLORREF(&map)[596][950];//先Y后X，BGR格式的地图
  //自定图像
  Target target[maxTargetNum];//只给10个识图名额
  int targetNum;//目标数量
  //卡槽信息
  Slot slot[2][maxSlotNum + 2];//卡槽信息(0.3M)
  SlotInfo originalSlot[2][maxSlotNum + 2];//ban卡前的卡槽
  int originalSlotNum[2];//ban卡前的卡槽数
  bool isBanEnabled[2];//ban卡是否启用
  //队列信息
  Queue queue[2][20];//放卡优先队列
  int queueSize[2];//队列大小

  //执行器参数：执行器设定的参数
  char track[2][maxPath];//轨道
  char pureTrack[2][maxPath];//去除ban卡列表的轨道路径
  char backupTrack[2][maxPath];//备份轨道路径
  int accelerationTime[2];//两个号的加速时间
  bool isAccelerationRequired;//是否需要加速跳结算
  bool isAccelerationOn;//加速是否已开启
  int maxGames;//执行局数
  int gameTime;//每局用时（0=自动检测）
  int host;//房主（0=1P，1=2P）
  int deck[2];//卡组
  int isProceed[2];//继续作战
  int isCollect[2];//自动收集
  POINT tag[2];//小号1,2标签坐标，初始值-1,-1
  char hallName[2][maxPath];
  char mainName[maxPath];
  //轨道参数：从轨道文件中读取的参数
  int isInvolved[2];//两个账号是否参与
  int isPerformed[2];//两个账号是否放卡
  int tower[2];//需要挑战的魔塔层数，非魔塔为0
  int star[2];//星级限制
  int restTime;//静置时间
  int quitTime[2];//退出关卡的时间，不退出为0
  bool isQuitted[2];//关卡是否已退出
  bool isQuitUsed;//退出功能是否已使用
  int roleLocNum[2][2];//2个轨道的1P和2P人物位置数量
  int roleLoc[2][2][63];//2个轨道的1P和2P人物位置
  int slotNum[2];//两个号的卡槽数
  bool isSkillMode;//是否刷技能模式
  //进度参数：只在关卡外发生变化的参数
  int games;//当前局数
  bool reloading;//是否处于变阵阶段
  //关卡参数：关卡内变化的参数
  int banner;//特殊关卡：0=未记载，1-10=漫游，11=悬浮梦境
  int progress;//进度条鼠头位置
  int wave, smallWave;//当前波数和小波
  int currentTime;//当前计时（秒）
  int energy[2];//当前火苗
  int realSlotNum[2];//游戏内实际卡槽数
  int slotX[2];//游戏内实际卡槽左端（用于判定卡片冷却状态）
  POINT cdPoint[2][maxSlotNum + 1];//每张卡判断cd的点位
  bool isCold[2][maxSlotNum + 2];//每张卡是否已冷却好，是=1，否=0
  bool isFrontCatched;//前线是否已经截取
  bool isBossExist;//boss是否已经出现
  bool isWaveEnded[2];//是否已到达结束波次（战利品、结算、翻牌等）
  DWORD oneAccountEndTick;//一个账号结束的时间
  bool isRatAppeared;//是否已经出怪
  int ratRow[7];//出怪表，用于寻敌模式，依次记录出怪的行
  int ratRowNum;//已出怪的行数
  int lackey[2][maxSlotNum + 2];
  int lackeyNum[2];
  COLORREF innateFront[frontHeight * 8];//初始前线颜色，前线颜色(4K)
  bool isLootSaved[2];//战利品是否已经截取
  //同步参数
  bool dualError;//同步任务2P是否报错
  char dualErrorTip[100];//同步任务2P报错提示
  char dualErrorString[100];//同步任务2P报错内容
  int dualErrorLevel;//同步任务2P报错等级
  //记录信息
  int isDpiAwareRequired[2];//在游戏窗口内点击时是否需要DPI换算
  int mapOffsetY[2];//因公会全屏导致的画面Y偏移
  time_t taskStartTime, taskEndTime;//任务启动、结束时间（秒）
  DWORD thisGameTick, lastGameTick;//本局、上局开始时刻（毫秒）
  DWORD levelTime[maxGamesAllowed + 1];//第i局通关时间（毫秒）
  DWORD averageTime;//用于计算平均时间
  HWND hWnd[2];//小号1窗口，小号2窗口，识图窗口
  char startTimeString[100], endTimeString[100];//启动、结束时间字符串
  char logDirectory[maxPath];//日志文件夹
  char lootDirectory[maxPath];//战利品文件夹（启动任务时创建并填写）
  char logPath[maxPath];//日志文件
  DWORD totalWaveTick[maxTotalWave];//各个波次的进入时刻，其中WaveTick[0]是进入关卡时刻
  int totalWaveExist[2][maxTotalWave];//两个账号各波次轨道是否存在
  int noImageTick;//出现无图像的时刻，用于超时后中断或提示
  DWORD lastCheckTick;//上次检查独立和极限卡的时刻
  DWORD lastSimilarityTick;//上次获取相似度的时刻
  int isPromptOn;//是否正在提示
  char embarkString[2000];//启动流程
  //刷新相关
  HWND hWndServer;//选服窗口
  RECT hallRect, serverRect;//大厅窗口区域和选服窗口区域（用于坐标换算）
  COLORREF *hallShot;//大厅截图，用于刷新时识别服务器位置
  COLORREF *pHallShot[4320];//大厅截图二级指针，高度不超过8K分辨率4320
  //战利品测试临时参数
  int blankSimilarity[2][5][10];//战利品row行column列与空格的像素重复度

  //颜色参数
  COLORREF heavyName[2][heavyNameHeight][heavyNameWidth];//2个号的粗角色名
  COLORREF lightName[2][lightNameHeight][lightNameWidth];//2个号的细角色名
  bool IsLightNameMatched[2];//2个号是否已匹配细角色名
  COLORREF towerPage[2][10][14];
  COLORREF lootScroll[lootScrollHeight][1];//战利品滚动条
  COLORREF totalLoot[lootHeight * 10][lootWidth * 10];//10x10累计战利品截图
  union
  {
    CustomCore custom[maxCustomNum];//自定卡槽核心（关卡前使用）
    byte colorExist[2097152];//模板中存在的颜色（关卡中使用）
    COLORREF loot[lootHeight * 5][lootWidth * 10];//5x10战利品截图（关卡末使用）
  };
  int customNum;//自定卡槽数量

  // 自动截图道具用的颜色信息
  COLORREF initialProp[lootHeight][lootWidth];//移动前的道具背包格子截图
  COLORREF movedProp[lootHeight][lootWidth];//移动后的道具背包格子截图
  COLORREF calibration[calibrationHeight][calibrationWidth];//校准区
  int propOffsetY;//道具背包翻页量（正数）

  Card card[2][maxSlotNum + 1];//自定卡片（关卡中使用）
  COLORREF wideTemplate[wideTemplateHeight][wideTemplateWidth];//卡片宽模板
  COLORREF background[2][wideTemplateHeight][wideTemplateWidth];//卡片宽模板背景
  int isBackgroundCatched[2];//背景是否成功截取
  int isGridCatched;//初始地图是否成功截取
  int isTemplateCatched[2][maxSlotNum + 1];//每张卡是否已经被截取模板
  int grid[5][8][10];//0-4层级各格子占用的卡片编号

  COLORREF recordedColor[templateWidth * templateHeight];
  int recordedColorNum;
  COLORREF erasedColor[templateWidth * templateHeight];

  //其他参数
  int starfishOrder[2];//两个账号的海星编号
  int magic[2], magicCore[2];//两个账号的幻鸡和幻鸡目标卡（没有目标设为0）
  int isMagicPlantedRecently[2];//幻幻鸡是否已种植

  Work() :map(*MallocMap(&hMemDC)) {}
};
//任务保留信息类型
struct Reserve
{
  COLORREF innateGrid[7][9][gridHeight][gridWidth];//地图7行9列的初始颜色
  byte maltoseTimes[7][9];//7行9列放棉麦的次数
  bool isMaltoseClearRequired;//是否需要在3分钟时清除棉麦次数（仅限音乐节）
  int cellY;//选卡池顶部Y
  int propY;//道具背包顶部Y
  int slotY;//防御卡背包顶部Y
  bool passwordReleased;//二级密码是否已解除
  int gamesFinished;//已完成的局数（普通任务为0）
  int lootNum[2][maxLootNum];//每种战利品的获得数量
  //同步信息
  bool arriveRefresh;//是否已到达StartTask的刷新判定处
  bool arriveFinish;//是否已到达FinishTask
  //记录信息
  int updateNum, successfulUpdateNum, iconicNum;//截图、截图成功、最小化帧数
};

//6. 普通任务参数
int getColdNum;//读取冷却次数
Monitor monitor[maxTaskNum];//任务的监视窗口信息
Param param[maxTaskNum];//任务设置参数
State state[maxTaskNum];//任务状态
Work work[maxTaskNum];//任务执行参数
Reserve reserve[maxTaskNum];//任务开始时不重置的信息，只在需要时填写
int isTaskStarted[maxTaskNum];//任务是否正在运行
int isTaskStarted_Confirm[maxTaskNum];//任务是否正在运行（确认退出任务线程后才会设为0）

//7. 执行器设置
const int recognitionFrame = 10;//识图帧率
const int unlimitedRefreshTimes = 50;//无限制选服次数
const int noImageTimeout = 120;//无图像处理所需时间
int mode = 1;//1=普通任务，2=高级任务
int zoom = 100;//执行器缩放比例（%）
int plantDelay;//放卡延迟
int noImageOperation;//无图像时操作（0=不处理，1=提示，2=停止）
int customOption;//带卡选择（0=最高星，1=最靠前）
int flipNum;//翻牌数量（最多5，因为第6张牌与“开始”按钮重合）
int bagFullOperation;//背包满时操作（0=停止，1=继续）
int expFullOperation;//经验满时操作（0=停止，1=继续）
int repairLevel;//补阵强度，数值1-10对应10%-100%
int operationSpeed;//操作速度，0=慢速，1=中速，2=快速
int missionRecovery;//识别任务自动还原
int maxRefreshTimes;//最大刷新次数
int globalAccelarationTime;//全局加速时间
int listStyle;//列表风格：0=正常列表 3=大赛列表 4=营地列表 5=主线列表
//默认等待时间
int WaitTime()
{
  return 10000;
}

//8. 执行器参数
IMAGE imageBackup;//执行器图像备份
int isRepaintRequired;//是否需要重新绘制
HWND hWndActuator;//执行器窗口句柄
char tracePrint[1000];//DEBUG专属：执行器下方输出内容
char programDirect[maxPath];//轨道执行器.exe所在目录，不带斜杠
char workDirect[maxPath];//程序工作目录
int lastClickMessage;//上次鼠标点击信息：单击1xxxxyyyy，右击2xxxxyyyy，无消息0
char lastTip[200];//上一次显示的提示（用于判断要不要更新提示）
float ratio;//窗口缩放比例。ratio=1.25意味着分辨率太高，点击位置判定偏大，需要除以该比例。
float originRatio;//100%缩放比例
POINT originalWndPos;//记录窗口原位
HCURSOR crossCursor;//十字光标
HFONT hMonitorFont;//监视器字体
HPEN hMonitorPen;//监视器画笔
int sleepTime;//等待时间

//9. 颜色信息
const COLORREF defaultFillColor = RGB(30, 30, 30);//默认填充色
//三类识别任务的颜色：公会（绿）情侣（粉）大赛（红）
const COLORREF missionStyleColor[3] = { RGB(24, 96, 24), RGB(96, 24, 96), RGB(96, 24, 24) };
//6种任务颜色
const COLORREF taskColor[6] = { RGB(255, 96, 128), RGB(224, 224, 0), RGB(96, 232, 96), RGB(0, 200, 240), RGB(255, 64, 255), RGB(255, 128, 0) };
//6种状态颜色
const COLORREF stateColor[6] = { RGB(192, 60, 80), RGB(160, 160, 0), RGB(32, 184, 32), RGB(0, 128, 192), RGB(192, 0, 192), RGB(192, 96, 0) };
const COLORREF grayWhite = RGB(224, 224, 224);//默认文字的灰白色
COLORREF cardTemplate[4][templateHeight][templateWidth];//海星和0-2转煮蛋的标准模板
int pageValue[12] = { 0, 2, 5, 7, 3, 4, 12, 1, 15, 13, 9, 8 };//魔塔翻页颜色信息
//0-13波，14=选卡/跨服跳出关卡，15=结算，16=翻牌，17=黑屏，18=魔塔
const COLORREF wavesColor[19] = {
  0xfad599, 0xffe9a6, 0xfcc88d, 0xffdf6b, 0xfff792,
  0xffe4ae, 0xfffb79, 0xffdf77, 0xeeda64, 0xffc47e,
  0x2e2519, 0x210d00, 0x877641, 0xa09b5a, 0x5d6a71,
  0x142a4d, 0x00194b, 0x000000, 0x3179c4 };
const int waveOutOfLevel = 100;//未进入关卡
const int waveGrade = 101;//结算
const int waveFlip = 102;//翻牌
const int waveNoImage = 103;//无图像
const int waveTower = 104;//魔塔
const int wavePet = 105;//宠塔
const int wavePeak = 106;//巅峰对决
const int waveLoot = 107;//战利品界面
const int ratColorWidth = 26;
COLORREF ratColor[ratColorWidth];
const int progressRatWidth = 31, progressHeadWidth = 11;//老鼠头宽度；进度条右侧固定颜色宽度
const int progressBarX = 696, progressBarY = 582, progressBarLength = 153;//进度条左端点和长度
COLORREF progressLeftColor[progressBarLength];//进度条左色
COLORREF progressRightColor[progressBarLength];//进度条右色
//进度条右侧可变颜色
const COLORREF possibleRightColor[] = { 0x45caec, 0x46c9ec, 0x48c9ec, 0x49c8ec, 0x4ac7ec, 0x4cc6ec, 0x4dc7ed };
const COLORREF customColor[2] = { 0x001f41, 0x1a4263 };//防御卡识别色

//按钮组类型
struct ButtonGroup
{
  int firstButton;//第一个按钮编号
  int row, column;//按钮的行数和列数
};

//10. 按钮信息
const int MaxButtonGroupNum = 100;
int buttonGroupNum;//按钮的类数
ButtonGroup buttonGroup[MaxButtonGroupNum];
const int maxButtonNum = 1000;
int buttonNum;//按钮的个数
MyRect buttons[maxButtonNum];
int ButtonArea[2][wndWidth][wndHeight];//每个坐标的区域值，初始为0，开局时注册按钮
int buttonParameter[2];//两个模式的参数区
int buttonState[2];//两个模式的状态区
int buttonParameterTitle[2];//两个模式的参数标题
int buttonStateTitle[2];//两个模式的状态标题
int buttonAdvance;//任务列表
int buttonOutputPage;//输出信息翻页
int buttonUpperSettings[2];//上方设置
int buttonLowerSettings[2];//下方设置
int buttonLock;//锁定

//获取任务task对应的列表编号
int GetList(int task)
{
  return (task - taskNum) / 2;
}
//获取列表list对应的主任务编号
int GetTask(int list)
{
  return taskNum + list * 2;
}
//任务task是否属于普通任务
bool IsNormal(int task)
{
  return task >= 0 && task < taskNum;
}
//任务task是否属于高级任务
bool IsAdvance(int task)
{
  return task >= taskNum;
}
//任务task是否属于同步任务的1P
bool IsDual1P(int task)
{
  return task >= taskNum && task % 2 == 1;
}
//任务task是否属于同步任务的2P
bool IsDual2P(int task)
{
  return task >= taskNum && task % 2 == 0;
}

//注册按钮内部小按钮的区域值
void RegisterCenterButton(int mode, int ButtonCode, int x1, int y1, int width1, int height1, int Area)
{
  int x = buttons[ButtonCode].x;
  int y = buttons[ButtonCode].y;
  int width = buttons[ButtonCode].width;
  int height = buttons[ButtonCode].height;
  if (ButtonArea[mode - 1][x + x1][y + y1] == Area)
    return;
  for (int i = x + x1; i < x + x1 + width1; i++)
    for (int j = y + y1; j < y + y1 + height1; j++)
      ButtonArea[mode - 1][i][j] = Area;
}
//注册左侧点击按钮的区域值
void RegisterLeftButton(int mode, int ButtonCode, int EffectiveWidth, int Area)
{
  int x = buttons[ButtonCode].x;
  int y = buttons[ButtonCode].y;
  int width = buttons[ButtonCode].width;
  int height = buttons[ButtonCode].height;
  if (ButtonArea[mode - 1][x][y] == Area)
    return;
  for (int i = x; i < x + EffectiveWidth; i++)
    for (int j = y; j < y + height; j++)
      ButtonArea[mode - 1][i][j] = Area;
}
//注册右侧点击按钮的区域值
void RegisterRightButton(int mode, int ButtonCode, int EffectiveWidth, int Area)
{
  int x = buttons[ButtonCode].x;
  int y = buttons[ButtonCode].y;
  int width = buttons[ButtonCode].width;
  int height = buttons[ButtonCode].height;
  if (ButtonArea[mode - 1][x + width - 18][y] == Area)
    return;
  for (int i = x + width - EffectiveWidth; i < x + width; i++)
    for (int j = y; j < y + height; j++)
      ButtonArea[mode - 1][i][j] = Area;
}
//注册水平调节按钮的区域值
void RegisterHorizontalAdjust(int mode, int ButtonCode, int LeftArea, int RightArea)
{
  int x = buttons[ButtonCode].x;
  int y = buttons[ButtonCode].y;
  int width = buttons[ButtonCode].width;
  int height = buttons[ButtonCode].height;
  if (ButtonArea[mode - 1][x][y] == LeftArea)
    return;
  for (int j = y; j < y + height; j++)
  {
    for (int i = x; i < x + 18; i++)
      ButtonArea[mode - 1][i][j] = LeftArea;
    for (int i = x + width - 18; i < x + width; i++)
      ButtonArea[mode - 1][i][j] = RightArea;
  }
}
//注册右侧调节按钮的区域值
void RegisterRightAdjust(int mode, int ButtonCode, int UpArea, int DownArea)
{
  int x = buttons[ButtonCode].x;
  int y = buttons[ButtonCode].y;
  int width = buttons[ButtonCode].width;
  int height = buttons[ButtonCode].height;
  if (ButtonArea[mode - 1][x + width - 18][y] == UpArea)
    return;
  for (int i = x + width - 18; i < x + width; i++)
  {
    for (int j = y; j < y + height / 2; j++)
      ButtonArea[mode - 1][i][j] = UpArea;
    for (int j = y + height / 2; j < y + height; j++)
      ButtonArea[mode - 1][i][j] = DownArea;
  }
}
//注册左侧调节按钮的区域值
void RegisterLeftAdjust(int mode, int ButtonCode, int UpArea, int DownArea)
{
  int x = buttons[ButtonCode].x;
  int y = buttons[ButtonCode].y;
  int width = buttons[ButtonCode].width;
  int height = buttons[ButtonCode].height;
  if (ButtonArea[mode - 1][x][y] == UpArea)
    return;
  for (int i = x; i < x + 18; i++)
  {
    for (int j = y; j < y + height / 2; j++)
      ButtonArea[mode - 1][i][j] = UpArea;
    for (int j = y + height / 2; j < y + height; j++)
      ButtonArea[mode - 1][i][j] = DownArea;
  }
}
//注册单个按钮的区域值
int RegisterButtonArea(int mode, int x, int y, int width, int height, int area)
{
  buttons[buttonNum].x = x;
  buttons[buttonNum].y = y;
  buttons[buttonNum].width = width;
  buttons[buttonNum].height = height;
  buttonNum++;
  for (int i = x; i < x + width; i++)
    for (int j = y; j < y + height; j++)
      ButtonArea[mode - 1][i][j] = area;
  return buttonNum - 1;//返回注册按钮的编号
}
//注册一组按钮的区域值（行数，列数，位置，区域值抬头，区域值中行、列的权重），返回按钮组编号
int RegisterButtonsArea(int mode, int row, int column, int x, int y, const int *width, const int *height, int title, int RowWeight, int ColumnWeight)
{
  if (ButtonArea[mode - 1][x][y] > zero)//已经注册过就不注册了
    return -1;

  buttonGroup[buttonGroupNum].firstButton = buttonNum;
  buttonGroup[buttonGroupNum].row = row;
  buttonGroup[buttonGroupNum].column = column;
  buttonGroupNum++;

  if (row > 100 || column > 100)
  {
    MessageBox(hWndActuator, "注册的按钮太多，数组越界！", "", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    exit(0);
  }
  int xPos[100] = { x };
  int yPos[100] = { y };
  for (int i = 1; i < row; i++)//填写宽高数据
    yPos[i] = yPos[i - 1] + height[i - 1];
  for (int j = 1; j < column; j++)
    xPos[j] = xPos[j - 1] + width[j - 1];
  for (int i = 0; i < row; i++)//注册按钮区域值（按先列后行的顺序）
    for (int j = 0; j < column; j++)
      RegisterButtonArea(mode, xPos[j], yPos[i], width[j], height[i], title + RowWeight * i + ColumnWeight * j);
  return buttonGroupNum - 1;
}
//注册等高按钮组，返回按钮组编号
int RegisterButtonsArea(int mode, int row, int column, int x, int y, int *width, int height, int title, int RowWeight, int ColumnWeight)
{
  int heightArray[100] = {};
  for (int i = 0; i < row; i++)
    heightArray[i] = height;
  int code = RegisterButtonsArea(mode, row, column, x, y, width, heightArray, title, RowWeight, ColumnWeight);
  return code;
}
//注册等宽按钮组，返回按钮组编号
int RegisterButtonsArea(int mode, int row, int column, int x, int y, int width, int *height, int title, int RowWeight, int ColumnWeight)
{
  int widthArray[100] = {};
  for (int i = 0; i < column; i++)
    widthArray[i] = width;
  int code = RegisterButtonsArea(mode, row, column, x, y, widthArray, height, title, RowWeight, ColumnWeight);
  return code;
}
//注册等高等宽按钮，返回按钮组编号
int RegisterButtonsArea(int mode, int row, int column, int x, int y, int width, int height, int title, int RowWeight, int ColumnWeight)
{
  int heightArray[100] = {};
  int widthArray[100] = {};
  for (int i = 0; i < row; i++)
    heightArray[i] = height;
  for (int i = 0; i < column; i++)
    widthArray[i] = width;
  int code = RegisterButtonsArea(mode, row, column, x, y, widthArray, heightArray, title, RowWeight, ColumnWeight);
  return code;
}
//注册多行一列按钮，返回按钮组编号
int RegisterRowsArea(int mode, int row, int x, int y, int width, int *height, int title, int RowWeight)
{
  return RegisterButtonsArea(mode, row, 1, x, y, &width, height, title, RowWeight, 0);
}
//注册多列一行按钮，返回按钮组编号
int RegisterColumnsArea(int mode, int column, int x, int y, int *width, int height, int title, int ColumnWeight)
{
  return RegisterButtonsArea(mode, 1, column, x, y, width, &height, title, 0, ColumnWeight);
}
//绘制按钮buttonCode的水平调节按钮
void PaintHorizontalAdjust(int buttonCode)
{
  PaintHorizontalAdjust(buttons[buttonCode].x, buttons[buttonCode].y,
    buttons[buttonCode].width, buttons[buttonCode].height);
}
void GetButtonCenter(int buttonClassCode, int row, int column, int *cx, int *cy)
{
  if (buttonClassCode < zero)//按钮类编号无效则退出
    return;
  int buttonCode = buttonGroup[buttonClassCode].firstButton + row * buttonGroup[buttonClassCode].column + column;
  *cx = buttons[buttonCode].x + buttons[buttonCode].width / 2;
  *cy = buttons[buttonCode].y + buttons[buttonCode].height / 2;
}
//在编号为ButtonClassCode的按钮组的第row行第column列显示内容Content
void PrintButton(int ButtonClassCode, int row, int column, const char *Content)
{
  if (ButtonClassCode < zero)//按钮类编号无效则退出
    return;
  int ButtonCode = buttonGroup[ButtonClassCode].firstButton + row * buttonGroup[ButtonClassCode].column + column;
  CenterView(Content, buttons[ButtonCode].x + buttons[ButtonCode].width / 2, buttons[ButtonCode].y + buttons[ButtonCode].height / 2);
}
//在编号为ButtonClassCode的按钮组的第row行第column列显示内容Content，偏移dx和dy，isGray决定是否临时使用灰色
void PrintButton(int buttonClassCode, int row, int column, const char *content,
  int dx, int dy, bool isGray = 0)
{
  if (buttonClassCode < zero)//按钮类编号无效则退出
    return;
  int ButtonCode = buttonGroup[buttonClassCode].firstButton + row * buttonGroup[buttonClassCode].column + column;
  COLORREF textcolor = gettextcolor();
  if (isGray)
    settextcolor(RGB(128, 128, 128));
  CenterView(content, buttons[ButtonCode].x + buttons[ButtonCode].width / 2 + dx,
    buttons[ButtonCode].y + buttons[ButtonCode].height / 2 + dy);
  settextcolor(textcolor);
}
//画出按钮ButttonCode的网格
void PaintButtonGrid(int ButtonCode)
{
  int Left = buttons[ButtonCode].x;
  int Right = Left + buttons[ButtonCode].width;
  int Top = buttons[ButtonCode].y;
  int Bottom = Top + buttons[ButtonCode].height;
  line(Left, Top, Left, Bottom);
  line(Right, Top, Right, Bottom);
  line(Left, Top, Right, Top);
  line(Left, Bottom, Right, Bottom);
}
//画出按钮Name区域的所有网格
void PaintButtonGroupGrid(int ButtonClassCode, int row1, int row2, int dy)
{
  if (ButtonClassCode < zero)//找不到名称为Name的按钮就退出
    return;

  int MaxRow = min(row2, buttonGroup[ButtonClassCode].row);

  int FirstButton = buttonGroup[ButtonClassCode].firstButton + (row1 - 1) * buttonGroup[ButtonClassCode].column;
  int LastButton = FirstButton + (MaxRow - row1 + 1) * buttonGroup[ButtonClassCode].column - 1;
  int Left = buttons[FirstButton].x;
  int Top = buttons[FirstButton].y + dy;
  int Right = buttons[LastButton].x + buttons[LastButton].width;
  int Bottom = buttons[LastButton].y + buttons[LastButton].height + dy;

  for (int i = 0; i <= MaxRow - row1; i++)//画横线
  {
    int ButtonCode = FirstButton + i * buttonGroup[ButtonClassCode].column;
    line(Left, buttons[ButtonCode].y + dy, Right, buttons[ButtonCode].y + dy);
  }
  line(Left, Bottom, Right, Bottom);
  for (int j = 0; j < buttonGroup[ButtonClassCode].column; j++)//画竖线
  {
    int ButtonCode = FirstButton + j;
    line(buttons[ButtonCode].x, Top, buttons[ButtonCode].x, Bottom);
  }
  line(Right, Top, Right, Bottom);
}
void PaintButtonGroupGrid(int ButtonClassCode)
{
  PaintButtonGroupGrid(ButtonClassCode, 1, INT_MAX, 0);
}
//寻找按钮类ButtonClassCode的第row行第column列的按钮编号
int FindButton(int ButtonClassCode, int row, int column)
{
  if (ButtonClassCode < zero)//找不到名称为Name的按钮则返回-1
    return -1;
  return buttonGroup[ButtonClassCode].firstButton + row * buttonGroup[ButtonClassCode].column + column;
}
/*自动转化函数*/
int transformStopSignal;//自动转化结束信号，1=结束程序
DWORD __stdcall AutoTransform(void *Title)//自动转化（无参数）
{
  HWND hWnd, hActiveWnd;
  transformStopSignal = 0;
  while (transformStopSignal == zero)//信号为0才执行剪贴板转化，信号为1立即退出线程
  {
    hWnd = FindWindow(NULL, (char *)Title);//查找轨道路径输入窗口
    if (hWnd != NULL)//窗口存在
    {
      hActiveWnd = GetForegroundWindow();//获得活动窗口句柄
      if (hWnd == hActiveWnd)//窗口为活动窗口
        CompleteClipboard();//窗口存在且活动才执行剪贴板转化，以免在外部生效
    }
    Sleep(100);
  }
  return 0;
}
//对话框函数

//初始化服务器组合框，
void InitServerCombo(HWND hDlg, int idItem, int serverSelected)
{
  char boxString[10] = {};
  HWND hCombo = GetDlgItem(hDlg, idItem);//组合框句柄
  for (int server = 0; server <= 6; server++) //添加组合框选项
    ComboBox_AddString(hCombo, serverName[server]);
  ComboBox_SetCurSel(hCombo, serverSelected);//设置默认选中
}
//设置对话框控件文本（如果是nullptr则空置）及最大输入长度length
void SetItemText(HWND hDlg, int idItem, const char *text, int length)
{
  if (text == nullptr)
    SetDlgItemText(hDlg, idItem, "");
  else
    SetDlgItemText(hDlg, idItem, text);
  SendMessage(GetDlgItem(hDlg, idItem), EM_LIMITTEXT, length, 0);//设置长度限制
}
//二级密码是否合格
bool IsPasswordLegal(const char *password)
{
  int length = strlen(password);
  if (length > 14)
    return false;
  for (int i = 0; i < length; i++)
    if (password[i] < 32 || password[i] > 126)
      return false;
  return true;
}
//服务器和二级密码输入框过程函数
INT_PTR CALLBACK ServerDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    //获取传入的参数：账号
    int account = (int)lParam;

    //对话框位置居中
    CenterDialog(hDlg);

    //设置对话框标题
    char title[100];
    sprintf_s(title, "%dP服务器和二级密码", account + 1);
    SetWindowText(hDlg, title);

    //初始化服务器组合框
    InitServerCombo(hDlg, idServer, plot[curList].server[account]);

    //设置密码输入框
    SetItemText(hDlg, idPassword, plot[curList].password[account], 14);

    SetWindowLongA(hDlg, DWLP_USER, account);//设置用户数据，以便在后面的消息中使用
    SetTimer(hDlg, 1, 1, NULL);//设置0ms定时器，将选中文本操作延后
  }
  return TRUE;
  case WM_TIMER:
    if (wParam == 1)
    {
      KillTimer(hDlg, 1);// 第一次处理就销毁定时器
      HWND hEditPassword = GetDlgItem(hDlg, idPassword);
      if (hEditPassword)
      {
        SetFocus(hEditPassword);// 设置焦点到密码输入框
        SendMessageA(hEditPassword, EM_SETSEL, 0, -1);// 选中密码输入框中的所有文本
      }
    }
    break;
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK) // 点击了“确定”按钮
    {
      //获取传入的参数：账号
      int account = GetWindowLongA(hDlg, DWLP_USER);

      //获取选择的服务器
      char serverString[100];
      int serverInput = 0;
      GetDlgItemTextA(hDlg, idServer, serverString, sizeof(serverString));
      for (int server = 0; server <= 6; server++)
        if (strcmp(serverString, serverName[server]) == 0)
          serverInput = server;

      //获取输入的密码
      char passwordInput[100];
      GetDlgItemTextA(hDlg, idPassword, passwordInput, sizeof(passwordInput));

      //密码输入不合格则报错重输
      if (!IsPasswordLegal(passwordInput))
      {
        MessageBoxA(hDlg, "二级密码不能含有中文。", "提示",
          MB_ICONINFORMATION | MB_SYSTEMMODAL);
        break;
      }

      //输入合格则保存输入
      plot[curList].server[account] = serverInput;
      strcpy_s(plot[curList].password[account], passwordInput);
      EndDialog(hDlg, IDOK);// 结束对话框
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL) // 点击了“取消”按钮
    {
      EndDialog(hDlg, IDCANCEL);
      return TRUE;
    }
    break;
  }
  return FALSE;
}
//输入服务器和二级密码
int EditServerAndPassword(int account)
{
  return DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SERVER),
    hWndActuator, ServerDialogProc, account);
}
//温馨礼包输入框过程函数
INT_PTR CALLBACK HarmonyDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    //对话框位置居中
    CenterDialog(hDlg);

    //设置对话框标题
    SetWindowText(hDlg, "温馨礼包");

    //设置提示信息
    const char tip[] = "输入账号绑定的openid，即可在签到时自动领取温馨礼包。\n"
      "方法：点击美食中心右上角的“...”，选择“复制链接”，将“openid=”后面的一串字母复制到下方即可。";
    SetDlgItemTextA(hDlg, IDC_TEXT, tip);

    //设置输入框初始文本
    for (int i = 0; i < dialogHarmonySize; i++)
      SetItemText(hDlg, idOpen1 + i, plot[curList].harmony[i], sizeof(plot[curList].harmony[i]) - 1);

    SetTimer(hDlg, 1, 1, NULL);//设置0ms定时器，将选中文本操作延后
  }
  return TRUE;
  case WM_TIMER:
    if (wParam == 1)
    {
      KillTimer(hDlg, 1);// 第一次处理就销毁定时器
      HWND hEditOpen1 = GetDlgItem(hDlg, idOpen1);
      if (hEditOpen1)
      {
        SetFocus(hEditOpen1);// 设置焦点到密码输入框
        SendMessageA(hEditOpen1, EM_SETSEL, 0, -1);// 选中密码输入框中的所有文本
      }
    }
    break;
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK) // 点击了“确定”按钮
    {
      //输入生效
      for (int i = 0; i < dialogHarmonySize; i++)
        GetDlgItemTextA(hDlg, idOpen1 + i, plot[curList].harmony[i], sizeof(plot[curList].harmony[i]));
      //移除空位
      int i = 0;//原数组指针
      int j = 0;//新数组指针
      while (i < maxHarmonySize)
      {
        if (strlen(plot[curList].harmony[i]) == 0)
          i++;
        else
        {
          if (i > j)
            strcpy_s(plot[curList].harmony[j], plot[curList].harmony[i]);
          i++;
          j++;
        }
      }
      while (j < maxHarmonySize)
      {
        strcpy_s(plot[curList].harmony[j], "");
        j++;
      }

      EndDialog(hDlg, IDOK);// 结束对话框
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL) // 点击了“取消”按钮
    {
      EndDialog(hDlg, IDCANCEL);
      return TRUE;
    }
    break;
  }
  return FALSE;
}
//输入服务器和二级密码
int EditHarmony()
{
  return DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_HARMONY),
    hWndActuator, HarmonyDialogProc, 0);
}
//新版本提示框过程函数
INT_PTR CALLBACK VersionDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    //对话框位置居中
    CenterDialog(hDlg);

    //设置对话框标题
    SetWindowText(hDlg, "提示");

    //设置提示信息
    char tip[1000];
    const char *highestVerString = (const char *)lParam;
    sprintf_s(tip, "轨道已更新至v%s，\n"
      "您可以在群文件下载最新版轨道。",
      highestVerString);
    SetDlgItemTextA(hDlg, IDC_TEXT, tip);

    //设置用户数据，以便在后面的消息中使用
    SetWindowLongPtr(hDlg, DWLP_USER, (LONG_PTR)highestVerString);
  }
  return TRUE;
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) // 点击“确定”或“取消”
    {
      //如果勾选了“不再提示”，记录不再提示的最高版本
      if (GetItemCheck(hDlg, IDC_NOTIP))
      {
        //获取用户数据：当前最高版本
        const char *highestVerString = (const char *)GetWindowLongPtr(hDlg, DWLP_USER);
        FILE *f;
        if (!fopen_s(&f, "用户参数\\不再提示.txt", "w"))
        {
          fprintf_s(f, "不再提示版本号=%s", highestVerString);
          fclose(f);
        }
      }
      EndDialog(hDlg, LOWORD(wParam));// 结束对话框
      return TRUE;
    }
    break;
  }
  return FALSE;
}
//弹出版本更新提示
int PopVersionTip(const char *highestVerString)
{
  return DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_VERSION),
    hWndActuator, VersionDialogProc, (LPARAM)highestVerString);
}
//从字符串版本号"a.b.c"获取整数表示的版本号100a+10b+c
int GetIntegerVersion(const char *versionString)
{
  int versions[3] = {};
  sscanf_s(versionString, "%d.%d.%d", &versions[0], &versions[1], &versions[2]);
  return versions[0] * 10000 + versions[1] * 100 + versions[2];
}
//FILE *fRunningTime;
//DWORD tickBegin, tickEnd;
//void WriteRunningTime(const char *title)
//{
//  tickEnd = GetTickCount();
//  fprintf(fRunningTime, "%s=%d ms\n", title, tickEnd - tickBegin);
//  tickBegin = tickEnd;
//}
struct InputBoxParam
{
  char *dest;//输入对象
  const char *prompt, *title, *defaultStr;//提示语，标题，初始文本
  int maxLength;//最大长度
};
//对话框过程函数
INT_PTR CALLBACK InputDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    //获取传入的参数
    InputBoxParam *params = (InputBoxParam *)lParam;

    //对话框位置居中
    CenterDialog(hDlg);

    SetWindowText(hDlg, params->title);//设置输入框标题
    if (params->defaultStr != NULL) //设置输入框的初始文本
      SetDlgItemText(hDlg, IDC_EDIT1, params->defaultStr);
    if (params->prompt != NULL) //设置输入框提示语
      SetDlgItemText(hDlg, IDC_TEXT, params->prompt);
    HWND hWndEdit = GetDlgItem(hDlg, IDC_EDIT1);
    SendMessage(hWndEdit, EM_LIMITTEXT, (WPARAM)(params->maxLength - 1), 0);//设置长度限制
    SetWindowLongPtr(hDlg, DWLP_USER, (LONG_PTR)params);//设置用户数据，以便在后面的消息中使用
    SetTimer(hDlg, 1, 1, NULL);//设置0ms定时器，将选中文本操作延后
  }
  return TRUE;
  case WM_TIMER:
    if (wParam == 1)
    {
      KillTimer(hDlg, 1);// 第一次处理就销毁定时器
      // 获取文本编辑框的句柄
      HWND hEdit = GetDlgItem(hDlg, IDC_EDIT1);
      if (hEdit != NULL)
      {
        SetFocus(hEdit);// 设置焦点到文本编辑框
        SendMessage(hEdit, EM_SETSEL, 0, -1);// 选中文本编辑框中的所有文本
      }
    }
    break;
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK) // 点击了“确定”按钮
    {
      // 获取编辑框中的内容
      InputBoxParam *params = (InputBoxParam *)GetWindowLongPtr(hDlg, DWLP_USER);
      GetDlgItemText(hDlg, IDC_EDIT1, params->dest, params->maxLength);
      EndDialog(hDlg, IDOK);// 结束对话框
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL) // 点击了“取消”按钮
    {
      EndDialog(hDlg, IDCANCEL);
      return TRUE;
    }
    break;
  }
  return FALSE;
}
// 自定义的InputBox函数
int NewInputBox(char *dest, int maxLength = maxPath, const char *prompt = "",
  const char *title = "输入", const char *defaultStr = "")
{
  InputBoxParam params = {}; //显示对话框并传递参数
  params.dest = dest;
  params.prompt = prompt;
  params.title = title;
  params.defaultStr = defaultStr;
  params.maxLength = maxLength;
  return DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWndActuator, InputDialogProc, (LPARAM)&params);
}
//带ban卡的轨道路径是否合法
bool IsBannedTrackLegal(const char *bannedPath)
{
  if (strchr(bannedPath, '*') || strchr(bannedPath, '?') || strchr(bannedPath, '\"')
    || strchr(bannedPath, '|') || strchr(bannedPath, '\t'))
    return false;
  if (bannedPath[0] == '<')//如果开头是'<'，按去卡符号处理
  {
    if (strchr(bannedPath + 1, '<')) //如果还有'<'，视为错误
      return false;
    const char *right = strchr(bannedPath + 1, '>');//查找'>'
    if (!right || strchr(right + 1, '>'))//如果没有'>'，或者有多于一个'>'，视为错误
      return false;
  }
  else if (strchr(bannedPath, '<') || strchr(bannedPath, '>')) //开头不是'<'，则不能有'<','>'
    return false;
  return true;
}
//轨道路径输入是否合法
bool IsTrackLegal(const char *extendedPath)
{
  const char *bar = strchr(extendedPath, '|');//查找竖线
  const char *bannedPath = extendedPath;//仅含ban卡指令的轨道
  if (bar) //如果有竖线，说明有加速指令
  {
    bannedPath = bar + 1;
    //提取加速部分
    char accelerationString[maxPath] = {};
    strcpy_s(accelerationString, extendedPath);
    accelerationString[bar - extendedPath] = 0;
    int accelerationTime = StrToNat(accelerationString);
    //加速时间必须是100-900 ms
    if (accelerationTime < 0 || accelerationTime > 1000)
      return false;
  }
  return IsBannedTrackLegal(bannedPath);
}
//输入轨道文件，直至符合格式要求为止。输入成功则返回1，否则返回0
int InputTrack(char(&track)[maxPath])
{
  char tempTrack[maxPath] = {};
  strcpy_s(tempTrack, track);

  CreateThread(NULL, 0, AutoTransform, (void *)"轨道路径", 0, NULL);//运行自动转化
  int inputButton = NewInputBox(tempTrack, maxPath,
    "输入相对路径（如Track.txt）或绝对路径（如C:\\Track.txt）。\n拖动文件到执行器内可自动填写。",
    "轨道路径", tempTrack);
  while (!IsTrackLegal(tempTrack) && inputButton == IDOK)
    inputButton = NewInputBox(tempTrack, maxPath,
      "路径中不能有【* ? \" < > |】，请重新输入。\n输入相对路径（如Track.txt）或绝对路径（如C:\\Track.txt）。\n拖动文件到执行器内可自动填写。",
      "轨道路径", tempTrack);
  transformStopSignal = 1;

  if (inputButton == IDOK)
  {
    strcpy_s(track, tempTrack);
    return 1;
  }
  return 0;
}
//输入二级密码，直到符合格式要求为止。输入成功则返回1，否则返回0
int InputPassword(char(&password)[15])
{
  char prompt[300] = "输入二级密码以使用清理背包等功能。";//输入提示
  char errorPrompt[300] = "您的输入不规范，请重新输入。\n";//输入错误时的提示
  strcat_s(errorPrompt, prompt);

  char passwordInput[15];//输入的二级密码
  strcpy_s(passwordInput, password);//先拷贝原密码

  int button = NewInputBox(passwordInput, 15, prompt, "二级密码", passwordInput);
  //如果点击了确定按钮，且输入不合格，则重新输入
  while (button == IDOK && !IsPasswordLegal(passwordInput))
    button = NewInputBox(passwordInput, 15, errorPrompt, "二级密码", passwordInput);
  if (button == IDOK) //退出循环时，如果点了确定，视为有效输入
  {
    strcpy_s(password, passwordInput);
    return 1;
  }
  return 0;
}
//将timer转化为输入字符串
void GetTimerString(char(&timerString)[10], int timer)
{
  char hms[9] = {};
  if (timer >= 100000) //~12:30:00
  {
    SecondToHms(hms, timer - 100000);
    sprintf_s(timerString, "~%s", hms);
  }
  else //12:00:00
  {
    SecondToHms(hms, timer);
    strcpy_s(timerString, hms);
  }
}
//将定时输入转化为timer，转化失败则返回-1
int GetTimer(const char *timerString)
{
  if (timerString[0] == '~')
  {
    int second = HmsToSecond(timerString + 1);
    if (second == -1)
      return -1;
    return 100000 + second;
  }
  else
    return HmsToSecond(timerString);
}
//输入倒计时，直至符合格式要求为止。输入成功则返回1，否则返回0
int InputTimer(int *pTimer)
{
  //char prompt[300] = "输入hh:mm:ss（如12:00:00表示定时到12点，可以只输hh:mm），若到点后前面的任务还未完成则自动放弃。\n"
  //  "输入~hh:mm:ss，则到点后继续执行未完成的任务。";
  char prompt[300] = "输入hh:mm:ss（如12:00:00，到点后放弃未完成的任务）\n"
    "或~hh:mm:ss（如~16:30:00，到点后继续未完成的任务）。\n"
    "可以只输hh:mm。启动后定时才会生效。";
  char errorPrompt[300] = "您的输入不规范，请重新输入。\n";
  strcat_s(errorPrompt, prompt);
  char timerString[10] = {};
  GetTimerString(timerString, *pTimer);
  int InputButton = NewInputBox(timerString, sizeof(timerString), prompt, "定时时刻", timerString);
  while (InputButton == IDOK && GetTimer(timerString) == -1)
    InputButton = NewInputBox(timerString, sizeof(timerString), errorPrompt, "定时时刻", timerString);
  if (InputButton == IDOK)//退出循环时，如果点了确定，视为有效输入
  {
    *pTimer = GetTimer(timerString);
    return 1;
  }
  return 0;
}
//输入含中文的字符串（需要自动转化）
bool InputChineseString(char *dest, int size, bool isLegal(const char *str),
  const char *prompt = "", const char *title = "输入")
{
  CreateThread(NULL, 0, AutoTransform, (void *)title, 0, NULL);//运行自动转化

  char illegalPrompt[500];//输入不规范的提示
  sprintf_s(illegalPrompt, "您的输入不规范，请重新输入。\n%s", prompt);

  char destCopy[100] = {};
  strcpy_s(destCopy, dest);//初始文本：自然数的原值

  //弹出输入框，输入文本到destCopy
  int inputButton = NewInputBox(destCopy, size, prompt, title, destCopy);
  while (inputButton == IDOK) //如果点击了确定，则检查输入
  {
    if (isLegal(destCopy))//输入在要求范围内，则保存输入
    {
      strcpy_s(dest, size, destCopy);
      transformStopSignal = 1;
      return true;
    }
    inputButton = NewInputBox(destCopy, size, illegalPrompt, title, destCopy);//否则继续输入
  }
  transformStopSignal = 1;
  return false;//取消输入
}
//任务标题是否合格
bool isTaskTitleLegal(const char *title)
{
  int length = strlen(title);
  return length > 0 && length <= 14;
}
//输入普通任务标题
bool InputTaskTitle(int task)
{
  return InputChineseString(param[task].title, 15, isTaskTitleLegal,
    "给任务取一个名字（不超过7个字）。", "任务标题");
}
//输入高级任务标题
bool InputAdvanceTitle(int list)
{
  int task = GetTask(list);
  if (InputTaskTitle(task))
  {
    strcpy_s(param[task + 1].title, param[task].title);
    return true;
  }
  return false;
}
//输入字符串到dest，直到isLegal(输入)为真。输入成功则返回1，否则返回0
template <size_t size>
bool InputString(char(&dest)[size], bool isLegal(const char *str),
  const char *prompt = "", const char *title = "输入")
{
  char illegalPrompt[500];//输入不规范的提示
  sprintf_s(illegalPrompt, "您的输入不规范，请重新输入。\n%s", prompt);

  char destCopy[100] = {};
  strcpy_s(destCopy, dest);//初始文本：自然数的原值

  //弹出输入框，输入文本到destCopy
  int inputButton = NewInputBox(destCopy, size, prompt, title, destCopy);
  while (inputButton == IDOK) //如果点击了确定，则检查输入
  {
    if (isLegal(destCopy))//输入在要求范围内，则保存输入
    {
      strcpy_s(dest, destCopy);
      return true;
    }
    inputButton = NewInputBox(destCopy, size, illegalPrompt, title, destCopy);//否则继续输入
  }
  return false;//取消输入
}
//输入温馨礼包OpenID
bool InputOpenID(char(&track)[maxPath])
{
  const char prompt[] = "输入账号绑定的openid，将在签到时领取温馨礼包。\n"
    "注：点击美食中心右上角的“...”，选择“复制链接”,\n"
    "将链接中“openid=”后面的一串字母复制到这里即可。";
  return InputString(track, [](const char *) {return true; }, prompt, "温馨礼包");
}
//判断魔塔输入是否合法
bool isTowerLegal(const char *tower)
{
  if (strlen(tower) < 2) //长度至少为2
    return false;
  if (!IsNature(tower + 1)) //第一个字母后面必须是自然数
    return false;
  int level = atoi(tower + 1);//获得这个自然数
  if (tower[0] == 'a' || tower[0] == 'A') //单人 A1-A165
    if (level >= 0 && level <= 165)
      return true;
  if (tower[0] == 'b' || tower[0] == 'B') //双人 B1-B100
    if (level >= 0 && level <= 100)
      return true;
  if (tower[0] == 'c' || tower[0] == 'C') //宠塔 C1-C25
    if (level >= 1 && level <= 25)
      return true;
  if (tower[0] == 'd' || tower[0] == 'D') //密室 D1-D4
    if (level >= 1 && level <= 4)
      return true;
  return false;
}
//输入魔塔关卡（单人1001-1165；双人2001-2100；密室3001-3004；宠塔4001-4025）
bool InputTower(int *pLevel)
{
  char towerString[5];//魔塔关卡名
  //根据关卡编号pLevel生成关卡名（如A165)
  sprintf_s(towerString, "%c%d", *pLevel / 1000 - 1 + 'A', *pLevel % 1000);
  const char *propmt =
    "单塔A1~A165，双塔B1~B100，宠塔C1~C25；\n"
    "A0/B0=自动爬单塔/双塔最高层；\n"
    "D1=炼金室，D2=藏经阁，D3=水晶宫，D4=威望屋。";
  if (InputString(towerString, isTowerLegal, propmt, "选择关卡"))
  {
    if (towerString[0] == 'a' || towerString[0] == 'A') //单人 A1-A165
      *pLevel = 1000;
    if (towerString[0] == 'b' || towerString[0] == 'B') //双人 B1-B100
      *pLevel = 2000;
    if (towerString[0] == 'c' || towerString[0] == 'C') //宠塔 C1-C25
      *pLevel = 3000;
    if (towerString[0] == 'd' || towerString[0] == 'D') //密室 D1-D4
      *pLevel = 4000;
    *pLevel += atoi(towerString + 1);
    return true;
  }
  return false;
}
//输入自然数到Num，直到满足范围Min~Max为止。输入成功则返回1，否则返回0
int InputNum(int *pNum, int minimum, int maximum, char *propmt = "", char *title = "输入")
{
  char maxString[11];
  sprintf_s(maxString, "%d", maximum);
  int maxLength = strlen(maxString) + 1;//获得最大输入长度
  char dest[11];
  char original[11];
  sprintf_s(original, "%d", *pNum);//初始文本：自然数的原值
  int InputButton = NewInputBox(dest, maxLength, propmt, title, original);//输入自然数到Dest
  int CurrentNum = StrToNat(dest);//Dest转化为自然数
  char newPrompt[1000];
  sprintf_s(newPrompt, "您的输入不规范，请重新输入。\n%s", propmt);
  while ((CurrentNum < minimum || CurrentNum > maximum) && InputButton == IDOK)//若范围不正确，且点击了确定
  {
    InputButton = NewInputBox(dest, maxLength, newPrompt, title, dest);//重新输入
    CurrentNum = StrToNat(dest);//记录到CurrentNum并重新检查
  }
  if (InputButton == IDOK)//退出循环时，如果点了确定，视为有效输入
  {
    *pNum = CurrentNum;
    return 1;
  }
  return 0;//否则视为无效输入
}
/*任务执行所需函数*/
//获取大厅标签数量
int GetTagNum(HWND hWndHall)
{
  char ClassName[256];
  GetClassName(hWndHall, ClassName, sizeof(ClassName));
  if (strcmp(ClassName, "ApolloRuntimeContentWindow") == 0)
    return 1; //微端标签数为1
  if (strcmp(ClassName, "DUIWindow") != 0)
    return 0; //不是微端也不是大厅，返回0
  //判断为游戏大厅窗口
  HWND hWndChild = NULL;
  int tagNum = 0;
  while (true)
  {
    hWndChild = FindWindowEx(hWndHall, hWndChild, "TabContentWnd", NULL); //查找一个小号窗口
    if (hWndChild == NULL)
      return tagNum;//没有小号窗口了，返回标签数量
    tagNum++;
  }
}
//弹出带OK/Cancel选项的消息框
int PopMessageOK(int task, const char *message)
{
  return MessageBox(hWndActuator, message, param[task].title, MB_OKCANCEL | MB_ICONINFORMATION | MB_SYSTEMMODAL);
}
//弹出消息框
void PopMessage(int task, const char *message)
{
  MessageBox(hWndActuator, message, param[task].title, MB_ICONINFORMATION | MB_SYSTEMMODAL);
}
//线程消息框参数
struct MessageParam
{
  int task;
  const char *message;
  bool posted;//参数是否已经传递
};
//消息框线程
DWORD __stdcall MessageThread(void *vpMessageParam)
{
  //由于传入的vpMessageParam很快被销毁，所以需要自行复制一份
  MessageParam *pMessageParam = (MessageParam *)vpMessageParam;
  int task = pMessageParam->task;
  const char *message = pMessageParam->message;
  pMessageParam->posted = true;
  PopMessage(task, message);
  return 0;
}
//创建线程弹出消息框
void PopMessageThread(int task, const char *message)
{
  MessageParam messageParam = { task, message, false };
  CreateThread(NULL, 0, MessageThread, (void *)&messageParam, 0, NULL);
  while (!messageParam.posted) //等待参数成功传递
    Sleep(10);
}
DWORD __stdcall PromptNoImage(void *pTask)//提示无图像
{
  int task = (int)pTask;
  work[task].isPromptOn = 1;
  char *message = "图像获取失败，请切换到放卡账号，且不要最小化。\n可以用执行器隐藏游戏大厅。";
  PopMessage(task, message);
  work[task].noImageTick = 0;//重新计算无图像时间
  work[task].isPromptOn = 0;//提示已完成，允许再次提示
  return 0;
}
//【公共】从秒数转化为“x分x秒”字符串
void GetTimeStringMSChinese(char *s, int Time)
{
  sprintf_s(s, 20, "%d分%d秒", Time / 60, Time % 60);
}
//反馈任务状态
void ReturnState(int task, const char *tip)
{
  if (isTaskStarted[task] == 1) //任务运行时才允许改变状态
  {
    time(&work[task].taskEndTime);//获取当前时间
    state[task].games = work[task].games;//写入各项状态
    state[task].maxGames = work[task].maxGames;
    if (work[task].gameTime > zero)
      work[task].wave = waveOutOfLevel;
    state[task].wave = work[task].wave;
    state[task].smallWave = work[task].smallWave;
    state[task].progress = work[task].progress;
    state[task].currentTime = work[task].currentTime;
    state[task].averageTime = work[task].averageTime;
    state[task].totalTime = (int)(work[task].taskEndTime - work[task].taskStartTime);
    sprintf_s(state[task].tip, "%s", tip);
    isRepaintRequired = 1;//发送重绘指令
  }
}
//点击大厅加速按钮
void Accelerate(int task)
{
  HWND hWndHall[2] = {};
  for (int account = 0; account < 2; account++)
    if (work[task].isInvolved[account])
    {
      hWndHall[account] = GetHallWindow(work[task].hWnd[account]);//寻找大厅窗口
      if (!hWndHall[account]) //无法找到大厅窗口，放弃加速
        return;
      if (hWndHall[account] == hWndHall[1 - account]) //两号大厅窗口相同，放弃加速
        return;
    }
  for (int account = 0; account < 2; account++)
  {
    RECT clientRect = {};
    GetClientRect(hWndHall[account], &clientRect);
    //经过DPI换算的加速按钮位置
    int accelerateX = clientRect.right - 340 * DPI / 96, accelerateY = 44 * DPI / 96;
    LeftClick(hWndHall[account], accelerateX, accelerateY);
  }
  work[task].isAccelerationOn = !work[task].isAccelerationOn;//记录加速开启/关闭
}
//退出任务
void ExitTask(int task)
{
  if (monitor[task].isExist) //停止任务时清空窗口
    monitor[task].ClearWindow();

  for (int account = 0; account < 2; account++)
    work[task].isQuitted[account] = false;//重置关卡退出状态
  if (work[task].isAccelerationOn) //关闭加速
    Accelerate(task);

  isTaskStarted[task] = 0;
  isTaskStarted_Confirm[task] = 0;
  isRepaintRequired = 1;//发送重绘指令
  if (work[task].hallShot) //如果内存还没释放，释放内存
  {
    free(work[task].hallShot);
    work[task].hallShot = nullptr;
  }
  ExitThread(0);
}
//任务task是否被手动刹停
bool isTaskStopped(int task)
{
  return isTaskStarted[task] == 0 ||
    (IsAdvance(task) && plot[GetList(task)].isAdvanceStarted == 0);
}
//填写高级任务关卡名
void GetLevelName(char(&levelName)[100], int type, int level)
{
  strcpy_s(levelName, "");
  const char secretName[5][10] = { "", "炼金室", "藏经阁", "水晶宫", "威望屋" };
  if (type == zero)//悬赏
    strcpy_s(levelName, bounty[level]);
  else if (type == 1)//勇士
    strcpy_s(levelName, challenge[level]);
  else if (type == 2)//魔塔
  {
    if (level / 1000 == 1)
      sprintf_s(levelName, "单%d", level % 1000);
    else if (level / 1000 == 2)
      sprintf_s(levelName, "双%d", level % 1000);
    else if (level / 1000 == 3)
      sprintf_s(levelName, "宠%d", level % 1000);
    else if (level / 1000 == 4)
      strcpy_s(levelName, secretName[level % 1000]);
  }
  else if (type == 3)//跨服
    sprintf_s(levelName, "%s%d", crossServer[level / 8], level % 8 + 8);
  else if (type == 4)//副本
    strcpy_s(levelName, instance[level]);
  else if (type == 5)//公会
    strcpy_s(levelName, missionName[level]);
  else if (type == 17)//特殊
    strcpy_s(levelName, special[level]);
  else if (type == 18)//控制
    strcpy_s(levelName, control[level]);
  else
    strcpy_s(levelName, islandLevel[type - basicTypeNum][level]);
}
//外显ID
char idString[4][200][5] = {};
//填写所有可能的外显ID字符串
void InitIDString()
{
  const char missionTitle[4][2] = { "", "G", "Q", "D" };//常规 公会 情侣 大赛
  for (int style = 0; style < 4; style++)
    for (int count = 0; count < 200; count++)
      sprintf_s(idString[style][count], "%s%d", missionTitle[style], count);
}
//获取高级任务order对应的外显ID
const char *GetID(int list, int order)
{
  int missionStyle = plot[list].advance[order].missionStyle;
  int count = 0;//同风格任务的数量
  for (int iOrder = 0; iOrder <= order; iOrder++)
    if (plot[list].advance[iOrder].missionStyle == missionStyle)
      count++;
  return idString[missionStyle][count];
}
//是否为巅峰对决任务
bool IsPeak(Advance &advance)
{
  return advance.type == 4 && advance.level == 4;
}
//是否为识别任务
bool IsMission(Advance &advance)
{
  return advance.type == 5;
}
//是否为大赛任务
bool IsContest(Advance &advance)
{
  return advance.type == 5 && advance.level == 2;
}
//是否为非战斗任务（特殊+控制）
bool IsNoBattle(Advance &advance)
{
  return advance.type == 17 || advance.type == 18;
}
//是否为控制任务
bool IsControl(Advance &advance)
{
  return advance.type == 18;
}
//是否为签到任务
bool IsSign(Advance &advance)
{
  return advance.type == 17 && advance.level == 0;
}
//是否为清包任务
bool IsClearBag(Advance &advance)
{
  return advance.type == 17 && advance.level == 2;
}
//是否为使用双倍卡任务
bool IsDoubleCardQuest(Advance &advance)
{
  return advance.type == 17 && (advance.level == 3 || advance.level == 4);
}
//是否为买魔塔任务
bool IsBuyTower(Advance &advance)
{
  return advance.type == 17 && advance.level == 5;
}
//是否为定时任务
bool IsTimer(Advance &advance)
{
  return advance.type == 18 && advance.level == 0;
}
//是否为刷新任务
bool IsRefresh(Advance &advance)
{
  return advance.type == 18 && advance.level == 1;
}
//是否为退服任务
bool IsExitServer(Advance &advance)
{
  return advance.type == 18 && advance.level == 2;
}
//是否为关机任务
bool IsShutdown(Advance &advance)
{
  return advance.type == 18 && advance.level == 3;
}
//是否为循环任务
bool IsCycle(Advance &advance)
{
  return advance.type == 18 && advance.level == 4;
}
//是否为无轨任务（识别任务或特殊任务）
bool IsNoTrack(Advance &advance)
{
  return IsMission(advance) || IsNoBattle(advance);
}
//是否无需填写轨道参数（无轨任务除去签到）
bool IsNoTrackParam(Advance &advance)
{
  return IsNoTrack(advance);
}
//是否为无需局数的任务（无轨任务除去双倍卡）
bool IsNoGames(Advance &advance)
{
  return IsNoTrack(advance) && !IsDoubleCardQuest(advance) && !IsBuyTower(advance);
}
//是否为无需卡组的任务（定时、关机、循环）
bool IsNoDeck(Advance &advance)
{
  return IsTimer(advance) || IsShutdown(advance) || IsCycle(advance);
}
//填写高级任务字符串（如"[2] 美味(神殿)"）
void GetQuestString(char(&str)[100], int list, int order)
{
  if (IsMission(plot[list].advance[order])) //识别任务
    sprintf_s(str, "[%s] %s任务识别", GetID(list, order), missionName[plot[list].advance[order].level]);
  else if (IsTimer(plot[list].advance[order])) //定时任务
  {
    tm local;
    localtime_s(&local, &plot[list].advance[order].realTimer);
    sprintf_s(str, "[%s] 定时到%02d/%02d %02d:%02d:%02d", GetID(list, order),
      local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
  }
  else //其他任务
  {
    char levelString[100];
    GetLevelName(levelString, plot[list].advance[order].type, plot[list].advance[order].level);
    sprintf_s(str, "[%s] %s(%s)", GetID(list, order), typeName[plot[list].advance[order].type], levelString);
  }
}
//创建日志文件，创建成功返回true，失败弹窗提示错误原因并返回false
bool CreateLogFile(int task, FILE **pFile, const char *code, bool report);
//写入日志文件头（启动时间，高级任务还包括已完成任务日志）
void WriteLogHead(int task, FILE *f)
{
  if (IsAdvance(task)) //高级任务：写入过去的日志
  {
    int list = GetList(task);
    fprintf(f, "%s", plot[list].logFinished);//填写已执行完毕的任务日志
    fprintf(f, "***** %s\n", plot[list].logQuestString);//填写当前任务信息
  }
  fprintf(f, "启动时间：%s\n", work[task].startTimeString); //写入启动时间
}
//往文件f中写入平均用时和每局用时
void WriteLevelTime(int task, FILE *f)
{
  //签到：记录温馨日志
  if (IsAdvance(task))
  {
    int list = GetList(task);
    if (IsSign(*plot[list].playingAdvance))
      for (int i = 0; i < plot[list].harmonyLogSize; i++)
        fprintf(f, "%s\n", plot[list].harmonyLog[i]);
  }

  char timeString[30] = {};//时间字符串
  //第二局开始后，每局开始时记录局时
  if (work[task].games > reserve[task].gamesFinished + 1)
  {
    GetTimeStringMSChinese(timeString, work[task].averageTime / 1000);
    fprintf(f, "平均用时：%s\n", timeString);//第2行写入平均用时
    //刷技能模式：10局记录一次
    if (work[task].isSkillMode)
    {
      DWORD totalTime = 0;
      for (int games = reserve[task].gamesFinished + 1; games <= work[task].games - 1; games++)//第3-n行写入各局用时
      {
        totalTime += work[task].levelTime[games];
        //到了整10场，或最后一场，则进行记录
        if (games % 10 == 0 || games == work[task].games - 1)
        {
          GetTimeStringMSChinese(timeString, totalTime / 1000);
          int firstGame = games - (games - 1) % 10;//本段起始局数
          if (firstGame == games)
            fprintf(f, "第%d局：%s\n", games, timeString);
          else
            fprintf(f, "第%d-%d局：%s\n", firstGame, games, timeString);
          totalTime = 0;
        }
      }
    }
    else //普通模式：每局记录1次
    {
      for (int games = reserve[task].gamesFinished + 1; games <= work[task].games - 1; games++)//第3-n行写入各局用时
      {
        GetTimeStringMSChinese(timeString, work[task].levelTime[games] / 1000);
        fprintf(f, "第%d局：%s\n", games, timeString);
      }
    }
  }
}
//等待time毫秒，然后检查任务是否被停止。每1000ms检查一次。
void CheckSleep(int task, int time);
//任务启动时建立日志文件夹，记录启动时间
void RecordStartTime(int task)
{
  time(&work[task].taskStartTime);//获取任务开始时间
  if (IsAdvance(task)) //高级任务启动时就填写了日志路径advanceFolderPath，复制过来即可
  {
    int list = GetList(task);
    strcpy_s(work[task].logDirectory, plot[list].advanceFolderPath);//填写日志文件夹路径
    //填写战利品文件夹路径
    sprintf_s(work[task].lootDirectory, "%s\\ID%s_战利品", work[task].logDirectory,
      GetID(list, plot[list].playingOrder));
    if (!(IsAdvance(task) && IsNoTrack(*plot[list].playingAdvance)))
      CreatePath(work[task].lootDirectory);//创建战利品文件夹（高级任务无轨关卡除外）
  }
  else //普通任务：以任务启动时间为名称创建文件夹
  {
    char startTime[100];//任务启动时间字符串
    GetTimeStringForFolder(startTime, work[task].taskStartTime);
    sprintf_s(work[task].logDirectory, "执行记录\\%s_普通任务%d", startTime, task);//填写日志文件夹路径
    sprintf_s(work[task].lootDirectory, "%s\\战利品", work[task].logDirectory);//填写战利品文件夹路径
    CreatePath(work[task].lootDirectory);//创建日志文件夹和战利品文件夹
  }
  sprintf_s(work[task].logPath, "%s\\任务日志.txt", work[task].logDirectory);//填写日志文件路径
  GetTimeString(work[task].startTimeString, work[task].taskStartTime);//开始时间转化为字符串

  if (IsDual2P(task))
    return;

  FILE *f;
  CreateLogFile(task, &f, "记录开始时间", true); ///创建日志文件
  WriteLogHead(task, f); //写入启动时间
  fclose(f);
  CheckSleep(task, 100); //防止文件关闭失败
  strcat_s(work[task].embarkString, "任务开始\n");//启动信息增加一条：开始循环刷图
}
const int recordLevel = 0, recordFinish = 1, recordAdvance = 2;
//记录每局时间（stage：0每局开始 1普通任务完成 2高级任务完成）
void RecordLevelTime(int task, int stage)
{
  if (IsDual2P(task))
    return;
  bool report = stage == 2 ? false : true;
  FILE *f;
  if (CreateLogFile(task, &f, "记录每局时间", report)) ///创建日志文件
  {
    WriteLogHead(task, f); //写入启动时间
    //每局记录和普通任务完成记录：更新本局时间
    if (stage == recordLevel || stage == recordFinish)
    {
      work[task].thisGameTick = GetTickCount();//获取当前系统时间
      //第2局开始后计算平均用时
      if (work[task].games > reserve[task].gamesFinished + 1)
      {
        work[task].levelTime[work[task].games - 1] = work[task].thisGameTick - work[task].lastGameTick;//本局秒数
        work[task].averageTime = 0;
        for (int games = reserve[task].gamesFinished + 1; games <= work[task].games - 1; games++)
          work[task].averageTime += work[task].levelTime[games];//计算第1~games-1局的总时间
        work[task].averageTime /= (work[task].games - (reserve[task].gamesFinished + 1));//计算每局平均用时，向下取整
      }
    }
    WriteLevelTime(task, f);//写入平均用时和每局用时
    //普通任务完成和高级任务完成：写入终止时间
    if (stage == 1 || stage == 2)
    {
      time(&work[task].taskEndTime);
      GetTimeString(work[task].endTimeString, work[task].taskEndTime);
      fprintf(f, "终止时间：%s\n", work[task].endTimeString);
    }
    fclose(f);
    //每局记录和普通任务完成记录：更新上局开始时刻
    if (stage == 0 || stage == 1)
      work[task].lastGameTick = work[task].thisGameTick;
  }
}
//根据大厅名称获取大厅句柄
HWND Hall(const char *hallName)
{
  HWND hWndHall = FindWindowA("DUIWindow", hallName);
  if (!hWndHall)
    hWndHall = FindWindowA("ApolloRuntimeContentWindow", hallName);
  return hWndHall;
}
//更新account窗口截图，成功返回1
int UpdateMap(int task, int account, bool report);
//记录中断截图
void RecordShot(int task)
{
  for (int account = 0; account < 2; account++) //中断截图
    if (work[task].isInvolved[account])
    {
      char shotPath[maxPath];//截图路径
      if (IsAdvance(task))
      {
        int list = GetList(task);
        char timesString[100] = "开始前";
        sprintf_s(timesString, "第%d次中断", plot[list].timesRefreshed[plot[list].playingOrder] + 1);
        sprintf_s(shotPath, "%s\\ID%s_%s_%dP.png", work[task].logDirectory,
          GetID(list, plot[list].playingOrder), timesString, account + 1);
      }
      else
        sprintf_s(shotPath, "%s\\中断截图_%dP.png", work[task].logDirectory, account + 1);
      if (UpdateMap(task, account, false)) //保存游戏窗口截图
        ColorToBitmap(work[task].map, shotPath);
      else //游戏窗口截不到就截大厅
        WindowToBitmap(Hall(work[task].hallName[account]), shotPath, 0, 0, INT_MAX, INT_MAX, 10);
    }
}
void WriteParameterToLog(int task, FILE *f)
//普通任务中断：记录启动流程和退出时参数
{
  fprintf(f, "退出时参数：\n");
  for (int account = 0; account < 2; account++)
    fprintf(f, "%dP轨道：%s\n", account + 1, work[task].track[account]);
  if (work[task].gameTime == zero)
    fprintf(f, "每局用时：自动检测\n");
  else
    fprintf(f, "每局用时：%d\n", work[task].gameTime);
  if (work[task].noImageTick > zero)
    fprintf(f, "（无图像）\n");
  else
    fprintf(f, "（有图像）\n");
}
//记录错误日志（report：创建日志失败时是否报错）
void RecordError(int task, bool report)
{
  FILE *f;
  if (CreateLogFile(task, &f, "记录错误信息", report)) ///创建日志文件
  {
    WriteLogHead(task, f); //写入启动时间（高级任务还包括已完成日志）
    WriteLevelTime(task, f);//写入平均用时和每局用时

    fprintf(f, "第%d局：%s\n", work[task].games, state[task].errorString);//写入错误原因和终止时间
    time(&work[task].taskEndTime);
    GetTimeString(work[task].endTimeString, work[task].taskEndTime);
    fprintf(f, "终止时间：%s\n", work[task].endTimeString);

    if (IsNormal(task)) //普通任务中断时记录任务参数
      WriteParameterToLog(task, f);
    fclose(f);
  }
}
//把错误信息发送到服务器
int SendErrorToServer(const char *tip, const char *errorString, int stopLevel)
{
  char u8Tip[1000];
  char u8ErrorString[1000];
  AnsiToUtf8(tip, u8Tip);
  AnsiToUtf8(errorString, u8ErrorString);

  // 目标 URL: https://doc.mstrack.cn/api/data
  LPCWSTR host = L"doc.mstrack.cn";
  LPCWSTR path = L"/api/data";

  // 构造 JSON 数据
  char json_data[1000];
  sprintf_s(json_data,
    "{"
    "\"type\": \"%s\","
    "\"message\": \"%s\","
    "\"stack\": \"%d\""
    "}",
    u8Tip, u8ErrorString, stopLevel);

  // 将 JSON 数据转为 UTF-16
  int jsonLen = (int)strlen(json_data);
  DWORD dwBytesWritten = 0;

  // 1. 打开 WinHTTP session
  HINTERNET hSession = WinHttpOpen(L"A WinHTTP Example/1.0",
    WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
    WINHTTP_NO_PROXY_NAME,
    WINHTTP_NO_PROXY_BYPASS, 0);

  if (!hSession)
    return 1;

  // 2. 连接服务器
  HINTERNET hConnect = WinHttpConnect(hSession, host,
    INTERNET_DEFAULT_HTTPS_PORT, 0);
  if (!hConnect)
  {
    WinHttpCloseHandle(hSession);
    return 1;
  }

  // 3. 创建请求
  HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", path,
    NULL, WINHTTP_NO_REFERER,
    WINHTTP_DEFAULT_ACCEPT_TYPES,
    WINHTTP_FLAG_SECURE);
  if (!hRequest)
  {
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return 1;
  }

  // 4. 设置 HTTP 头
  LPCWSTR headers = L"Content-Type: application/json\r\n";
  WinHttpAddRequestHeaders(hRequest, headers, -1L, WINHTTP_ADDREQ_FLAG_ADD);

  // 5. 发送请求
  BOOL bResults = WinHttpSendRequest(hRequest,
    WINHTTP_NO_ADDITIONAL_HEADERS,
    0, (LPVOID)json_data,
    jsonLen, jsonLen, 0);
  if (bResults)
  {
    // 6. 接收响应
    bResults = WinHttpReceiveResponse(hRequest, NULL);
    if (bResults)
    {
      // 7. 读取响应数据
      DWORD dwSize = 0;
      do
      {
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
          break;
        if (!dwSize)
          break;

        char *buffer = (char *)malloc(dwSize + 1);
        if (!buffer)
          break;

        ZeroMemory(buffer, dwSize + 1);
        DWORD dwDownloaded = 0;
        WinHttpReadData(hRequest, (LPVOID)buffer, dwSize, &dwDownloaded);

        free(buffer);

        if (dwDownloaded == 0)
          break;
      } while (dwSize > 0);
    }
  }

  // 8. 清理
  WinHttpCloseHandle(hRequest);
  WinHttpCloseHandle(hConnect);
  WinHttpCloseHandle(hSession);

  return 0;
}
const int RETRY_TASK = 1, SKIP_TASK = 2, END_TASK = 3;//报错等级
// 报错中断：更新状态并结束任务。log：是否记录日志
void ReportError0(int task, const char *tip, const char *errorString,
  int stopLevel = RETRY_TASK, bool log = true)
{
  SendErrorToServer(tip, errorString, stopLevel);//发送报错信息到服务器
  RecordShot(task);//中断截图
  ReturnState(task, tip);//返回状态信息
  strcpy_s(state[task].errorString, errorString);//记录出错原因

  if (IsDual2P(task)) //同步任务2P的报错转交给1P
  {
    strcpy_s(work[task - 1].dualErrorTip, tip);
    strcpy_s(work[task - 1].dualErrorString, errorString);
    work[task - 1].dualErrorLevel = stopLevel;
    work[task - 1].dualError = true;
    CheckSleep(task, 1000000);//等待任务被刹停
    return;
  }

  if (log)
    RecordError(task, true);

  for (int account = 0; account < 2; account++)//重置退出状态
    work[task].isQuitted[account] = false;

  if (IsAdvance(task)) //高级任务根据报错等级决定刷新等级
  {
    int list = GetList(task);
    if (maxRefreshTimes == 0) //自动刷新关闭：任何stopLevel都是强制终止
      state[task].refreshLevel = END_TASK;
    else //自动刷新开启：分类讨论
    {
      //大赛/营地模式
      if (listStyle == CONTEST || listStyle == CAMP)
      {
        //刷新重试（1）且次数未满则继续
        if (stopLevel == RETRY_TASK && plot[list].timesRefreshed[plot[list].playingOrder] < maxRefreshTimes)
          state[task].refreshLevel = RETRY_TASK;
        else //刷新跳过（2）、终止（3）或刷新次数已满，则终止
          state[task].refreshLevel = END_TASK;
      }
      else //非大赛模式
      {
        if (stopLevel == RETRY_TASK) //刷新重试（1）：次数未满则重试，次数已满则跳过
        {
          if (plot[list].timesRefreshed[plot[list].playingOrder] < maxRefreshTimes)
            state[task].refreshLevel = RETRY_TASK;
          else
            state[task].refreshLevel = SKIP_TASK;
        }
        else //跳过模式直接跳过，终止模式直接终止
          state[task].refreshLevel = stopLevel;
      }
    }
  }
  //普通任务中断，或高级任务遇到需要终止的情形，则弹窗报错
  if (IsNormal(task) || state[task].refreshLevel == END_TASK)
  {
    isTaskStarted[task] = 0;//让按钮显示“启动”
    isRepaintRequired = 1;//发送重绘指令
    PopMessage(task, errorString);
  }
  ExitTask(task);
}
//报错中断
void ReportError(int task, int account, const char *tip, const char *errorString,
  int stopLevel = RETRY_TASK)
{
  char errorStringWithPlayer[1000];//带[nP]前缀的报错原因
  sprintf_s(errorStringWithPlayer, "[%dP]%s", account + 1, errorString);
  ReportError0(task, tip, errorStringWithPlayer, stopLevel);
}
//获取当前列表的下一个killTime，没有则返回0
time_t GetKillTime(int list)
{
  for (int order = plot[list].playingOrder + 1; order < plot[list].advanceNum; order++)
  {
    Advance &advance = plot[list].advance[order];
    if (advance.isSelected && IsTimer(advance) && advance.timer < 100000)
      return advance.realTimer;
  }
  return 0;
}
//等待time毫秒，然后检查任务是否被停止。每1000ms检查一次。
void CheckSleep(int task, int sleepTime)
{
  while (sleepTime >= 0)
  {
    Sleep(sleepTime >= 1000 ? 1000 : sleepTime);
    if (isTaskStopped(task))
    {
      strcpy_s(state[task].tip, "已停止");//修改状态为“已停止”
      ExitTask(task);
    }
    if (IsDual1P(task))
    {
      int list = GetList(task);
      time_t killTime = GetKillTime(list);
      //如果已经到达定时时刻，报错并放弃任务
      if (killTime > 0 && time(nullptr) > killTime)
        ReportError0(task, "定时到达", "定时已到，任务被放弃", SKIP_TASK);
    }
    if (work[task].dualError) //检查同步任务报错
    {
      work[task].dualError = false;
      ReportError0(task, work[task].dualErrorTip, work[task].dualErrorString, work[task].dualErrorLevel);
    }
    sleepTime -= 1000;
  }
}
//创建日志文件，创建成功返回true，失败弹窗提示错误原因并返回false
bool CreateLogFile(int task, FILE **pFile, const char *code, bool report)
{
  if (fopen_s(pFile, work[task].logPath, "w"))
  {
    char message[500];
    if (report) //在普通任务内触发报错
    {
      ReportLastError();//弹窗提示打开失败的原因
      sprintf_s(message, "打开日志失败。路径：\n%s\n代码位置：%s", work[task].logPath, code);
      ReportError0(task, "打开日志失败", message, END_TASK, false);
    }
    return false;
  }
  return true;
}
//记录已完成的任务日志
void GetLogFinished(int list)
{
  int task = GetTask(list);
  plot[list].logFinished[0] = 0;
  FILE *f;
  if (fopen_s(&f, work[task].logPath, "r"))//打开高级任务日志
    return;
  char s[1000];
  while (!feof(f))
  {
    fgets(f, s);//读取一行
    if (strlen(s) == 0)
      break;
    strcat_s(plot[list].logFinished, s);
    strcat_s(plot[list].logFinished, "\n");//将本行内容及换行符写入LogFinished
  }
  fclose(f);
  if (strlen(plot[list].logFinished) > maxLogLength - 25000)
    ReportError0(task, "日志爆满", "日志爆满", END_TASK);
}
//新增一条输出信息
void AddOutput(int list, char *message)
{
  strcpy_s(plot[list].output[plot[list].outputNum], message);//填写输出信息
  plot[list].outputNum++;//输出信息条数+1
  plot[list].outputPage = (plot[list].outputNum - 1) / 6 + 1;//自动翻到最新一页
  isRepaintRequired = 1;//发送重绘指令
}
//删除最后一条输出信息
void DeleteOutput(int list)
{
  plot[list].outputNum--;//输出信息条数-1
}
//载入战利品
void LoadLoot()
{
  const char folder[] = "附加程序\\图片\\物品";
  if (!FileExist(folder))
    ReportMissingFile(folder);

  //载入双爆卡图像
  BitmapToColor("附加程序\\图片\\物品\\特殊\\双经卡.png", doubleCard[0]);
  BitmapToColor("附加程序\\图片\\物品\\特殊\\双爆卡.png", doubleCard[1]);

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, lootList, maxLootNum);//查找所有png文件

  char path[maxPath] = {};
  lootNum = 0;
  for (int i = 0; i < filesNum && lootNum < maxLootNum; i++)
  {
    sprintf_s(path, "%s\\%s", folder, lootList[i]);
    if (FileExist(path)) //如果文件存在
    {
      BitmapToColor(path, loot[lootNum].image);//读取卡片图像到image
      lootList[i][strlen(lootList[i]) - 4] = 0;//删除".png"
      strcpy_s(loot[lootNum].name, lootList[i]);//记录名称
      loot[lootNum].isReplacedRequired = true;
      lootNum++;
    }
  }
}
//门票类型
struct Ticket
{
  char name[50];//门票名称
  int style;//门票种类：0=钥匙 1=徽章A 2=徽章B
  COLORREF image[propHeight][propWidth];//门票图像
};
Ticket ticket[maxTicketNum];//门票
int ticketNum;//门票种数
//新增一张门票
void AddTicket(const char *path, const char *name, int style)
{
  if (!FileExist(path))
    ReportMissingFile(path);
  BitmapToColor(path, ticket[ticketNum].image);//读取图像到image
  strcpy_s(ticket[ticketNum].name, name);//记录名称
  ticket[ticketNum].style = style;//记录类型
  ticketNum++;
}
//读取所有门票
void LoadTicket()
{
  const char folder[] = "附加程序\\图片\\物品\\门票";
  if (!FileExist(folder))
    return;

  //1. 读取"门票顺序.txt"，载入所有的钥匙、徽章名称
  char key[100][50] = {};//钥匙名称
  char badge[100][50] = {};//徽章名称
  int keyNum = 0, badgeNum = 0;
  const char sequence[] = "附加程序\\图片\\物品\\门票\\门票顺序.txt";
  FILE *f;
  if (fopen_s(&f, sequence, "r"))
    ReportMissingFile(sequence);
  char s[50] = {};
  fgets(f, s);
  fgets(f, s);
  bool badgeArea = false;//是否已读到徽章区
  while (strlen(s) > 0)
  {
    if (strstr(s, "--徽章类--"))
      badgeArea = true;
    else
    {
      if (badgeArea)
        strcpy_s(badge[badgeNum++], s);
      else
        strcpy_s(key[keyNum++], s);
    }
    fgets(f, s);
  }
  fclose(f);

  //2. 依次读取每个门票文件
  char path[maxPath] = {};
  ticketNum = 0;
  //读取钥匙
  for (int i = 0; i < keyNum && ticketNum < maxTicketNum; i++)
  {
    sprintf_s(path, "%s\\%s.png", folder, key[i]); //钥匙图片路径
    AddTicket(path, key[i], 0);
  }
  //读取徽章
  for (int i = 0; i < badgeNum && ticketNum < maxTicketNum - 1; i++)
  {
    sprintf_s(path, "%s\\%sA.png", folder, badge[i]); //徽章A图片路径
    AddTicket(path, badge[i], 1);
    sprintf_s(path, "%s\\%sB.png", folder, badge[i]); //徽章B图片路径
    AddTicket(path, badge[i], 2);
  }
}
const int textWidth = 14, textHeight = 14;//情侣、大赛文字宽度和高度
//字库文字类型
struct Text
{
  char name[10];
  COLORREF image[textHeight][textWidth];
};
const int maxTextNum = 1000;
Text text[maxTextNum];//字库中的文字
char textList[maxTextNum][maxPath];
int textNum;//文字数量

//用于文字识别的字典树
struct Trie
{
  int next[2];//0子树和1子树的编号，0表示无子树
  char text[4];//是否有到此为止的文字
};
const int maxNodesNum = 90750;//最大节点数量
Trie trie[maxNodesNum];//用于文字识别的字典树
int nodesNum;//当前节点数量
//向字典树中插入文字
void TrieInsert(const char *text, COLORREF(&textImage)[textHeight][textWidth], int width)
{
  int curNode = 0;//当前节点编号
  for (int x = 0; x < width; x++)
    for (int y = 0; y < textHeight; y++)
    {
      bool color = textImage[y][x] == 0xffffff;//这一点的颜色，白1黑0
      if (trie[curNode].next[color] == 0)
        trie[curNode].next[color] = nodesNum++;
      curNode = trie[curNode].next[color];//前往下一个节点
    }
  strcpy_s(trie[curNode].text, text);
}
//从字典树中查询指定图像对应的文本
bool TrieSearch(char(&text)[4], COLORREF(&textImage)[textHeight][textWidth], int width)
{
  memset(text, 0, sizeof(text));
  int curNode = 0;//当前节点编号
  for (int x = 0; x < width; x++)
    for (int y = 0; y < textHeight; y++)
    {
      bool color = textImage[y][x] == 0xffffff;//这一点的颜色，白1黑0
      if (trie[curNode].next[color] == 0)
        return false;
      curNode = trie[curNode].next[color];//前往下一个节点
    }
  strcpy_s(text, trie[curNode].text);
  return strlen(text) > 0;
}
//使用"字典树.dat"构建字典树
void LoadTrie()
{
  FILE *f;
  if (fopen_s(&f, "附加程序\\字典树.dat", "rb"))
    ExitMessage("缺少依赖文件：\n附加程序\\字典树.dat");
  fread_s(trie, sizeof(trie), sizeof(Trie), maxNodesNum, f);
  fclose(f);
}
//使用字库构建文字识别字典树
void BuildTrie()
{
  const char folder[] = "文字";
  if (!FileExist(folder))
    return;

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, textList, maxTextNum);//查找所有png文件

  textNum = 0;
  nodesNum = 1;//建树前已经有一个根节点
  char path[maxPath] = {};
  int width = 0, height = 0;//文字图片尺寸
  for (int i = 0; i < filesNum && textNum < maxTextNum; i++)
  {
    sprintf_s(path, "%s\\%s", folder, textList[i]);
    if (FileExist(path)) //如果文件存在
    {
      BitmapToColor(path, text[textNum].image, &width, &height);//读取图像到image
      textList[i][strlen(textList[i]) - 4] = 0;//删除".png"
      strcpy_s(text[textNum].name, textList[i]);//记录名称
      //如果是小写英文字母（文件名为"aa.png"），名称取第1个字符
      if (text[textNum].name[0] >= 'a' && text[textNum].name[0] <= 'z')
        text[textNum].name[1] = 0;
      TrieInsert(text[textNum].name, text[textNum].image, width);//插入到字典树
      textNum++;
    }
  }

  //开发者模式记录节点数
  if (developerMode)
  {
    char message[1000] = {};
    FILE *fNode;
    fopen_s(&fNode, "节点数.txt", "w");
    fprintf_s(fNode, "节点数=%d", nodesNum);
    fclose(fNode);
  }

  //保存"字典树.dat"
  FILE *f;
  fopen_s(&f, "附加程序\\字典树.dat", "wb");
  fwrite(trie, sizeof(Trie), maxNodesNum, f);
  fclose(f);
}
//载入自定卡片
void LoadCard()
{
  const char folder[] = "用户参数\\自定卡片";
  if (!FileExist(folder))
    return;

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, cardList, maxCardNum);//查找所有png文件

  cardNum = 0;
  char path[maxPath] = {};
  for (int i = 0; i < filesNum && cardNum < maxCardNum; i++)
  {
    sprintf_s(path, "%s\\%s", folder, cardList[i]);
    if (FileExist(path)) //如果文件存在
    {
      //如果图片名称合格，则进行记录
      if (IsItemPathLegal(cardList[i], card[i].name, &card[cardNum].requiredSimilarity,
        &card[cardNum].offsetX, &card[cardNum].offsetY, &card[cardNum].width, &card[cardNum].height))
      {
        BitmapToColor(path, card[cardNum].image);//读取卡片图像到image
        cardNum++;
      }
    }
  }
}
//返回名称为itemName的物件编号
int FindItem(char *itemName)
{
  for (int i = 0; i < itemNum; i++)
    if (strcmp(item[i].name, itemName) == 0)
      return i;
  return -1;
}
//根据卡片名获取卡片序号
int GetOrder(int task, int account, const char *cardName)
{
  for (int order = 0; order <= work[task].slotNum[account]; order++)
    if (strcmp(work[task].slot[account][order].name, cardName) == 0)
      return order;
  return -1;
}
//返回名称为itemName的目标编号
int FindTargetz(int task, const char *itemName)
{
  for (int i = 0; i < work[task].targetNum; i++)
    if (strcmp(work[task].target[i].name, itemName) == 0)
      return i;
  return -1;
}
//将卡槽order的图像加入目标列表（成功返回0，图像不存在返回-1，目标数量已满返回-2）
int AddTarget(int task, int account, int order)
{
  int targetCode = FindTargetz(task, work[task].slot[account][order].image);
  if (targetCode == -1) //目标列表不存在该物件，增加一项目标
  {
    if (work[task].targetNum >= maxTargetNum)
      return -2;
    Target &target = work[task].target[work[task].targetNum];
    strcpy_s(target.name, work[task].slot[account][order].image);
    work[task].targetNum++;//目标数量+1
  }
  return 0;
}

//星级判定区
const int starCoreX = 9, starCoreY = 8, starCoreWidth = 6, starCoreHeight = 8;
COLORREF starCore[17][starCoreHeight][starCoreWidth];//17个星级的图像
//读取星级颜色
void LoadStar()
{
  char path[maxPath];
  for (int star = 1; star <= 16; star++)
  {
    sprintf_s(path, "附加程序\\图片\\星级\\%d.png", star);
    BitmapToColor(path, starCore[star]);
  }
}
//Work[task].map (x0,y0)位置是几星级
int GetStarXY(int task, int x0, int y0)
{
  for (int star = 1; star <= 16; star++)
    if (IsBitmapEqual(work[task].map, starCore[star], starCoreWidth, starCoreHeight, x0, y0))
      return star;
  return 0;
}
//读取选卡界面row行column列卡片的星级
int GetCustomStar(int task, int row, int column)
{
  int startX = cellX + starCoreX + column * customWidth;
  int startY = reserve[task].cellY + starCoreY + row * customHeight;
  return GetStarXY(task, startX, startY);
}
//读取防御卡背包row行column列卡片的星级
int GetSlotStar(int task, int row, int column)
{
  int startX = propX + starCoreX + column * customWidth;
  int startY = reserve[task].slotY + starCoreY + row * customHeight;
  return GetStarXY(task, startX, startY);
}
//保存本页星级图片
void SaveStar(int task)
{
  char path[maxPath];
  for (int row = 0; row < 4; row++)
    for (int column = 0; column < 11; column++)
    {
      sprintf_s(path, "星级图片\\%d,%d.png", row, column);
      int startX = cellX + starCoreX + column * customWidth;
      int startY = reserve[task].cellY + starCoreY + row * customHeight;
      ColorToBitmap(work[task].map, path, startX, startY, starCoreWidth, starCoreHeight);
    }
  ReportError(task, 0, "保存星级完成", "保存星级完成", END_TASK);
}
//识别位置(x0,y0)的卡是否为灰色
bool IsCustomGray(int task, int code, int x0, int y0)
{
  for (int x = 0; x < customCoreWidth; x++)
    for (int y = 0; y < customCoreHeight; y++)
    {
      int r = bgrRValue(work[task].map[y0 + y][x0 + x]);
      int g = bgrGValue(work[task].map[y0 + y][x0 + x]);
      int b = bgrBValue(work[task].map[y0 + y][x0 + x]);
      if (r != g || g != b)
        return false;
    }
  return true;
}
//识别位置(x0,y0)的卡和自定卡code是否相等
bool IsCustomEqualXY(int task, int code, int x0, int y0)
{
  for (int x = 0; x < customCoreWidth; x++)
    for (int y = 0; y < customCoreHeight; y++)
      if (work[task].custom[code].image[y][x] != work[task].map[y0 + y][x0 + x])
        return false;
  return true;
}
//给定背包左上角(x0,y0)，判断row行column列的卡和自定卡code是否相等
bool IsCustomEqual(int task, int code, int x0, int y0, int row, int column)
{
  //卡片不绑定，返回-1
  int lockX = x0 + column * customWidth + 13;
  int lockY = y0 + row * customHeight + 51;
  if (work[task].map[lockY][lockX] != 0xff2b9c)
    return false;

  //判断卡片是否匹配
  int startX = x0 + column * customWidth + customX;
  int startY = y0 + row * customHeight + customY;
  return IsCustomEqualXY(task, code, startX, startY);
}
//给定背包左上角(x0,y0)，判断row行column列的道具和门票code是否相等
bool IsTicketEqual(int task, int code, int x0, int y0, int row, int column)
{
  //门票左上角位置
  int ticketX = x0 + column * propWidth;
  int ticketY = y0 + row * propHeight;

  //门票数量区超出背包下界，返回false
  if (ticketY > 386)
    return false;

  //判断门票是否匹配
  return IsBitmapEqual(work[task].map, ticket[code].image, lootCoreWidth, lootCoreHeight,
    ticketX + lootCoreX, ticketY + lootCoreY, lootCoreX, lootCoreY);
}
//若选卡界面row行column列的卡和自定卡code相等则返回星级，否则返回-1
//option：0=任意星级 1=指定targetStar星 2=不超过targetStar星
int GetEqualCustomStar(int task, int code, int row, int column, int option, int targetStar)
{
  //卡片不绑定或不匹配：返回-1
  if (!IsCustomEqual(task, code, cellX, reserve[task].cellY, row, column))
    return -1;

  //读取星级
  int star = GetCustomStar(task, row, column);
  if (option == 0) //任意星级
    return star;
  else if (option == 1) //指定星级
  {
    if (star == targetStar)
      return star;
  }
  else if (option == 2) //最高星级
  {
    if (star <= targetStar)
      return star;
  }
  return -1;
}
//若防御卡背包row行column列的卡和自定卡code相等则返回星级，否则返回-1
int GetEqualSlotStar(int task, int code, int row, int column)
{
  //卡片不绑定或不匹配：返回-1
  if (!IsCustomEqual(task, code, propX, reserve[task].slotY, row, column))
    return -1;
  return GetSlotStar(task, row, column);
}
//读取(x0,y0)位置的道具数量数字（无数字返回-1）
int GetPropDigit(int task, int x0, int y0)
{
  for (int num = 0; num <= 9; num++)
    if (IsBitmapCovering(work[task].map, lootDigit[num], lootDigitWidth, lootDigitHeight, x0, y0, 0, 0, 0xffffff))
      return num;
  return -1;
}
//读取row行column列的道具数量（无数量返回1）
int GetPropCount(int task, int row, int column)
{
  //道具数量个位数位置
  int digitX = propX + column * propWidth + lootDigitX;
  int digitY = reserve[task].propY + row * propHeight + lootDigitY;

  const int digitBase[5] = { 1, 10, 100, 1000, 10000 };//数位基数
  int count = 0;
  //从个位开始读取各位数字，计算总数count
  for (int i = 0; i < 5; i++)
  {
    int digit = GetPropDigit(task, digitX - lootDigitDistance * i, digitY);
    if (digit == -1)
      break;
    count += digit * digitBase[i];
  }
  //没有显示数量说明数量为1
  if (count == 0)
    count = 1;
  return count;
}
//若道具背包row行column列的物品和门票code相等则返回数量，否则返回-1
int GetEqualTicketCount(int task, int code, int row, int column)
{
  //门票数量区越界或不匹配：返回-1
  if (!IsTicketEqual(task, code, propX, reserve[task].propY, row, column))
    return -1;
  return GetPropCount(task, row, column);
}
//在本页44张卡中查找自定卡code的位置，返回星级。找不到返回-1
//option：0=最靠前的卡 1=指定targetStar星级 2=不超过targetStar的最高星级
int FindCustom(int task, int code, int option, int targetStar, int *pRow = nullptr, int *pColumn = nullptr)
{
  int maxStar = -1;//本页最高星级
  for (int row = 0; row < 4; row++)
    for (int column = 0; column < 11; column++)
    {
      //查找row行column卡片是否匹配，是则记录星级
      int star = GetEqualCustomStar(task, code, row, column, option, targetStar);
      if (star > maxStar)//如果匹配且超过已记录的最高星级
      {
        if (pRow) //记录行列
          *pRow = row;
        if (pColumn)
          *pColumn = column;
        maxStar = star; //更新最高星级
        if (option == 0 || option == 1) //如果是任意星级或指定星级，匹配成功1次即返回
          return star;
      }
    }
  //查找完成返回maxStar，如果maxStar==-1则说明查找失败
  return maxStar;
}
//在防御卡背包本页查找自定卡code的位置，返回星级。（不存在为-1）
int FindSlotInPage(int task, int code, int *pRow = nullptr, int *pColumn = nullptr)
{
  int maxStar = -1;//本页最高星级
  for (int row = 0; row < 6; row++)
    for (int column = 0; column < 9; column++)
    {
      //查找row行column卡片是否匹配，是则记录星级
      int star = GetEqualSlotStar(task, code, row, column);
      if (star > maxStar)//如果匹配且超过已记录的最高星级
      {
        if (pRow) //记录行列
          *pRow = row;
        if (pColumn)
          *pColumn = column;
        maxStar = star; //更新最高星级
      }
    }
  //查找完成返回maxStar，如果maxStar==-1则说明查找失败
  return maxStar;
}
//公会任务相关函数
const int guildStateX = 357, guildStateWidth = 10, guildStateHeight = 10;//“进行中”字样位置
int guildNum;//公会任务数量
char guildInProgress[guildStateWidth][guildStateHeight];//进行中字样01谱（1=白色）
bool guildImage[maxAdvanceNum][guildWidth][guildHeight];//公会任务名01谱（1=guildColor）
const COLORREF guildColor = 0x001b32;//公会任务名边框颜色
const COLORREF guildBarColor = 0x0c497d;//公会任务名左侧竖条颜色
//载入公会任务名图像到guildImage数组中
void LoadGuildImage()
{
  COLORREF tempGuildImage[guildHeight][guildWidth] = {};
  char path[maxPath];
  for (guildNum = 0; guildNum < maxAdvanceNum; guildNum++)//载入公会任务
  {
    sprintf_s(path, "附加程序\\公会任务\\%d.png", guildNum + 1);
    if (!FileExist(path))
      break;
    BitmapToColor(path, tempGuildImage);
    for (int x = 0; x < guildWidth; x++)
      for (int y = 0; y < guildHeight; y++)
        guildImage[guildNum][x][y] = tempGuildImage[y][x] == guildColor;
  }
  //读取“进行中”字样
  FILE *f;
  fopen_s(&f, "附加程序\\公会任务\\进行中.txt", "r");
  for (int x = 0; x < guildStateWidth; x++)
    for (int y = 0; y < guildStateHeight; y++)
      guildInProgress[x][y] = fgetc(f) - '0';
  fclose(f);
}
//将Work[task].Map图像中guildY高度的任务记入curGuildImage
void GetCurGuildImage(int task, int guildY)
{
  int list = GetList(task);
  for (int x = 0; x < guildWidth; x++)
    for (int y = 0; y < guildHeight; y++)
      plot[list].curGuildImage[x][y] = work[task].map[guildY + y][guildX + x] == guildColor;
}
//curGuild与guild[order]是否一致
bool guildEqual(int task, int order)
{
  int list = GetList(task);
  for (int x = 0; x < guildWidth; x++)
    for (int y = 0; y < guildHeight; y++)
      if (plot[list].curGuildImage[x][y] != guildImage[order][x][y]) //有一个颜色不匹配就返回false
        return false;
  return true;
}
//成长值是否已满
bool isGrowthFull(const char *growth)
{
  const char *slash = strchr(growth, '/');//找斜杠位置
  if (!slash) //没有斜杠，识别失败，视为成长值已满
    return true;
  int currentGrowth = atoi(growth);
  int maxGrowth = atoi(slash + 1);
  return currentGrowth >= maxGrowth;
}
const int ListX = 138, ListY = 126, ListWidth = 23, ListHeight = 229;//房间列表
//8类数字名称
const int LIST = 0, INDOOR = 1, CROSS_LIST = 2, CROSS_INDOOR = 3, FIRE = 4, TREE = 5,
LIST_WIN7 = 6, INDOOR_WIN7 = 7, CROSS_LIST_WIN7 = 8, CROSS_INDOOR_WIN7 = 9, FIRE_WIN7 = 10, TREE_WIN7 = 11;

//数字类型：保存房间号和火苗0-9数字图像，用于读数
template <size_t width, size_t height>
class DigitType
{
private:
  bool digit[11][width][height];//0-9十个数字和斜杠的图像：颜色等于digitColor为true，否则为false
  //get函数使用的方法：0=列表；1=房间内；2=跨服列表；3=跨服房间内
  int method;
  const char *name;
  int digitNum;
public:
  int digitX, digitY, digitDX;//起始XY坐标和数字间距
  int roomDX, roomDY;//房间间距（只有跨服列表需要设定）
  int lengthDX[6];//数字间距，长度增加1位带来的左偏移
  bool IsTextColor(COLORREF color)
  {
    if (method == LIST || method == LIST_WIN7)
      return color == 0xf8ffc6;
    else if (method == INDOOR)
      return color == 0xffffff;
    else if (method == CROSS_LIST || method == CROSS_LIST_WIN7)
      return color == 0xffffff;
    else if (method == CROSS_INDOOR)
      return color == 0xdceeff;
    else if (method == FIRE || method == FIRE_WIN7)
      return color == 0x000000;
    else if (method == TREE || method == TREE_WIN7)
      return color == 0x1ae641 || color == 0x00ff00 || color == 0xff0000;
    else if (method == INDOOR_WIN7 || method == CROSS_INDOOR_WIN7)
    {
      //win7数字判据：颜色为底色与数字颜色的混合色，且更偏向于数字颜色
      int value[3] = { bgrRValue(color), bgrGValue(color), bgrBValue(color) };//RGB分量
      if (method == INDOOR_WIN7) //房间内：R,G,B相等且不小于128
        return value[0] == value[1] && value[1] == value[2] &&
        value[0] >= 128 && value[1] >= 128 && value[2] >= 128;
      else //跨服房间内：需要计算比例
      {
        const int black[3] = { 8, 51, 92 };
        const int white[3] = { 220, 238, 255 };
        double maxRatio = 0, minRatio = 1;
        for (int i = 0; i < 3; i++)
        {
          double ratio = (double)(value[i] - black[i]) / (white[i] - black[i]);
          maxRatio = max(maxRatio, ratio);
          minRatio = min(minRatio, ratio);
        }
        return minRatio >= 0.5 && maxRatio - minRatio <= 0.015;
      }
    }
    return false;
  }
private:
  //判断(x0,y0)起始的图像是否为数字n
  bool Equal(int task, int x0, int y0, int n)
  {
    for (int x = 0; x < width; x++)
      for (int y = 0; y < height; y++)
        if (IsTextColor(work[task].map[y0 + y][x0 + x]) != digit[n][x][y])
          return false;
    return true;
  }
  //读取从(x0,y0)开始的数字
  int GetDigitXY(int task, int x0, int y0)
  {
    for (int n = 0; n < digitNum; n++)
      if (Equal(task, x0, y0, n))
        return n;
    return -1;
  }
  //读取第order位数字，提供行（row）列（column）位数（length）指定高度（height）信息
  int GetDigit(int task, int order, int row = 0, int column = 0, int length = 0, int height = 0)
  {
    int x0 = digitX + column * roomDX + (order - 1) * digitDX - lengthDX[length];
    int y0 = height ? height - 7 : digitY + row * roomDY;
    return GetDigitXY(task, x0, y0);
  }
public:
  //载入名称为name的数字图像
  DigitType(int method) :method(method), digitNum(10), roomDX(0), roomDY(0)
  {
    memset(lengthDX, 0, sizeof(lengthDX));
    switch (method)
    {
    case LIST:case LIST_WIN7:
      name = method == LIST ? "列表房间号" : "列表房间号_win7";
      digitX = 150, digitY = 0, digitDX = 6;
      for (int i = 1; i <= 4; i++)
        lengthDX[i] = i * 3;
      break;
    case INDOOR: case INDOOR_WIN7:
      name = method == INDOOR ? "房间内房间号" : "房间内房间号_win7";
      digitX = 164, digitY = 28, digitDX = 7;
      for (int i = 1; i <= 4; i++)
        lengthDX[i] = i * 3;
      break;
    case CROSS_LIST:case CROSS_LIST_WIN7:
      name = method == CROSS_LIST ? "列表房间号" : "列表房间号_win7";
      digitX = 409, digitY = 116, digitDX = 6;
      roomDX = 263, roomDY = 123;
      break;
    case CROSS_INDOOR:case CROSS_INDOOR_WIN7:
      name = method == CROSS_INDOOR ? "跨服房间内房间号" : "跨服房间内房间号_win7";
      digitX = 64, digitY = 26, digitDX = 8;
      break;
    case FIRE:case FIRE_WIN7:
      name = method == FIRE ? "火苗" : "火苗_win7";
      digitX = 172, digitY = 75, digitDX = 7;
      lengthDX[2] = 4, lengthDX[3] = 7, lengthDX[4] = 11, lengthDX[5] = 14;
      break;
    case TREE:case TREE_WIN7:
      name = method == TREE ? "列表房间号" : "列表房间号_win7";
      digitNum = 11;//除了0-9还要识别斜杠
      digitX = 192, digitY = 103, digitDX = 6;
      break;
    }
  }
  //载入数字图像
  void Load()
  {
    COLORREF digitMap[height][width] = {};//数字
    char path[maxPath];
    for (int n = 0; n < digitNum; n++)
    {
      //载入图像
      sprintf_s(path, "附加程序\\数字\\%s\\%d.png", name, n);
      BitmapToColor(path, digitMap);
      //图像中的白色部分为文字
      for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
          digit[n][x][y] = digitMap[y][x] == 0xffffff;
    }
  }
  //保存图片文件
  void SavePng()
  {
    char path[maxPath];
    COLORREF digitMap[height][width] = {};//数字
    //数字部分用白色表示（其他部分用黑色表示）
    for (int n = 0; n < digitNum; n++)
    {
      for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
          digitMap[y][x] = digit[n][x][y] ? 0xffffff : 0;
      sprintf_s(path, "附加程序\\新数字\\%s\\%d.png", name, n);
      ColorToBitmap(digitMap, path);
    }
  }
  //读取位置不定的成长值，返回字符串长度
  int GetMovingGrowth(int task, char(&str)[10], int movingGrowthX,
    int movingGrowthY, int movingGrowthWidth)
  {
    memset(str, 0, sizeof(str));

    int slashX = 0;//斜杠位置
    //检测斜杠位置
    for (int x = movingGrowthX; x < movingGrowthX + movingGrowthWidth; x++)
      if (Equal(task, x, movingGrowthY, 10))
      {
        slashX = x;
        break;
      }
    if (slashX == 0)
      return 0;
    //读取最多9位数字
    int length = 0;
    for (int x = slashX - 4 * digitDX; x <= slashX + 4 * digitDX; x += digitDX)
    {
      int digit = GetDigitXY(task, x, movingGrowthY);
      if (digit != -1)
        str[length++] = digit == 10 ? '/' : '0' + digit;
    }
    return length;
  }
  //读取公会树进度条的总成长值，返回字符串长度
  int GetTotalGrowth(int task, char(&str)[10])
  {
    const int totalGrowthX = 434, totalGrowthY = 546, totalGrowthWidth = 48;
    return GetMovingGrowth(task, str, totalGrowthX, totalGrowthY, totalGrowthWidth);
  }
  //读取公会树成长值，返回字符串长度
  int GetGrowth(int task, char(&str)[10])
  {
    const int growthX = 192, growthY = 103, growthWidth = 58;
    return GetMovingGrowth(task, str, growthX, growthY, growthWidth);
  }
  //读取可摘果实，返回字符串长度
  int GetFruit(int task, char(&str)[10])
  {
    memset(str, 0, sizeof(str));
    const int FruitX = 178, FruitY = 129;
    for (int i = 1; i <= 10; i++)//最多9位，第10位必定匹配失败
    {
      int digit = GetDigitXY(task, FruitX + (i - 1) * digitDX, FruitY);//读取第i位数字
      if (digit == -1) //匹配失败则结束
        return strlen(str);
      str[i - 1] = digit == 10 ? '/' : '0' + digit;
    }
    return 10;
  }
  //将length位数的第order位保存为数字n
  void SaveDigit(int n, int order, int length)
  {
    int x0 = digitX + (order - 1) * digitDX - lengthDX[length];
    int y0 = digitY;
    COLORREF digitMap[height][width] = {};//数字
    //数字部分用白色表示（其他部分用黑色表示）
    for (int y = 0; y < height; y++)
      for (int x = 0; x < width; x++)
        if (IsTextColor(work[0].map[y0 + y][x0 + x]))
          digitMap[y][x] = 0xffffff;
    char path[100];
    sprintf_s(path, "附加程序\\保存的数字\\%s\\%d.png", name, n);
    ColorToBitmap(digitMap, path);
  }
  //读取数字
  int Get(int task, int row = 0, int column = 0, int height = 0, int account = 0)
  {
    int length = 5;//跨服房间号为5位
    //普通房间号和火苗需要获得长度
    if (method == LIST || method == LIST_WIN7 || method == INDOOR || method == INDOOR_WIN7
      || method == FIRE || method == FIRE_WIN7)
    {
      int maxLength = 4;
      if (method == FIRE || method == FIRE_WIN7)//读取火苗需要根据卡槽数确定digitX
      {
        maxLength = 5;
        digitX = work[task].slotX[account] - 41;
      }
      for (length = maxLength; length >= 1; length--)
        if (GetDigit(task, 1, row, column, length, height) >= 0)
          break;
      if (length == 0)
        return -1;
    }
    else //跨服房间号：无需length和height参数，只需检查第1位是不是数字
    {
      if (GetDigit(task, 1, row, column) == -1)
        return -1;
    }
    int room = 0;
    for (int i = 1; i <= length; i++)
    {
      int digit = GetDigit(task, i, row, column, length, height);
      if (digit == -1)
        return -1;
      room = room * 10 + digit;
    }
    return room;
  }
};

DigitType<5, 8> listDigit(LIST);//列表房间号
DigitType<5, 8> listDigitWin7(LIST_WIN7);//Win7列表房间号
DigitType<6, 8> indoorDigit(INDOOR);//房间内房间号
DigitType<6, 8> indoorDigitWin7(INDOOR_WIN7);//Win7房间内房间号
DigitType<5, 8> crossListDigit(CROSS_LIST);//跨服列表房间号
DigitType<5, 8> crossListDigitWin7(CROSS_LIST_WIN7);//Win7跨服列表房间号
DigitType<7, 10> crossIndoorDigit(CROSS_INDOOR);//跨服房间内房间号
DigitType<7, 10> crossIndoorDigitWin7(CROSS_INDOOR_WIN7);//Win7跨服房间内房间号
DigitType<6, 10> energyDigit(FIRE);//火苗数值
DigitType<6, 10> energyDigitWin7(FIRE_WIN7);//Win7火苗数值
DigitType<5, 8> growthDigit(TREE);//公会树成长值
DigitType<5, 8> growthDigitWin7(TREE_WIN7);//Win7公会树成长值

//载入房间号数字图像
void LoadDigit()
{
  listDigit.Load();
  listDigitWin7.Load();
  indoorDigit.Load();
  indoorDigitWin7.Load();
  crossListDigit.Load();
  crossListDigitWin7.Load();
  crossIndoorDigit.Load();
  crossIndoorDigitWin7.Load();
  energyDigit.Load();
  energyDigitWin7.Load();
  growthDigit.Load();
  growthDigitWin7.Load();
}
//读取公会树今日成长值（考虑win7）
int GetGrowth(int task, char(&str)[10])
{
  char str1[10] = {}, str2[10] = {};
  int length1 = growthDigit.GetGrowth(task, str1);
  int length2 = growthDigitWin7.GetGrowth(task, str2);
  if (length1 >= length2)
  {
    strcpy_s(str, str1);
    return length1;
  }
  strcpy_s(str, str2);
  return length2;
}
//读取公会树总成长值（考虑win7）
int GetTotalGrowth(int task, char(&str)[10])
{
  char str1[10] = {}, str2[10] = {};
  int length1 = growthDigit.GetTotalGrowth(task, str1);
  int length2 = growthDigitWin7.GetTotalGrowth(task, str2);
  if (length1 >= length2)
  {
    strcpy_s(str, str1);
    return length1;
  }
  strcpy_s(str, str2);
  return length2;
}
//读取可摘果实数量（考虑win7）
int GetFruit(int task, char(&str)[10])
{
  char str1[10] = {}, str2[10] = {};
  int length1 = growthDigit.GetFruit(task, str1);
  int length2 = growthDigitWin7.GetFruit(task, str2);
  if (length1 >= length2)
  {
    strcpy_s(str, str1);
    return length1;
  }
  strcpy_s(str, str2);
  return length2;
}
//成长值是否已满
bool isFull(int task)
{
  char growth[10] = {};
  GetGrowth(task, growth);
  char fruit[10] = {};
  GetFruit(task, fruit);

  //bool result = isGrowthFull(growth) || strcmp(fruit, "0/0") != 0;
  //char message[100];
  //sprintf_s(message, "isFull\ngrowth=%s fruit=%s result=%d", growth, fruit, result);
  //PopMessage(task, message);
  //return result;

  //如果今日成长值已满，或者可摘果实不是0/0（即公会树成熟），则视为成长值已满
  return isGrowthFull(growth) || strcmp(fruit, "0/0") != 0;
}
//总成长值是否已满
bool isTotalFull(int task)
{
  char growth[10] = {};
  GetTotalGrowth(task, growth);

  //bool result = isGrowthFull(growth);
  //char message[100];
  //sprintf_s(message, "isTotalFull\nTotalGrowth=%s result=%d", growth, result);
  //PopMessage(task, message);
  //return result;

  return isGrowthFull(growth);
}
//从map中读取房间内房间号（考虑win7）
int GetIndoorDigit(int task)
{
  int digit1 = indoorDigit.Get(task);
  int digit2 = indoorDigitWin7.Get(task);
  return max(digit1, digit2);
}
//从map中读取跨服房间内房间号（考虑win7）
int GetCrossIndoorDigit(int task)
{
  int digit1 = crossIndoorDigit.Get(task);
  int digit2 = crossIndoorDigitWin7.Get(task);
  return max(digit1, digit2);
}
//从map中读取列表房间号（考虑win7）
int GetListDigit(int task, int row, int column, int height)
{
  int digit1 = listDigit.Get(task, row, column, height);
  int digit2 = listDigitWin7.Get(task, row, column, height);
  return max(digit1, digit2);
}
//从map中读取跨服列表房间号（考虑win7）
int GetCrossListDigit(int task, int row, int column)
{
  int digit1 = crossListDigit.Get(task, row, column);
  int digit2 = crossListDigitWin7.Get(task, row, column);
  return max(digit1, digit2);
}
//从map中读取火苗（考虑win7）
int GetEnergyDigit(int task, int row, int column, int height, int account)
{
  int digit1 = energyDigit.Get(task, row, column, height, account);
  int digit2 = energyDigitWin7.Get(task, row, column, height, account);
  return max(digit1, digit2);
}
//【跨服列表】读取列表房间号（返回房间数量，每个房间的位置和房间号）
int GetCrossList(int task, int *x, int *y, int *room)
{
  int roomNum = 0;//房间数量
  int row, column;
  int roomDigit;
  for (row = 0; row <= 2; row++)
    for (column = 0; column <= 1; column++)
    {
      roomDigit = GetCrossListDigit(task, row, column);
      if (roomDigit == -1)
        return roomNum;
      x[roomNum] = crossListDigit.digitX + column * crossListDigit.roomDX;
      y[roomNum] = crossListDigit.digitY + row * crossListDigit.roomDY;
      room[roomNum] = roomDigit;
      roomNum++;
    }
  return roomNum;
}
//读取列表房间号（返回房间数量，每个房间的高度和房间号）
int GetRoomList(int task, int *height, int *room)
{
  int roomNum = 0;
  int x;
  int row = 0;
  for (int y = ListY; y < ListY + ListHeight; y++)
  {
    for (x = ListX; x < ListX + ListWidth; x++)
      if (listDigit.IsTextColor(work[task].map[y][x])) //记录连续白色的行数
      {
        row++;
        break;
      }
    if (x == ListX + ListWidth)//一旦断连，行数清零
      row = 0;
    if (row == 8)//连续8行白色视为有效房间号
    {
      height[roomNum] = y;
      room[roomNum] = GetListDigit(task, 0, 0, y);
      roomNum++;
      row = 0;
    }
  }
  return roomNum;
}
struct Picture
{
  COLORREF color[20][20];//颜色信息
  char name[40];//位图名称
  int width, height;//宽度和高度
  int x, y;//起始位置，只适用于定点识别。模糊找图设为-1
};
const int maxPictureNum = 500;
char pictureList[maxPictureNum][maxPath];
int pictureNum;//用于识图的10x10位图张数
Picture picture[maxPictureNum];
//根据图片文件生成图片目录
void MakePictureList()
{
  int fileNum = GetFileList("附加程序\\图片\\界面\\*", pictureList, maxPictureNum);
  FILE *f;
  fopen_s(&f, "附加程序\\图片目录.txt", "w");
  for (int i = 0; i < fileNum; i++)
    fprintf(f, "%s\n", pictureList[i]);
  fclose(f);
}
//读取位图列表，返回读取的数量
int LoadPictureList(char *dir, char(*bitmapList)[maxPath])
{
  FILE *f;
  char path[maxPath];
  if (fopen_s(&f, "附加程序\\图片目录.txt", "r"))
    return 0;
  int num = 0;
  fgets(f, path);
  while (strlen(path) > 0)
  {
    strcpy_s(bitmapList[num++], path);
    fgets(f, path);
  }
  fclose(f);
  return num;
}
//载入单个位图。位图名称："(100,200)位图.png"或"位图.png"
void LoadPicture(char *fileName)
{
  char path[maxPath] = {};
  char name[100];

  if (fileName[0] == '(')//括号开头，获取位图名称name和起始坐标
    sscanf_s(fileName, "(%d,%d)%s", &picture[pictureNum].x, &picture[pictureNum].y, name, sizeof(name));
  else //否则(x,y)=(-1,-1)，即找图模式
  {
    strcpy_s(name, fileName);
    picture[pictureNum].x = -1;
    picture[pictureNum].y = -1;
  }
  name[strlen(name) - 4] = 0;//名称删去最后4个字符".png"
  strcpy_s(picture[pictureNum].name, name);//记录位图名称
  sprintf_s(path, "附加程序\\图片\\界面\\%s", fileName);//填写位图完整路径
  if (!FileExist(path))
  {
    char message[100];
    sprintf_s(message, "缺少依赖文件：\n%s\n请检查是否更新完整。", path);
    MessageBox(NULL, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    exit(0);
  }
  //保存图片的颜色信息和尺寸
  BitmapToColor(path, picture[pictureNum].color, &picture[pictureNum].width, &picture[pictureNum].height);
  pictureNum++;//记录位图数量+1
}
//载入所有图片
void LoadPicture()
{
  int num = LoadPictureList("附加程序\\图片\\界面", pictureList);//读取所有位图的名称，返回数量
  pictureNum = 0;
  for (int order = 0; order < num; order++)
    LoadPicture(pictureList[order]);
}
//载入标准卡片模板
void LoadCardTemplate()
{
  char path[maxPath] = {};
  for (int i = 0; i < 4; i++)
  {
    sprintf_s(path, "附加程序\\图片\\卡片模板\\%d.png", i);
    BitmapToColor(path, cardTemplate[i]);
  }
}
//找图类型：1. 在固定位置查找图片：需要给定固定坐标。由于图片已有坐标，只需给偏移值
//2. 在一定范围内查找图片：需要给定矩形范围x1,y1,x2,y2
void SetRect(RECT &rect, int left, int top, int right, int bottom)
{
  rect.left = left;
  rect.top = top;
  rect.right = right;
  rect.bottom = bottom;
}
//【基本函数】从map截图中查找图片，找到返回1
//图片名无前缀默认全屏找图，有前缀默认定点找图
//填写(x0,y0)则定点找图，填写(x1,y1)则范围找图
int FindPictureInMap_Code(int task, int bitmapCode, int *px = nullptr, int *py = nullptr,
  int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0, COLORREF specificColor = 0)
{
  bool areaSearch = true;//是否区域找图
  RECT area;//找图区域
  //先判断找图模式
  if (picture[bitmapCode].x == -1 && picture[bitmapCode].y == -1)//默认全屏找图
  {
    if (x1 > 0 || y1 > 0)//区域找图
    {
      areaSearch = true;
      SetRect(area, x0, y0, x1, y1);
    }
    else if (x0 > 0 || y0 > 0)//定点找图
    {
      areaSearch = false;
      SetRect(area, x0, y0, 0, 0);
    }
    else //全屏找图
    {
      areaSearch = true;
      SetRect(area, 0, 0, gameWidth, gameHeight);
    }
  }
  else //默认定点找图
  {
    if (x1 > 0 || y1 > 0)//区域找图
    {
      areaSearch = true;
      SetRect(area, x0, y0, x1, y1);
    }
    else //定点找图
    {
      areaSearch = false;
      SetRect(area, picture[bitmapCode].x + x0, picture[bitmapCode].y + y0, 0, 0);
    }
  }

  if (areaSearch)//区域找图
  {
    for (int y = area.top; y <= area.bottom - picture[bitmapCode].height; y++)
      for (int x = area.left; x <= area.right - picture[bitmapCode].width; x++)
        if (IsBitmapEqual(work[task].map, picture[bitmapCode].color, picture[bitmapCode].width, picture[bitmapCode].height,
          x, y, 0, 0, specificColor))
        {
          if (px)
            *px = x;
          if (py)
            *py = y;
          return 1;
        }
  }
  else //定点找图
  {
    if (IsBitmapEqual(work[task].map, picture[bitmapCode].color, picture[bitmapCode].width,
      picture[bitmapCode].height, area.left, area.top, 0, 0, specificColor))
    {
      if (px)
        *px = area.left;
      if (py)
        *py = area.top;
      return 1;
    }
  }
  return 0;
}
//根据名称查找图片编号，找不到返回-1
int GetPictureCode(const char *pictureName)
{
  for (int code = 0; code < pictureNum; code++)
    if (strcmp(pictureName, picture[code].name) == 0)
      return code;
  return -1;
}
//从map截图中查找图片
int FindPictureInMap(int task, const char *pictureName, int *px = NULL, int *py = NULL,
  int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0, COLORREF specificColor = 0)
{
  int picture = GetPictureCode(pictureName);
  if (picture == -1)
  {
    char errorString[100] = {};
    sprintf_s(errorString, "缺少图片文件[%s]", pictureName);
    ReportError0(task, "缺少图片", errorString, END_TASK);
  }
  return FindPictureInMap_Code(task, picture, px, py, x0, y0, x1, y1, specificColor);
}
//从map截图中查找多张图片
template <size_t pictureNum>
int FindPicturesInMap(int task, const char *(&pictureName)[pictureNum], int *px = NULL, int *py = NULL,
  int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0)
{
  //获得每一张图片的编号，有一张图片不存在则报错退出
  int picture[pictureNum] = {};
  for (int i = 0; i < pictureNum; i++)
  {
    picture[i] = GetPictureCode(pictureName[i]);
    if (picture[i] == -1)
    {
      char errorString[100] = {};
      sprintf_s(errorString, "缺少图片文件[%s]", pictureName[i]);
      ReportError0(task, "缺少图片", errorString, END_TASK);
    }
  }

  //从截图中查找图片
  for (int i = 0; i < pictureNum; i++)
    if (FindPictureInMap_Code(task, picture[i], px, py, x0, y0, x1, y1))
      return 1;
  return 0;
}
//从游戏窗口查找多张图片（最多10张）
template <size_t pictureNum>
int FindPictures(int task, int account, const char *(&pictureName)[pictureNum],
  int *px = NULL, int *py = NULL, int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0)
{
  //获得每一张图片的编号，有一张图片不存在则报错退出
  int picture[pictureNum] = {};
  for (int i = 0; i < pictureNum; i++)
  {
    picture[i] = GetPictureCode(pictureName[i]);
    if (picture[i] == -1)
    {
      char errorString[100] = {};
      sprintf_s(errorString, "缺少图片文件[%s]", pictureName[i]);
      ReportError0(task, "缺少图片", errorString, END_TASK);
    }
  }

  //截图游戏窗口
  if (!IsWindow(work[task].hWnd[account]))
    ReportError(task, account, "游戏窗口已关闭", "游戏窗口已关闭，无法识图");
  if (!IsGameWindowVisible(work[task].hWnd[account])) //窗口未显示，无法识图
    return 0;
  ForcedUpdateMap(task, account);//强制获取屏幕图像，以免判定失误

  //从截图中查找图片
  for (int i = 0; i < pictureNum; i++)
    if (FindPictureInMap_Code(task, picture[i], px, py, x0, y0, x1, y1))
      return 1;
  return 0;
}
//从游戏窗口中查找一张图片
int FindPicture(int task, int account, const char *pictureName, int *px = NULL, int *py = NULL,
  int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0)
{
  const char *picturesName[1] = { pictureName };
  return FindPictures(task, account, picturesName, px, py, x0, y0, x1, y1);
}
const int APPEAR = 1, DISAPPEAR = 0;
//等待多张图片出现或消失，记录图片左上角位置，最多等待timeout ms
template <size_t pictureNum>
int WaitPictures(int appearMode, int task, int account, const char *(&pictureName)[pictureNum], int *px = nullptr, int *py = nullptr,
  int x0 = 0, int y0 = 0, int timeout = 0, bool report = true)
{
  if (timeout == 0)
    timeout = WaitTime();
  int counter = 0;
  char message[100] = {};
  while (FindPictures(task, account, pictureName, px, py, x0, y0) != appearMode)
  {
    counter++;
    if (counter >= timeout / 100)//timeout ms还未进入预期界面
    {
      if (report == true) //如果开启了报错，则报错中断
      {
        if (appearMode == APPEAR)
        {
          sprintf_s(message, "未能进入[%s]界面", pictureName[0]);
          ReportError(task, account, "未进入指定界面", message);
        }
        else
        {
          sprintf_s(message, "未能关闭[%s]界面", pictureName[0]);
          ReportError(task, account, "未关闭指定界面", message);
        }
      }
      return 0; //否则只是返回0
    }
    CheckSleep(task, 100);
  }
  return 1;
}
//等待一张图片出现或消失
int WaitPicture(int appearMode, int task, int account, const char *pictureName, int *px = nullptr, int *py = nullptr,
  int dx = 0, int dy = 0, int timeout = 0, bool report = true)
{
  if (timeout == 0)
    timeout = WaitTime();
  const char *picturesName[1] = { pictureName };
  return WaitPictures(appearMode, task, account, picturesName, px, py, dx, dy, timeout, report);
}
//尝试等待多张图片出现或消失
template <size_t pictureNum>
int TryWaitPictures(int appearMode, int task, int account, const char *(&pictureName)[pictureNum], int timeout,
  int *px = nullptr, int *py = nullptr)
{
  return WaitPictures(appearMode, task, account, pictureName, px, py, 0, 0, timeout, false);
}
//尝试等待一张图片出现或消失
int TryWaitPicture(int appearMode, int task, int account, const char *pictureName, int timeout,
  int *px = nullptr, int *py = nullptr)
{
  const char *picturesName[1] = { pictureName };
  return TryWaitPictures(appearMode, task, account, picturesName, timeout, px, py);
}
//无图像处理：10分钟无图强制终止，2分钟无图提示
void NoImageProcess(int task)
{
  if (work[task].gameTime == zero)//自动检测模式下需要对无图像进行处理
  {
    if (work[task].noImageTick == zero)//如果是第一次无图像，记录无图像时间
      work[task].noImageTick = GetTickCount();
    else //否则检查是否已超时
    {
      int NoImageTime = GetTickCount() - work[task].noImageTick;//检查无图像时间
      if (NoImageTime > 600000) //如果连续超时10分钟，强制中断
        ReportError0(task, "图像获取失败", "连续10分钟未获取到图像，任务已终止", END_TASK);
      else if (NoImageTime >= noImageTimeout * 1000)//否则检查是否超过设定时间
      {
        if (noImageOperation == 1 && work[task].isPromptOn == zero)//提示
          CreateThread(NULL, 0, PromptNoImage, (void *)task, 0, NULL);//提示无图像
        else if (noImageOperation == 2)//中断
          ReportError0(task, "图像获取失败", "连续2分钟未获取到图像，任务已终止", END_TASK);
      }
    }
  }
}
//更新account窗口截图，成功返回1
int UpdateMap(int task, int account, bool report = true)
{
  reserve[task].updateNum++;
  if (!IsWindow(work[task].hWnd[account]))
  {
    if (!report)
      return 0;
    ReportError(task, account, "游戏窗口已关闭", "游戏窗口已关闭，无法截图");
  }
  if (!IsGameWindowVisible(work[task].hWnd[account]))
  {
    reserve[task].iconicNum++;
    return 0;
  }
  work[task].noImageTick = 0;//运行到这里就是有图像了
  int result = MultiPrintWindow(work[task].hWnd[account], work[task].hMemDC, 5);//截图5次直到成功，记录结果
  InvalidateRect(work[task].hWnd[account], NULL, false);//重画
  //无偏移：原位删除首字节
  if (work[task].mapOffsetY[account] == 0)
  {
    for (int y = 0; y < gameHeight; y++)
      for (int x = 0; x < gameWidth; x++)
        work[task].map[y][x] &= 0x00ffffff;
  }
  //有偏移：移动画面并删除首字节
  else
  {
    for (int y = gameHeight - 1; y >= -work[task].mapOffsetY[account]; y--)
      for (int x = 0; x < gameWidth; x++)
        work[task].map[y][x] = work[task].map[y + work[task].mapOffsetY[account]][x] & 0x00ffffff;
    for (int y = -work[task].mapOffsetY[account] - 1; y >= 0; y--)
      for (int x = 0; x < gameWidth; x++)
        work[task].map[y][x] = 0;
  }
  if (result == 1)
    reserve[task].successfulUpdateNum++;
  return result;
}
//从账号account的游戏窗口中查找num个点的颜色。识图成功返回1，否则返回0
int GetMultiColor(int task, int account, int num, COLORREF *color, POINT *point)
{
  if (!UpdateMap(task, account))
    return 0;
  for (int i = 0; i < num; i++)//直接从屏幕读取各点颜色
    color[i] = work[task].map[point[i].y][point[i].x];
  return 1;
}
COLORREF GetColor(int task, int account, POINT point)
{
  COLORREF color = 0;
  if (GetMultiColor(task, account, 1, &color, &point))
    return color;
  return 0;
}
//识别绑定窗口中指定点的颜色
COLORREF GetColor(int task, int account, int x, int y)
{
  POINT point = { x, y };
  return GetColor(task, account, point);
}
//从Map截图寻找出怪行，并将新行写入RatRow数组
void FindRatFromMap(int task)
{
  int isRowRatted[8] = {};//各行是否出怪
  int isRowRattedLast[8] = {};//上次识别时各行是否出怪
  int differentPixelStreak[8] = {};//每一行连续不同像素数量
  int maxDifferentPixelStreak[8] = {};//每一行最大连续不同像素数量

  for (int i = 0; i < work[task].ratRowNum; i++)//记录之前已有的出怪行
    isRowRattedLast[work[task].ratRow[i]] = 1;

  //与最初的前线作对比，统计每一行的最大连续变色像素数量
  for (int row = 0; row <= 7; row++)
    for (int y = 0; y < frontHeight; y++)
      if (work[task].innateFront[row * frontHeight + y] != work[task].map[frontY + row * frontHeight + y][frontX])
      {
        differentPixelStreak[row]++;
        maxDifferentPixelStreak[row] = max(maxDifferentPixelStreak[row], differentPixelStreak[row]);
      }
      else
        differentPixelStreak[row] = 0;

  //记录出怪行：本行连续像素数大于20，上一行至少有1个像素，则视为出怪
  for (int row = 1; row <= 7; row++)
    if (maxDifferentPixelStreak[row] > 20 && maxDifferentPixelStreak[row - 1] > zero)
    {
      isRowRatted[row] = 1;
      work[task].isRatAppeared = true;//记录本局已经出过老鼠了
    }

  //如果发现新的出怪行，记入RatRow
  for (int row = 1; row <= 7; row++)
    if (isRowRatted[row] == 1 && isRowRattedLast[row] == zero)
    {
      work[task].ratRow[work[task].ratRowNum] = row;
      work[task].ratRowNum++;
    }
}
const COLORREF crossColor = 0x1d6fb1;//跨服颜色
const COLORREF challengeColor = 0x013759;//勇士颜色
const COLORREF buttonColor = 0x05294b;//下方功能按钮颜色
const COLORREF boxColor = 0x7ec8f5;//蓝色提示框颜色
//获得任务task的第一个放卡账号
int GetFirstPerformedAccount(int task)
{
  return work[task].isPerformed[0] ? 0 : 1;
}
//从当前截图中读取主波次
int GetMainWave(int task)
{
  if (FindPictureInMap(task, "战利品"))
    return waveLoot;
  if (FindPictureInMap(task, "巅峰对决"))
    return wavePeak;
  if (FindPictureInMap(task, "萌宠神殿"))
    return wavePet;
  const char *indoor[] = { "选卡界面", "选卡界面_实验室" };
  if (FindPicturesInMap(task, indoor))
    return waveOutOfLevel;
  COLORREF waveColor = work[task].map[552][670];
  COLORREF hostColor = work[task].map[46][100];
  for (int i = 0; i <= 18; i++)
    if (waveColor == wavesColor[i])
    {
      if (i >= 14)
        return waveOutOfLevel + i - 14;
      else
        return i;
    }
  if (hostColor == crossColor || hostColor == challengeColor || waveColor == buttonColor)
    return waveOutOfLevel;//回到跨服界面、勇士界面视为退出关卡，波次为waveOutOfLevel
  return -1;//无法辨识：可能是非关卡内，或被继续作战遮挡
}
//根据头像颜色判断是否处于关卡内(box==0)或继续作战状态(box==1)
bool CheckHeadColor(int task, int box, int offset = 0)
{
  const COLORREF headColor[2] = { 0xfcf3ca, 0x4c4a3d };
  const int headX = 100;//头像特征颜色判定位置X
  bool inLevel = true;
  for (int y = 33; y <= 46; y++)
    if (work[task].map[y + offset][headX] != headColor[box])
      return false;
  return true;
}
//识别关卡进入状态、波数和中央弹窗。识别成功返回1。
int GetGameState(int task, int account, int *pEntry, int *pWave, int *pBox)
{
  const COLORREF roleColor = 0xfcf3ca;//关卡内左上角人物框颜色
  const COLORREF loadingColor = 0x0f3372;//加载中颜色
  const COLORREF readyColor = 0x990000;//准备框颜色
  const COLORREF proceedColor = 0xd9baac;//继续作战提示框颜色

  const int pointNum = 6;
  //准备，进入后的人物1，人物2，波数，普通弹窗（掉线/背包满），经验满弹窗
  POINT point[pointNum] = { { 356, 161 }, { 100, 46 }, { 100, 137 }, { 670, 552 }, { 475, 280 }, { 475, 246 } };
  int entry, wave, box;

  /*预设无图像参数*/
  if (pEntry != NULL)
    *pEntry = 3;
  if (pBox != NULL)
    *pBox = 0;

  COLORREF color[pointNum];
  if (!IsGameWindowVisible(work[task].hWnd[account]))
    return 0;
  if (GetMultiColor(task, account, pointNum, color, point) == 0)//查找pointNum个点的颜色，记录是否识图成功
    return 0;//识图失败则退出函数，写入预设的无图像参数

  if (color[1] == roleColor)//检测到房主号在关卡内
  {
    if (color[2] == roleColor)//如果队友号也在关卡内
      entry = 2;//双人进入关卡
    else
      entry = 1;//单人进入关卡
  }
  else if (color[1] == loadingColor)
    entry = -1;//加载中
  else if (color[1] == zero && work[task].gameTime > 0)
    entry = 3;//识图失败，非识图模式视为已进入关卡
  else if (color[0] == readyColor)
    entry = -2;//队友已准备（未进入关卡时检测）
  else
    entry = 0;

  wave = GetMainWave(task);

  if (color[5] == boxColor) //经验满
    box = 3;
  else if (color[4] == boxColor) //掉线/背包满
    box = 2;
  else if (CheckHeadColor(task, 1)) //继续作战
    box = 1;
  else
    box = 0;

  if (pEntry)
    *pEntry = entry;
  if (pWave && wave != -1 && wave != waveNoImage) //不是-1或waveNoImage才更新波数
    *pWave = wave;
  if (pBox)
    *pBox = box;

  return 1;
}
//任选一个号识别关卡状态
int GetGameStateFromAnyAccount(int task, int *Entry, int *Wave, int *Box)
{
  bool isImagePicked = false;//是否识图成功
  for (int account = 0; account < 2; account++)
    if (work[task].isInvolved[account] && !work[task].isQuitted[account])
    {
      if (isImagePicked == false)
        if (GetGameState(task, account, Entry, Wave, Box) == 1)
          isImagePicked = true;
    }
  if (isImagePicked == false) //如果没有一个号能识别到图像，则进行无图像处理（非识图模式不会处理）
  {
    NoImageProcess(task);
    return 0;
  }
  return 1;
}

const int bannerX = 388, bannerY = 470, bannerWidth = 100, bannerHeight = 12;
const int maxBannerNum = 30;
int bannerNum;
char bannerList[maxBannerNum][maxPath];
struct Banner
{
  char name[20];
  COLORREF image[bannerHeight][bannerWidth];
};
Banner banner[maxBannerNum];
//截图关卡名
void SaveBanner(int task)
{
  char path[maxPath] = {};
  sprintf_s(path, "附加程序\\图片\\关卡名\\0.png");
  ColorToBitmap(work[task].map, path, bannerX, bannerY, bannerWidth, bannerHeight);
  ReportError0(task, "关卡名截图成功", "关卡名截图成功", END_TASK);
}
//载入特殊关卡名
void LoadBanner()
{
  const char folder[] = "附加程序\\图片\\关卡名";
  if (!FileExist(folder))
    return;

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, bannerList, maxBannerNum);//查找所有png文件

  char path[maxPath] = {};
  bannerNum = 0;
  for (int i = 0; i < filesNum && bannerNum < maxBannerNum; i++)
  {
    sprintf_s(path, "%s\\%s", folder, bannerList[i]);
    if (FileExist(path)) //如果文件存在
    {
      BitmapToColor(path, banner[bannerNum].image);//读取卡片图像到image
      bannerList[i][strlen(bannerList[i]) - 4] = 0;//删除".png"
      strcpy_s(banner[bannerNum].name, bannerList[i]);//记录名称
      bannerNum++;
    }
  }
}
//获取特殊关卡编号
int GetBannerCode(int task)
{
  for (int code = 0; code < bannerNum; code++)
  {
    if (IsBitmapEqual(work[task].map, banner[code].image, bannerWidth, bannerHeight,
      bannerX, bannerY))
      return code;
  }
  return -1;
}
//获取特殊关卡类型
int GetBanner(int task)
{
  int code = GetBannerCode(task);
  if (code == -1) //如果是未收录的关卡
  {
    //白天关卡判定为鼠以群聚（编号3），只放棉花糖
    if (FindPictureInMap(task, "白天关卡", nullptr, nullptr, 0, 0, 0, 0, 0xffffff))
      return 3;
    else //夜晚关卡判定为“未收录”，木塞子、盘子、棉花都放
      return 0;
  }
  return atoi(banner[code].name); //返回图片名开头的数字
}
//按下指定键
BOOL Press(int task, int account, int keycode)
{
  if (work[task].isInvolved[account] == 0) //如果账号不参与，则不执行按键
    return 0;
  return PostMessage(work[task].hWnd[account], WM_KEYDOWN, keycode, 0);//按键
}
//在Ctrl按下的时候按键
void PressCtrl(int task, int account, int keycode)
{
  keybd_event(VK_CONTROL, 0, 0, 0); //按下Ctrl键
  CheckSleep(task, 250);
  Press(task, account, keycode);
  CheckSleep(task, 250);
  keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); //释放Ctrl键
}
void Drag(int task, int account, int x1, int y1, int x2, int y2)
{
  if (work[task].isInvolved[account] == 0) //如果账号不参与，则不执行拖拽
    return;
  int _x1 = x1, _x2 = x2, _y1 = y1, _y2 = y2;
  if (work[task].isDpiAwareRequired[account] == 1)//坐标缩放修正
  {
    _x1 = x1 * DPI / 96;
    _y1 = y1 * DPI / 96;
    _x2 = x2 * DPI / 96;
    _y2 = y2 * DPI / 96;
  }
  PostMessage(work[task].hWnd[account], WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(_x1, _y1));
  PostMessage(work[task].hWnd[account], WM_MOUSEMOVE, 0, MAKELPARAM(_x2, _y2));
  PostMessage(work[task].hWnd[account], WM_LBUTTONUP, 0, MAKELPARAM(_x2, _y2));
}
//单击指定位置（考虑画面偏移）
BOOL Click(int task, int account, int x, int y)
{
  //如果账号不参与或已退出关卡，则不执行点击
  if (!work[task].isInvolved[account] || work[task].isQuitted[account])
    return 0;
  if (!IsWindow(work[task].hWnd[account]))//如果窗口不见了
    ReportError(task, account, "游戏窗口已关闭", "游戏窗口已关闭，无法点击");
  if (work[task].isDpiAwareRequired[account] == 1)//如果需要DPI修正
    return LeftClickDPI(work[task].hWnd[account], x, y + work[task].mapOffsetY[account]);
  else
    return LeftClick(work[task].hWnd[account], x, y + work[task].mapOffsetY[account]);
}
//单击指定位置（考虑画面偏移）
BOOL Click(int task, int account, POINT location)
{
  return Click(task, account, location.x, location.y);
}
//鼠标移动到指定位置
BOOL MoveTo(int task, int account, int x, int y)
{
  if (work[task].isInvolved[account] == 0) //如果账号不参与，则不执行点击
    return 0;
  if (work[task].isDpiAwareRequired[account] == 1)//如果需要DPI修正
    return MouseMoveDPI(work[task].hWnd[account], x, y + work[task].mapOffsetY[account]);
  else
    return MouseMove(work[task].hWnd[account], x, y + work[task].mapOffsetY[account]);
}
BOOL MoveTo(int task, int account, POINT point)
{
  return MoveTo(task, account, point.x, point.y);
}
//单击地图某个格子
void Lay(int task, int account, int row, int column)
{
  Click(task, account, gridX + (column - 1) * gridWidth + gridWidth / 2,
    gridY + (row - 1) * gridHeight + gridHeight / 2);
}
//获取轨道中的人物个数
int GetTrackRoleNum(int task, int account)
{
  return (work[task].roleLocNum[account][0] > 0) + (work[task].roleLocNum[account][1] > 0);
}
//放置人物
void LayRole(int task, int account)
{
  int roleAccount = 0;//决定人物位置的轨道
  if (work[task].isPerformed[0] && work[task].isPerformed[1]) //双轨模式
  {
    int roleNum[2] = { GetTrackRoleNum(task, 0), GetTrackRoleNum(task, 1) };//两个轨道的人数
    if (roleNum[0] == 1 && roleNum[1] == 1) //两个轨道都是单人：由本轨道决定
      roleAccount = account;
    else if (roleNum[0] == 1 && roleNum[1] == 2) //1P单人2P双人：由2P轨道决定
      roleAccount = 1;
    else //1P双人：由1P轨道决定
      roleAccount = 0;
  }
  else //单轨模式：该轨道即为决定轨道
    roleAccount = work[task].isPerformed[0] ? 0 : 1;

  int player = roleAccount ^ account;
  for (int times = 1; times <= work[task].roleLocNum[roleAccount][player]; times++)
  {
    Lay(task, account, work[task].roleLoc[roleAccount][player][times] / 10,
      work[task].roleLoc[roleAccount][player][times] % 10);
    Sleep(1);
  }
}
//触碰地图某个格子
void Touch(int task, int account, int row, int column)
{
  MoveTo(task, account, gridX + (column - 1) * gridWidth + gridWidth / 2,
    gridY + (row - 1) * gridHeight + gridHeight / 2);
}
//使用n号宝石
void UseSkill(int task, int account, int n)
{
  Click(task, account, skillX, skillY + (n - 1) * skillHeight);
}
//使用所有宝石
void UseAllSkill(int task, int account)
{
  for (int n = 1; n <= 3; n++)
    UseSkill(task, account, n);
}
//魔塔翻到第page页
void PageTower(int task, int account, int tag, int page)
{
  const POINT towerPage = { tag == 3 ? 247 : 95, 571 };//页码位置
  const int towerPageWidth = 14, towerPageHeight = 10;//页码尺寸
  const POINT firstPage = { tag == 3 ? 190 : 45, 576 };//翻到第1页按钮
  const POINT pageDown = { tag == 3 ? 295 : 150, 576 };//向后翻页按钮
  const char *singlePage1[2] = { "单塔第1页", "单塔第1页_win7" };
  const char *doublePage1[2] = { "双塔第1页", "双塔第1页_win7" };
  const char *petPage1[2] = { "宠塔第1页", "宠塔第1页_win7" };
  const char *(*page1)[2] = &singlePage1;
  if (tag == 2)
    page1 = &doublePage1;
  else if (tag == 3)
    page1 = &petPage1;

  //翻到第一页
  int counter = 0;
  while (!FindPictures(task, account, *page1))
  {
    counter++;
    if (counter > 10)
      ReportError(task, account, "魔塔翻页失败", "魔塔无法翻到第一页");
    Click(task, account, firstPage.x, firstPage.y);
    CheckSleep(task, 200);
  }
  RegionalMapShot(work[task].hWnd[account], work[task].towerPage[1], towerPage.x, towerPage.y);//记录第1页
  for (int i = 2; i <= page; i++)
  {
    memcpy(work[task].towerPage[0], work[task].towerPage[1], sizeof(work[task].towerPage[0]));//记录新页数到LastPage
    Click(task, account, pageDown.x, pageDown.y);//点击翻页按钮
    //等待翻页成功
    counter = 0;
    while (IsBitmapEqual(work[task].towerPage[1], work[task].towerPage[0], towerPageWidth, towerPageHeight))
    {
      counter++;
      if (counter > 5)
        ReportError(task, account, "魔塔翻页失败", "魔塔无法向后翻页");
      CheckSleep(task, 200);
      RegionalMapShot(work[task].hWnd[account], work[task].towerPage[1], 95, 571);
    }
  }
}
//强制获取图像，10次截图失败则报错
void ForcedUpdateMap(int task, int account)
{
  int counter = 0;
  while (!UpdateMap(task, account))
  {
    counter++;
    if (counter > 10)
      ReportError(task, account, "无法获取图像", "无法获取图像");
    CheckSleep(task, 50);//如果截图失败或图像全黑，就等50 ms再截
  }
}
//计算颜色的亮度
int Brightness(COLORREF color)
{
  return bgrRValue(color) + bgrGValue(color) + bgrBValue(color);
}
//大赛滚动条中心标准位置
const int contestScrollCY[7] = { 372, 403, 434, 465, 496, 527, 558 };
const int contestScrollX = 537;
const int contestScrollY[7] = { 358, 394, 430, 466, 502, 538, 574 };
//获取大赛滚动条中心高度
int GetContestScrollCY(int task)
{
  int count = 0;
  int averageY = 0;
  for (int y = contestScrollY[0] - 2; y < contestScrollY[6] + 2; y++)
    if (Brightness(work[task].map[y][contestScrollX]) > 300)
    {
      count++;
      averageY += y;
    }
  return count == 0 ? -1 : averageY / count;
}
//发现邀请时接受(1)或拒绝(0)邀请并等0.1秒，未发现邀请时不点击
int DealInvitation(int task, int account, int deal)
{
  POINT DealButton[2] = { { 609, 403 }, { 449, 403 }, };//拒绝和接受按钮位置
  if (FindPicture(task, account, "被邀请界面"))
  {
    Click(task, account, DealButton[deal]);//处理邀请
    CheckSleep(task, 100);
    WaitPicture(DISAPPEAR, task, account, "被邀请界面");//等待邀请消失
    CheckSleep(task, 100);
    return 1;
  }
  return 0;
}
//点击大赛滚动条，直到滚动生效，但最多等1000ms
void ScrollContest(int task, int account, int page)
{
  POINT scrollLocation = { contestScrollX, contestScrollY[page] };

  DealInvitation(task, account, 0);//拒绝邀请并获得无邀请的图像
  int cy = GetContestScrollCY(task);//从截图中获取滚动条CY
  int count = -1;
  while (abs(cy - contestScrollCY[page]) > 3)
  {
    count++;
    if (count >= 50) //5秒翻不动直接报错
      ReportError(task, account, "大赛无法翻页", "大赛无法翻页");
    if (count % 10 == 0) //每1000ms重复点击一次
      Click(task, account, scrollLocation);
    CheckSleep(task, 100); //每100ms截图一次
    DealInvitation(task, account, 0);//拒绝邀请并获得无邀请的图像
    cy = GetContestScrollCY(task);//从截图中获取滚动条CY
  }
}
//从公共卡槽信息获取候选卡信息
void GetCandidateFromPublic(const char *slotName, Candidate(&candidate))
{
  candidate.num = 0;
  //按编号从大到小遍历自定卡
  for (int code = customNum - 1; code >= 0; code--)
    //如果某张自定卡的名称正好是order号卡槽，则记录
    if (strcmp(custom[code].name, slotName) == 0)
    {
      candidate.code[candidate.num] = code;
      candidate.priority[candidate.num] = custom[code].priority;
      candidate.num++;
    }
}
//从任务卡槽信息中获取候选卡信息：卡片slotName的所有候选卡的编号和优先级（按优先级从大到小排列）
void GetCandidate(int task, const char *slotName, Candidate(&candidate))
{
  candidate.num = 0;
  //按编号从大到小遍历自定卡
  for (int code = work[task].customNum - 1; code >= 0; code--)
    //如果某张自定卡的名称正好是order号卡槽，则记录
    if (strcmp(work[task].custom[code].name, slotName) == 0)
    {
      candidate.code[candidate.num] = code;
      candidate.priority[candidate.num] = work[task].custom[code].priority;
      candidate.num++;
    }
}
//获取卡槽order的所有候选自定卡的编号和优先级（优先级从大到小排列）
void GetCandidate(int task, int account, int order, Candidate(&candidate))
{
  GetCandidate(task, work[task].originalSlot[account][order].name, candidate);
}
//判断战利品个数
int GetLootQuantity(int task, int x0, int y0)
{
  for (int num = 2; num <= 9; num++)
    if (IsBitmapCovering(work[task].loot, lootDigit[num], lootDigitWidth, lootDigitHeight, x0, y0, 0, 0, 0xffffff))
      return num;
  return 1;
}
const int maxStyleNum = 12;//底色数量
//各类型的名称
const char *styleName[maxStyleNum] = { "B普通材料", "C稀有材料", "D普通配方1", "E普通配方2", "F稀有配方",
"H初级技能书1", "I初级技能书2", "J高级技能书1", "K高级技能书2", "L终极技能书1", "M终极技能书2", "R透明底色" };
//各类型的首字母
const char styleLetter[maxStyleNum] = { 'B', 'C', 'D', 'E', 'F', 'H', 'I', 'J', 'K', 'L', 'M', 'R' };
COLORREF base[maxStyleNum][propHeight][propWidth];//物品底色

//判断物品code与底色style的重合颜色数
int GetLootSimilarity(int code, int style)
{
  int counter = 0;
  for (int y = 0; y < propHeight; y++)
    for (int x = 0; x < propWidth; x++)
      if (base[style][y][x] && loot[code].image[y][x] == base[style][y][x])
        counter++; //只统计不是黑色的点
  return counter;
}
//判断物品code的分类
int GetLootStyle(int code)
{
  int maxSimilarity = -1;
  int bestStyle = -1;
  for (int style = 0; style < maxStyleNum; style++)
  {
    int similarity = GetLootSimilarity(code, style);
    if (similarity > maxSimilarity)
    {
      maxSimilarity = similarity;
      bestStyle = style;
    }
  }
  return bestStyle;
}

//自动截图道具模块

//增加一张道具截图
void AddProp_Original(int task, int thisPropX, int thisPropY)
{
  //如果和已有物品匹配，则退出函数
  for (int code = 0; code < lootNum; code++)
    if (IsBitmapEqual(work[task].map, loot[code].image, lootCoreWidth, lootCoreHeight,
      thisPropX + lootCoreX, thisPropY + lootCoreY, lootCoreX, lootCoreY))
      return;

  CopyMap(loot[lootNum].image, work[task].map, thisPropX, thisPropY);//记录物品图像
  int style = GetLootStyle(lootNum);//判断物品类别

  //获取新物品文件名，保存物品名称和截图
  char path[maxPath] = {};
  for (int code = 0; code < 1000; code++)
  {
    sprintf_s(path, "资源文件\\道具\\%c%d.png", styleLetter[style], code);
    if (!FileExist(path))
    {
      sprintf_s(loot[lootNum].name, "%c%d", styleLetter[style], code);//记录物品名称
      ColorToBitmap(loot[lootNum].image, path);//保存物品截图
      break;
    }
  }
  lootNum++;
}
//（带互斥锁）增加一张道具截图
void AddProp(int task, int thisPropX, int thisPropY)
{
  WaitForSingleObject(hMutexProp, INFINITE);
  AddProp_Original(task, thisPropX, thisPropY);
  ReleaseMutex(hMutexProp);
}
//等待物品进入仓库
void WaitInStore(int task, int account)
{
  int counter = 0;
  while (true)
  {
    UpdateMap(task, account);
    //当仓库第1格物品与移动前的背包物品相同（差距不超过5像素）时，视为物品已进入仓库
    if (IsBitmapEqual_Tolerance(work[task].map, work[task].initialProp, lootCoreWidth, lootCoreHeight,
      storeX + lootCoreX, storeY + lootCoreY, lootCoreX, lootCoreY, 500))
      break;
    counter++;
    if (counter > 30)
    {
      ColorToBitmap(work[task].map, "store1.png", storeX + lootCoreX, storeY + lootCoreY, lootCoreWidth, lootCoreHeight);
      ColorToBitmap(work[task].initialProp, "prop1.png", lootCoreX, lootCoreY, lootCoreWidth, lootCoreHeight);
      ReportError(task, account, "物品入库失败", "物品无法放入仓库");
    }
    CheckSleep(task, 100);
  }
}
//等待拿起仓库物品
void WaitPickStore(int task, int account, int thisPropX, int thisPropY)
{
  int counter = 0;
  while (true)
  {
    UpdateMap(task, account);
    //当背包格子与移动后不同时，视为仓库物品已经拿起
    if (!IsBitmapEqual(work[task].map, work[task].movedProp, propWidth, propHeight,
      thisPropX, thisPropY))
      break;
    counter++;
    if (counter > 30)
      ReportError(task, account, "物品拿起失败", "物品无法从仓库拿起");
    CheckSleep(task, 100);
  }
}
//等待物品回到背包（适用于数量为1的物品）
void WaitOutStore(int task, int account, int thisPropX, int thisPropY)
{
  int counter = 0;
  while (true)
  {
    UpdateMap(task, account);
    //当背包中的物品与移动前相同时，视为物品已回到背包
    if (IsBitmapEqual_Tolerance(work[task].map, work[task].initialProp, lootCoreWidth, lootCoreHeight,
      thisPropX + lootCoreX, thisPropY + lootCoreY, lootCoreX, lootCoreY, 500))
      break;
    counter++;
    if (counter > 30)
      ReportError(task, account, "物品出库失败", "物品无法放回背包");
    CheckSleep(task, 100);
  }
}
//等待物品数字复现
void WaitDigitRecover(int task, int account, int digitX, int digitY)
{
  int counter = 0;
  while (true)
  {
    UpdateMap(task, account);
    if (GetPropDigit(task, digitX, digitY) != -1)
      break;
    counter++;
    if (counter > 30)
      ReportError(task, account, "物品出库失败", "物品无法放回背包");
    CheckSleep(task, 100);
  }
}
//把道具背包(row,column)格道具移动到储物仓并截图，然后移动回来
void SaveProp(int task, int account, int row, int column)
{
  const POINT store = { 55, 117 };
  const POINT confirm = { 412, 360 };
  const POINT move_max = { 594, 325 };
  const POINT move_dec = { 390, 325 };

  int thisPropX = propX + column * propWidth;//道具格子左上角X
  int thisPropY = propY0 + row * propHeight - work[task].propOffsetY;//道具格子左上角Y（已计算偏移）
  int digitX = thisPropX + lootDigitX, digitY = thisPropY + lootDigitY;//道具数量个位数位置

  ForcedUpdateMap(task, account);
  CopyMap(work[task].initialProp, work[task].map, thisPropX, thisPropY);//保存移动前的道具图像，用于检验物品入库出库
  int numInProp = GetPropDigit(task, digitX, digitY);//获取道具数量

  if (column <= 1) //如果列数为0或1，保存本行图像
    CopyMap(work[task].calibration, work[task].map, thisPropX + calibrationX, thisPropY + calibrationY);

  //1. 移动到储藏室
  Click(task, account, thisPropX + 6, thisPropY + 5);//点击道具格子
  if (TryWaitPicture(APPEAR, task, account, "使用物品", 500))
    Click(task, account, thisPropX + 39, thisPropY + 48); //如果有使用菜单，点击移动按钮
  WaitPicture(APPEAR, task, account, "储物仓左上角_空");//等待储物仓左上角亮起
  Click(task, account, store);//点击储物仓第1格

  //数量为1：可直接移动，等待储物仓亮圈消失即可
  if (numInProp == -1)
  {
    WaitPicture(DISAPPEAR, task, account, "储物仓左上角_亮");
    CheckSleep(task, 200);
  }
  else //数量不为1：需要设置移动数量并确认
  {
    WaitPicture(APPEAR, task, account, "移动数量1");//等待移动数量变为1
    Click(task, account, move_max);//点击数量MAX
    WaitPicture(APPEAR, task, account, "移动数量-");
    Click(task, account, move_dec);//点击数量-1
    WaitPicture(APPEAR, task, account, "移动数量+");
    Click(task, account, confirm);//点击确认
  }
  WaitInStore(task, account);//等待物品进入储藏室

  //保存移动后的道具图像（如果物品数量为1，则此图像为空）；用于检验储藏室物品拿起
  CopyMap(work[task].movedProp, work[task].map, thisPropX, thisPropY);

  //数量不为1的物品在此时保存截图
  if (numInProp != -1)
    AddProp(task, thisPropX, thisPropY);

  //2. 放回道具背包
  const int storeDigitX = storeX + lootDigitX, storeDigitY = storeY + lootDigitY;
  int numInStore = GetPropDigit(task, storeDigitX, storeDigitY);//获取储物区道具数量

  Click(task, account, store);//点击储物仓第1格
  WaitPickStore(task, account, thisPropX, thisPropY);//等待储藏室物品被拿起

  if (column <= 1) //如果列数为0或1，利用保存的图像校准翻页量
  {
    ForcedUpdateMap(task, account);
    for (int y = propY0 + 6 * propHeight; y >= propY0; y--) //从底部往上寻找
      if (IsBitmapEqual(work[task].map, work[task].calibration, calibrationWidth, calibrationHeight,
        thisPropX + calibrationX, y))
      {
        work[task].propOffsetY += thisPropY + calibrationY - y;//更新翻页量
        thisPropY = propY0 + row * propHeight - work[task].propOffsetY;//重新计算thisPropY
        digitY = thisPropY + lootDigitY;//重新计算digitY
        break;
      }
  }

  Click(task, account, thisPropX + 6, thisPropY + 5);//点击道具格子

  //数量为1的物品可直接拿回，数量不为1则需要设置移动数量并确认
  if (numInStore != -1)
  {
    WaitPicture(APPEAR, task, account, "移动数量1");//等待移动数量变为1
    Click(task, account, move_max);//点击数量MAX
    WaitPicture(APPEAR, task, account, "移动数量-");
    Click(task, account, confirm);//点击确认
  }
  CheckSleep(task, 200);

  //若道具数量为1，等待道具背包出现物品后截图
  if (numInProp == -1)
  {
    WaitOutStore(task, account, thisPropX, thisPropY);
    AddProp(task, thisPropX, thisPropY);//保存道具截图
  }
  else //否则等待个位数字出现
    WaitDigitRecover(task, account, digitX, digitY);
  isRepaintRequired = 1;
}
//批量截图道具
void AutoSaveProp(int task, int account)
{
  for (int i = 22; i < INT_MAX; i++)
  {
    int row = i / 9;
    int column = i % 9;
    SaveProp(task, account, row, column);
  }
}

/*
//同类物品中某点出现过的所有颜色
const int maxColorNum = 100;
struct ColorListType
{
  int colorNum;//该点出现过的颜色数量
  COLORREF color[maxColorNum];//第i种颜色
  int picNum[maxColorNum];//第i种颜色的共色图片数
};
ColorListType colorList[propHeight][propWidth];//同类物品各点的颜色表
//记录(x,y)出现了颜色color
void RecordColorToList(COLORREF color, int x, int y)
{
  if (color == 0 || color == 0xffffff) //黑白两色不予记录
    return;
  //如果color已在颜色列表中，记录picNum++
  for (int i = 0; i < colorList[y][x].colorNum; i++)
    if (colorList[y][x].color[i] == color)
    {
      colorList[y][x].picNum[i]++;
      return;
    }
  //否则新增一种颜色
  colorList[y][x].color[colorList[y][x].colorNum] = color;
  colorList[y][x].picNum[colorList[y][x].colorNum] = 1;
  colorList[y][x].colorNum++;
}
//生成一类物品的底色（类名styleName）
void MakeBaseImage(const char *styleName)
{
  COLORREF tempBase[propHeight][propWidth] = {};//同类物品底色

  //0. 清空colorList
  memset(colorList, 0, sizeof(colorList));

  //1. 将A类道具的图片读入prop
  char folder[maxPath] = {};
  sprintf_s(folder, "资源文件\\%s", styleName);
  if (!FileExist(folder))
    return;

  //删除原有的底色文件
  char path[maxPath] = {};
  sprintf_s(path, "%s\\底色.png", folder);
  remove(path);

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, lootList, maxLootNum);//查找所有png文件

  lootNum = 0;
  for (int i = 0; i < filesNum && lootNum < maxLootNum; i++)
  {
    sprintf_s(path, "%s\\%s", folder, lootList[i]);
    if (_access(path, 0) == 0) //如果文件存在
    {
      BitmapToColor(path, loot[lootNum].image);//读取卡片图像到image
      lootList[i][strlen(lootList[i]) - 4] = 0;//删除".png"
      strcpy_s(loot[lootNum].name, lootList[i]);//记录名称
      lootNum++;
    }
  }

  //2. 记录共同底色
  for (int code = 0; code < lootNum; code++)
    for (int y = 0; y < propHeight; y++)
      for (int x = 0; x < propWidth; x++)
        RecordColorToList(loot[code].image[y][x], x, y);

  //3. 计算每一点的最优势颜色
  for (int y = 0; y < propHeight; y++)
    for (int x = 0; x < propWidth; x++)
    {
      int maxPicNum = 0;
      COLORREF bestColor = 0;
      for (int i = 0; i < colorList[y][x].colorNum; i++)
        if (colorList[y][x].picNum[i] > maxPicNum)
        {
          maxPicNum = colorList[y][x].picNum[i];
          bestColor = colorList[y][x].color[i];
        }
      if (maxPicNum <= 1) //出现不超过1次的点涂黑
        tempBase[y][x] = 0;
      else
        tempBase[y][x] = bestColor;
    }

  //4. 保存底色图案
  sprintf_s(path, "%s\\底色.png", folder);
  ColorToBitmap(tempBase, path);
}
//生成所有类型物品的底色
void MakeBaseImage()
{
  for (int i = 0; i < maxStyleNum; i++)
    MakeBaseImage(styleName[i]);
}
//道具自动分类
void AutoClassification()
{
  const char folder[] = "资源文件\\道具";
  if (!FileExist(folder))
    return;

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, lootList, maxLootNum);//查找所有png文件

  lootNum = 0;
  char path[maxPath] = {};
  char newPath[maxPath] = {};
  for (int i = 0; i < filesNum && lootNum < maxLootNum; i++)
  {
    sprintf_s(path, "%s\\%s", folder, lootList[i]);
    if (FileExist(path)) //如果文件存在
    {
      BitmapToColor(path, loot[lootNum].image);//读取卡片图像到image
      int style = GetLootStyle(lootNum);
      sprintf_s(newPath, "%s\\%c%s", folder, styleLetter[style], lootList[i]);//在名称前添加分类字母
      rename(path, newPath);
      lootList[i][strlen(lootList[i]) - 4] = 0;//删除".png"
      strcpy_s(loot[lootNum].name, lootList[i]);//记录名称
      lootNum++;
    }
  }
}
*/
//载入道具底色
void LoadBase()
{
  char path[maxPath] = {};
  for (int i = 0; i < maxStyleNum; i++)
  {
    sprintf_s(path, "附加程序\\图片\\物品\\底色\\%s.png", styleName[i]);
    BitmapToColor(path, base[i]);
  }
}
//计算某战利品与空格子的颜色重复数
int GetBlankSimilarity(int task, int row, int column)
{
  //战利品格子左上角位置
  int x0 = lootWidth * column;
  int y0 = lootHeight * row;

  int similarity = 0;
  for (int y = 0; y < lootHeight; y++)
    for (int x = 0; x < lootWidth; x++)
      if (work[task].loot[y0 + y][x0 + x] == 0x1a4263)
        similarity++;
  return similarity;
}
//获取战利品截图第(row,column)格的物品编号（返回值）和数量（quantity）
int GetLootCodeAndQuantity_Original(int task, int row, int column, int &quantity)
{
  //战利品格子左上角位置
  int x0 = lootWidth * column;
  int y0 = lootHeight * row;

  //填写战利品数量
  quantity = GetLootQuantity(task, x0 + lootDigitX, y0 + lootDigitY);

  //如果空格数目过多，直接认为是空格子
  if (GetBlankSimilarity(task, row, column) >= 10)
    return 0;

  //如果和已有战利品匹配，则返回战利品编号
  for (int code = 0; code < lootNum; code++)
    if (IsBitmapEqual(work[task].loot, loot[code].image, lootCoreWidth, lootCoreHeight,
      x0 + lootCoreX, y0 + lootCoreY, lootCoreX, lootCoreY))
    {
      if (loot[code].isReplacedRequired)
      {
        CopyMap(loot[code].image, work[task].loot, x0, y0);//用实战不绑截图替换loot
        loot[code].isReplacedRequired = false;
      }
      return code;
    }

  //如果不匹配，且战利品种数没超过上限，则新增战利品截图
  if (lootNum < maxLootNum)
  {
    CopyMap(loot[lootNum].image, work[task].loot, x0, y0);//记录新战利品截图
    int style = GetLootStyle(lootNum);//自动判断战利品分类（仅限BCDEF HIJKLM R类）
    sprintf_s(loot[lootNum].name, "%c001", styleLetter[style]);//直接命名为X001，反正数字不参与判定
    lootNum++;
    return lootNum - 1;
  }
  return -1;//新增不了就返回-1
}
//（带互斥锁）获取战利品截图第(row,column)格的物品编号（返回值）和数量（quantity）
int GetLootCodeAndQuantity(int task, int row, int column, int &quantity)
{
  WaitForSingleObject(hMutexLoot, INFINITE);
  int code = GetLootCodeAndQuantity_Original(task, row, column, quantity);
  ReleaseMutex(hMutexLoot);
  return code;
}
//将整数num分解得到各位数字
int GetDigits(int num, int(&digits)[9])
{
  memset(digits, 0, sizeof(digits));
  if (num == 0) //0视为一位数
    return 1;
  int digitsNum = 0;
  while (num > 0)
  {
    digits[digitsNum++] = num % 10;
    num = num / 10;
  }
  return digitsNum;
}
//记录战利品总收益
void SaveTotalLoot(int task, int account)
{
  char bitmapPath[maxPath] = {};
  sprintf_s(bitmapPath, "%s\\%dP_总收益.png", work[task].lootDirectory, account + 1);

  int lootTypeNum = 0;
  int digits[9] = {};
  memset(work[task].totalLoot, 0, sizeof(work[task].totalLoot));//先把图像清空
  for (char style = 'Z'; style >= 'B'; style--) //按从Z到B的顺序保存（不保存A类：空格）
    for (int code = lootNum - 1; code >= 0; code--)
      if (loot[code].name[0] == style)
        if (reserve[task].lootNum[account][code] > 0)
        {
          //要记录的战利品在总战利品中的行列
          int row = lootTypeNum / 10;
          int column = lootTypeNum % 10;
          //总战利品格子左上角位置
          int totalLootX = lootWidth * column;
          int totalLootY = lootHeight * row;
          //把战利品图片保存到总战利品截图中
          CopyMap(work[task].totalLoot, totalLootX, totalLootY, lootWidth, lootHeight,
            loot[code].image, 0, 0);
          //把战利品数量保存到总战利品截图中
          int digitsNum = GetDigits(reserve[task].lootNum[account][code], digits);
          int quantityWidth = digitsNum * lootDigitDistance + 1;
          const int quantityHeight = lootDigitHeight + 2;
          int quantityTop = totalLootY + lootDigitY - 1;//数量上方Y
          int quantityRight = totalLootX + lootDigitX + lootDigitWidth;//数量右侧X
          //把数量区涂黑
          for (int y = quantityTop; y < quantityTop + quantityHeight; y++)
            for (int x = quantityRight - quantityWidth + 1; x <= quantityRight; x++)
              work[task].totalLoot[y][x] = 0;
          //填写各位数字
          for (int i = 0; i < digitsNum; i++)
          {
            int digitX = quantityRight - (i + 1) * lootDigitDistance + 1;
            int digitY = quantityTop + 1;
            CopyMap(work[task].totalLoot, digitX, digitY,
              lootDigitWidth, lootDigitHeight, lootDigit[digits[i]], 0, 0);
          }
          lootTypeNum++;//战利品种数+1
        }
  //战利品种数>0时，保存总战利品截图
  if (lootTypeNum > 0)
  {
    int row = (lootTypeNum - 1) / 10 + 1;
    ColorToBitmap(work[task].totalLoot, bitmapPath, 0, 0, 10 * lootWidth, row * lootHeight);
  }
}
//战利品各页标准进度条，用于判断翻页是否完成。最多等0.2秒
COLORREF standardLootScroll[4][lootScrollHeight][1];
//载入战利品标准滚动条图像
void LoadLootScroll()
{
  char path[maxPath] = {};
  for (int page = 0; page < 4; page++)
  {
    sprintf_s(path, "附加程序\\图片\\战利品滚动条\\%d.png", page);
    BitmapToColor(path, standardLootScroll[page]);
  }
}
//战利品是否已翻到第page页
bool IsLootPaged(int task, int page)
{
  int counter = 0;
  for (int y = 0; y < lootScrollHeight; y++)
  {
    if ((work[task].map[lootScrollY + y][lootScrollX] == scrollBlankColor)
      == (standardLootScroll[page][y][0] == scrollBlankColor))
      counter++;
  }
  if (page == 0 || page == 3) //顶部和底部要求精确匹配
    return counter == lootScrollHeight;
  //中间2页偏差在10个像素以内（即滚动条位置差小于等于5像素）就算合格
  return counter >= lootScrollHeight - 10;
}
int cycleArea[lootHeight][lootWidth];
//战利品中的(x,y)点是否处在圆环带中
bool IsInCycle(int x, int y, double r1, double r2)
{
  const double x0 = 24.5, y0 = 23;
  double value = Sqr(x - x0) + Sqr(y - y0);
  return value >= Sqr(r1) && value <= Sqr(r2);
}
//战利品中的(x,y)点处于第几圆环带
int GetCycleArea(int x, int y)
{
  if (IsInCycle(x, y, 0, 5))
    return 0;
  if (IsInCycle(x, y, 6, 12))
    return 1;
  if (IsInCycle(x, y, 13, 40))
    return 2;
  return -1;
}
//填写战利品格子各点所属的圆环带
void WriteCycleArea()
{
  for (int y = 0; y < lootHeight; y++)
    for (int x = 0; x < lootWidth; x++)
      cycleArea[y][x] = GetCycleArea(x, y);
}
//颜色color是否为“加载中”圆环颜色
bool IsLoadingColor(COLORREF color)
{
  int r = bgrRValue(color);
  int g = bgrGValue(color);
  int b = bgrBValue(color);
  int diff = b * 3 - g * 2;
  return r > 250 && g > 150 && diff >= -5 && diff <= 5;
}
//(x0,y0)位置的物品是否正在加载
template <size_t height, size_t width>
bool IsItemLoading(COLORREF(&color)[height][width], int x0, int y0)
{
  int bingoNum[3] = {};//3个区域符合圆环颜色的像素数量
  for (int y = 0; y < lootHeight; y++)
    for (int x = 0; x < lootWidth; x++)
    {
      int cycle = cycleArea[y][x];
      if (cycle == 0 || cycle == 2)
      {
        if (color[y0 + y][x0 + x] == 0x314759)//圆环内外的深蓝底色
          bingoNum[cycle]++;
      }
      else if (cycle == 1)
      {
        if (IsLoadingColor(color[y0 + y][x0 + x])) //圆环颜色
          bingoNum[cycle]++;
      }
    }
  //3个圆环带颜色匹配数分别达到50,200,1200，判定为“加载中”
  return bingoNum[0] >= 50 && bingoNum[1] >= 200 && bingoNum[2] >= 1200;
}
//战利品第row行column列是否正在加载
bool IsLootLoading(int task, int row, int column)
{
  //战利品格子左上角位置
  int x0 = lootWidth * column;
  int y0 = lootHeight * row;
  return IsItemLoading(work[task].loot, x0, y0);
}
//道具背包第row行column列是否正在加载
bool isPropLoading(int task, int row, int column)
{
  int x0 = propX + column * propWidth;
  int y0 = reserve[task].propY + row * propHeight;
  return IsItemLoading(work[task].map, x0, y0);
}
//防御卡背包第row行column列是否正在加载
bool isSlotLoading(int task, int row, int column)
{
  int x0 = propX + column * customWidth + 32;
  int y0 = reserve[task].slotY + row * customHeight + 7;
  //只需一个点是灰色，就判定正在加载
  return work[task].map[y0][x0] == 0x899eaf;
}
//颜色color是不是道具格子顶部颜色
bool IsPropTopColor(COLORREF color)
{
  return color == 0x14263d || color == 0x001f41;
}
//截图高度y是不是道具/防御卡背包顶部
bool IsPropTop(int task, int y)
{
  const int topCenterX = 492;//格顶中心X
  COLORREF topCenterColor = work[task].map[y][topCenterX]; //格顶中心颜色
  //如果格顶中心颜色都不对，说明不是格顶
  if (!IsPropTopColor(topCenterColor))
    return false;
  //如果中心颜色对，那么周围10格都要是这个颜色，才判断为格顶
  for (int x = topCenterX - 10; x < topCenterX + 10; x++)
    if (work[task].map[y][x] != topCenterColor)
      return false;
  return true;
}
//获取道具背包顶部纵坐标，未找到返回-1
int GetPropTop(int task)
{
  const int y1 = propY0, y2 = propY0 + propHeight + 10;
  for (int y = y1; y < y2; y++)
    if (IsPropTop(task, y))
    {
      int propY = y + 1;//道具顶部Y=格顶判定线Y+1
      //把propY修正到[propY0, propY0 + propHeight)范围内
      while (propY >= propY0 + propHeight - 1)
        propY -= propHeight;
      return propY;
    }
  return -1;
}
//获取防御卡背包顶部纵坐标，未找到返回-1
int GetSlotTop(int task)
{
  const int y1 = propY0, y2 = propY0 + customHeight + 10;
  int x = 492;
  for (int y = y1; y < y2; y++)
    if (work[task].map[y][x] == customColor[0] && work[task].map[y + 1][x] == customColor[1]
      && work[task].map[y + 2][x] == customColor[1])
    {
      int slotY = y;//道具顶部Y=格顶判定线Y+1
      //把slotY修正到[propY0, propY0 + customHeight)范围内
      while (slotY >= propY0 + customHeight - 1)
        slotY -= customHeight;
      return slotY;
    }
  return -1;
}
//确定道具背包顶部位置propY
void LocatePropY(int task, int account)
{
  reserve[task].propY = GetPropTop(task);
  if (reserve[task].propY == -1)
    ReportError(task, account, "道具定位失败", "道具定位失败");
}
//确定防御卡背包顶部位置slotY
void LocateSlotY(int task, int account)
{
  reserve[task].slotY = GetSlotTop(task);
  if (reserve[task].slotY == -1)
    ReportError(task, account, "防御卡定位失败", "防御卡定位失败");
}
//截图后判断道具背包是否已加载好
bool isPropLoaded(int task, int account)
{
  ForcedUpdateMap(task, account);//强制截图
  for (int row = 0; row < 7; row++)
    for (int column = 0; column < 9; column++)
      if (isPropLoading(task, row, column))
        return false;
  return true;
}
//截图后判断防御卡背包是否已加载好
bool isSlotLoaded(int task, int account)
{
  ForcedUpdateMap(task, account);//强制截图
  for (int row = 0; row < 6; row++)
    for (int column = 0; column < 9; column++)
      if (isSlotLoading(task, row, column))
        return false;
  return true;
}
//记录战利品出错截图
int RecordLootError(int task, int account, int page, const char *tip)
{
  char lootPath[maxPath] = {};//本局战利品截图
  sprintf_s(lootPath, "%s\\(error)%dP_第%d局_%d(%s).png", work[task].lootDirectory,
    account + 1, work[task].games, page, tip);
  ColorToBitmap(work[task].map, lootPath, 200, 421, 600, 140);
  return 0;
}
//保存战利品截图
int SaveLoot(int task, int account)
{
  const COLORREF topColor = 0x082e4c;
  const POINT top = { lootScrollX, lootScrollY + 2 };
  const POINT bottom = { lootScrollX, lootScrollY + lootScrollHeight - 2 };
  const POINT pageDown = { lootScrollX, lootScrollY + lootScrollHeight + 8 };
  char lootPath[maxPath] = {};//本局战利品截图

  int lootGridY[4] = { 454, -1, -1, 503 };//格子起始高度
  const int lootPageY[4] = { 474, 501, 517, 533 };//翻页点击位置
  //进行4次翻页识别
  for (int page = 0; page < 4; page++)
  {
    CheckSleep(task, 100);
    UpdateMap(task, account);//点击前主动截图1次，避免隐藏状态点击失效
    Click(task, account, lootScrollX, lootPageY[page]); //点击滚动条对应位置
    int counter = 0;
    while (true) //等待滚动条移动到标准位置
    {
      CheckSleep(task, 50);
      if (UpdateMap(task, account)) //如果截图成功
      {
        //图中没有战利品3个字，则保存本页战利品截图，退出函数
        if (!FindPictureInMap(task, "战利品"))
          return RecordLootError(task, account, page, "无战利品字样");
        //如果滚动条移动到了标准位置，退出循环
        if (IsLootPaged(task, page))
          break;
      }
      counter++;
      if (counter >= 10) //500ms没有移动到标准位置，则保存本页战利品截图，退出函数
        return RecordLootError(task, account, page, "未翻到标准位置");
    }
    if (page == 0) //顶部：截取前2行
      CopyMap(work[task].loot, 0, 0, lootWidth * 10, lootHeight * 2, work[task].map, lootGridX, lootGridY[0]);
    else if (page == 3) //底部：截取最后一行
      CopyMap(work[task].loot, 0, lootHeight * 4, lootWidth * 10, lootHeight, work[task].map, lootGridX, lootGridY[3]);
    else //中间两页：需要先判定格子位置
    {
      const COLORREF gridTopColor[2] = { 0x14263d, 0x001f41 };//格顶标准颜色
      for (int y = 456; y < 456 + lootHeight; y++) //从上往下查找格顶颜色
      {
        bool isGridTop = true;
        const int gridTopCenterX = 233;//格顶中心X
        COLORREF gridTopCenterColor = work[task].map[y][gridTopCenterX]; //格顶中心颜色
        //如果格顶中心颜色都不对，说明此y不是格顶
        if (gridTopCenterColor != gridTopColor[0] && gridTopCenterColor != gridTopColor[1])
          isGridTop = false;
        else //如果中心颜色对，那么周围10格都要是这个颜色，才判断为格顶
          for (int x = gridTopCenterX - 10; x < gridTopCenterX + 10; x++)
            if (work[task].map[y][x] != gridTopCenterColor)
              isGridTop = false;
        if (isGridTop)
        {
          lootGridY[page] = y + 1; //记录格顶位置
          break;
        }
      }
      if (lootGridY[page] == -1) //如果找不到格底颜色
        return RecordLootError(task, account, page, "找不到格顶");
      if (page == 1)
        CopyMap(work[task].loot, 0, lootHeight * 2, lootWidth * 10, lootHeight, work[task].map, lootGridX, lootGridY[1]);
      else if (page == 2)
        CopyMap(work[task].loot, 0, lootHeight * 3, lootWidth * 10, lootHeight, work[task].map, lootGridX, lootGridY[2]);
    }
  }
  //如果某一格为“加载中”，结束函数
  for (int row = 4; row >= 0; row--)
    for (int column = 9; column >= 0; column--)
      if (IsLootLoading(task, row, column))
        return 0;

  sprintf_s(lootPath, "%s\\%dP_第%d局.png", work[task].lootDirectory, account + 1, work[task].games);
  ColorToBitmap(work[task].loot, lootPath);//保存5x10战利品截图
  //对loot的50个格子进行统计
  for (int row = 4; row >= 0; row--)
    for (int column = 9; column >= 0; column--)
    {
      //work[task].blankSimilarity[account][row][column] = GetBlankSimilarity(task, row, column);
      int quantity = 0;
      int code = GetLootCodeAndQuantity(task, row, column, quantity);
      if (code >= 0)
        reserve[task].lootNum[account][code] += quantity;
    }
  SaveTotalLoot(task, account);

  work[task].isLootSaved[account] = true;
  return 1;
}
//滚动条第page次翻页。滚动条位置不变最多等1秒，已经翻到底返回false
template <size_t scrollHeight>
bool PageScroll(int task, int account, int page, int scrollX, int scrollY, COLORREF(&scroll)[scrollHeight][1])
{
  //记录原滚动条
  for (int y = 0; y < (int)scrollHeight; y++)
    scroll[y][0] = work[task].map[scrollY + y][scrollX];

  //点击翻页
  bool isPageRequired = true;
  if (page == 0) //第1次翻页：点击顶部
  {
    if (scroll[0][0] != scrollBlankColor) //如果滚动条已经在顶部，本次无需翻页
      isPageRequired = false;
    else //否则翻到顶部
      Click(task, account, scrollX, scrollY + 2);
  }
  else //后续翻页：点击滚动条后的第1个像素
  {
    int barY = -1;//滚动条起始Y
    int emptyY = -1;//滚动条后的空白区起始Y
    for (int y = scrollY; y < scrollY + (int)scrollHeight; y++)
      if (work[task].map[y][scrollX] != scrollBlankColor) //滚动条区域
      {
        if (barY == -1) //记录滚动条起始Y
          barY = y;
      }
      else if (barY != -1) //非滚动条区域：如果已发现滚动条，记录滚动条后空白区起始Y
      {
        emptyY = y;
        break;
      }
    //如果不存在滚动条，或滚动条已到底，结束
    if (barY == -1 || emptyY == -1)
      return false;
    int barLength = emptyY - barY;
    int clickY = barY + barLength * 5 / 4;//翻3/4页
    if (clickY > scrollY + (int)scrollHeight)
      clickY = scrollY + (int)scrollHeight;
    Click(task, account, scrollX, clickY);
  }

  //每隔100ms截图，直到滚动条变化
  if (isPageRequired)
  {
    int counter = 0;
    do
    {
      counter++;
      if (counter > 10) //截图10次检测不到变化，不再检测
        return true;
      CheckSleep(task, 100);
      ForcedUpdateMap(task, account);
    } while (IsBitmapEqual(work[task].map, scroll, 1, scrollHeight, scrollX, scrollY));
  }

  return true;
}
//自动带卡首轮查找（输入空卡槽信息，输出每张卡的最佳候选卡的优先级和星级）
//只查找星级不超过maxStar的卡片
void ScanCustomzz(int task, int account, bool(&isSlotRequired)[maxSlotNum + 1],
  int(&bestCode)[maxSlotNum + 1], int(&bestStar)[maxSlotNum + 1], int maxStar)
{
  const POINT top = { customScrollX, customScrollY + 2 };
  const POINT pageDown = { customScrollX, customScrollY + customScrollHeight + 8 };
  int row = -1, column = -1;

  Candidate candidate[maxSlotNum + 1] = {};//21张卡的候选卡信息
  int bestPriority[maxSlotNum + 1] = {};//21张卡已找到的最高优先级
  bool isCandidateUnique = true;//所有卡槽是否只有1个优先级的候选卡

  for (int order = 1; order <= work[task].originalSlotNum[account]; order++)
    if (isSlotRequired[order])
    {
      GetCandidate(task, account, order, candidate[order]);//获取该卡槽的所有候选卡优先级
      if (candidate[order].num == 0) //没有截图的卡不携带
        isSlotRequired[order] = false;
      if (candidate[order].num > 1)
        isCandidateUnique = false;
      bestPriority[order] = -99;//最高优先级预设为-99（不存在）
      bestCode[order] = -1;//最高优先级编号预设为-1（不存在）
      bestStar[order] = -1;//最高星级预设为-1（不存在）
    }

  //带卡模式为“最靠前”，且所有空卡槽只有1个优先级，则取消首轮查找，直接记录
  if (customOption == 1 && isCandidateUnique)
  {
    for (int order = 1; order <= work[task].originalSlotNum[account]; order++)
      if (isSlotRequired[order])
        bestCode[order] = candidate[order].code[0];
    return;
  }

  //进行20次翻页识别
  COLORREF customScroll[customScrollHeight][1] = {};//背包滚动条
  ForcedUpdateMap(task, account);
  for (int page = 0; page <= 20; page++)
  {
    //翻页并截图
    if (!PageScroll(task, account, page, customScrollX, customScrollY, customScroll))
      return;

    //确定防御卡起始识别高度bagY
    int x = cellX + customX;
    for (int y = cellY1; y < cellY2; y++)
      if (work[task].map[y][x] == customColor[0] && work[task].map[y + 1][x] == customColor[1]
        && work[task].map[y + 2][x] == customColor[1])
      {
        reserve[task].cellY = y;
        break;
      }

    //在本页搜索每一个空卡槽
    for (int order = 1; order <= work[task].originalSlotNum[account]; order++)
      if (isSlotRequired[order])
        //搜索这个卡槽的所有候选卡（优先级从高到低）
        for (int i = 0; i < candidate[order].num; i++)
        {
          //如果此候选卡的优先级小于已找到的最高优先级，则无需查找
          if (candidate[order].priority[i] < bestPriority[order])
            break;

          //查找此候选卡在本页的最高星级
          int star = FindCustom(task, candidate[order].code[i], 2, maxStar);
          if (star >= 0) //如果查找成功
            //如果此卡优先级更高，或优先级相同但星级更高，更新这张卡已找到的最高优先级和最高星级
            if (candidate[order].priority[i] > bestPriority[order] || star > bestStar[order])
            {
              bestPriority[order] = candidate[order].priority[i];
              bestStar[order] = star;
              bestCode[order] = candidate[order].code[i];
            }
        }
  }
}
//携带自定卡片，返回是否携带成功。option==0携带任意星级，否则携带targetStar星级
bool TakeCustom(int task, int account, int customCode, int option, int targetStar)
{
  const COLORREF customColor[2] = { 0x001f41, 0x1a4263 };//防御卡识别色
  if (customCode < 0 || customCode >= maxCustomNum)
    return false;

  const POINT top = { 930, 198 };
  const POINT pageDown = { 930, 400 };
  int row = -1, column = -1;

  //进行20次翻页识别
  COLORREF customScroll[customScrollHeight][1] = {};//背包滚动条
  ForcedUpdateMap(task, account);
  for (int page = 0; page <= 20; page++)
  {
    //翻页并截图
    if (!PageScroll(task, account, page, customScrollX, customScrollY, customScroll))
      return false;

    //确定防御卡起始识别高度bagY
    int x = cellX + customX;
    for (int y = cellY1; y < cellY2; y++)
      if (work[task].map[y][x] == customColor[0] && work[task].map[y + 1][x] == customColor[1]
        && work[task].map[y + 2][x] == customColor[1])
      {
        reserve[task].cellY = y;
        break;
      }

    //在本页找卡，如果找到
    if (FindCustom(task, customCode, option, targetStar, &row, &column) >= 0)
    {
      //卡片识别区起始位置
      int startX = cellX + column * customWidth + customX;
      int startY = reserve[task].cellY + row * customHeight + customY;

      int counter = 0;
      do
      {
        counter++;
        if (counter > 15) //1500ms未携带成功则报错
        {
          char message[100] = {};
          sprintf_s(message, "无法携带[%s]卡片", work[task].custom[customCode].name);
          ReportError(task, account, "带卡失败", message);
        }
        if (counter % 5 == 1) //每500ms携带一次卡片
          Click(task, account, startX, startY);
        CheckSleep(task, 100);
        ForcedUpdateMap(task, account);//截图
      } while (!IsCustomGray(task, customCode, startX, startY));
      //若卡片没有变灰，继续点击
      return true; //携带成功
    }
  }
  return false; //携带失败
}
//移除宽模板中的背景色
void RemoveBackground(COLORREF(&Background)[wideTemplateHeight][wideTemplateWidth],
  COLORREF(&WideTemplate)[wideTemplateHeight][wideTemplateWidth])
{
  for (int y = 0; y < wideTemplateHeight; y++)
    for (int x = 0; x < wideTemplateWidth; x++)
      if (Background[y][x] == WideTemplate[y][x])
        WideTemplate[y][x] = 0;
}
//记录颜色color存在
void RecordColor(int task, COLORREF color)
{
  work[task].colorExist[color >> 3] |= 1 << (color & 0x00000007);
}
//取消颜色color的记录
void EraseColor(int task, COLORREF color)
{
  work[task].colorExist[color >> 3] &= ~(1 << (color & 0x00000007));
}
//判断颜色color是否存在
int IsColorExist(int task, COLORREF color)
{
  return work[task].colorExist[color >> 3] & 1 << (color & 0x00000007);
}
//统计模板Template和ColorExist的相同颜色数量，计算完恢复ColorExist
template <size_t width, size_t height>
int CountSameColor(int task, COLORREF(&Template)[height][width])
{
  int bingoNum = 0;//重合的颜色数量
  int erasedColorNum = 0;//被取消的颜色数量

  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
    {
      COLORREF color = Template[y][x];
      if (IsColorExist(task, color))
      {
        bingoNum++;
        work[task].erasedColor[erasedColorNum++] = color;
        EraseColor(task, color);
      }
    }

  for (int i = 0; i < erasedColorNum; i++) //恢复被删除的记录
    RecordColor(task, work[task].erasedColor[i]);

  return bingoNum;
}
//从WideTemplate中截取一部分作为模板
void CopyTemplate(COLORREF(&Template)[templateHeight][templateWidth],
  COLORREF(&WideTemplate)[wideTemplateHeight][wideTemplateWidth], int x0, int y0)
{
  for (int y = 0; y < templateHeight; y++)
    for (int x = 0; x < templateWidth; x++)
      Template[y][x] = WideTemplate[y0 + y][x0 + x];
}
//将模板Template中的所有颜色记入ColorExist数组，记录RecordedColor和RecordedColorNum
template <size_t width, size_t height>
void RecordTemplate(int task, COLORREF(&Template)[width][height], int x0 = 0)
{
  work[task].recordedColorNum = 0;
  for (int x = 0; x < templateWidth; x++)
    for (int y = 0; y < templateHeight; y++)
    {
      COLORREF color = Template[y][x0 + x];
      if (!IsColorExist(task, color))
      {
        work[task].recordedColor[work[task].recordedColorNum++] = color;
        RecordColor(task, color);
      }
    }
}
//清除记录的颜色
void ClearColor(int task)
{
  for (int i = 0; i < work[task].recordedColorNum; i++)
    EraseColor(task, work[task].recordedColor[i]);
  work[task].recordedColorNum = 0;
}
//将账号account卡片order的模板记入ColorExist数组，记录模板颜色总数
void RecordTemplate(int task, int account, int order)
{
  RecordTemplate(task, work[task].card[account][order].image);
  work[task].slot[account][order].colorNum = work[task].recordedColorNum;
}
//将账号account卡片order的模板记入ColorExist数组，记录模板颜色总数
void RecordTemplateTarget(int task, int targetCode)
{
  Target &target = work[task].target[targetCode];
  int itemCode = FindItem(target.name);
  //仅当查找itemCode成功时记录颜色，查找失败则模板不含任何颜色
  if (itemCode != -1)
    RecordTemplate(task, item[itemCode].image);
  target.colorNum = work[task].recordedColorNum;
}
//截取账号account第order张卡的模板，截取成功返回1，否则返回0
int CatchTemplate(int task, int account, int order)
{
  if (!IsGameWindowVisible(work[task].hWnd[account]))
    return 0;
  MoveTo(task, account, moveToTemplateX, moveToTemplateY);//鼠标移动到模板位置
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
    isShotSuccessful = RegionalMapShot(work[task].hWnd[account], work[task].wideTemplate,
      wideTemplateX, wideTemplateY);//截取宽模板
  } while (isShotSuccessful == 0 || IsBitmapEqual(work[task].background[account], work[task].wideTemplate, wideTemplateWidth, wideTemplateHeight));
  RemoveBackground(work[task].background[account], work[task].wideTemplate);//宽模板背景色设为0
  //宽模板截取完毕，开始识别卡片种类

  //第一步：查看是不是自定卡片
  for (int code = 0; code < cardNum; code++)
  {
    RecordTemplate(task, card[code].image);//将第i个自定卡片存入ColorExist
    int similarity = CountSameColor(task, work[task].wideTemplate);
    bool isMatched = similarity >= work[task].recordedColorNum / 3;
    ClearColor(task);//清空ColorExist
    //匹配1/3的颜色则认为匹配成功
    if (isMatched)
    {
      memcpy(&work[task].card[account][order], &card[code], sizeof(Card));
      return 1;
    }
  }
  //第二步：如果不在自定卡片中，沿用旧方案

  int slotStyle = 0;//卡片类型：0=正常，1=海星，2=煮蛋，3=其他宽卡
  RecordTemplate(task, work[task].wideTemplate, 20);//将宽模板中指定64x60的颜色记入ColorExist
  for (int i = 0; i < 4; i++)//逐一匹配4个标准卡片
  {
    int similarity = CountSameColor(task, cardTemplate[i]);
    if (similarity >= work[task].recordedColorNum / 3)
    {
      if (i == 0)
        slotStyle = 1;//海星
      else
        slotStyle = 2;//煮蛋
      break;
    }
  }
  ClearColor(task);//清空ColorExist
  //如果匹配结果为0，看是不是主神或投手（从右侧开始寻找）
  POINT rightMost = { -1, -1 };//模板最右点
  int cardHeight = INT_MAX;
  if (slotStyle == 0)
  {
    //计算卡片高度
    int topMost = -1, bottomMost = -1;
    for (int y = 0; y < wideTemplateHeight; y++) //从上往下
    {
      for (int x = 0; x < wideTemplateWidth; x++) //从左往右
        if (work[task].wideTemplate[y][x])//如果找到了非黑色，记录最右侧匹配位置
        {
          topMost = y;
          break;
        }
      if (topMost >= 0)
        break;
    }
    for (int y = wideTemplateHeight - 1; y > 0; y--) //从下往上
    {
      for (int x = 0; x < wideTemplateWidth; x++) //从左往右
        if (work[task].wideTemplate[y][x])//如果找到了非黑色，记录最右侧匹配位置
        {
          bottomMost = y;
          break;
        }
      if (bottomMost >= 0)
        break;
    }
    cardHeight = bottomMost - topMost;
    for (int x = wideTemplateWidth - 1; x >= 0; x--) //从右往左
    {
      for (int y = 0; y < wideTemplateHeight; y++) //从上往下
        if (work[task].wideTemplate[y][x])//如果找到了非黑色，记录最右侧匹配位置
        {
          rightMost.x = x;
          rightMost.y = y;
          break;
        }
      if (rightMost.x >= 0)
        break;
    }
    // 根据最右点特性判断类型
    if (rightMost.x >= 91)//最右点X>=91，属于宽卡，可能是主神或投手类
    {
      if (rightMost.y <= 10)//最右点Y<=10，可能是主神的长矛
      {
        for (int x = wideTemplateWidth - 1; x >= 0; x--)//在Y=40高度从右往左寻找
          if (work[task].wideTemplate[40][x]) //找到最右点时
          {
            if (x <= 70) //如果X<=70，判定为主神类
              slotStyle = 4;
            break;
          }
      }
      //如果没有判定为主神类，且最右点X>=97，判定为投手类
      if (slotStyle == 0 && rightMost.x >= 97)
        slotStyle = 3;
    }
  }

  if (slotStyle == 2)//煮蛋：偏移36,10
    CopyTemplate(work[task].card[account][order].image, work[task].wideTemplate, 36, 10);
  else if (slotStyle == 3)//投手（雷神双鱼等）：采取右对齐，但扣除最右边5个像素
    CopyTemplate(work[task].card[account][order].image, work[task].wideTemplate, rightMost.x - 65, 6);
  else if (slotStyle == 4)//主神：偏移0,4
    CopyTemplate(work[task].card[account][order].image, work[task].wideTemplate, 0, 4);
  else //常规卡或海星：偏移20,0
    CopyTemplate(work[task].card[account][order].image, work[task].wideTemplate, 20, 0);
  work[task].card[account][order].offsetX = 0;
  //高度小于62的卡，识别位置要偏移
  work[task].card[account][order].offsetY = cardHeight >= 62 ? 0 : min(62 - cardHeight, 18);
  work[task].card[account][order].width = slotTemplateWidth;
  work[task].card[account][order].height = slotTemplateHeight;
  work[task].card[account][order].requiredSimilarity = 40;//0%强度识图标准为40

  if (slotStyle == 1) //记录海星编号
    work[task].starfishOrder[account] = order;
  return 1;
}
//拾起卡槽
void PickSlot(int task, int account, int order)
{
  if (order <= 14)
    Click(task, account, slotX1 + (order - 1) * slotWidth, slotY1);//卡槽1-14
  else
    Click(task, account, slotX1 + 13 * slotWidth, slotY1 + (order - 14) * slotHeight);//卡槽15-21
}
//若卡槽未拾起，则拾起卡槽
void TryPickSlot(int task, int account, int order, bool &isSlotPicked)
{
  if (isSlotPicked == false)
  {
    PickSlot(task, account, order);
    isSlotPicked = true;
  }
}
//如果需要识别，则截取模板。若卡槽已拾起则返回true
bool TryCatchTemplate(int task, int account, int order)
{
  bool isSlotPicked = false;//卡槽是否已经拾起
  //如果需要识别且未获取模板，则截取模板
  if ((work[task].slot[account][order].isSimilarityRequired || monitor[task].isExist)
    && !work[task].isTemplateCatched[account][order])
  {
    PickSlot(task, account, order);//拾起卡槽
    isSlotPicked = true;
    if (work[task].isBackgroundCatched[account])//如果截取了背景，则截取模板，成功则记录已截取
      work[task].isTemplateCatched[account][order] = CatchTemplate(task, account, order);
  }
  return isSlotPicked;
}
//将卡片order放到指定格子
void PlantToGrid(int task, int account, int order, int row, int column, int plantTimes)
{
  if (row >= 1 && row <= 7 && column >= 1 && column <= 9)
  {
    if (order == zero) //铲子：拿起后点3下格子
    {
      PostMessage(work[task].hWnd[account], WM_KEYDOWN, 97, 0);//按下键盘1
      for (int i = 0; i < plantTimes; i++)
      {
        Lay(task, account, row, column);
        if (i < plantTimes - 1)
          CheckSleep(task, 1);
      }
      Lay(task, account, 3, -1);//释放卡片，防止影响下次种植
    }
    else //卡片：拿起、放卡、释放整个过程重复3次
    {
      for (int i = 0; i < plantTimes; i++)
      {
        bool isSlotPicked = TryCatchTemplate(task, account, order);
        if (!isSlotPicked)
          PickSlot(task, account, order);//拾起卡槽
        Lay(task, account, row, column);
        Lay(task, account, 3, -1);//释放卡片，防止影响下次种植
        if (i < plantTimes - 1)
          CheckSleep(task, 1);
      }
    }
  }
}
//索敌位置信息（下标0为第一个放卡位置）
struct Hunter
{
  bool enabled;//索敌是否开启
  int size;//放置次数
  int row[63], column[63];//放置位置
};
//执行第times次放卡（row,column强制指定放卡位置）
void PlantTimes(int task, int account, int order, int times, int row = 0, int column = 0)
{
  //如果row为0，按阵型放置，否则放在指定行数。column同理
  if (row == 0)
    row = work[task].slot[account][order].row[times];
  if (column == 0)
    column = work[task].slot[account][order].column[times];
  //卡片：连同拿卡槽一起重复3次
  //铲子：拿起后点击3次
  PlantToGrid(task, account, order, row, column, 3);
}
//指定一定高度范围进行颜色匹配
void AreaSimilarity(int task, int x0, int y0, int yBegin, int yEnd, int &erasedNum, int &count)
{
  for (int y = yBegin; y < yEnd; y++)
    for (int x = 0; x < templateWidth; x++)
    {
      COLORREF color = work[task].map[y0 + y][x0 + x];
      if (IsColorExist(task, color))
      {
        count++;
        work[task].erasedColor[erasedNum++] = color;
        EraseColor(task, color);
      }
    }
}
bool isCoveredByMaltose[gridHeight][gridWidth];//各点是否被麦芽糖覆盖
//计算row行column列与初始格子相似度
int GetInnateGridSimilarity(int task, int row, int column)
{
  int y0 = innateGridY + (row - 1) * gridHeight;
  int x0 = innateGridX + (column - 1) * gridWidth;
  int counter = 0;
  for (int y = 0; y < gridHeight; y++)
    for (int x = 0; x < gridWidth; x++)
      if (isCoveredByMaltose[y][x]) //只统计棉麦覆盖范围内的点
        if (reserve[task].innateGrid[row - 1][column - 1][y][x] == work[task].map[y0 + y][x0 + x])
          counter++;
  return counter;
}
//海星相似度计算专用函数
int GetStarfishSimilarity(int task, int x0, int y0)
{
  // 上区：y0-14~y0；中区：y0~y0+TemplateHeight-14；
  // 下区：y0+TemplateHeight-14~y0+TemplateHeight

  int lowerErasedColorNum = 0;//被取消的颜色数量
  int lowerSimilarity = 0;//相似度

  //公共区域计算，擦除颜色记录到lowerErasedColorNum，相似度记录到lowerSimilarity
  AreaSimilarity(task, x0, y0, 0, templateHeight - 14, lowerErasedColorNum, lowerSimilarity);
  int upperSimilarity = lowerSimilarity;//相似度赋值到上区
  int upperErasedColorNum = lowerErasedColorNum;

  //上区计算，擦除颜色记录到upperErasedColorNum，相似度记录到upperSimilarity
  AreaSimilarity(task, x0, y0, -14, 0, upperErasedColorNum, upperSimilarity);
  //恢复上区匹配的颜色
  for (int i = lowerErasedColorNum; i < upperErasedColorNum; i++)
    RecordColor(task, work[task].erasedColor[i]);

  //下区计算，擦除颜色记录到lowerErasedColorNum，相似度记录到lowerSimilarity
  AreaSimilarity(task, x0, y0, templateHeight - 14, templateHeight, lowerErasedColorNum, lowerSimilarity);
  //恢复公共区域和下区匹配的颜色
  for (int i = 0; i < lowerErasedColorNum; i++)
    RecordColor(task, work[task].erasedColor[i]);

  return upperSimilarity >= 20 && lowerSimilarity >= 20 ? max(upperSimilarity, lowerSimilarity) : 0;
}
//计算从地图(x0,y0)截取的实物与模板相同的颜色数量。算完后恢复ColorExist
int GetSimilarityXY(int task, int x0, int y0, int realWidth = templateWidth, int realHeight = templateHeight)
{
  int BingoNum = 0;//重合的颜色数量
  int ErasedColorNum = 0;//被取消的颜色数量

  for (int y = 0; y < realHeight; y++)
    for (int x = 0; x < realWidth; x++)
    {
      COLORREF color = work[task].map[y0 + y][x0 + x];
      if (IsColorExist(task, color))
      {
        BingoNum++;
        work[task].erasedColor[ErasedColorNum++] = color;
        EraseColor(task, color);
      }
    }

  for (int i = 0; i < ErasedColorNum; i++) //恢复被删除的记录
    RecordColor(task, work[task].erasedColor[i]);

  return BingoNum;
}
//从账号account的row行column列截取实物与模板卡进行比色，返回颜色匹配的像素数量
int GetSimilarity(int task, int account, int order, int row, int column)
{
  Card &card = work[task].card[account][order];
  //不需要种植的格子视为无卡（棉麦相似度9999，其他卡0）
  if (!work[task].slot[account][order].isPlantedInMap[row][column])
    return work[task].slot[account][order].isMaltose ? 9999 : 0;
  //棉花糖和麦芽糖识别与初始地图的相似度
  if (work[task].slot[account][order].isMaltose)
    return GetInnateGridSimilarity(task, row, column);
  //海星需要在两个位置识别
  if (order == work[task].starfishOrder[account])
    return GetStarfishSimilarity(task, objectX + (column - 1) * gridWidth, objectY + (row - 1) * gridHeight);
  return GetSimilarityXY(task, objectX + card.offsetX + (column - 1) * gridWidth,
    objectY + card.offsetX + (row - 1) * gridHeight, card.width, card.height);
}
//从账号account的row行column列截取实物与模板物件进行比色，返回颜色匹配的像素数量
int GetSimilarityItem(int task, int itemCode, int row, int column)
{
  return GetSimilarityXY(task, objectX + item[itemCode].offsetX + (column - 1) * gridWidth,
    objectY + item[itemCode].offsetY + (row - 1) * gridHeight, item[itemCode].width, item[itemCode].height);
}
//根据补阵强度和测得的相似度判断是否已占用
void GetOccupied(int task, int account, int order, int level)
{
  //临界相似度
  int requiredSimilarity = work[task].card[account][order].requiredSimilarity * (repairLevel + 2) / 2;
  Slot &slot = work[task].slot[account][order];

  //棉麦：看已放次数
  if (work[task].slot[account][order].isMaltose)
  {
    for (int row = 1; row <= 7; row++)
      for (int column = 1; column <= 9; column++)
      {
        short maxSimilarity = work[task].slot[account][order].maxSimilarity[row][column];

        //棉麦已放次数为0，一定无棉麦；次数为1-2，看相似度；次数 >= 3，一定有棉麦
        if (reserve[task].maltoseTimes[row - 1][column - 1] >= 3 ||
          (reserve[task].maltoseTimes[row - 1][column - 1] >= 1 && maxSimilarity < maltoseRequiredSimilarity))
          work[task].grid[level][row][column] = account * 100 + order;
      }
  }
  else //其他卡：相似度大于临界相似度，判定有卡
  {
    for (int row = 1; row <= 7; row++)
      for (int column = 1; column <= 9; column++)
        if (work[task].slot[account][order].maxSimilarity[row][column] >= requiredSimilarity)
          work[task].grid[level][row][column] = account * 100 + order;
  }
}
//获取账号account卡片order每个格子的相似度（同时计算每一格的最大相似度）
void GetSimilarity(int task, int account, int order)
{
  Slot &slot = work[task].slot[account][order];
  int level = slot.level;//卡片层级
  RecordTemplate(task, account, order);//将模板颜色记入ColorExist

  for (int i = recognitionFrame - 1; i >= 1; i--) //将最近recognitionRate-1次相似度右移一格
    for (int row = 1; row <= 7; row++)
      for (int column = 1; column <= 9; column++)
        slot.similarity[row][column][i] = slot.similarity[row][column][i - 1];

  for (int row = 1; row <= 7; row++) //获取每格与模板的相似度
    for (int column = 1; column <= 9; column++)
      if (level > 0 && work[task].grid[level][row][column] > 0)//1-4层级如果这一格已占用
        //棉麦的相似度为9999，其他卡相似度为0（代表本格没有本卡）
        slot.similarity[row][column][0] = slot.isMaltose ? 9999 : 0;
      else //否则计算相似度，看是不是本卡
        slot.similarity[row][column][0] = GetSimilarity(task, account, order, row, column);

  //临界相似度
  int requiredSimilarity = work[task].card[account][order].requiredSimilarity * (repairLevel + 2) / 2;

  //求取最近1秒测量的相似度的最大值
  slot.num = 0;
  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 9; column++)
    {
      //统计最大相似度
      slot.maxSimilarity[row][column] = slot.similarity[row][column][0];
      for (int i = 1; i < recognitionFrame; i++)
        slot.maxSimilarity[row][column] = max(slot.maxSimilarity[row][column], slot.similarity[row][column][i]);
      //海星和棉麦最大相似度不变，其他卡最大相似度归一化到1000
      if (slot.colorNum > 0 && order != work[task].starfishOrder[account] && !slot.isMaltose)
        slot.maxSimilarity[row][column] = slot.maxSimilarity[row][column] * 1000 / slot.colorNum;
      if (slot.maxSimilarity[row][column] >= requiredSimilarity)
        slot.num++;
    }

  //根据每格的最大相似度和全局最大相似度计算占用（0层级不记录占用）（此时RecordedColorNum=模板颜色总数）
  if (level > 0)
    GetOccupied(task, account, order, level);

  ClearColor(task); //清空ColorExist
}
//新增记录一个目标位置，保持相似度从大到小排列
void AddTargetLocation(Target &target, int row, int column)
{
  int insertLoc = target.num;//插入位置
  for (int i = 0; i < target.num; i++)
  {
    int iRow = target.location[i] / 10;
    int iColumn = target.location[i] % 10;
    //寻找第一个相似度小于待插入格子的位置
    if (target.maxSimilarity[iRow][iColumn] < target.maxSimilarity[row][column])
    {
      insertLoc = i;
      break;
    }
  }
  for (int i = target.num; i > insertLoc; i--)
    target.location[i] = target.location[i - 1];
  target.location[insertLoc] = row * 10 + column;
  target.num++;
}
//获取目标targetCode每个格子的相似度（同时计算每一格的最大相似度）
void GetSimilarityTarget(int task, int targetCode)
{
  Target &target = work[task].target[targetCode];
  RecordTemplateTarget(task, targetCode);//将模板颜色记入ColorExist

  for (int i = recognitionFrame - 1; i >= 1; i--) //将最近recognitionRate-1次相似度右移一格
    for (int row = 1; row <= 7; row++)
      for (int column = 1; column <= 10; column++)
        target.similarity[row][column][i] = target.similarity[row][column][i - 1];

  int itemCode = FindItem(target.name);
  for (int row = 1; row <= 7; row++) //获取每格与模板的相似度
    for (int column = 1; column <= 10; column++)
      target.similarity[row][column][0] = itemCode == -1 ? 0 : GetSimilarityItem(task, itemCode, row, column);

  //求取近recognitionRate次测量的相似度的最大值，统计目标个数
  target.globalMaxSimilarity = 0;
  target.num = 0;
  int combo = 0;//一行内连续存在物件的格子数
  int requiredSimilarity = itemCode == -1 ? 1000 : item[itemCode].requiredSimilarity;
  for (int row = 1; row <= 7; row++)
  {
    combo = 0;//进入新的一行，连续格数清零
    for (int column = 1; column <= 10; column++)
    {
      //统计最大相似度
      target.maxSimilarity[row][column] = target.similarity[row][column][0];
      for (int i = 1; i < recognitionFrame; i++)
        target.maxSimilarity[row][column] = max(target.maxSimilarity[row][column], target.similarity[row][column][i]);
      //最大相似度归一化到1000
      if (target.colorNum > 0)
        target.maxSimilarity[row][column] = target.maxSimilarity[row][column] * 1000 / target.colorNum;
      //记录物件数量
      if (target.maxSimilarity[row][column] >= requiredSimilarity)
      {
        combo++; //连续存在格数+1
        if (combo % 3 == 1) //连续格数达到1,4,7,10时增加1个物件
          AddTargetLocation(target, row, column);
      }
      else
        combo = 0; //连续存在格数清零
    }
  }

  for (int i = 0; i < work[task].recordedColorNum; i++) //清空ColorExist
    EraseColor(task, work[task].recordedColor[i]);
}
//指定颜色函数获取卡槽数，存入RealSlotNum，并记录SlotX
void GetSlotNum(int task, int account)
{
  const COLORREF slotColor = 0xcac7d9;//关卡内卡槽识别点颜色
  POINT point[33] = { { 217, 11 } };
  for (int i = 1; i <= 13; i++)//point[0..21]对应1-12槽的情形
  {
    point[i].x = point[i - 1].x + slotWidth;
    point[i].y = point[i - 1].y;
  }
  for (int i = 14; i <= 20; i++)
  {
    point[i].x = point[i - 1].x;
    point[i].y = point[i - 1].y + slotHeight;
  }
  for (int i = 0; i <= 11; i++)//point[21..32]对应1-12槽的情形
  {
    point[21 + i].x = point[i].x + 21;
    point[21 + i].y = point[i].y;
  }

  //使用颜色函数获取各点颜色
  COLORREF color[33] = {};
  for (int i = 0; i < 33; i++)
    color[i] = work[task].map[point[i].y][point[i].x];

  int num1, num2;
  for (num1 = 0; num1 <= 20; num1++) //13-21槽判定点
    if (color[num1] != slotColor)
      break;
  for (num2 = 21; num2 <= 32; num2++) //1-12槽判定点
    if (color[num2] != slotColor)
      break;
  int slotNum = max(num1, num2 - 21);//本次测得的卡槽数
  if (slotNum > work[task].realSlotNum[account]) //卡槽数只增不减
    work[task].realSlotNum[account] = slotNum;

  if (work[task].realSlotNum[account] <= 12)//根据卡槽数量确定卡槽左端位置
    work[task].slotX[account] = 213;
  else
    work[task].slotX[account] = 192;

  POINT(&cdPoint)[22] = work[task].cdPoint[account];
  cdPoint[1].x = work[task].slotX[account] + 44;
  cdPoint[1].y = 70;
  for (int i = 2; i <= 14; i++) //2-14槽
  {
    cdPoint[i].x = cdPoint[i - 1].x + slotWidth;
    cdPoint[i].y = cdPoint[i - 1].y;
  }
  for (int i = 15; i <= 21; i++) //15-21槽
  {
    cdPoint[i].x = cdPoint[i - 1].x;
    cdPoint[i].y = cdPoint[i - 1].y + slotHeight;
  }
}
//判断进度条i点的区域：0=左；1=鼠头；2=右
int GetProgressArea(int task, int i)
{
  //鼠头左侧区域
  if (work[task].map[progressBarY][progressBarX + i] == progressLeftColor[i])
    return 0;
  //鼠头右侧可变色区域
  if (i >= progressRatWidth && i < progressBarLength - progressHeadWidth)
  {
    for (COLORREF color : possibleRightColor)
      if (work[task].map[progressBarY][progressBarX + i] == color)
        return 2;
  }
  //鼠头右侧固定色区域
  else if (work[task].map[progressBarY][progressBarX + i] == progressRightColor[i])
    return 2;
  return 1;
}
//判定进度位置，无效进度条返回-1
int GetProgress(int task)
{
  int progressArea[progressBarLength] = {}; //进度条各点区域
  int left = -1, right = progressBarLength;//鼠头区域的入点和出点
  int ratMatchNum = 0;//鼠头颜色匹配数量
  int firstRatMatch = -1;//第一个鼠头固定色匹配位置
  for (int i = 0; i < progressBarLength; i++)
  {
    progressArea[i] = GetProgressArea(task, i); //获得进度条i点的区域
    //如果右边的区域值比左边小，判定进度条无效
    if (i > 0 && progressArea[i] < progressArea[i - 1])
      return -1;
    //鼠头区域：进行固定色匹配
    if (progressArea[i] == 1)
    {
      if (ratMatchNum < ratColorWidth && work[task].map[progressBarY][progressBarX + i] == ratColor[ratMatchNum])
      {
        if (ratMatchNum == 0)
          firstRatMatch = i;
        ratMatchNum++;
      }
    }
    //第一次进入鼠头区域，进行记录
    if (left == -1 && progressArea[i] == 1)
      left = i;
    //第一次退出鼠头区域，进行记录
    if (right == progressBarLength && progressArea[i] == 2)
      right = i;
  }
  //有效条件：有鼠头区域，鼠头区域宽度不超过32，匹配度26
  if (left >= 0 && right - left <= progressRatWidth + 1 && ratMatchNum == 26)
    return firstRatMatch;
  return -1;
}
const int bossBarX = 445, bossBarY = 574, bossBarLength = 430;//boss血量判定区位置 X->445(+4) Y->574(+8)
const COLORREF bossBarColor[2] = { 0x96e0f2, 0xa4e3f9 };//单/双BOSS血条边框颜色
//计算与bossNum个BOSS血条边框的匹配度
int BossBarFitness(int task, int bossNum)
{
  int fitness = 0;
  for (int i = 0; i < bossBarLength; i++)
    if (work[task].map[bossBarY][bossBarX + i] == bossBarColor[bossNum - 1])
      fitness++;
  return fitness;
}
//判断BOSS数量（抗遮挡）
int GetBossNum(int task)
{
  if (BossBarFitness(task, 1) >= 50)
    return 1;
  if (BossBarFitness(task, 2) >= 50)
    return 2;
  return 0;
}
//从截图中更新波次
void GetWave(int task)
{
  bool isWaveChanged = false;//波次是否改变
  int wave = GetMainWave(task);//获取主波次
  //悬浮梦境：忽略进度条中大于7的波次
  if (work[task].banner == 11)
    if (wave >= 7 && wave <= 23)
      wave = -1;

  //主波次不为-1：直接更新主波次
  if (wave != -1 && wave != waveNoImage)
  {
    //如果波次在0~maxWave-1之间，读取小波
    if (wave >= 0 && wave < maxWave)
    {
      int progress = GetProgress(task);//读取鼠头进度
      //如果大波更新，则小波清零
      if (wave != work[task].wave)
      {
        work[task].smallWave = 0;
        if (progress != -1) //进度条有效则更新进度条
          work[task].progress = progress;
        else
          work[task].progress = 0;
        isWaveChanged = true;
      }
      //如果大波不变，若进度条有效，则更新进度位置及小波
      else if (progress != -1)
      {
        //若原进度为0，说明这是第一次读进度，只记录进度，不改小波
        if (work[task].progress != 0) //若原进度不是0，根据进度变化更新小波
        {
          //鼠头左移或显著右移：小波+1，但最多不超过11
          if (progress < work[task].progress || progress - work[task].progress > 80)
          {
            if (work[task].smallWave < maxSmallWave - 1)
            {
              work[task].smallWave++;
              isWaveChanged = true;
            }
          }
          //鼠头不动或轻微右移：如果是0.0波且已经出怪，则设为0.1波
          else if (work[task].isRatAppeared && work[task].wave == 0 && work[task].smallWave == 0)
          {
            work[task].smallWave = 1;
            isWaveChanged = true;
          }
        }
        work[task].progress = progress;//记录鼠头位置
      }
    }
    else //波次不在0~ maxWave - 1之间，小波和鼠头位置均设为0
    {
      work[task].smallWave = 0;
      work[task].progress = 0;
    }
    work[task].wave = wave;//更新大波
    work[task].isBossExist = false;//检测到非-1的波次，说明没有boss
  }
  else if (wave == -1) //-1波：看看有没有BOSS
  {
    const COLORREF bossHealthColor[2] = { 0x7f1443, 0x642344 };//单/双boss有血量的颜色
    const COLORREF bossEmptyColor[2] = { 0x0c324f, 0x052d4a };//单/双boss血条空的颜色
    const POINT bossPoint[2] = { { 370, 580 }, { 370, 558 } };//判断两个boss是否有血的位置
    //继续作战状态肯定没有BOSS了，无需特殊处理
    int bossNum = GetBossNum(task);//boss数量
    int bossState = 0;//有血为1，无血为0，不能确定为-1
    if (bossNum == 0)
      bossState = 0;
    else if (bossNum == 1)
    {
      if (work[task].map[bossPoint[0].y][bossPoint[0].x] == bossHealthColor[0])
        bossState = 1;
      else if (work[task].map[bossPoint[0].y][bossPoint[0].x] == bossEmptyColor[0])
        bossState = 0;
      else
        bossState = -1;
    }
    else if (bossNum == 2)
    {
      if (work[task].map[bossPoint[0].y][bossPoint[0].x] == bossHealthColor[1]
        || work[task].map[bossPoint[1].y][bossPoint[1].x] == bossHealthColor[1])
        bossState = 1;
      else if (work[task].map[bossPoint[0].y][bossPoint[0].x] == bossEmptyColor[1]
        && work[task].map[bossPoint[1].y][bossPoint[1].x] == bossEmptyColor[1])
        bossState = 0;
      else
        bossState = -1;
    }
    if (bossState == 1) //boss有血，则将进度归零，这样boss血条消失后不会加小波
      work[task].progress = 0;
    //只有确定boss有血或无血才进行更新操作
    if (bossState != -1)
    {
      //如果boss原本没有出现，现在出现了，小波+1
      if (!work[task].isBossExist && bossState == 1)
      {
        work[task].smallWave++;
        isWaveChanged = true;
      }
      //如果boss原本存在，现在消失了（或血条空了），大波+1，小波清零
      else if (work[task].isBossExist && bossState == 0)
      {
        if (work[task].wave < maxWave - 1) //只有大波小于 maxWave - 1才能加
        {
          work[task].wave++;
          work[task].smallWave = 0;
          isWaveChanged = true;
        }
      }
      work[task].isBossExist = bossState == 1;//记录boss存在情况
    }
  }
  if (isWaveChanged)
    ReturnState(task, "运行中");
}
//新冷却读取方式，从函数取点
void GetCold(int task, int account)
{
  getColdNum++;
  GetSlotNum(task, account); //先获取卡槽
  DWORD getColdTick = GetTickCount();

  //卡片cd归零，铲子和宝石cd设为1
  memset(work[task].isCold[account], 0, sizeof(work[task].isCold[account]));
  work[task].isCold[account][0] = 1;
  work[task].isCold[account][maxSlotNum + 1] = 1;

  //从Map判断每一张卡的冷却状态
  for (int order = 1; order <= work[task].realSlotNum[account]; order++)
  {
    COLORREF color = work[task].map[work[task].cdPoint[account][order].y][work[task].cdPoint[account][order].x];
    if (bgrRValue(color) >= 240 && bgrGValue(color) >= 240 && bgrBValue(color) >= 240)
    {
      work[task].isCold[account][order] = true;
      if (work[task].slot[account][order].avaliableTick == 0) //如果还没记录卡槽亮起时刻，记录之
        work[task].slot[account][order].avaliableTick = getColdTick;
    }
    else //如果卡槽亮起过，即使这次检查是熄灭的，也不取消亮起时刻，因为这种熄灭不是CD没到导致的，不会引起吞承载卡
      work[task].isCold[account][order] = false;
  }
}
const COLORREF levelColor[7] = { RGB(255, 255, 0), RGB(255, 128, 0), RGB(0, 255, 0), RGB(255, 255, 255),
RGB(255, 0, 255), RGB(0, 224, 255), RGB(0, 224, 255) };//人物，承载，护罩，普通，升级，冷却，特殊
const int levelHeight[5] = { 60, 48, 32, 16, 0 };
//清空监视器
void ClearMonitor(int task, int account)
{
  monitor[task].ClearWindow();//清空窗口
}
//输出关卡信息到监视器
void PrintLevelInfo(int task, int account)
{
  monitor[task].BeginBatchDraw();//开始批量绘图
  monitor[task].ClearWindow();//清空窗口
  bool isPrintRequired = true;//是否需要绘制
  if (!IsGameWindowVisible(work[task].hWnd[account])) //窗口不显示不绘制
    isPrintRequired = false;
  if (!(work[task].wave >= 0 && work[task].wave < maxWave)) //不在关卡内不绘制
    isPrintRequired = false;
  if (!isPrintRequired)
  {
    monitor[task].EndBatchDraw();//结束批量绘图
    return;
  }

  //各格各层级识别到的卡片
  monitor[task].SetBackgroundColor(RGB(16, 16, 16));
  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 9; column++)
    {
      for (int level = 1; level <= 4; level++)
      {
        monitor[task].settextcolor(levelColor[level]);
        int cardAccount = work[task].grid[level][row][column] / 100;
        int order = work[task].grid[level][row][column] % 100;
        if (order > 0)
          monitor[task].outtextxy(work[task].slot[cardAccount][order].name, objectX + (column - 1) * gridWidth,
            objectY + (row - 1) * gridHeight + levelHeight[level]);
      }
    }

  //识别到的图像
  monitor[task].settextcolor(RGB(255, 0, 0));
  monitor[task].SetBackgroundColor(WHITE);
  int itemNum[8][10] = {};//各格已有物件数量
  for (int order = 0; order < work[task].targetNum; order++)
  {
    Target &target = work[task].target[order];
    if (target.num > 0)
    {
      //相似度最大的位置
      int row = target.location[0] / 10;
      int column = target.location[0] % 10;
      if (itemNum[row][column] < 4)
      {
        monitor[task].outtextxy(target.name, objectX + (column - 1) * gridWidth,
          objectY + (row - 1) * gridHeight + levelHeight[4 - itemNum[row][column]]);
        itemNum[row][column]++;
      }
    }
  }
  monitor[task].EndBatchDraw();//结束批量绘图
}
//载入麦芽糖覆盖区域
void LoadMaltoseCoverage(char *path)
{
  FILE *f;
  fopen_s(&f, path, "r");
  for (int y = 0; y < gridHeight; y++)
    for (int x = 0; x < gridWidth; x++)
      if (fgetc(f) == '0')
        isCoveredByMaltose[y][x] = false;
  fclose(f);
}
//载入麦芽糖覆盖区域
void LoadMaltoseCoverage()
{
  memset(isCoveredByMaltose, true, sizeof(isCoveredByMaltose));
  LoadMaltoseCoverage("附加程序\\覆盖区域\\麦芽糖.txt");
  LoadMaltoseCoverage("附加程序\\覆盖区域\\麦芽糖一转.txt");
  LoadMaltoseCoverage("附加程序\\覆盖区域\\棉花糖.txt");
  LoadMaltoseCoverage("附加程序\\覆盖区域\\棉花糖一转.txt");
  LoadMaltoseCoverage("附加程序\\覆盖区域\\棉花糖二转.txt");
}
//从各自账号截图中读取信息（CD和火苗）
void ReadExclusiveInfo(int task, int account)
{
  GetCold(task, account);//读取冷却
  work[task].energy[account] = GetEnergyDigit(task, 0, 0, 0, account);//读取火苗
}
//从第一个放卡账号截图中获取共用信息
void ReadCommonInfo(int task)
{
  GetWave(task);//读取波次
  if (!(work[task].wave >= 0 && work[task].wave < maxWave))
    return;

  //第4秒记录初始前线（宙斯变身已经结束，迷雾已驱散）
  if (!work[task].isFrontCatched && work[task].currentTime >= 4)
  {
    for (int y = 0; y < 8 * frontHeight; y++)
      work[task].innateFront[y] = work[task].map[frontY + y][frontX];
    work[task].isFrontCatched = true;
  }
  //第5秒以后更新出怪行
  if (work[task].isFrontCatched && work[task].currentTime >= 5)
    FindRatFromMap(task);

  memset(work[task].grid, 0, sizeof(work[task].grid));//格子占用清零

  if (repairLevel == 10)
    return;

  //第3分钟需要重铺棉麦
  if (reserve[task].isMaltoseClearRequired && work[task].currentTime >= 180)
  {
    reserve[task].isMaltoseClearRequired = false;
    memset(reserve[task].maltoseTimes, 0, sizeof(reserve[task].maltoseTimes));
  }

  //识别卡片和图像的相似度
  for (int account = 0; account < 2; account++)
    if (work[task].isPerformed[account] && !work[task].isQuitted[account])
    {
      //获取每张卡的相似度
      for (int order = 1; order <= maxSlotNum; order++)
        if ((work[task].slot[account][order].isSimilarityRequired || monitor[task].isExist)
          && work[task].isTemplateCatched[account][order])
          GetSimilarity(task, account, order);//识别这张卡在各个格子的相似度
    }
  //获取每个目标的相似度
  for (int targetCode = 0; targetCode < work[task].targetNum; targetCode++)
    GetSimilarityTarget(task, targetCode);//识别目标targetCode在各个格子的相似度，记录globalMaxSimilarity
}
//wave是否为关卡外的波次
bool IsOutOfLevel(int wave)
{
  return wave == waveOutOfLevel || wave == waveTower || wave == wavePet || wave == wavePeak;
}
//wave是否为关卡结束后的波次
bool IsEndWave(int wave)
{
  return wave == waveGrade || wave == waveFlip || wave == waveLoot || IsOutOfLevel(wave);
}
//账号account关卡是否已经结束
bool IsAccountLevelEnded(int task, int account)
{
  //主动退出或波次结束，视为关卡已结束
  return work[task].isQuitted[account] || work[task].isWaveEnded[account];
}
//所有账号是否已经关卡结束
bool IsLevelEnded(int task)
{
  for (int account = 0; account < 2; account++)
    if (work[task].isInvolved[account] && !IsAccountLevelEnded(task, account))
      return false;
  return true;
}
//记录波次已经结束
void SetWaveEnded(int task, int account)
{
  work[task].isWaveEnded[account] = true;
  if (!work[task].oneAccountEndTick)
    work[task].oneAccountEndTick = GetTickCount();
}
//读取两个账号的关卡信息
void ReadLevelInfo(int task, bool *result = nullptr)
{
  for (int account = 0; account < 2; account++)
    if (work[task].isInvolved[account] && !work[task].isQuitted[account])
    {
      //如果是仅参与账号且不显示
      if (!work[task].isPerformed[account] && !IsGameWindowVisible(work[task].hWnd[account]))
      {
        //如果另一个账号已结束，则本账号视为结束
        if (work[task].isWaveEnded[1 - account])
          SetWaveEnded(task, account);
        continue;//无需进行截图
      }
      if (!UpdateMap(task, account)) //截图账号account，截图失败需记录，不再继续
        continue;
      //至此已成功获取图像
      int wave = GetMainWave(task);//读取主波次
      if (IsEndWave(wave)) //判断波次是否已结束
        SetWaveEnded(task, account);
      if (wave == waveLoot) //如果某个号已经达到了战利品波次，截图战利品
        if (!work[task].isLootSaved[account])
          SaveLoot(task, account);
      //放卡账号继续读取信息
      if (work[task].isPerformed[account])
      {
        if (result) //记录截图成功
          result[account] = true;
        if (account == GetFirstPerformedAccount(task)) //从1PP截图中读取波次、出怪、卡片、图像
          ReadCommonInfo(task);
        if (!work[task].isWaveEnded[account])
          ReadExclusiveInfo(task, account);//读取本账号专属信息（火苗CD）
      }
    }
  //输出显示信息
  if (monitor[task].isExist)
    PrintLevelInfo(task, GetFirstPerformedAccount(task));
}
//智能识别空位，在所有空位尝试种植卡片
void SmartMovePlant(int task, int account, int order, Hunter *pHunter)
{
  Slot &slot = work[task].slot[account][order];
  int level = slot.level;//卡片层级

  int maxTimes = 0;//放卡次数
  int row[maxPlantTimes + 1] = {};//放卡位置序列
  int column[maxPlantTimes + 1] = {};
  bool plantable[8][10] = {};//row行column列是否可以放卡

  //有索敌时，只有索敌位点能放卡
  if (pHunter && pHunter->size > 0)
    for (int times = 0; times < pHunter->size; times++)
      plantable[pHunter->row[times]][pHunter->column[times]] = true;
  else //无索敌时，全屏位点都能放卡
    memset(plantable, true, sizeof(plantable));

  //有阵型时，放卡序列即为阵型序列
  if (slot.maxTimes > 0)
  {
    maxTimes = slot.maxTimes;
    for (int times = 1; times <= maxTimes; times++)
    {
      row[times] = slot.row[times];
      column[times] = slot.column[times];
    }
  }
  //无阵型时，放卡序列为索敌相似度序列
  else if (pHunter && pHunter->size > 0)
  {
    for (int times = 0; times < pHunter->size; times++)
      if (pHunter->row[times] > 0 && pHunter->column[times] > 0)
      {
        maxTimes++;
        row[maxTimes] = pHunter->row[times];
        column[maxTimes] = pHunter->column[times];
      }
  }

  if (level == 6) //幻鸡按被复制卡层级来
    level = work[task].slot[account][work[task].magicCore[account]].level;
  if (order == zero) //铲子
  {
    for (int times = 1; times <= maxTimes; times++)//依次单点每个要放置的格子
      if (plantable[row[times]][column[times]])
      {
        PostMessage(work[task].hWnd[account], WM_KEYDOWN, 97, 0);//按下键盘1
        Lay(task, account, row[times], column[times]);
        Sleep(1);
      }
    Lay(task, account, 3, -1);//释放卡片，防止影响下次种植
  }
  else if (level == 5)//5层级卡：不截取模板，直接暴力补阵
  {
    bool isSlotPicked = false;
    for (int times = 1; times <= maxTimes; times++)//本次补卡还是遍历
      if (plantable[row[times]][column[times]])
      {
        TryPickSlot(task, account, order, isSlotPicked);
        Lay(task, account, row[times], column[times]);
        Sleep(1);
      }
    if (isSlotPicked)
      Lay(task, account, 3, -1);//释放卡片，防止影响下次种植
  }
  else //0-4层级：智能补阵
  {
    //需要截取模板则截取
    bool isSlotPicked = TryCatchTemplate(task, account, order);//卡槽是否已经拾起

    if (level == 0) //0层级：临时填写本卡占用（1-4层级的占用信息已在ReadLevelInfo时填好）
    {
      memset(work[task].grid[0], 0, sizeof(work[task].grid[0]));//重置0层级占用
      GetOccupied(task, account, order, level);
    }

    //有阵型则依次放置
    for (int times = 1; times <= maxTimes; times++)
    {
      int row0 = row[times];
      int column0 = column[times];
      //如果这个格子本层级未占用，则进行放置
      if (plantable[row0][column0] && work[task].grid[level][row0][column0] == 0)
      {
        //如果是木盘子，还要求2,3,4层级也没有占用才能放
        if (strcmp(slot.name, "木盘子") == 0)
          if (work[task].grid[2][row0][column0] || work[task].grid[3][row0][column0] || work[task].grid[4][row0][column0])
            continue;
        //如果是棉麦，进行一次普通放置后退出
        if (slot.isMaltose)
        {
          PlantTimes(task, account, order, 0, row0, column0);
          reserve[task].maltoseTimes[row0 - 1][column0 - 1]++;//棉麦放置次数+1
          return;
        }
        TryPickSlot(task, account, order, isSlotPicked);//若卡槽未拾起，则拾起卡槽
        Lay(task, account, row0, column0);
        Sleep(1);
      }
    }
    if (isSlotPicked)
      Lay(task, account, 3, -1);//若卡槽已拾起，释放卡片，防止影响下次种植
  }
}
//清除出怪行第order项
void DeleteRatRow(int task, int order)
{
  for (int i = order + 1; i < work[task].ratRowNum; i++)
    work[task].ratRow[i - 1] = work[task].ratRow[i];
  work[task].ratRow[work[task].ratRowNum - 1] = 0;
  work[task].ratRowNum--;
}
//第row行是否已经放过卡
int IsRowPlanted(int task, int account, int order, int row)
{
  for (int times = 1; times <= work[task].slot[account][order].times; times++)//检查放过的每一次卡
    if (work[task].slot[account][order].row[times] == row)//如果有一次放置行数为row
      return 1;
  return 0;
}
//检查本次放卡在队列中的位置
int GetQueueLocation(int task, int account, int order)
{
  Slot &slot = work[task].slot[account][order];
  int count = 0;//该卡在队列中出现的次数
  //遍历优先队列
  for (int i = 0; i < work[task].queueSize[account]; i++)
    if (strcmp(work[task].queue[account][i].name, slot.name) == 0)
    {
      count++;
      if (count == slot.times + 1)
        return i;
    }
  return work[task].queueSize[account];
}
//种植order号卡片。(row,column)为指定行列，默认按阵型来
bool Plant(int task, int account, int order, Hunter *pHunter = nullptr)
{
  Slot &slot = work[task].slot[account][order];
  int queueLoc = GetQueueLocation(task, account, order);//本次放卡在队列中的位置
  if (queueLoc < work[task].queueSize[account]) //如果在队列中，记录已放置
    work[task].queue[account][queueLoc].planted = true;

  if (slot.search && slot.times < min(7, slot.maxTimes))//索敌卡
  {
    for (int i = 0; i < work[task].ratRowNum; i++)//检查出怪行顺序
    {
      if (IsRowPlanted(task, account, order, work[task].ratRow[i]) == zero)//如果这一行还没放过，先放这一行（进行位置插入或改行号）
      {
        //检查后续有没有种在这一行的，有，则执行插入；没有，则直接替换
        for (int times = slot.times + 1; times <= 7; times++)//若只有5次种植，则row[7]==0;
        {
          //如果第times次种在这一行，开始插入
          if (slot.row[times] == work[task].ratRow[i])
          {
            int tempRow = slot.row[times];
            int tempColumn = slot.column[times];
            for (int j = times - 1; j >= slot.times + 1; j--)
            {
              slot.row[j + 1] = slot.row[j];
              slot.column[j + 1] = slot.column[j];
            }
            slot.row[slot.times + 1] = tempRow;
            slot.column[slot.times + 1] = tempColumn;
            break;//插入后跳出循环
          }
          //如果没有种在这一行的，直接改行号
          if (times == 7)
            slot.row[slot.times + 1] = work[task].ratRow[i];
        }
        DeleteRatRow(task, i);//清除出怪行
        break;//改完位置跳出循环
      }
      //如果出怪行都放过了，就不做更改
    }
  }
  slot.times++;//记录已放置次数+1

  if (order == maxSlotNum + 1)//使用宝石
    UseAllSkill(task, account);
  else //使用铲子或放置卡片
  {
    //如果卡槽数不为0，order超过卡槽数，则不放置
    if (work[task].realSlotNum[account] > 0 && order > work[task].realSlotNum[account])
      return false;
    if (slot.repair)//需要补阵
      SmartMovePlant(task, account, order, pHunter);
    else //不需要补阵
    {
      if (pHunter && pHunter->size > 0)
        PlantTimes(task, account, order, slot.times, pHunter->row[0], pHunter->column[0]);
      else
        PlantTimes(task, account, order, slot.times);
    }
  }
  return true;
}
//关闭文档f，写入错误信息到info，返回出错的行数line。hMutex：需要释放的互斥锁
int WriteInfo(char(&info)[1000], FILE *f, const char *path, int line, char *content, const char *tip,
  HANDLE hMutex)
{
  if (f)
    fclose(f);
  if (hMutex)
    ReleaseMutex(hMutexTrack);
  if (info == NULL)
    return line;
  const int maxLength = 20;
  char shortPath[50], shortContent[50];
  int length = strlen(path);
  if (length > maxLength)
  {
    if (IsChineseSecond(path, length - maxLength))
      sprintf_s(shortPath, "...%s", path + length - maxLength + 1);//只显示最后39字节
    else
      sprintf_s(shortPath, "...%s", path + length - maxLength);//显示最后40字节
  }
  else
    strcpy_s(shortPath, path);//完整显示
  length = strlen(content);
  if (length > maxLength)
  {
    if (IsChineseFirst(content, maxLength - 1))
    {
      strncpy_s(shortContent, content, maxLength - 1);
      shortContent[maxLength - 1] = 0;
    }
    else
    {
      strncpy_s(shortContent, content, maxLength);
      shortContent[maxLength] = 0;
    }
    strcat_s(shortContent, "...");
  }
  else
    strcpy_s(shortContent, content);

  size_t len = strlen(shortContent);
  for (size_t i = 0; i < len; i++) //tab改空格
    if (shortContent[i] == '\t')
      shortContent[i] = ' ';

  if (tip == NULL)
    sprintf_s(info, "文件【%s】\n第%d行出错：%s\n请修改或删除该文件后重试。", shortPath, line, shortContent);
  else
    sprintf_s(info, "文件【%s】\n第%d行出错：%s\n（%s）\n请修改或删除该文件后重试。", shortPath, line, shortContent, tip);
  return line;
}
//从带指令的轨道路径extendedPath中提取加速和ban卡信息，返回剩下的轨道路径
//ban卡信息写入Work[task].isSlotBanned[account]，加速信息写入Work[task].accelerationTime[account]
bool ExtractTrack(int task, int account, const char *extendedTrack, char(&pureTrack)[maxPath])
{
  work[task].accelerationTime[account] = 0;
  const char *bannedPath = extendedTrack;//仅含ban卡的轨道文件路径
  const char *bar = strchr(extendedTrack, '|');//加速指令分隔符
  if (bar) //如果存在加速指令分隔符，使用该轨道的加速时间
  {
    work[task].accelerationTime[account] = atoi(extendedTrack);
    work[task].isAccelerationRequired = true;
    bannedPath = bar + 1;
  }
  else //否则使用全局加速时间
  {
    work[task].accelerationTime[account] = globalAccelarationTime;
    if (globalAccelarationTime > 0)
      work[task].isAccelerationRequired = true;
  }
  const char *path = bannedPath;

  work[task].restTime = 0;//静置时间预设为0
  memset(work[task].originalSlot[account], 0, sizeof(work[task].originalSlot[account]));//重置原卡槽信息
  work[task].isBanEnabled[account] = bannedPath[0] == '<';
  if (work[task].isBanEnabled[account])//如果存在ban卡列表，记录ban卡列表和真正的轨道路径
  {
    path = strchr(bannedPath, '>') + 1;//获取去掉ban卡列表后的轨道路径
    char banList[maxPath];//ban卡列表，如"2,3,4"
    //获取banList
    strcpy_s(banList, bannedPath + 1);
    strchr(banList, '>')[0] = 0;
    char *context = nullptr, *info = strtok_s(banList, ",", &context);
    while (info)
    {
      char tempInfo[100] = {};
      //如果info不以数字开头，插入数字0
      sprintf_s(tempInfo, "%s%s", (info[0] >= '0' && info[0] <= '9') || info[0] == '-' ? "" : "0", info);
      int originalOrder = atoi(tempInfo);//获取被ban的卡序号
      if (originalOrder < 0) //负数表示静置时间
        work[task].restTime = -originalOrder;
      else if (originalOrder >= 0 && originalOrder <= maxSlotNum) //记录被ban的卡
      {
        SlotInfo &originalSlot = work[task].originalSlot[account][originalOrder];
        originalSlot.banned = true; //记录卡片被ban
        //记录替代卡的名称
        int tempOrder = 0;
        char tempAlternate[100] = {};
        sscanf_s(tempInfo, "%d%s", &tempOrder, tempAlternate, sizeof(tempAlternate));
        if (strlen(tempAlternate) > 6)
          return false;
        strcpy_s(originalSlot.alternate, tempAlternate);
      }
      else
        return false;
      info = strtok_s(nullptr, ",", &context);
    }
  }
  strcpy_s(pureTrack, path);
  return true;
}
//检查放卡队列
void CheckNormalPlant(int task, int account)
{
  DWORD plantBeginTick = GetTickCount();//放卡开始时刻
  DWORD minTick = 4294967295;
  int orderSelected = -1;
  for (int order = 0; order <= maxSlotNum + 1; order++)
  {
    if (order > work[task].slotNum[account] && order <= maxSlotNum) //不存在的卡槽无需比较
      continue;
    int nextTimes = work[task].slot[account][order].times + 1;//下次放卡的序数
    int nextMoment = work[task].slot[account][order].moment[nextTimes];//下次放卡的轨道时刻（单位0.1秒）
    if (nextMoment == 0) //已经放完的卡无需比较
      continue;
    DWORD nextTick = work[task].slot[account][order].startTick + nextMoment * 100;//下次放卡的系统时刻
    if (work[task].slot[account][order].independent == 0) //同步卡还要加上累计放卡耗时
      nextTick += work[task].slot[account][order].plantDuring;
    if (nextTick < plantBeginTick && nextTick < minTick) //筛选时刻值最小的
    {
      minTick = nextTick;
      orderSelected = order;
    }
  }
  if (orderSelected == -1) //如果没有能放的卡，退出函数
    return;

  /*运行到这里，说明有需要放的卡，执行放置*/

  //检查卡槽是否亮起，没亮起则稍等
  Slot &slot = work[task].slot[account][orderSelected];
  //只针对开启独立计时的普通卡槽（不针对铲子0和宝石22）
  if (slot.independent && orderSelected > 0 && orderSelected <= work[task].slotNum[account])
    if (IsGameWindowVisible(work[task].hWnd[account])) //如果窗口可见
      if (!work[task].isCold[account][orderSelected]) //卡片还没冷却好
      {
        if (slot.oldPlantTick == 0) //如果还没被搁置过
          slot.oldPlantTick = minTick;//记录原始应放时刻
        if (plantBeginTick - slot.oldPlantTick < 1000) //累计等待时间还没超过1秒，则搁置本轮放卡（其他卡不受影响）
        {
          slot.moment[slot.times + 1] = (plantBeginTick - slot.startTick) / 100;//将下次放卡时刻设为此刻
          return;
        }
      }
  //运行到这里，就是没有被搁置，准备放卡了。卡片重置为“没有搁置过”
  slot.oldPlantTick = 0;

  Plant(task, account, orderSelected);//种植卡片
  DWORD plantEndTick = GetTickCount();//放卡结束时刻
  work[task].slot[account][orderSelected].lastTick = plantEndTick;//记录最近一次放卡时刻
  DWORD plantDuring = plantEndTick - plantBeginTick + plantDelay;//本次放卡耗时

  //如果放的是张同步卡，把所有卡片的PlantDuring增加PlantDuringTime
  if (work[task].slot[account][orderSelected].independent == 0)
  {
    for (int account = 0; account < 2; account++)
      if (work[task].isPerformed[account] == 1)
        for (int order = 0; order <= maxSlotNum + 1; order++)
        {
          if (order > work[task].slotNum[account] && order <= maxSlotNum) //跳过空卡槽
            continue;
          work[task].slot[account][order].plantDuring += plantDuring;
        }
  }
}
//条件是否到达
bool IsConditionMet(int task, int account, int order)
{
  Slot &slot = work[task].slot[account][order];
  //能量条件
  if (slot.energy > 0)
  {
    if (work[task].energy[account] < slot.energy)
      return false;
  }
  //波次条件
  if (slot.wave[0])
  {
    int mainWave = 0, smallWave = 0;
    sscanf_s(slot.wave, "%d.%d", &mainWave, &smallWave);
    if (work[task].wave * 100 + work[task].smallWave < mainWave * 100 + smallWave)
      return false;
  }
  //卡片数量条件
  if (slot.cardOrder > 0 && slot.cardNum > 0)
  {
    if (work[task].slot[account][slot.cardOrder].num < slot.cardNum)
      return false;
  }
  //图像数量条件
  if (slot.image[0] && slot.imageNum > 0)
  {
    int targetCode = FindTargetz(task, slot.image);
    Target &target = work[task].target[targetCode];
    if (target.num < slot.imageNum)
      return false;
  }
  return true;
}
//获取索敌位置。如果无图像条件或无图像，返回0,0
void GetHunterLocation(int task, int account, int order, Hunter &hunter)
{
  memset(&hunter, 0, sizeof(hunter)); //默认不索敌
  if (order == maxSlotNum + 1) //宝石不索敌
    return;
  Slot &slot = work[task].slot[account][order];
  //未开启索敌不索敌
  if (strlen(slot.hunterRow) == 0 && strlen(slot.hunterColumn) == 0)
    return;
  //图像数量条件（含索敌）
  if (slot.image[0] && slot.imageNum > 0)
  {
    int targetCode = FindTargetz(task, slot.image);
    Target &target = work[task].target[targetCode];
    if (target.num < slot.imageNum) //不满足图像条件不索敌
      return;
    //如果识图成功，进行索敌放置
    hunter.size = target.num;
    for (int i = 0; i < target.num; i++)
    {
      if (slot.hunterRow[0] == 'a') //行索敌
      {
        int row = target.location[i] / 10;//目标所在行
        //计算偏移
        if (slot.hunterRow[1] == '+')
          row += (int)(slot.hunterRow[2] - '0');
        else if (slot.hunterRow[1] == '-')
          row -= (int)(slot.hunterRow[2] - '0');
        if (row >= 1 && row <= 7)//如果计算结果在1-7内，记录索敌行
          hunter.row[i] = row;
        else
          hunter.row[i] = 0;
      }
      else if (slot.hunterRow[0] >= '1' && slot.hunterRow[0] <= '7') //固定行
        hunter.row[i] = (int)(slot.hunterRow[0] - '0');

      if (slot.hunterColumn[0] == 'b') //列索敌
      {
        int column = target.location[i] % 10;//目标所在列
        //计算偏移
        if (slot.hunterColumn[1] == '+')
          column += (int)(slot.hunterColumn[2] - '0');
        else if (slot.hunterColumn[1] == '-')
          column -= (int)(slot.hunterColumn[2] - '0');
        if (column >= 1 && column <= 9)//如果计算结果在1-9内，记录索敌列
          hunter.column[i] = column;
        else
          hunter.column[i] = 0;
      }
      else if (slot.hunterColumn[0] >= '1' && slot.hunterColumn[0] <= '9') //固定列
        hunter.column[i] = (int)(slot.hunterColumn[0] - '0');
    }
  }
}
//检查卡片是否可放置，能则填写时钟
void GetPlantableTick(int task, int account, int order, DWORD checkTick)
{
  Slot &slot = work[task].slot[account][order];

  //如果没有绝对优先，需检查优先队列条件
  if (!slot.prior)
  {
    int queueLoc = GetQueueLocation(task, account, order);//该卡本次放置在队列中的位置
    //该位置之前的卡有一个未放置，则禁止放置本卡
    for (int i = 0; i < queueLoc; i++)
      if (!work[task].queue[account][i].planted)
        return;
  }

  //开始时间条件
  DWORD limitStartTick = slot.startTick + slot.maxMoment * 100;//极限成阵开始时刻
  if (!slot.independent) //同步卡还要加上累计放卡耗时
    limitStartTick += slot.plantDuring;
  if (!slot.limit || checkTick < limitStartTick)//如果还没到达开始时刻
  {
    slot.plantableTick = 0;//不可放置
    return;
  }
  //终止时间条件
  if (slot.endTime > 0)
  {
    DWORD limitEndTick = slot.startTick + slot.endTime * 1000;//极限成阵终止时刻
    if (checkTick > limitEndTick) //如果已经过了终止时刻
    {
      slot.plantableTick = 0;//不可放置
      return;
    }
  }
  //终止数量条件
  if (slot.endNum > 0)
    if (slot.successfulTimes >= slot.endNum)
    {
      slot.plantableTick = 0;//不可放置
      return;
    }

  //维持模式
  if (!slot.triggerMode)
  {
    if (slot.plantableTick) //可放置时刻已存在，直接返回
      return;
    if (IsConditionMet(task, account, order))
      slot.plantableTick = checkTick + slot.delay * 1000;//满足条件，则设置“可放置时刻”
    else
      slot.plantableTick = 0;//否则设置为“不可放置”
    return;
  }

  //触发模式
  if (slot.plantableTick) //可放置时刻已存在，直接返回
    return;
  if (slot.triggerTick) //如果已经触发，可放置时刻为触发时刻
  {
    slot.plantableTick = slot.triggerTick;
    return;
  }
  //如果还未触发，条件满足时触发
  if (IsConditionMet(task, account, order))
  {
    //满足条件，则设置“触发时刻”和“可放置时刻”
    slot.triggerTick = checkTick + slot.delay * 1000;
    slot.plantableTick = checkTick + slot.delay * 1000;
  }
  else
    slot.plantableTick = 0;//否则设置为“不可放置”
}
//加入一张跟班卡
void PushLackey(int task, int account, int order)
{
  for (int i = 0; i < work[task].lackeyNum[account]; i++)
    if (work[task].lackey[account][i] == order)
      return;
  work[task].lackey[account][work[task].lackeyNum[account]++] = order;
}
//移除一张跟班卡
void PopLackey(int task, int account)
{
  work[task].lackeyNum[account]--;
}
//卡槽slot是否开了完整索敌（存在无阵放卡的可能）
bool IsHunterEnabled(Slot &slot)
{
  return strlen(slot.hunterRow) > 0 && strlen(slot.hunterColumn) > 0;
}
//主动检查order号卡是否可放置，可以则放置
bool CheckLimitPlant(int task, int account, int order, DWORD checkTick)
{
  GetPlantableTick(task, account, order, checkTick);//检查order号卡是否可放置，获取plantableTick
  Slot &slot = work[task].slot[account][order];
  if (slot.plantableTick && checkTick >= slot.plantableTick) //如果已经到达“可放置时刻”
  {
    slot.plantableTick = 0;//无论能否放置都删除“可放置时刻”
    DWORD plantGap = checkTick - slot.lastTick;//距离上次种植经过的时间
    //最小放卡间隔（3秒，刚刚放了幻鸡则是5秒）
    DWORD minPlantGap = 3000;//最小放置间隔
    //铲子没开补阵且没开索敌条件时最小间隔为0（开补阵或索敌条件则依然是3000）
    if (order == 0 && !slot.repair && !IsHunterEnabled(slot))
      minPlantGap = 0;
    else if (order == work[task].magicCore[account] && work[task].isMagicPlantedRecently[account])
      minPlantGap = 5000; //放完幻鸡后被复制卡的最小间隔为5000
    if (plantGap >= minPlantGap) //条件1：已经过了最小放卡间隔
    {
      //如果本卡是幻鸡目标卡，说明刚才已经等够了5000ms，以后不用等了
      if (order == work[task].magicCore[account])
        work[task].isMagicPlantedRecently[account] = 0;
      if (slot.followOrder == 0 || !work[task].isCold[account][slot.followOrder])//条件2：无跟随或被跟随卡已黑
        if (order == maxSlotNum + 1 || work[task].isCold[account][order]) //条件3：宝石或卡片冷却完毕
        {
          //如果是承载卡，距离卡槽亮起时刻超过1秒才能放置
          if (slot.level == 1)
          {
            if (slot.avaliableTick && checkTick - slot.avaliableTick >= 1000)
              slot.avaliableTick = 0;
            else
              return false;
          }
          //如果本卡是幻鸡，查4次相似度
          if (order == work[task].magic[account])
          {
            for (int i = 0; i < 4; i++)
            {
              if (i > 0)
                CheckSleep(task, 100);
              if (repairLevel < 10)
                ReadLevelInfo(task);//临时记录一次相似度
            }
            work[task].lastSimilarityTick = GetTickCount();
            work[task].isMagicPlantedRecently[account] = 1;//记录幻鸡已放置
          }
          Hunter hunter = {};
          GetHunterLocation(task, account, order, hunter);//获得索敌位置
          Plant(task, account, order, &hunter);//种植卡片
          slot.lastTick = GetTickCount();//记录最近一次放卡时刻

          //如果本卡不是铲子，所有跟班卡倒序入栈（序号小的先被处理）
          int lackeyNum = 0;
          if (order > 0)
            for (int lackeyOrder = maxSlotNum + 1; lackeyOrder >= 0; lackeyOrder--)//跟班卡编号
            {
              if (lackeyOrder > work[task].slotNum[account] && lackeyOrder <= maxSlotNum) //跳过无效卡槽
                continue;
              if (lackeyOrder == order)//不准跟随自己
                continue;
              if (work[task].slot[account][lackeyOrder].followOrder == order)
              {
                PushLackey(task, account, lackeyOrder);
                lackeyNum++;
              }
            }
          //被跟随卡和记次数的卡放完等100 ms，下次检查才能判断是否放置成功
          if (lackeyNum > 0 || slot.endNum > 0)
            CheckSleep(task, 100);
          //记次卡要标记刚才已放置，方便下一轮读冷却时统计成功放置次数
          if (slot.endNum > 0)
            slot.planted = true;
          return true;
        }
    }
  }
  return false;
}
//检查极限卡，主动放置至多1张符合要求的卡
bool CheckLimitPlant(int task, int account)
{
  DWORD checkTick = GetTickCount();//检查时刻

  //检查计次卡
  for (int order = 0; order <= maxSlotNum + 1; order++)
  {
    Slot &slot = work[task].slot[account][order];
    if (order > work[task].slotNum[account] && order <= maxSlotNum) //跳过无效卡槽
      continue;
    //如果这张卡是计次卡，且刚才已放置
    if (slot.endNum > 0 && slot.planted)
    {
      //如果现在是未冷却的，则记录成功放置次数+1
      if (!work[task].isCold[account][order])
        slot.successfulTimes++;
      slot.planted = false;
    }
  }

  //判断跟随栈里的每张卡可否放置
  for (int i = work[task].lackeyNum[account] - 1; i >= 0; i--)
  {
    PopLackey(task, account);//出栈
    if (CheckLimitPlant(task, account, work[task].lackey[account][i], checkTick))//检查该卡能否放置，能则放置并退出
      return true;
  }

  //判断每张非跟班卡是否可放置
  for (int order = 0; order <= maxSlotNum + 1; order++)
  {
    Slot &slot = work[task].slot[account][order];
    if (order > work[task].slotNum[account] && order <= maxSlotNum) //跳过无效卡槽
      continue;
    //没开补阵、没开索敌条件，且次数放满，则跳过
    if (!slot.repair && !IsHunterEnabled(slot) && slot.times >= slot.maxTimes)
      continue;
    if (slot.followOrder > 0 || !slot.limit) //跳过开启了跟随的卡和非极限卡
      continue;
    if (CheckLimitPlant(task, account, order, checkTick))//检查该卡能否放置，能则放置并退出
      return true;
  }
  return false;
}
//从轨道文件f读取一张卡，返回出错的行数，成功读取返回0（path为原路径）
template<size_t size>
int LoadSingleCard(Slot &slot, FILE *f, char(&s)[size], char(&seg)[size],
  const char *path, int &line, char(&info)[1000], int totalWave, int totalWaveTick)
{
  strcpy_s(seg, s);//将s复制到seg，用于分割

  if (seg[0] == '\t')//没有卡片名称
    return WriteInfo(info, f, path, line, s, "卡片名称不能为空", nullptr);

  //子串1：卡片名称
  char *nextName = nullptr;
  char *name = strtok_s(seg, "\t", &nextName);
  if (strlen(name) > 6)
    return WriteInfo(info, f, path, line, s, "卡片名称不能超过3个汉字", nullptr);
  strcpy_s(slot.name, name);

  //是否棉麦
  slot.isMaltose = strcmp(name, "棉花糖") == 0 || strcmp(name, "麦芽糖") == 0;

  //子串2：冷却及锁定状态（7s锁补极）
  name = strtok_s(NULL, "\t", &nextName);
  if (name == NULL || strstr(name, "|"))
    return WriteInfo(info, f, path, line, s, "第二项应为冷却时间和轨道状态", nullptr);

  //从第0.1波开始，继承卡无需修改信息（放一次卡视为继承）
  if (totalWave > 0 && (strstr(name, "继") || slot.once))
    return 0;

  //未开启继承：重置卡槽信息和计时器，读取新波次轨道
  ResetSlot(slot);//重置卡槽信息
  slot.startTick = totalWaveTick;//将计时器重置为本波开始时刻
  slot.oldPlantTick = 0;

  //读取新波次轨道
  char *nextCondition = nullptr;
  char *condition = NewStrTok(name, ",", &nextCondition);//第一项：7[3]补极
  slot.level = 0;//卡片层级默认为0（表示不占格子）
  sscanf_s(condition, "%d[%d]", &slot.cd, &slot.level);//读取卡片层级

  //单次放置卡：第0波填写单次放置轨道
  if (totalWave == 0 && slot.once)
  {
    slot.level = 0;//卡片层级默认为0（表示不占格子）
    slot.limit = true;//极
    slot.repair = true;//补
    slot.endNum = 1;//1次
    //填写63个放卡位置
    slot.maxTimes = 0;
    for (int column = 9; column >= 1; column--)
      for (int row = 1; row <= 7; row++)
      {
        slot.maxTimes++;
        slot.row[slot.maxTimes] = row;
        slot.column[slot.maxTimes] = column;
        slot.isPlantedInMap[row][column] = 1;//记录该行该列种植了该卡
      }
    return 0;
  }

  slot.search = strstr(condition, "索") != nullptr;//索敌
  slot.independent = strstr(condition, "独") != nullptr;//独立计时
  slot.repair = strcmp(slot.name, "宝石") == 0 || strstr(condition, "补");//补阵（宝石强制开）
  slot.limit = strstr(condition, "极") != nullptr;//极限成阵
  slot.prior = strstr(condition, "绝") != nullptr;//绝对优先
  slot.retain = strstr(condition, "继") != nullptr;//继承（仅用于默认轨道）


  //第2-n项：极限条件
  memset(&slot.delay, 0, sizeof(ConditionType));
  for (int i = 0; i < 13; i++)
  {
    condition = NewStrTok(nullptr, ",", &nextCondition);
    if (!condition)
      break;
    switch (i)
    {
    case 0://延迟
      slot.delay = atoi(condition);
      if (slot.delay < 0 || slot.delay > 960)
        return WriteInfo(info, f, path, line, s, "【延迟】范围为0~960", nullptr);
      break;
    case 1://达成
      slot.triggerMode = atoi(condition) ? 1 : 0;
      break;
    case 2://火苗
      slot.energy = atoi(condition);
      if (slot.energy < 0 || slot.energy > 15000)
        return WriteInfo(info, f, path, line, s, "【火苗】范围为0~15000", nullptr);
      break;
    case 3://波次：只能是m.n格式
    {
      if (!IsWaveLegal(condition)) //波次输入不合格
        return WriteInfo(info, f, path, line, s, waveErrorString, nullptr);
      int mainWave = 0, smallWave = 0;
      int scannedNum = sscanf_s(condition, "%d.%d", &mainWave, &smallWave);
      if (smallWave > 0)
        sprintf_s(slot.wave, "%d.%d", mainWave, smallWave);
      else if (mainWave > 0)
        sprintf_s(slot.wave, "%d", mainWave);
      else
        sprintf_s(slot.wave, "");
    }
    break;
    case 4://卡片
      if (strcmp(condition, "NULL") == 0)
        strcpy_s(slot.card, "");
      else
      {
        if (strlen(condition) > 6)
          return WriteInfo(info, f, path, line, s, "【卡片】长度不能超过3个汉字", nullptr);
        strcpy_s(slot.card, condition);
      }
      break;
    case 5://卡片数量
      slot.cardNum = atoi(condition);
      if (slot.cardNum < 0 || slot.cardNum > 63)
        return WriteInfo(info, f, path, line, s, "【卡片数量】范围为0~63", nullptr);
      break;
    case 6://图像
      if (strcmp(condition, "NULL") == 0)
        strcpy_s(slot.image, "");
      else
      {
        if (strlen(condition) > 8)
          return WriteInfo(info, f, path, line, s, "【图像】长度不能超过4个汉字", nullptr);
        strcpy_s(slot.image, condition);
      }
      break;
    case 7://图像数量
      slot.imageNum = atoi(condition);
      if (slot.imageNum < 0 || slot.imageNum > 63)
        return WriteInfo(info, f, path, line, s, "【图像数量】范围为0~63", nullptr);
      break;
    case 8://索敌行:a,4,a+4,a-4
      if (strcmp(condition, "NULL") == 0)
        strcpy_s(slot.hunterRow, "");
      else
      {
        const char *tip = "【索敌行数】应为n,a,a±n三种格式之一";
        bool isLegal = true;
        int length = strlen(condition);
        if (length != 0 && length != 1 && length != 3)
          return WriteInfo(info, f, path, line, s, tip, nullptr);
        if (length == 1) //a或一位数
        {
          if (!((condition[0] >= '1' && condition[0] <= '7') || condition[0] == 'a'))
            return WriteInfo(info, f, path, line, s, tip, nullptr);
        }
        else if (length == 3) //a+n
        {
          if (!(condition[0] == 'a' && (condition[1] == '+' || condition[1] == '-') && condition[2] >= '1' && condition[2] <= '6'))
            return WriteInfo(info, f, path, line, s, tip, nullptr);
        }
        strcpy_s(slot.hunterRow, condition);
      }
      break;
    case 9://索敌列
      if (strcmp(condition, "NULL") == 0)
        strcpy_s(slot.hunterColumn, "");
      else
      {
        const char *tip = "【索敌列数】应为n,b,b±n三种格式之一";
        bool isLegal = true;
        int length = strlen(condition);
        if (length != 0 && length != 1 && length != 3)
          return WriteInfo(info, f, path, line, s, tip, nullptr);
        if (length == 1) //b或一位数
        {
          if (!((condition[0] >= '1' && condition[0] <= '9') || condition[0] == 'b'))
            return WriteInfo(info, f, path, line, s, tip, nullptr);
        }
        else if (length == 3) //b+n
        {
          if (!(condition[0] == 'b' && (condition[1] == '+' || condition[1] == '-') && condition[2] >= '1' && condition[2] <= '9'))
            return WriteInfo(info, f, path, line, s, tip, nullptr);
        }
        strcpy_s(slot.hunterColumn, condition);
      }
      break;
    case 10://跟随
      if (strcmp(condition, "NULL") == 0)
        strcpy_s(slot.image, "");
      else
      {
        if (strlen(condition) > 6)
          return WriteInfo(info, f, path, line, s, "【跟随卡片】长度不能超过3个汉字", nullptr);
        strcpy_s(slot.follow, condition);
      }
      break;
    case 11://终止数量
      slot.endNum = atoi(condition);
      if (slot.endNum < 0 || slot.endNum > 63)
        return WriteInfo(info, f, path, line, s, "【终止数量】范围为0~63", nullptr);
      break;
    case 12://终止时间
      slot.endTime = atoi(condition);
      if (slot.endTime < 0 || slot.endTime > 960)
        return WriteInfo(info, f, path, line, s, "【终止时间】范围为0~960", nullptr);
      break;
    }
  }

  slot.maxTimes = 0;//记录阵型中的最大种植次数
  int times = 0;//种植次数先记为0
  int second, decisecond, location;
  name = strtok_s(NULL, "\t", &nextName);//第一个ttt|xy信息
  while (name)//从子串3-n读取种植时刻和种植位置
  {
    if (strlen(name) > 10 || strchr(name, '|') == NULL)//确保MomentString不会爆内存
      return WriteInfo(info, f, path, line, s, "放卡格式应为ttt(.t)|xy，以Tab键分隔", nullptr);
    times++;//读到了ttt.s|xy型轨道，则次数+1

    if (sscanf_s(name, "%d.%d|%d", &second, &decisecond, &location) == 3)//有小数点就按小数读取
    {
      while (decisecond >= 10)
        decisecond = decisecond / 10;//只保留1位小数
    }
    else //没有小数点就按整数读取
    {
      sscanf_s(name, "%d|%d", &second, &location);
      decisecond = 0;
    }
    slot.moment[times] = second * 10 + decisecond;//读取种植时刻
    if ((slot.moment[times] != zero && slot.moment[times] < 10)
      || slot.moment[times] > 9600)
      return WriteInfo(info, f, path, line, s, "放卡时间范围为1~960", nullptr);

    if (slot.moment[times] > slot.maxMoment)//记录最大轨道时间
      slot.maxMoment = slot.moment[times];

    int row = location / 10;
    int column = location % 10;

    if (location != zero)//如果位置不是00
      if (row < 1 || row > 7 || column == zero)
        return WriteInfo(info, f, path, line, s, "行数范围为1~7，列数范围为1~9", nullptr);

    if (location > zero)
    {
      slot.maxTimes++;//记录最大种植次数
      slot.isPlantedInMap[row][column] = 1;//记录该行该列种植了该卡
    }

    slot.row[times] = row;
    slot.column[times] = column;
    name = strtok_s(NULL, "\t", &nextName);
  }
  //读完了，如果开启了极限成阵，取消最后一个非零时间轨道
  if (slot.limit)
  {
    //寻找第一个零时刻
    for (times = 1; times <= maxPlantTimes; times++)
      if (slot.moment[times] == 0)
        break;
    //最后一个非零时刻设为零
    if (times > 1)
      slot.moment[times - 1] = 0;
  }
  return 0;
}
// 检查账号account的轨道文件有哪些波次，填写WaveExist。
// 检查无误返回0，否则返回出错的行数，错误信息写入info
int GetWaveExist(int task, int account, const char *path, char(&info)[1000])
{
  if (!IsTxtFile(path))
  {
    strcpy_s(info, "轨道文件应为txt格式。");
    return -1;
  }
  if (!FileExist(path))
  {
    strcpy_s(info, "轨道文件不存在。");
    return -1;
  }
  FILE *f;
  if (fopen_s(&f, path, "r"))//打开轨道文件
  {
    strcpy_s(info, "轨道文件无法打开。");
    return -2;
  }

  memset(work[task].totalWaveExist[account], 0, sizeof(work[task].totalWaveExist[account]));
  work[task].totalWaveExist[account][0] = 1;//第0波默认存在
  int existedWaveNum = 1;//轨道中的波次数量
  int wave = 0, smallWave = 0;

  int line = 0;
  char s[maxLineSize];
  while (!feof(f))
  {
    fgets(f, s);
    line++;
    if (strlen(s) == 0)
      break;

    int result = ScanWave(s, &wave, &smallWave);//检查是否是--第%d.%d波--
    if (result == -1)
      return WriteInfo(info, f, path, line, s, "波次范围为0~23（小波范围为0~11）", nullptr);
    else if (result == 1)
    {
      existedWaveNum++;
      if (existedWaveNum > 12)
        return WriteInfo(info, f, path, line, s, "最多只允许设置12个波次", nullptr);
      work[task].totalWaveExist[account][wave * maxSmallWave + smallWave] = 1;
    }
  }
  fclose(f);
  return 0;
}
//是否属于三岛漫游
bool IsRoamLevel(int type, int level)
{
  return (type == 6 || type == 7 || type == 9) && level == 0;
}
const char roamName[11][20] = {
  "0未收录", "1施工现场", "2exciting", "3鼠以群聚", "4峡道空袭", "5神殿集会",
  "6百鬼夜行", "7拆迁大队", "8罐头炸弹", "9平民鼠的逆袭", "10车来了" };
//从指定文件中载入默认轨道（skippedSlotNum==1跳过宝石，skippedSlotNum==2跳过宝石和铲子）
int LoadDefalutTrackFrom(const char *path, Slot slot[], int &slotNum, char(&s)[maxLineSize],
  char(&seg)[maxLineSize], char(&info)[1000], int skippedSlotNum)
{
  char tip[100] = {};
  FILE *fTray;
  WaitForSingleObject(hMutexTray, INFINITE);
  if (fopen_s(&fTray, path, "r") == 0)//如果能打开承载文件，载入承载方案
  {
    int line = 0;
    do
    {
      fgets(fTray, s);
      line++;
      if (strlen(s) == 0)
        return WriteInfo(info, fTray, path, line, s, "找不到卡槽数", hMutexTray);
    } while (!strstr(s, "卡槽数"));
    sscanf_s(s, "卡槽数=%d", &slotNum);
    slotNum += 2 - skippedSlotNum;//如果不跳过宝石和铲子，要增加相应的卡槽数
    for (int i = 0; i < skippedSlotNum; i++) //选择性跳过宝石和铲子
    {
      fgets(fTray, s);
      line++;
    }
    for (int order = 0; order < slotNum; order++)
    {
      fgets(fTray, s);//读取一行卡片轨道，如小火炉 7s极补锁 002|12
      line++;

      //如果卡槽还没读完，就遇到了空行或--第x波--，说明卡槽数填多了
      if (strlen(s) == 0 || IsWaveTitle(s))
      {
        sprintf_s(tip, "卡槽数=%d，但只读取到%d个卡槽", slotNum, order);
        return WriteInfo(info, fTray, path, line, s, tip, hMutexTray);
      }
      //读取一张承载卡的轨道信息
      if (LoadSingleCard(slot[order], fTray, s, seg, path, line, info, 0, 0) > 0)
      {
        ReleaseMutex(hMutexTray);
        return line;//读取错误则返回出错的行数（错误信息已经写入info）
      }
      if (slot[order].row[1] > 0 && slot[order].moment[1] == 0) //有阵型无轨道
      {
        if (strcmp(slot[order].name, "铲子") == 0)//铲子：开补并添加第1秒轨道（开局全铲）
        {
          slot[order].repair = true;
          slot[order].moment[1] = 10;
        }
        else //其他卡视为极限成阵
          slot[order].limit = true;
      }
    }
    fclose(fTray);
  }
  ReleaseMutex(hMutexTray);
  return 0;
}
//（仅第0波）载入默认轨道（常用卡和承载卡）。载入成功或无默认轨道返回0，否则返回出错行数
int LoadDefaultTrack(int task, Slot slot[], int &slotNum, char(&s)[maxLineSize],
  char(&seg)[maxLineSize], char(&info)[1000])
{
  int traySlotNum = 0;//承载卡数
  if (IsAdvance(task)) //高级任务载入承载轨道
  {
    int list = GetList(task);
    Advance &currentAdvance = *plot[list].playingAdvance;
    int type = currentAdvance.type;
    int level = currentAdvance.level;
    char levelName[100];
    GetLevelName(levelName, type, level);
    char trayPath[maxPath] = {};//承载卡默认轨道
    if (IsRoamLevel(type, level)) //漫游关卡
      sprintf_s(trayPath, "预制轨道\\默认轨道\\漫游\\%s.txt", roamName[work[task].banner]);
    else if (type == 2) //魔塔
    {
      level %= 1000;//提取层数
      if (level % 5 != 0) //161-164转化为165
        level = level / 5 * 5 + 5;
      sprintf_s(trayPath, "预制轨道\\默认轨道\\%d%s\\%d.txt", type, typeName[type], level);
    }
    else
      sprintf_s(trayPath, "预制轨道\\默认轨道\\%d%s\\%d%s.txt", type, typeName[type], level, levelName);

    int trayLine = LoadDefalutTrackFrom(trayPath, slot, traySlotNum, s, seg, info, 1);
    if (trayLine > 0)
      return trayLine;
  }
  slotNum = traySlotNum;
  return 0;
}
//获得上一张非冰幻卡编号
int GetLastNonColdOrder(int task, int account, int slotOrder)
{
  //向前查找第一个普通卡（不含铲子）
  for (int order = slotOrder - 1; order >= 1; order--)
    if (work[task].slot[account][order].level <= 4)
      return order;
  return -1;//slotName前面没有普通卡，返回-1
}
//slot1和slot2是不是相同的卡（棉麦视为同一张卡）
bool IsSameSlot(Slot &slot1, Slot &slot2)
{
  if (slot1.isMaltose && slot2.isMaltose) //都是棉麦，视为同一张卡
    return true;
  return strcmp(slot1.name, slot2.name) == 0; //否则名称必须严格相同
}
//高级任务list账号account增加一张需要的卡片
void AddUsedSlot(int list, int account, const char *slotName)
{
  //数量已满不记录
  if (plot[list].usedSlotNum[account] >= maxUsedSlotNum)
    return;

  //宝石和铲子不予记录
  if (strcmp(slotName, "宝石") == 0 || strcmp(slotName, "铲子") == 0)
    return;

  //如果该卡片已经记录，不予重复记录
  for (int code = 0; code < plot[list].usedSlotNum[account]; code++)
    if (strcmp(plot[list].usedSlot[account][code], slotName) == 0)
      return;

  //记录该卡片的候选卡信息
  int task = GetList(list);
  GetCandidateFromPublic(slotName, plot[list].candidate[account][plot[list].usedSlotNum[account]]);
  //记录卡片名称
  strcpy_s(plot[list].usedSlot[account][plot[list].usedSlotNum[account]], slotName);
  plot[list].usedSlotNum[account]++;
}
//从文件当前位置读取波次信息，读取slotnum张卡片。读取成功返回0，否则返回出错的行数
int LoadCurrentWave(int task, int account, FILE *f, char(&s)[maxLineSize], char(&seg)[maxLineSize],
  const char *path, int &line, char(&info)[1000], int totalWave)
{
  char tip[100];
  //读取默认承载方案（仅限高级任务第0波，已指定PlayingOrder）
  Slot defaultSlot[20] = {};
  int defaultSlotNum = 0;
  if (totalWave == 0)
  {
    int trayLine = LoadDefaultTrack(task, defaultSlot, defaultSlotNum, s, seg, info);
    if (trayLine > 0)
      return trayLine;
  }

  //读取各卡槽轨道
  work[task].magic[account] = -1;//幻鸡卡槽预设为-1
  work[task].magicCore[account] = -1;//幻鸡目标卡槽预设为-1

  //读取每一行卡片轨道（行数：originalSlotNum+2），跳过被ban的轨道

  //originalLineOrder：ban卡前的行数编号（行号-1对应卡槽22）
  //originalOrder：ban卡前的卡槽编号
  //lineOrder：ban卡后的行数编号（行号-1对应卡槽22）
  //order：ban卡后的卡槽编号

  int order = 0;//实际卡槽编号
  for (int originalLineOrder = -1; originalLineOrder <= work[task].originalSlotNum[account]; originalLineOrder++)
  {
    fgets(f, s);//读取一行卡片轨道，如小火炉 7s极补锁 002|12
    line++;
    int originalOrder = originalLineOrder == -1 ? maxSlotNum + 1 : originalLineOrder;//原卡槽编号

    //如果卡槽还没读完，就遇到了空行或--第x波--，说明卡槽数填多了
    if (strlen(s) == 0 || IsWaveTitle(s))
    {
      sprintf_s(tip, "卡槽数=%d，但只读取到%d个卡槽", work[task].originalSlotNum[account], originalLineOrder - 1);
      return WriteInfo(info, f, path, line, s, tip, nullptr);
    }

    //原卡槽（ban卡前的排序）
    SlotInfo &originalSlot = work[task].originalSlot[account][originalOrder];

    //记录原卡槽信息
    if (strchr(s, '\t'))
    {
      //记录原卡槽的名称、冷却和层级
      sscanf_s(s, "%s\t%d[%d]", originalSlot.name, sizeof(originalSlot.name),
        &originalSlot.cd, &originalSlot.level);
      //高级任务检查阶段：记录该卡片或它的替换卡已使用
      if (IsAdvance(task))
      {
        int list = GetList(task);
        if (plot[list].isCountingSlot)
        {
          if (strlen(originalSlot.alternate) > 0)
            AddUsedSlot(list, account, originalSlot.alternate);
          else
            AddUsedSlot(list, account, originalSlot.name);
        }
      }
    }

    //如果这个卡槽被ban且无替换品，则跳过
    if (originalLineOrder > 0 && originalSlot.banned && strlen(originalSlot.alternate) == 0)
      continue;

    //记录实际卡槽编号
    if (originalLineOrder == -1)
    {
      if (strstr(s, "宝石")) //第-1行如果是宝石，实际编号为MaxSlotNum + 1
        order = maxSlotNum + 1;
      else //否则实际编号为0，并且视为第0行
        order = originalLineOrder = 0;
    }
    else if (originalLineOrder == 0) //铲子实际编号0
      order = 0;
    else //遇到没有跳过的卡，实际卡槽编号+1
      order++;

    //读取卡槽信息
    Slot &slot = work[task].slot[account][order];
    slot.once = false;
    if (work[task].reloading) //读取新波次阶段：无轨替换卡标记为放一次卡
      if (!slot.used && strlen(originalSlot.alternate) > 0)
        slot.once = true;

    if (LoadSingleCard(slot, f, s, seg, path, line, info, totalWave, work[task].totalWaveTick[totalWave]) > 0)
      return line;//返回出错的行数（错误信息已经写入info）

    //如果开启了替换，修改原卡槽卡片名称，用于携带替换卡
    if (strlen(originalSlot.alternate) > 0)
      strcpy_s(originalSlot.name, originalSlot.alternate);

    //如果开启了极限成阵，记录图像信息
    if (slot.limit)
    {
      char tip[100] = {};
      //填写目标图像信息
      if (slot.image[0] && slot.imageNum > 0)
      {
        int itemCode = FindItem(slot.image);
        if (itemCode == -1)
        {
          sprintf_s(tip, "缺少自定图像[%s.png]", slot.image);
          return WriteInfo(info, f, path, line, s, tip, nullptr);
        }
        if (AddTarget(task, account, order) == -2)
        {
          sprintf_s(tip, "最多只能识别%d张图片", maxTargetNum);
          return WriteInfo(info, f, path, line, s, tip, nullptr);
        }
      }
    }

    bool isSorbet = strcmp(slot.name, "冰沙") == 0;//该卡是否为冰沙（如果是，需要将放卡位置统一赋值为7行1列）

    int defalutOrder = -1;//该卡是默认轨道中第几张卡
    for (int n = 0; n < defaultSlotNum; n++)
      if (IsSameSlot(slot, defaultSlot[n]))
      {
        defalutOrder = n;
        break;
      }

    if (defalutOrder >= 0) //如果是默认轨道中的卡（只有第0波能触发）
    {
      //如果开启默认轨道（第0波继承）
      if (slot.retain)
      {
        //如果阵型为空，拷贝默认卡阵型（包括种植范围、补阵、索敌）
        if (slot.row[1] == 0)
        {
          memcpy(slot.isPlantedInMap, defaultSlot[defalutOrder].isPlantedInMap, sizeof(defaultSlot[defalutOrder].isPlantedInMap));
          slot.maxTimes = defaultSlot[defalutOrder].maxTimes;
          slot.repair = defaultSlot[defalutOrder].repair;
          slot.search = defaultSlot[defalutOrder].search;
          for (int i = 1; i <= maxPlantTimes; i++)
          {
            slot.row[i] = defaultSlot[defalutOrder].row[i];
            slot.column[i] = defaultSlot[defalutOrder].column[i];
          }
        }
        //拷贝默认卡轨道（包括独立计时、极限成阵、放卡条件）
        slot.independent = defaultSlot[defalutOrder].independent;
        slot.limit = defaultSlot[defalutOrder].limit;
        memcpy(&slot.delay, &defaultSlot[defalutOrder].delay, sizeof(ConditionType));//拷贝极限条件
        slot.maxMoment = defaultSlot[defalutOrder].maxMoment;
        for (int i = 1; i <= maxPlantTimes; i++)
          slot.moment[i] = defaultSlot[defalutOrder].moment[i];
      }
    }

    if (isSorbet == true) //如果这张卡是冰沙
    {
      slot.repair = true;//开启补阵
      slot.maxTimes = 3;//种植次数为3（用于补阵）
      for (int i = 1; i <= 3; i++)//设置补阵位置
      {
        slot.row[i] = sorbetLoc[i - 1] / 10;
        slot.column[i] = sorbetLoc[i - 1] % 10;
      }
    }

    if (slot.repair && slot.level <= 4) //0-4层级补阵卡需要识别相似度
      slot.isSimilarityRequired = true;

    //检查阶段：开极或有轨的卡片标记为已使用
    if (!work[task].reloading)
      if (slot.limit || slot.moment[1])
      {
        originalSlot.used = true;
        slot.used = true;
      }
  }

  //读完当前波次的轨道再读一行
  fgets(f, s);
  line++;

  //检查绑定的是哪张卡
  for (int lineOrder = -1; lineOrder <= work[task].slotNum[account]; lineOrder++)
  {
    int order = lineOrder == -1 ? maxSlotNum + 1 : lineOrder;
    Slot &slot = work[task].slot[account][order];

    //如果开启了极限成阵，记录卡片信息
    if (slot.limit)
    {
      char tip[100] = {};
      //如果开启了卡片条件，填写条件卡编号
      if (slot.card[0] && slot.cardNum > 0)
      {
        int cardOrder = GetOrder(task, account, slot.card);
        if (cardOrder <= 0)
        {
          sprintf_s(tip, "未找到[%s]的条件卡[%s]", slot.name, slot.card);
          return WriteInfo(info, f, path, line, s, tip, nullptr);
        }
        slot.cardOrder = cardOrder;
        //如果条件卡片属于0-4层级，则条件卡片需要识别相似度
        if (work[task].slot[account][slot.cardOrder].level <= 4)
          work[task].slot[account][slot.cardOrder].isSimilarityRequired = true;
      }
      //如果开启了跟随条件，填写跟随卡编号
      if (slot.follow[0])
      {
        int followOrder = GetOrder(task, account, slot.follow);
        if (followOrder <= 0)
        {
          sprintf_s(tip, "未找到[%s]的跟随卡[%s]", slot.name, slot.follow);
          return WriteInfo(info, f, path, line, s, tip, nullptr);
        }
        slot.followOrder = followOrder;
      }
      //如果slot是幻幻鸡，记录跟随卡
      if (slot.level == 6)
      {
        work[task].magic[account] = order;//记录幻鸡
        if (slot.followOrder == 0)
        {
          int lastOrder = GetLastNonColdOrder(task, account, order);
          if (lastOrder > 0)
            slot.followOrder = lastOrder;
        }
        if (slot.followOrder > 0)
        {
          work[task].magicCore[account] = slot.followOrder;//记录幻鸡跟随卡
          if (slot.row[1] == 0)//如果幻鸡位置为空
            for (int i = 1; i <= maxPlantTimes; i++)//拷贝复制目标的位置
            {
              slot.maxTimes = work[task].slot[account][slot.followOrder].maxTimes;
              slot.row[i] = work[task].slot[account][slot.followOrder].row[i];
              slot.column[i] = work[task].slot[account][slot.followOrder].column[i];
            }
        }
      }
    }
  }

  if (strlen(s) > 0 && !IsWaveTitle(s))//如果下一行非空，且不是第x波，说明还有卡槽内容没读
  {
    sprintf_s(tip, "卡槽数=%d，但读完%d个卡槽仍未结束", work[task].slotNum[account], work[task].slotNum[account]);
    return WriteInfo(info, f, path, line, s, tip, nullptr);
  }

  return 0;
}
//（开局或变阵时）读取轨道第totalWave波信息。读取成功返回0，否则返回出错的行数
int ReloadWave(int task, int account, const char *path, char(&info)[1000], int totalWave)
{
  FILE *f;
  if (fopen_s(&f, path, "r"))
    return -1;
  char s[maxLineSize], seg[maxLineSize];
  int line = 0;
  int slotNum = 0;
  //先跳到卡槽数位置，读取卡槽数
  do
  {
    fgets(f, s);
    line++;
    if (strlen(s) == 0)
      return line;
  } while (sscanf_s(s, "卡槽数=%d", &slotNum) != 1);

  //如果不是第0波，则需要跳到指定波次
  if (totalWave > 0)
  {
    char waveString[10];//波次表达式（如"1.5"）
    GetWaveString(waveString, totalWave);
    char waveTitle[20];//波次标题
    sprintf_s(waveTitle, "--第%s波--", waveString);
    do
      fgets(f, s);
    while (strlen(s) > 0 && strcmp(s, waveTitle) != 0);
    //如果找不到指定波次，不用更新轨道信息，直接返回0
    if (strlen(s) == 0)
    {
      fclose(f);
      return 0;
    }
  }

  //更新本波轨道
  work[task].reloading = true; //标记正在读取新波次轨道
  if (LoadCurrentWave(task, account, f, s, seg, path, line, info, totalWave) != 0)
    return line;
  work[task].reloading = false;

  fclose(f);
  return 0;
}
//（开局或变阵时）读取轨道第totalWave波信息。读取成功返回0，否则返回出错的行数
int ReloadWave(int task, int account, int totalWave)
{
  char info[1000] = {};
  return ReloadWave(task, account, work[task].backupTrack[account], info, totalWave);
}
//从完整路径提取纯轨道路径
const char *GetPureTrack(const char *extendedTrack)
{
  const char *bannedTrack = extendedTrack;
  const char *bar = strchr(extendedTrack, '|');
  if (bar)
    bannedTrack = bar + 1;
  const char *pureTrack = bannedTrack;
  const char *braket = strchr(bannedTrack, '>');
  if (braket)
    pureTrack = braket + 1;
  return pureTrack;
}
//从完整路径提取轨道前缀
void GetTrackPrefix(const char *extendedPath, char(&prefix)[maxPath])
{
  memset(prefix, 0, sizeof(prefix));
  strncpy_s(prefix, extendedPath, GetPureTrack(extendedPath) - extendedPath);
}
//轨道路径为+返回1，++返回2，其他返回0
int IsEmptyTrack(const char *extendedPath)
{
  const char *pureTrack = GetPureTrack(extendedPath);
  if (strcmp(pureTrack, "+") == 0)
    return 1;
  if (strcmp(pureTrack, "++") == 0)
    return 2;
  return 0;
}
//从指定路径读取轨道并完整检查。检查合格返回0，无轨道返回-1，打不开返回-2，否则返回出错行数
int LoadTrackFrom(int task, int account, const char *extendedPath, char(&info)[1000])
{
  //填写ban卡数组，获得轨道路径
  if (!ExtractTrack(task, account, extendedPath, work[task].pureTrack[account]))
  {
    strcpy_s(info, "<>指令不合规范");
    return -1;
  }
  const char *path = work[task].pureTrack[account];

  //根据文件名判断星级限制
  char starRequirement[10] = {};//星级词条
  work[task].star[account] = 16;//默认星级限制为16
  for (int star = 0; star <= 16; star++)
  {
    sprintf_s(starRequirement, "_星%d", star);
    if (strstr(path, starRequirement))
      work[task].star[account] = star;
  }

  WaitForSingleObject(hMutexTrack, INFINITE);

  //检查轨道文件能否打开，能则读取存在的波次
  int line = GetWaveExist(task, account, path, info);
  if (line != 0)
  {
    ReleaseMutex(hMutexTrack);
    return line;
  }

  FILE *f;
  fopen_s(&f, path, "r");//打开轨道文件（这次不用检查）

  //读取文件头
  char *name = nullptr, *nextName = nullptr;
  char s[maxLineSize], seg[maxLineSize], tip[100];
  line = 0;//读取到的行数
  fgets(f, s);//读取第1行：轨道版本号
  line++;
  if (strstr(s, "轨道版本号=")) //有版本号则再读一行
  {
    fgets(f, s);//读取第2行：人物1位置
    line++;
  }
  if (strstr(s, "注释=")) //有注释则读取注释，再读一行
  {
    fgets(f, s);//读取第2行：人物1位置
    line++;
  }

  bool isAnotherTrackEmpty = IsEmptyTrack(param[task].track[1 - account]) > 0;//另一个号是否为空轨道

  //读取人物位置
  char roleString[100] = {};//"人物n位置="后面的字段
  char format[20] = {};//读取人物位置的格式字符串
  for (int player = 0; player < 2; player++)
  {
    work[task].roleLocNum[account][player] = 0;//人物位置个数预设为0
    sprintf_s(format, "人物%d位置=%%s", player + 1);
    if (sscanf_s(s, format, roleString, sizeof(roleString)) == 1)
    {
      nextName = nullptr;
      name = strtok_s(roleString, ",", &nextName);
      while (name)
      {
        int loc = atoi(name);
        if (loc / 10 < 1 || loc / 10 > 7 || loc % 10 == zero)//人物位置超出范围
          return WriteInfo(info, f, path, line, s, "行数范围为1~7，列数范围为1~9", hMutexTrack);
        work[task].roleLocNum[account][player]++;//人物位置个数+1
        work[task].roleLoc[account][player][work[task].roleLocNum[account][player]] = loc;//记录人物位置
        name = strtok_s(nullptr, ",", &nextName);
      }
      fgets(f, s);
      line++;
    }
    else if (player == 0) //没有人物1位置报错
      return WriteInfo(info, f, path, line, s, "缺少人物1位置", hMutexTrack);
  }

  //读取魔塔层数
  if (sscanf_s(s, "魔塔层数=%d", &work[task].tower[account]) == 1)
  {
    if (work[task].tower[account] < zero || work[task].tower[account] > 165)
      return WriteInfo(info, f, path, line, s, "魔塔层数范围为0~165", hMutexTrack);
    if (IsAdvance(task))
      work[task].tower[account] = 0;
    fgets(f, s);
    line++;
  }
  else
    work[task].tower[account] = 0;

  //读取退出时间
  if (sscanf_s(s, "退出时间=%d", &work[task].quitTime[account]) == 1)
  {
    if (work[task].quitTime[account] < zero || work[task].quitTime[account] > 960)
      return WriteInfo(info, f, path, line, s, "退出时间范围为0~960", hMutexTrack);
    fgets(f, s);
    line++;
  }
  else
    work[task].quitTime[account] = 0;

  //读取刷技能模式
  if (strstr(s, "刷技能模式=") == s)
  {
    work[task].isSkillMode = atoi(s + 11) != 0;
    fgets(f, s);
    line++;
  }
  else
    work[task].isSkillMode = false;

  //读取优先队列
  work[task].queueSize[account] = 0;
  if (strstr(s, "优先队列=") == s)
  {
    char *context = nullptr;
    char *name = strtok_s(s + 9, ",", &context);
    while (name)
    {
      strcpy_s(work[task].queue[account][work[task].queueSize[account]++].name, name);
      name = strtok_s(nullptr, ",", &context);
    }
    fgets(f, s);
    line++;
  }

  //跳过宝石波次
  if (strstr(s, "宝石波次="))
  {
    fgets(f, s);
    line++;
  }

  //读取卡槽数
  int slotNum = 0;
  if (sscanf_s(s, "卡槽数=%d", &slotNum) != 1)
    return WriteInfo(info, f, path, line, s, "缺少卡槽数", hMutexTrack);
  else
  {
    if (slotNum < 0 || slotNum > 21)
      return WriteInfo(info, f, path, line, s, "卡槽数范围为0~21", hMutexTrack);
    work[task].originalSlotNum[account] = slotNum;//记录原卡槽数
  }

  //计算剩余卡槽数
  work[task].slotNum[account] = work[task].originalSlotNum[account];
  for (int originalOrder = 1; originalOrder <= work[task].originalSlotNum[account]; originalOrder++)
    if (work[task].originalSlot[account][originalOrder].banned
      && strlen(work[task].originalSlot[account][originalOrder].alternate) == 0)
      work[task].slotNum[account]--;

  //超出卡槽数的卡全ban了
  if (work[task].isBanEnabled[account])
    for (int originalOrder = work[task].originalSlotNum[account] + 1; originalOrder <= maxSlotNum; originalOrder++)
      work[task].originalSlot[account][originalOrder].banned = true;

  //检查每个波次
  for (int order = 0; order <= maxSlotNum + 1; order++)
    work[task].slot[account][order].used = false;

  for (int totalWave = 0; totalWave < maxTotalWave; totalWave++)
    if (work[task].totalWaveExist[account][totalWave])
    {
      //读取本波（会跳过下波标题），如果出错则结束读取，返回行数（info已在LoadCurrentWave中填好）
      if (LoadCurrentWave(task, account, f, s, seg, path, line, info, totalWave) != 0)
      {
        ReleaseMutex(hMutexTrack);
        return line;
      }
      if (strlen(s) > 0 && !IsWaveTitle(s))
      {
        sprintf_s(tip, "卡槽数=%d，但读完%d个卡槽仍未结束", slotNum, slotNum);
        return WriteInfo(info, f, path, line, s, tip, hMutexTrack);
      }
    }

  fclose(f);
  ReleaseMutex(hMutexTrack);

  //全波检查完后，如果0卡槽有替换，转移到第1个空卡槽
  if (strlen(work[task].originalSlot[account][0].alternate) > 0)
  {
    bool alternated = false;
    //先查一遍原卡组有没有0槽替换卡，有则转移到原卡槽
    for (int originalOrder = 1; originalOrder <= work[task].originalSlotNum[account]; originalOrder++)
    {
      SlotInfo &originalSlot = work[task].originalSlot[account][originalOrder];
      if (strcmp(originalSlot.name, work[task].originalSlot[account][0].alternate) == 0)
      {
        if (originalSlot.banned) //如果原卡组有0槽替换卡，且该卡被ban
        {
          sprintf_s(info, "[%s]与卸除%d槽矛盾", work[task].originalSlot[account][0].alternate, originalOrder);
          return -1;
        }
        strcpy_s(originalSlot.alternate, work[task].originalSlot[account][0].alternate);
        strcpy_s(originalSlot.name, originalSlot.alternate);
        alternated = true;
        break;
      }
    }
    //原卡组没有0槽替换卡，则转移到第1个空卡槽
    if (!alternated)
      for (int originalOrder = 1; originalOrder <= work[task].originalSlotNum[account]; originalOrder++)
      {
        SlotInfo &originalSlot = work[task].originalSlot[account][originalOrder];
        //既没ban也没使用，不是咖啡粉、棉花糖、麦芽糖
        if (!originalSlot.banned && !originalSlot.used && strcmp(originalSlot.name, "咖啡粉") &&
          strcmp(originalSlot.name, "棉花糖") && strcmp(originalSlot.name, "麦芽糖"))
        {
          strcpy_s(originalSlot.alternate, work[task].originalSlot[account][0].alternate);
          strcpy_s(originalSlot.name, originalSlot.alternate);
          alternated = true;
          break;
        }
      }
    //如果没有空卡槽可供替换
    if (!alternated)
    {
      sprintf_s(info, "没有卡槽可替换为[%s]", work[task].originalSlot[account][0].alternate);
      return -1;
    }
    //取消卡槽0的替换
    strcpy_s(work[task].originalSlot[account][0].alternate, "");
    strcpy_s(work[task].originalSlot[account][0].name, "铲子");
  }
  return 0;
}
//用模板轨道和+轨道生成++轨道
bool MakeTrackPP(int task, int account, const char *trackP, const char *trackPP, char(&info)[1000])
{
  //1. 根据模板和模板+建立1P卡组和2P卡组的映射关系
  SlotInfo slot[2][maxSlotNum + 2] = {};//模板和模板+各卡槽名称
  int slotNum[2] = {};//模板和模板+卡槽数
  int slotMap[maxSlotNum + 2] = {};//卡组映射：模板的i号卡槽对应模板+的几号卡槽
  for (int order = 0; order <= maxSlotNum + 1; order++)
    slotMap[order] = -1;

  //1.1 读取模板轨道
  int loadResult = LoadTrackFrom(task, account, "预制轨道\\通用轨道\\模板.txt", info);
  if (loadResult != 0) //轨道打不开，记录报错info并退出
    return false;
  slotNum[0] = work[task].slotNum[account];
  for (int order = 0; order <= work[task].slotNum[account]; order++)
  {
    strcpy_s(slot[0][order].name, work[task].slot[account][order].name);
    slot[0][order].cd = work[task].slot[account][order].cd;
    slot[0][order].level = work[task].slot[account][order].level;
  }

  //1.2 读取模板+轨道
  loadResult = LoadTrackFrom(task, account, "预制轨道\\通用轨道\\模板+.txt", info);
  if (loadResult != 0) //轨道打不开，记录报错info并退出
    return false;
  slotNum[1] = work[task].slotNum[account];
  for (int order = 0; order <= work[task].slotNum[account]; order++)
    strcpy_s(slot[1][order].name, work[task].slot[account][order].name);

  //1.3 建立模板到模板+的卡组映射
  for (int order0 = 0; order0 <= slotNum[0]; order0++)
    for (int order1 = 0; order1 <= slotNum[1]; order1++)
      if (strcmp(slot[0][order0].name, slot[1][order1].name) == 0)
        slotMap[order0] = order1;
  for (int order0 = slotNum[0] + 1; order0 <= maxSlotNum; order0++)
    slotMap[order0] = order0 - slotNum[0] + slotNum[1];
  slotMap[maxSlotNum + 1] = maxSlotNum + 1;

  //2. 打开并检验+轨道
  loadResult = LoadTrackFrom(task, account, trackP, info);
  if (loadResult != 0) //轨道打不开，记录报错info并退出
    return false;

  //3. +轨道和模板轨道都能正常打开，则编写++轨道
  FILE *fin, *fout;
  char s[maxLineSize] = {};
  fopen_s(&fin, trackP, "r");//打开+轨道
  if (fopen_s(&fout, trackPP, "w"))//打开++轨道
  {
    sprintf_s(info, "无法生成++轨道：\n%s", trackP);
    return false;
  }
  //拷贝+轨道内容到++轨道，但是卡槽数要改变，波次要变化
  int slotNumP = 0;//+轨道卡槽数
  int slotNumPP = 0;//++轨道卡槽数
  char slotTextP[maxSlotNum + 2][maxLineSize] = {};//+轨道卡槽文本
  while (!feof(fin))
  {
    fgets(fin, s);
    if (strlen(s) == 0)
      break;
    //读取到"卡槽数="或"--第%d波--"时，表示1个波次开始
    if (strstr(s, "卡槽数=") == s || strstr(s, "--第") == s)
    {
      //1. 卡槽数要修改，波次则照搬
      if (strstr(s, "卡槽数="))
      {
        sscanf_s(s, "卡槽数=%d", &slotNumP);//读取+轨道卡槽数
        slotNumPP = slotNumP + slotNum[0] - slotNum[1];
        fprintf(fout, "卡槽数=%d\n", slotNumPP);//写入++轨道卡槽数
      }
      else
        fputs(fout, s);
      //2. 读取+轨道各卡槽文本
      for (int lineOrder1 = -1; lineOrder1 <= slotNumP; lineOrder1++)
      {
        int order1 = lineOrder1 == -1 ? maxSlotNum + 1 : lineOrder1;
        fgets(fin, slotTextP[order1]);
      }
      //3. 写入++轨道各卡槽文本
      for (int lineOrder2 = -1; lineOrder2 <= slotNumPP; lineOrder2++)
      {
        int order2 = lineOrder2 == -1 ? maxSlotNum + 1 : lineOrder2;//++轨道卡槽号
        if (slotMap[order2] >= 0) //如果该卡槽是+轨道的卡槽，原样写入
          fputs(fout, slotTextP[slotMap[order2]]);
        else //如果该卡槽是1P模板独有的卡槽，写入空卡槽
          fprintf(fout, "%s\t%d[%d]\n", slot[0][order2].name, slot[0][order2].cd, slot[0][order2].level);
      }
    }
    else //不是波次行则照抄
      fputs(fout, s);
  }
  fclose(fin);
  fclose(fout);
  return true;
}
//用1 - account的正常轨道生成account的空轨道（+/++）
bool MakeEmptyTrack(int task, int account, const char *templateTrack, const char *emptyTrack, char(&info)[1000])
{
  //1. 从1 - account轨道读取人物2位置
  int loadResult = LoadTrackFrom(task, 1 - account, param[task].track[1 - account], info);
  if (loadResult != 0) //轨道打不开，记录报错info并退出
    return false;
  //若为单人轨道，提示“aP轨道为单人，无法确定bP人物位置”
  if (GetTrackRoleNum(task, 1 - account) == 1)
  {
    sprintf_s(info, "%dP轨道为单人，无法确定%dP人物位置。", (1 - account) + 1, account + 1);
    return false;
  }
  //至此已成功打开1 - account的双人轨道，roleLoc[1-account][1]为该轨道的2P人物，即空轨道的1P人物
  int roleLocNum = work[task].roleLocNum[1 - account][1];//模板轨道的2P人物数量
  int roleLoc[63] = {};//模板轨道的2P人物位置
  memcpy(roleLoc, work[task].roleLoc[1 - account][1], sizeof(work[task].roleLoc[1 - account][1]));

  //2. 从模板轨道读取卡组信息（必须用纯轨道，因为替换前缀如<8热狗>会改变originalSlotName）
  loadResult = LoadTrackFrom(task, account, templateTrack, info);
  if (loadResult != 0) //轨道打不开，记录报错info并退出
    return false;

  //3. 制作空轨道
  FILE *fout;//轨道文件句柄
  if (fopen_s(&fout, emptyTrack, "w"))
  {
    sprintf_s(info, "无法生成空轨道：\n%s", param[task].track[account]);
    return false;
  }
  //写入轨道开头
  fprintf(fout, "轨道版本号=%s\n", version);
  fprintf(fout, "注释=\n");
  fprintf(fout, "人物1位置=");
  for (int times = 1; times <= roleLocNum; times++)
  {
    fprintf(fout, "%d", roleLoc[times]);
    if (times < roleLocNum)
      fprintf(fout, ",");
    else
      fprintf(fout, "\n");
  }
  fprintf(fout, "魔塔层数=0\n");
  fprintf(fout, "退出时间=0\n");
  fprintf(fout, "刷技能模式=0\n");
  fprintf(fout, "优先队列=\n");
  fprintf(fout, "卡槽数=%d\n", work[task].slotNum[account]);
  //写入各卡槽空轨道
  for (int lineOrder = -1; lineOrder <= work[task].slotNum[account]; lineOrder++)
  {
    int order = lineOrder == -1 ? maxSlotNum + 1 : lineOrder;
    Slot &slot = work[task].slot[account][order];
    fprintf(fout, "%s\t%d[%d]\n", slot.name, slot.cd, slot.level);
  }
  fclose(fout);
  return true;
}
//判断++轨道是否可由+轨道转化，并填写+轨道路径
bool IsTrackPPEffective(const char *trackPP, char(&trackP)[maxPath])
{
  int length = strlen(trackPP);
  //如果是++轨道
  if (strstr(trackPP, "++.txt") == trackPP + length - 6)
  {
    //填写+轨道路径
    strcpy_s(trackP, trackPP);//拷贝++轨道路径
    DeleteString(trackP + length - 6, 1);//删除一个"+"，变成"+.txt"
    return FileExist(trackP);//判断+轨道是否存在
  }
  return false;
}
//将param[task].track[account]读入work数组。读取成功返回1，失败返回0并记录错误信息
//如果是+或++轨道，自动生成空轨道并读取空轨道
int LoadTrack(int task, int account, char(&info)[1000])
{
  char track[maxPath] = {};//实际传入LoadTrack的轨道路径
  strcpy_s(track, param[task].track[account]);//拷贝轨道路径

  char prefix[maxPath] = {};//轨道路径前缀
  GetTrackPrefix(param[task].track[account], prefix);//获取轨道前缀指令

  char tempTrack[maxPath] = {};//临时生成的轨道路径
  sprintf_s(tempTrack, "自动备份\\Temp%d.txt", task);

  int emptyType = IsEmptyTrack(param[task].track[account]);//获取空轨道类型
  if (emptyType > 0) //空轨道
  {
    //生成空轨道的条件：另一个号有正常轨道
    bool generable = strlen(param[task].track[1 - account]) > 0
      && IsEmptyTrack(param[task].track[1 - account]) == 0;
    if (!generable) //如果另一个号没有正常轨道
    {
      strcpy_s(info, "另一个号有轨道才能使用+记号。");
      return 0;
    }
    if (emptyType == 1) //+空轨道：用"模板+"生成
    {
      if (!MakeEmptyTrack(task, account, "预制轨道\\通用轨道\\模板+.txt", tempTrack, info))
        return 0;
    }
    else if (emptyType == 2) //++空轨道：用另一个号的正常轨道生成
    {
      if (!MakeEmptyTrack(task, account, GetPureTrack(param[task].track[1 - account]), tempTrack, info))
        return 0;
    }
    sprintf_s(track, "%s%s", prefix, tempTrack);
  }
  else //非空轨道
  {
    const char *pureTrack = GetPureTrack(param[task].track[account]);//纯轨道路径
    char trackP[maxPath] = {};//+轨道路径
    //如果轨道不存在，且属于可由+轨道生成的++轨道，则由+轨道生成
    if (!FileExist(pureTrack) && IsTrackPPEffective(pureTrack, trackP))
    {
      if (!MakeTrackPP(task, account, trackP, tempTrack, info))
        return 0;
      sprintf_s(track, "%s%s", prefix, tempTrack);
    }
  }

  if (LoadTrackFrom(task, account, track, info) != 0)
    return 0;
  return 1;
}
//载入轨道信息到Work数组中。读取成功返回人数，失败返回0并记录错误信息
void LoadTrack(int task, int account)
{
  char info[1000];
  LoadTrack(task, account, info);
}
//检查并放置卡片；如果波次更新则变阵
void CheckPlant(int task)
{
  DWORD currentTick = GetTickCount();//当前检查时间
  if (currentTick - work[task].lastCheckTick < 100)//距离上次检查不到100ms，则退出
    return;
  work[task].lastCheckTick = currentTick;//记录本次检查时间

  //每隔一定时间获取关卡信息
  bool isInfoGotten[2] = {};//信息获取是否成功
  DWORD recognitionGap = 1000 / recognitionFrame;//检查间隔
  if (currentTick - work[task].lastSimilarityTick >= recognitionGap)
  {
    work[task].lastSimilarityTick = currentTick;
    ReadLevelInfo(task, isInfoGotten);
  }

  //识别到0~maxWave-1波时，若是第一次进入该波，更新波次开始时间；若存在该波轨道，载入该波轨道
  if (work[task].wave >= 0 && work[task].wave < maxWave)
  {
    int currentTotalWave = work[task].wave * maxSmallWave + work[task].smallWave;//计算总波次
    if (work[task].totalWaveTick[currentTotalWave] == 0) //如果还没有记录波次开始时间，说明刚进入这一波
    {
      int totalWaveRequired[2] = { -1, -1 };//两个账号分别需要载入第几波
      for (int totalWave = currentTotalWave; totalWave > 0; totalWave--) //倒序检查本波及以前的所有波次
      {
        //如果已经记录时刻，就结束检查
        if (work[task].totalWaveTick[totalWave])
          break;
        //否则记录波次开始时刻
        work[task].totalWaveTick[totalWave] = currentTick;
        for (int account = 0; account < 2; account++)//筛选第一个存在轨道的波次
          if (totalWaveRequired[account] == -1 && work[task].totalWaveExist[account][totalWave])
            totalWaveRequired[account] = totalWave;//记录为需要载入的波次
      }
      //检查是否需要载入波次，需要则载入
      for (int account = 0; account < 2; account++)
        if (work[task].isPerformed[account] && !work[task].isQuitted[account])
          if (totalWaveRequired[account] >= 0)
            ReloadWave(task, account, totalWaveRequired[account]);
    }
  }
  for (int account = 0; account < 2; account++)//检查放置符合要求的卡（最多1张）
  {
    bool isPlanted = false;//是否已放卡
    if (work[task].isPerformed[account] && !IsAccountLevelEnded(task, account))
    {
      if (isInfoGotten[account]) //识别cd成功才放置极限卡
        isPlanted = CheckLimitPlant(task, account);
      if (!isPlanted) //极限卡没有放置才放置普通卡
        CheckNormalPlant(task, account);
    }
  }
}
//每局开始前重置数据：种植次数和宝石使用次数归零，宝石计时器关闭
void ResetData(int task)
{
  for (int account = 0; account < 2; account++)
  {
    work[task].isWaveEnded[account] = false;
    work[task].isLootSaved[account] = false;
    work[task].isBackgroundCatched[account] = 0;//背景未截取
    work[task].energy[account] = 0;//能量归零
    work[task].realSlotNum[account] = 0;//实际卡槽数归零
    work[task].lackeyNum[account] = 0;//跟随卡数清零
    work[task].isQuitted[account] = false;//退出状态清零
    for (int order = 0; order <= work[task].slotNum[account]; order++)
      work[task].isTemplateCatched[account][order] = 0;//卡片模板为未截取状态

    //优先队列中的卡，卡组中有的预设为未放置，卡组中没有的预设为已放置
    for (int i = 0; i < work[task].queueSize[account]; i++)
    {
      work[task].queue[account][i].planted = true; //预设为已放置
      //如果该卡出现在卡组中，预设为未放置
      for (int order = 0; order <= work[task].slotNum[account]; order++)
        if (strcmp(work[task].slot[account][order].name, work[task].queue[account][i].name) == 0)
          work[task].queue[account][i].planted = false;
    }
  }
  work[task].oneAccountEndTick = 0;
  work[task].lastCheckTick = 0;
  work[task].lastSimilarityTick = 0;
  work[task].isBossExist = false;
  work[task].isRatAppeared = false;
  work[task].ratRowNum = 0;
  work[task].currentTime = 0;
  memset(work[task].totalWaveTick, 0, sizeof(work[task].totalWaveTick));//波次进入时刻清零，表示未到达波次
  memset(work[task].ratRow, 0, sizeof(work[task].ratRow));//出怪行清零
  memset(work[task].grid, 0, sizeof(work[task].grid));//地图占用信息清零
}
//从Param获取参数到Work
void GetWorkFromParam(int task)
{
  //常规任务和同步任务2P都能从param获取房主、局数、轨道、卡组、钥匙、每局用时
  const POINT noTag = { -1, -1 };
  work[task].games = reserve[task].gamesFinished + 1;
  work[task].maxGames = param[task].maxGames;
  work[task].gameTime = param[task].gameTime;
  work[task].host = param[task].host;
  for (int account = 0; account < 2; account++)
  {
    strcpy_s(work[task].track[account], param[task].track[account]);
    work[task].deck[account] = param[task].deck[account];
    work[task].isProceed[account] = param[task].isProceed[account];
  }
  //同步任务2P：从param[task - 1]获取收集、句柄信息
  if (IsDual2P(task))
  {
    strcpy_s(work[task].mainName, param[task - 1].mainName);//主大厅
    //沿用param[task - 1]的收集和句柄信息
    for (int account = 0; account < 2; account++)
    {
      strcpy_s(work[task].hallName[account], param[task - 1].hallName[account]);//大厅
      work[task].isCollect[account] = param[task - 1].isCollect[account];//收集
    }
    work[task].tag[0] = noTag;//1P不参与，所以没有tag和hWnd
    work[task].hWnd[0] = NULL;
    work[task].tag[1] = param[task - 1].tag[1];//2P的tag和hWnd使用param[task - 1]的2P
    work[task].hWnd[1] = param[task - 1].hWnd[1];
  }
  else //常规任务
  {
    strcpy_s(work[task].mainName, param[task].mainName);//主大厅
    for (int account = 0; account < 2; account++)
    {
      strcpy_s(work[task].hallName[account], param[task].hallName[account]);//大厅
      work[task].isCollect[account] = param[task].isCollect[account];//收集
      //只把参与的账号位置和句柄传过去，不参与的账号句柄为NULL
      if (work[task].isInvolved[account])
      {
        work[task].tag[account] = param[task].tag[account];
        work[task].hWnd[account] = param[task].hWnd[account];
      }
      else
      {
        work[task].tag[account] = noTag;
        work[task].hWnd[account] = NULL;
      }
    }
  }
  strcpy_s(work[task].embarkString, "已传入任务参数\n");
}
//恢复隐藏的窗口，如果窗口没被隐藏则不改变位置
void RecoverWindow_NoMove(HWND hWnd)
{
  RECT rect;
  int ScrWidth, ScrHeight, WndWidth, WndHeight;
  if (!IsWindow(hWnd))
    return;//没有窗口则结束程序
  if (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)//如果窗口被隐藏任务栏图标
  {
    SetWindowLong(hWnd, GWL_EXSTYLE, 0);//恢复任务栏图标
    ShowWindow(hWnd, SW_RESTORE);//窗口常规显示
    GetWindowRect(hWnd, &rect);
    WndWidth = rect.right - rect.left, rect;
    WndHeight = rect.bottom - rect.top;
    ScrWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    ScrHeight = GetSystemMetrics(SM_CYFULLSCREEN);
    MoveWindow(hWnd, (ScrWidth - WndWidth) / 2, (ScrHeight - WndHeight * 29 / 30) / 2, WndWidth, WndHeight, 1);//窗口回到屏幕并居中
  }
  else //否则说明只是被最小化或移位
    ShowWindow(hWnd, SW_RESTORE);//窗口常规显示
}
//居中显示窗口
void CenterShow(HWND hWnd)
{
  if (!IsWindow(hWnd))
    return;//没有窗口则结束
  ShowWindow(hWnd, SW_SHOW);//显示窗口
  RECT rect;
  GetWindowRect(hWnd, &rect);
  int wndWidth = rect.right - rect.left;
  int wndHeight = rect.bottom - rect.top;
  int scrWidth = GetSystemMetrics(SM_CXFULLSCREEN);
  int scrHeight = GetSystemMetrics(SM_CYFULLSCREEN);
  MoveWindow(hWnd, (scrWidth - wndWidth) / 2, (scrHeight - wndHeight * 29 / 30) / 2,
    wndWidth, wndHeight, true);//窗口回到屏幕并居中
}
//将隐藏的窗口恢复到隐藏前的位置
void RecoverWindow(HWND hWnd, HideInfo &hideInfo)
{
  hideInfo.hidden = false;
  if (!IsWindow(hWnd))
    return;//没有窗口则退出
  SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_TOOLWINDOW);//恢复任务栏图标
  ShowWindow(hWnd, SW_RESTORE);//窗口常规显示
  RECT &rect = hideInfo.rect;
  MoveWindow(hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 1);
  if (hideInfo.maximized) //如果隐藏前是最大化的
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);//最大化
}
//隐藏窗口，记录隐藏前的位置
int HideWindow(HWND hWnd, HideInfo &hideInfo)
{
  if (!IsWindow(hWnd))
    return 1;//没有窗口
  if (IsZoomed(hWnd))
    hideInfo.maximized = true;
  else
    hideInfo.maximized = false;
  ShowWindow(hWnd, SW_RESTORE);//窗口去最大化
  ShowWindow(hWnd, SW_RESTORE);//窗口去最大化
  RECT &rect = hideInfo.rect;
  GetWindowRect(hWnd, &rect);
  //窗口沉底到Y=3000，如果移动失败，可能是权限不足
  if (!MoveWindow(hWnd, 0, 3000, rect.right - rect.left, rect.bottom - rect.top, 1))
    return 2;
  hideInfo.hidden = true;
  SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);//隐藏任务栏图标
  return 0;
}
//hWndGame是不是游戏窗口
int IsGameWindow(HWND hWndGame)
{
  if (!IsWindow(hWndGame)) //不是窗口，返回0
    return 0;
  char ClassName[256];
  GetClassNameA(hWndGame, ClassName, 256);//获取游戏窗口类名
  if (strcmp(ClassName, "NativeWindowClass") == 0 || strcmp(ClassName, "WebPluginView") == 0)
    return 1; //符合微端、大厅的游戏窗口类名，则返回1
  return 0; //否则返回0
}
//颜色color是否符合最近服务器颜色
bool isServerColor(COLORREF color, int platform)
{
  switch (platform)
  {
  case 1://4399平台
    return bgrRValue(color) >= 246 && bgrGValue(color) >= 240 && bgrBValue(color) >= 214 &&
      bgrRValue(color) <= 255 && bgrGValue(color) <= 252 && bgrBValue(color) <= 226;
  case 2://QQ空间
    return color == 0xfdffea;
  case 3://QQ大厅
    return color == 0x3c3c3d;
  case 4://纯白色，可能是4399微端(4)/QQ空间登录界面(6)/4399网页还没加载出来
    return color == 0xffffff;
  case 5://断网界面
    return bgrRValue(color) >= 211 && bgrGValue(color) >= 227 && bgrBValue(color) >= 236 &&
      bgrRValue(color) <= 213 && bgrGValue(color) <= 229 && bgrBValue(color) <= 237;
  }
  return false;
}
const int headWidth = 87, headHeight = 87;//头像宽度和高度
//计算头像head和大厅中(headX,headY)处头像的颜色方差
int headDifference(COLORREF head[headHeight][headWidth], COLORREF **hall, int headX, int headY)
{
  int diff = 0;
  for (int x = 0; x < headWidth; x++)
    for (int y = 0; y < headHeight; y++)
    {
      int diffR = bgrRValue(head[y][x]) - bgrRValue(hall[headY + y][headX + x]);
      int diffG = bgrGValue(head[y][x]) - bgrGValue(hall[headY + y][headX + x]);
      int diffB = bgrBValue(head[y][x]) - bgrBValue(hall[headY + y][headX + x]);
      diff += diffR * diffR + diffG * diffG + diffB * diffB;
    }
  return diff;
}
//向文件path中追加一行content
void addToRefreshLog(int task, char *content, bool withTime = true)
{
  int list = GetList(task);
  WaitForSingleObject(plot[list].hMutexRefreshLog, INFINITE);
  FILE *f;
  if (!fopen_s(&f, plot[list].refreshLog, "a"))
  {
    if (withTime)
    {
      char timeString[100];
      GetTimeString(timeString, time(NULL));
      fprintf(f, "%s %s\n", timeString, content);
    }
    else
      fprintf(f, "%s\n", content);
    fclose(f);
  }
  ReleaseMutex(plot[list].hMutexRefreshLog);
}
//点击刷新按钮并记录
void ClickRefresh(int task, int account)
{
  char message[100];
  int refreshX = 228 * DPI / 96, refreshY = 44 * DPI / 96;
  LeftClick(Hall(param[task].hallName[account]), refreshX, refreshY); //重新点击刷新按钮
  sprintf_s(message, "%dP点击刷新按钮(%d,%d)", account + 1, refreshX, refreshY);
  addToRefreshLog(task, message);
}
//从大厅窗口中判断平台并返回色块位置，返回平台并记录位置，找不到返回0
int FindLatestServer0(int task, int account, int *px, int *py)
{
  int list = GetList(task);

  int hallWidth = 0, hallHeight = 0;//大厅窗口尺寸
  //截取大厅图像
  if (!HallShot(Hall(work[task].hallName[account]), work[task].hallShot, &hallWidth, &hallHeight, 20))
    return 0;
  //截图成功，则构造二级指针
  for (int i = 0; i < hallHeight; i++)
    work[task].pHallShot[i] = work[task].hallShot + i * hallWidth;

  //各平台的色块尺寸：1=4399网页 2=QQ空间 3=QQ大厅 4=纯白（包含4=4399微端 6=选头像或4399网页未加载）5=断网
  POINT bar[6] = { { 0, 0 }, { 220, 135 }, { 210, 70 }, { 220, 50 }, { 220, 50 }, { 220, 50 } };

  //判断5个平台
  int platform = 0;//平台
  for (int iPlatform = 1; iPlatform <= 5; iPlatform++)
  {
    int width = bar[iPlatform].x, height = bar[iPlatform].y;//需要识别的色块大小
    const int step = 2;//识图步长
    for (int x = hallWidth - width; x >= 0; x -= step)//屏幕找图
    {
      for (int y = 0; y <= hallHeight - height; y += step)
        if (IsBitmapFit(isServerColor, work[task].pHallShot, iPlatform, width, height, x, y, step))
        {
          if (px)
            *px = x;
          if (py)
            *py = y;
          platform = iPlatform;
          break;
        }
      if (platform > 0)
        break;
    }
    if (platform > 0)
      break;
  }
  //如果识别结果为4（纯白），则计算面积
  if (platform == 4)
  {
    long long whiteCount = 0;//选服窗口中的白色像素数量
    for (int y = 0; y < hallHeight; y++)
      for (int x = 0; x < hallWidth; x++)//屏幕找图
        if (work[task].pHallShot[y][x] == 0xffffff)
          whiteCount++;
    long long hallArea = (long long)hallHeight * hallWidth;//选服窗口面积
    int whiteRatio = (int)(100 * whiteCount / hallArea);//白色百分比
    if (whiteRatio > 50) //白色百分比大于50%，判断为选头像界面
      platform = 6;
    //char message[1000] = {};
    //sprintf_s(message, "白色百分比：%d%%", whiteRatio);
    //ReportError(task, account, "白色百分比", message, END_TASK);
  }

  //识别结果为未知、4399网页、QQ空间、QQ大厅、微端登录页：直接返回
  if (platform <= 4)
    return platform;

  if (platform == 5) //如果识别到了断网界面，点击重试并返回0
  {
    int retryX = (work[task].serverRect.right - work[task].serverRect.left) / 2;
    int retryY = 350 * DPI / 96;
    LeftClick(work[task].hWndServer, retryX, retryY);
    char message[200];
    sprintf_s(message, "%dP检测到断网，已点击重试(%d,%d)", account + 1, retryX, retryY);
    addToRefreshLog(task, message);
    CheckSleep(task, 2000);//等待2秒
    work[task].hWndServer = GetActiveServerWindow(Hall(work[task].hallName[account]));
    return 0;//返回WaitServerInWindow函数，继续判断选服界面
  }

  //如果找到了纯白色块，判断是否QQ大厅登录界面
  int headAreaLength = 0;//头像区域累积长度（一行全白色则累积中断）
  int headX = 0, headY = 0;//头像右上角尺寸
  int isHeadFound = 0;//头像是否找到
  for (int y = 120; y < hallHeight && isHeadFound == 0; y++) //从高度120开始从上到下扫描
  {
    for (int x = hallWidth - 20; x >= hallWidth / 2; x--) //从右往左扫，最多扫到半屏
      if (work[task].pHallShot[y][x] != 0xffffff) //非白色区位头像区
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
    return 0;

  int headNum = 0;//头像数量
  POINT head[10] = {};//每个头像的位置
  for (int i = 0; i < sizeof(head) / sizeof(head[0]); i++) //填写头像位置高度
    head[i].y = headY;
  for (int x = headX; x > 20; x--) //从右往左扫描
    for (int y = headY; y < headY + 87; y++) //在头像纵坐标区域扫描
      if (work[task].pHallShot[y][x] != 0xffffff) //非白色区域为头像区，累积长度+1
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

  char message[1000];
  if (headNum <= 1) //没有头像，或者只有二维码
  {
    sprintf_s(message, "%dP未识别到头像", account + 1);
    addToRefreshLog(task, message);
    ClickRefresh(task, account);//点击刷新按钮
    CheckSleep(task, 3000);//刷新完等待3秒，然后return -1;
    return -1;//代码-1：识别到的头像数目不足
  }

  //识图选择头像：如果有1P和2P头像截图，点击相似度最高的头像
  int headSelected = -1;//选择第几个头像
  COLORREF headShot[87][87];//头像截图
  COLORREF initialHead[87][87];//初始头像截图
  char headPath[maxPath], initialHeadPath[maxPath];
  sprintf_s(headPath, "用户参数\\QQ头像截图\\%dP.png", account + 1);//填写头像截图路径
  sprintf_s(initialHeadPath, "用户参数\\QQ头像截图\\默认头像.png");//填写初始头像截图路径
  if (!BitmapToColor(headPath, headShot)) //载入截图
    ReportError(task, account, "缺少头像截图", "缺少QQ头像截图，无法登录", END_TASK);
  if (!BitmapToColor(initialHeadPath, initialHead)) //载入初始截图
    ReportError(task, account, "缺少头像截图", "缺少默认头像截图，无法登录", END_TASK);
  int minDiff = 10000000;//最小方差，小于1000w才能匹配
  int diff = 0;
  for (int i = 0; i < headNum - 1; i++)
  {
    diff = headDifference(initialHead, work[task].pHallShot, head[i].x, head[i].y);//比较与初始头像的相似度
    if (diff < 10000000) //如果某头像与初始头像相似，需要重新刷新
    {
      sprintf_s(message, "%dP头像未加载", account + 1);
      addToRefreshLog(task, message);
      ClickRefresh(task, account);//点击刷新按钮
      CheckSleep(task, 3000);//刷新完等待3秒，然后return -2;
      return -2;//代码-2：头像未加载
    }
    //如果都加载了，则比较该头像当前账号头像截图的相似度
    diff = headDifference(headShot, work[task].pHallShot, head[i].x, head[i].y);
    if (diff < minDiff) //选择相似度更小者
    {
      minDiff = diff;
      headSelected = i;
    }
  }
  if (headSelected == -1) //都加载出来了，还没有匹配的头像，刷新后重新检测
  {
    sprintf_s(message, "%dP未发现与截图匹配的头像", account + 1);
    addToRefreshLog(task, message);
    ClickRefresh(task, account);//点击刷新按钮
    CheckSleep(task, 3000);//刷新完等待3秒，然后return -3;
    return -3;//代码-3：未发现与截图匹配的头像
  }

  int headXInServer = (head[headSelected].x + 43) * DPI / 96 + work[task].hallRect.left - work[task].serverRect.left;
  int headYInServer = (head[headSelected].y + 43) * DPI / 96 + work[task].hallRect.top - work[task].serverRect.top;
  sprintf_s(message, "%dP识别到%d个头像，点击第%d个头像，位置(%d,%d)", account + 1, headNum - 1, headNum - 1 - headSelected, headXInServer, headYInServer);
  addToRefreshLog(task, message);
  char shotPath[maxPath];//截图路径
  char timesString[100] = "开始前";
  if (plot[list].timesRefreshed[plot[list].playingOrder] > 0)
    sprintf_s(timesString, "第%d次中断", plot[list].timesRefreshed[plot[list].playingOrder]);
  sprintf_s(shotPath, "%s\\ID%s_%s_%dP登录.png", work[task].logDirectory,
    GetID(list, plot[list].playingOrder), timesString, account + 1);
  WindowToBitmap(Hall(work[task].hallName[account]), shotPath);
  LeftClick(work[task].hWndServer, headXInServer, headYInServer);

  CheckSleep(task, 5000);//等待5秒
  return 0;//返回WaitServerInWindow函数，继续判断选服界面
}
//从大厅窗口中寻找平台platform的最近服务器，找到了则返回位置
int FindLatestServer(int task, int account, int *px, int *py)
{
  int list = GetList(task);
  int platform = FindLatestServer0(task, account, px, py);
  if (work[task].hallShot)
  {
    free(work[task].hallShot);
    work[task].hallShot = nullptr;
  }
  return platform;
}
//在大厅窗口中等待最近服务器出现，返回所属平台
int WaitServerInWindow(int task, int account, int *px = nullptr, int *py = nullptr)
{
  int list = GetList(task);
  int times = 0;
  while (true)
  {
    times++;
    if (times >= 10)//10秒还未进入预期界面，则报错退出
      ReportError(task, account, "未找到服务器", "未找到最近服务器");
    //判断平台并查找最近服务器位置
    int platform = FindLatestServer(task, account, px, py);
    //找到了4399网页、QQ空间、QQ大厅、4399微端则直接返回
    if (platform >= 1 && platform <= 4)
      return platform;
    //如果在QQ选头像界面出现错误则继续识别
    int counter = 0;
    while (platform < 0)
    {
      counter++;
      if (counter >= 5)//刷新5次还不能解决，则强制中断
      {
        char message[200] = {};
        if (platform == -1)
          sprintf_s(message, "%dP登录界面头像数量不足", account + 1);
        else if (platform == -2)
          sprintf_s(message, "%dP登录界面头像未加载", account + 1);
        else if (platform == -3)
          sprintf_s(message, "%dP登录界面没有与截图匹配的头像", account + 1);
        ReportError(task, account, "无法选择头像", message, END_TASK);
      }
      platform = FindLatestServer(task, account, px, py);//继续识别
    }
    if (GetActiveGameWindow(Hall(work[task].hallName[account]))) //自动出现了游戏窗口，视为微端网址，后续无需操作
      return 0;
    CheckSleep(task, 1000);
  }
  return 0;
}
//等待屏幕中(x,y)位置targetColor颜色消失
void WaitColorDisappear(int task, int account, int x, int y, COLORREF targetColor, const char *tip = nullptr)
{
  COLORREF color = 0;
  int counter = 0;
  do
  {
    counter++;
    if (counter > 100)
    {
      if (tip == nullptr)
      {
        char message[100];
        sprintf_s(message, "(%d,%d)颜色%x存在超时", x, y, targetColor);
        ReportError(task, account, "颜色存在超时", message);
      }
      else
        ReportError(task, account, tip, tip);
    }
    color = GetColor(task, account, x, y);
    CheckSleep(task, 100);
  } while (color == targetColor || color == 0);//如果颜色还没消失，则继续等
}
//高级任务刷新所有游戏窗口
void Refresh(int task)
{
  //重置偏移
  for (int account = 0; account < 2; account++)
    work[task].mapOffsetY[account] = 0;

  int list = GetList(task);
  char timesString[100] = "开始前";
  if (plot[list].timesRefreshed[plot[list].playingOrder] > 0)
    sprintf_s(timesString, "第%d次中断", plot[list].timesRefreshed[plot[list].playingOrder]);
  sprintf_s(plot[list].refreshLog, "%s\\ID%s_%s_刷新记录.txt", work[task].logDirectory,
    GetID(list, plot[list].playingOrder), timesString);

  //刷新记录文件不存在则创建
  WaitForSingleObject(plot[list].hMutexRefreshLog, INFINITE);
  if (!FileExist(plot[list].refreshLog))
  {
    FILE *f;
    if (fopen_s(&f, plot[list].refreshLog, "w") == 0)
    {
      fprintf(f, "刷新原因：%s\n", state[task].errorString);
      strcpy_s(state[task].errorString, "");//记录完刷新原因就清空
      fprintf(f, "DPI=%d\n", DPI);
      fclose(f);
    }
  }
  ReleaseMutex(plot[list].hMutexRefreshLog);

  char message[100] = {};

  bool isRefreshRequired[2] = {};//两个号是否需要刷新
  //同步任务：任务5只刷新1P，任务6只刷新2P
  if (plot[list].playingAdvance->host == 2)
  {
    if (IsDual2P(task))
      isRefreshRequired[1] = true;
    else
      isRefreshRequired[0] = true;
  }
  else //单线程任务：刷新所有参与的号
  {
    for (int account = 0; account < 2; account++)
      if (plot[list].advanceInvolved[account])
        isRefreshRequired[account] = true;
  }
  HWND hWndOrigin[2] = {};//两个账号原来的游戏窗口
  for (int account = 0; account < 2; account++) //双号依次点击刷新按钮
    if (isRefreshRequired[account])
    {
      //点击刷新按钮前记录游戏窗口
      hWndOrigin[account] = GetActiveGameWindow(Hall(param[task].hallName[account]));
      int refreshX = 228 * DPI / 96, refreshY = 44 * DPI / 96;
      LeftClick(Hall(param[task].hallName[account]), refreshX, refreshY); //点击刷新按钮
      sprintf_s(message, "%dP点击刷新按钮(%d,%d)", account + 1, refreshX, refreshY);
      addToRefreshLog(task, message);
    }

  for (int account = 0; account < 2; account++) //双号依次等待原来的游戏窗口关闭
    if (isRefreshRequired[account])
    {
      int counter = 0;
      while (hWndOrigin[account] && hWndOrigin[account] == GetActiveGameWindow(Hall(param[task].hallName[account])))
      {
        counter++;
        if (counter > 100)
          ReportError(task, account, "刷新失败", "刷新按钮点击无效");
        CheckSleep(task, 100);
      }
      sprintf_s(message, "%dP游戏窗口已关闭", account + 1);
      addToRefreshLog(task, message);
    }
  CheckSleep(task, 1000);//游戏窗口关闭后等待1秒

  int platform[2] = {};//两个号的平台
  for (int account = 0; account < 2; account++) //双号依次点击最近服务器
    if (isRefreshRequired[account])
    {
      work[task].hWndServer = GetActiveServerWindow(Hall(param[task].hallName[account]));//找到选服窗口
      sprintf_s(message, "%dP已找到选服窗口(%d)", account + 1, (int)work[task].hWndServer);
      addToRefreshLog(task, message);

      int ServerWidth, ServerHeight;
      GetWindowRect(Hall(param[task].hallName[account]), &work[task].hallRect);//获取大厅窗口位置
      GetWindowRect(work[task].hWndServer, &work[task].serverRect);//获取选服窗口位置
      GetWindowSize(work[task].hWndServer, &ServerWidth, &ServerHeight);
      sprintf_s(message, "%dP已获取选服窗口尺寸(%d*%d)", account + 1, ServerWidth, ServerHeight);
      addToRefreshLog(task, message);

      int x = 0, y = 0;
      platform[account] = WaitServerInWindow(task, account, &x, &y);//等待找到最近服务器，并识别平台类型
      const char *const platformName[5] = { "无需选服", "4399网页", "QQ空间", "QQ大厅", "4399微端" };//平台名称
      sprintf_s(message, "%dP已识别到选服面板，平台为%d(%s)，识别位置(%d,%d)", account + 1, platform[account], platformName[platform[account]], x, y);
      addToRefreshLog(task, message);

      //根据平台和大厅判定坐标确定最近服务器在选服窗口中的坐标
      int latestServerX = 0, latestServerY = 0;//最近服务器在选服窗口中的坐标（缩放后的坐标）
      if (platform[account] == 1) //4399网页：根据识别位置判断
      {
        latestServerX = (x - 285) * DPI / 96 + work[task].hallRect.left - work[task].serverRect.left;
        latestServerY = (y + 45) * DPI / 96 + work[task].hallRect.top - work[task].serverRect.top;
      }
      else if (platform[account] == 2)//QQ空间
      {
        if (plot[list].server[account] < 0 || plot[list].server[account] > 6)
          plot[list].server[account] = 0;
        if (plot[list].server[account] == 0) //空间服最近服务器
        {
          latestServerX = (work[task].serverRect.right - work[task].serverRect.left) / 2 - 115 * DPI / 96;//中央位置左偏115
          latestServerY = 267 * DPI / 96;//固定坐标267
        }
        else //3366固定服务器
        {
          const int Server3366X = 176, Server3366Y = 571, Server3366Width = 184, Server3366Height = 36;
          int centerX = (work[task].serverRect.right - work[task].serverRect.left) / 2;//中心X位置
          latestServerX = centerX + (Server3366X - (plot[list].server[account] - 1) % 3 * Server3366Width) * DPI / 96;//中央位置左偏115
          latestServerY = (Server3366Y - (plot[list].server[account] - 1) / 3 * Server3366Height) * DPI / 96;//固定坐标267
        }
      }
      else if (platform[account] == 3) //QQ大厅
      {
        latestServerX = (work[task].serverRect.right - work[task].serverRect.left) / 2 + 30 * DPI / 96;//中央位置右偏30
        latestServerY = 580 * DPI / 96;//固定坐标580
      }
      else if (platform[account] == 4) //4399微端
      {
        latestServerX = (x - 264) * DPI / 96 + work[task].hallRect.left - work[task].serverRect.left;
        latestServerY = (y + 41) * DPI / 96 + work[task].hallRect.top - work[task].serverRect.top;
      }

      sprintf_s(message, "%dP最近服务器在选服窗口中的位置为(%d,%d)", account + 1, latestServerX, latestServerY);
      addToRefreshLog(task, message);
      CheckSleep(task, 500);

      param[task].hWnd[account] = NULL;//重新获取游戏窗口
      int counter = 0;
      while (param[task].hWnd[account] == NULL) //每1000ms获取1次游戏窗口
      {
        counter++;
        if (counter > 10)
          ReportError(task, account, "无法进入服务器", "无法进入服务器");
        if (platform[account] >= 1 && platform[account] <= 4) //4399网页、QQ空间、QQ大厅、4399微端需要点击最近服务器
        {
          char serverShotPath[maxPath];//选服截图

          char timesString[100] = "开始前";
          if (plot[list].timesRefreshed[plot[list].playingOrder] > 0)
            sprintf_s(timesString, "第%d次中断", plot[list].timesRefreshed[plot[list].playingOrder]);
          sprintf_s(serverShotPath, "%s\\ID%s_%s_%dP第%d次选服.png", work[task].logDirectory,
            GetID(list, plot[list].playingOrder), timesString, account + 1, counter);

          WindowToBitmap(Hall(param[task].hallName[account]), serverShotPath);

          LeftClick(work[task].hWndServer, latestServerX, latestServerY);//点击最近服务器

          sprintf_s(message, "%dP点击最近服务器(%d,%d)", account + 1, latestServerX, latestServerY);
          addToRefreshLog(task, message);
        }
        CheckSleep(task, 1000);
        param[task].hWnd[account] = GetActiveGameWindow(Hall(param[task].hallName[account]));
      }
      sprintf_s(message, "%dP已进入服务器，游戏窗口(%d)", account + 1, (int)param[task].hWnd[account]);
      addToRefreshLog(task, message);
    }

  //游戏窗口已出现，双号依次等待进入游戏
  for (int account = 0; account < 2; account++)
    if (isRefreshRequired[account])
    {
      work[task].hWnd[account] = param[task].hWnd[account];
      const char *jumpButton[] = { "跳转按钮", "跳转按钮_高亮", "假期特惠" };
      WaitPictures(APPEAR, task, account, jumpButton, NULL, NULL, 0, 0, 60000);//等待出现跳转按钮，最多等60秒
      //非QQ大厅还要等健康提示
      if (platform[account] == 0 || platform[account] == 1 || platform[account] == 2 || platform[account] == 4)
        TryWaitPicture(APPEAR, task, account, "健康提示", WaitTime());
      sprintf_s(message, "%dP刷新成功", account + 1);
      addToRefreshLog(task, message);
    }
}
//根据大厅句柄hWndHall和坐标Account绑定游戏窗口hWndGame。成功绑定返回1，原本就有窗口返回2
int BindWindow(int task, int account, HWND &hWndGame, char(&hallName)[maxPath], POINT Tag)
{
  char message[1000] = {};
  HWND hWndHall = Hall(hallName);
  sprintf_s(message, "未找到游戏大厅：\n%s", hallName);

  //如果无法从大厅名称获取大厅句柄，报错“未找到游戏大厅”
  if (!hWndHall)
    ReportError(task, account, "未找到游戏大厅", message, END_TASK);

  //需要重新绑定的情形：1. hWndGame已失效 2. hWndGame所属大厅无法获取或不是hWndHall
  bool isBindRequired = false;
  //游戏窗口失效：需要重新绑定
  if (!IsGameWindow(hWndGame))
    isBindRequired = true;
  //游戏窗口有效：若所属大厅无法获取或不是hWndHall则需要重新绑定
  else if (GetHallWindow(hWndGame) != hWndHall)
    isBindRequired = true;

  if (isBindRequired)//如果hWndGame不存在或已失效，则通过大厅句柄寻找游戏窗口句柄
  {
    //大厅窗口不存在，报错“未找到游戏大厅”
    if (!IsWindow(hWndHall))
      ReportError(task, account, "未找到游戏大厅", message, END_TASK);

    char ClassName[256];
    GetClassName(hWndHall, ClassName, 256);//获取窗口类名

    //不是游戏大厅或微端，直接退出
    if (strcmp(ClassName, "ApolloRuntimeContentWindow") != zero && strcmp(ClassName, "DUIWindow") != zero)
      ReportError(task, account, "未找到游戏窗口",
        "未找到游戏窗口。本程序只能用于\n360游戏大厅（极速模式）或微端。", END_TASK);

    //运行到这里说明是游戏大厅或微端
    if (!PostMessage(hWndHall, WM_MOUSEMOVE, 0, MAKELPARAM(0, 0))) //检验权限
      ReportError(task, account, "权限不足", "权限不足。请先关闭当前窗口，然后\n右键执行器选择“以管理员身份运行”。", END_TASK);

    if (GetWindowLong(hWndHall, GWL_STYLE) & WS_MAXIMIZE)//如果窗口最大化，点击高度-8
      LeftClick(hWndHall, Tag.x, Tag.y - 8 * DPI / 96);//点击账号标签
    else
      LeftClick(hWndHall, Tag.x, Tag.y);//点击账号标签
    CheckSleep(task, 500);
    hWndGame = GetActiveGameWindow(hWndHall);//获取小号窗口

    if (!hWndGame) //如果无法通过大厅句柄获取游戏窗口句柄，则刷新或中断
    {
      if (IsAdvance(task)) //高级任务：刷新两个账号
        ReportError(task, account, "未找到游戏窗口", "未找到游戏窗口");
      else
        ReportError(task, account, "未找到游戏窗口", "未找到游戏窗口。本程序只能用于\n360游戏大厅（极速模式）或微端。");
    }
    return 1;
  }

  //如果hWndGame已存在，向hWndGame检验权限
  if (!PostMessage(hWndGame, WM_MOUSEMOVE, 0, MAKELPARAM(0, 0))) //检验权限
    ReportError(task, account, "权限不足", "权限不足。请先关闭当前窗口，然后\n右键执行器选择“以管理员身份运行”。", END_TASK);
  return 2;
}
//检测是否处于岛屿主界面，并校准偏移值
int CheckOffsetAdvance(int task, int account)
{
  //检查内容：排行和假期特惠有一即可
  int list = GetList(task);
  const char *ranking[] = { "排行", "假期特惠" };
  int counter = 0;
  //对于偏移0和偏移-2，分别检查是否处于岛屿主界面
  for (int offset = 0; offset >= -2; offset -= 2)
    if (plot[list].advanceInvolved[account])
    {
      //判断是否处于岛屿主界面
      if (FindPicturesInMap(task, ranking, nullptr, nullptr, 0, offset))
      {
        work[task].mapOffsetY[account] = offset;
        return 1;
      }
    }
  return 0;
}
// 高级任务启动时绑定Param.hWnd。注意放到第一个任务线程里面去，放在传参到Work之前。
// 如果无法绑定，或者窗口不在可跳转界面，执行刷新。
void AdvanceBindWindow(int task)
{
  int list = GetList(task);
  if (plot[list].isAdvanceWindowBound == 0)
  {
    time(&work[task].taskStartTime);

    for (int account = 0; account < 2; account++)//根据高级任务的参与人数绑定账号
      if (plot[list].advanceInvolved[account] == 1)
        BindWindow(task, account, param[task].hWnd[account], param[task].hallName[account], param[task].tag[account]);

    for (int account = 0; account < 2; account++)//验证所有参与账号是否显示
      if (plot[list].advanceInvolved[account])
      {
        if (!IsGameWindowVisible(param[task].hWnd[account]))
          ReportError(task, account, "账号未显示",
            "所有参与刷图的账号都要显示。\n双开请点击大厅右上角“多窗口玩游戏”。", END_TASK);

        //work.hWnd临时填写parma.hWnd，以便使用识图函数
        work[task].hWnd[account] = param[task].hWnd[account];
        work[task].mapOffsetY[account] = 0;//偏移预设为0
        ForcedUpdateMap(task, account);
        int islandState = CheckOffsetAdvance(task, account);//偏移校准
        //账号已显示，如果不在角色头像显示界面，刷新两个账号
        if (islandState == 0)
          ReportError(task, account, "不在岛屿主界面", "不在岛屿主界面");
        work[task].hWnd[account] = 0;
      }
    plot[list].isAdvanceWindowBound = 1;
  }
}
//每个任务启动时通过Work.hWndHall绑定Work.hWnd，并根据游戏窗口大小判断点击时是否需要DPI修正
void BindWindow(int task, int account)
{
  int result = BindWindow(task, account, work[task].hWnd[account], work[task].hallName[account], work[task].tag[account]);
  char message[100];
  if (result == 1)
    sprintf_s(message, "已找到%dP窗口\n", account);
  else
    sprintf_s(message, "已继承%dP窗口\n", account);
  if (IsNormal(task)) //普通任务顺带更新Param句柄
    param[task].hWnd[account] = work[task].hWnd[account];
  strcat_s(work[task].embarkString, message);
  RECT GameRect;
  GetClientRect(work[task].hWnd[account], &GameRect);//记录游戏窗口大小
  if (GameRect.right > 950)
    work[task].isDpiAwareRequired[account] = 1;//如果游戏窗口宽度大于950，则需要DPI修正
  else
    work[task].isDpiAwareRequired[account] = 0;//否则不需要修正
}
//清空work[task]，保留前8字节（截图的内存和地址）
void ResetWork(int task)
{
  memset((char *)&work[task] + 8, 0, sizeof(work[task]) - 8);//清空work[task]
}
//清空卡槽，保留卡片名、是否棉麦、是否放置一次
void ResetSlot(Slot &slot)
{
  memset((char *)&slot + 13, 0, sizeof(slot) - 13);//清空slot
}
//读取轨道、绑定窗口并切换到放卡账号（读取公会任务不读轨道）
void LoadTrackAndBindWindow(int task)
{
  strcat_s(work[task].embarkString, "已进入启动程序\n");

  //初始化波数、本局用时、平均用时、启动时间
  work[task].wave = waveOutOfLevel;//初始状态为未进入关卡
  work[task].currentTime = 0;
  work[task].averageTime = 0;
  time(&work[task].taskStartTime);

  //普通任务或高级任务有轨关卡需要载入轨道
  if (IsNormal(task) || !IsNoTrack(*plot[GetList(task)].playingAdvance))
  {
    for (int account = 0; account < 2; account++) //读取已填轨道信息
      if (strlen(work[task].track[account]) > (size_t)zero)
      {
        LoadTrack(task, account); //完整读取轨道（第1局执行前）
        sprintf_s(work[task].backupTrack[account], "自动备份\\Actuator%d%d.txt", task, account);
        Copy(work[task].pureTrack[account], work[task].backupTrack[account]);
      }
  }
  strcat_s(work[task].embarkString, "已读取轨道\n");

  //绑定窗口句柄
  HWND hWndExist = NULL;
  for (int account = 0; account < 2; account++)
    if (work[task].isInvolved[account] == 1)
    {
      BindWindow(task, account);
      if (hWndExist && hWndExist == work[task].hWnd[account])
        ReportError0(task, "绑定冲突", "1P和2P绑定了同一个账号", END_TASK);
      hWndExist = work[task].hWnd[account];
    }

  //开局切换账号
  for (int account = 0; account < 2; account++)
    if (work[task].isPerformed[account])//如果小号1放卡，若小号1不可见，则点击切换到小号1
    {
      if (!IsGameWindowVisible(work[task].hWnd[account]))
      {
        if (GetWindowLong(Hall(work[task].hallName[account]), GWL_STYLE) & WS_MAXIMIZE)//如果窗口最大化，点击高度-8
          LeftClick(Hall(work[task].hallName[account]), work[task].tag[account].x, work[task].tag[account].y - 8 * DPI / 96);
        else
          LeftClick(Hall(work[task].hallName[account]), work[task].tag[account].x, work[task].tag[account].y);
        CheckSleep(task, 500);
      }
      break;
    }
}
//重置状态信息（保留错误原因，不然刷新记录看不到）
void ResetState(int task)
{
  char errorString[100];
  strcpy_s(errorString, state[task].errorString);
  memset(&state[task], 0, sizeof(state[task]));
  strcpy_s(state[task].errorString, errorString);
  sprintf_s(state[task].tip, "未开始");//任务状态
}
//将param[task]传递到work[task], 记录任务已启动，返回1
int TaskReady(int task, int(&Involved)[2], int(&Performed)[2])
{
  ResetWork(task);//重置Work[task]
  ResetState(task);//重置任务task的状态
  work[task].isInvolved[0] = Involved[0];//记录账号是否参与和放卡
  work[task].isInvolved[1] = Involved[1];
  work[task].isPerformed[0] = Performed[0];
  work[task].isPerformed[1] = Performed[1];
  GetWorkFromParam(task);//获取任务工作参数
  return 1;
}
//是否为魔塔任务（含普通任务）
bool IsTowerTask(int task)
{
  if (IsAdvance(task))
  {
    int list = GetList(task);
    return plot[list].playingAdvance->type == 2;
  }
  else
    return work[task].tower[0] > 0;
}
//是否为巅峰对决任务
bool IsPeakTask(int task)
{
  if (IsAdvance(task))
  {
    int list = GetList(task);
    return IsPeak(*plot[list].playingAdvance);
  }
  else
    return false;
}
//是否为单塔或密室（含普通任务的单塔）
bool IsSingleTowerTask(int task)
{
  if (IsAdvance(task))
  {
    int list = GetList(task);
    return plot[list].playingAdvance->type == 2 &&
      (plot[list].playingAdvance->level / 1000 == 1 || plot[list].playingAdvance->level / 1000 == 4);
  }
  else
    return work[task].tower[0] > 0;
}
//检查param[task]能否启动，能则传递参数到work[task]，不能则报错
int IsParamRunnable(int task, bool isDualTask = false)
{
  ResetWork(task);//清空work[task]

  // 非同步任务的2P任务槽不需要从param获取参数
  // 只需要把games设为1，maxGames设为0，这样就不会在StartQuest中创建2P线程
  if (IsDual2P(task) && isDualTask == false)
  {
    work[task].games = 1;
    work[task].maxGames = 0;
    return 1; //检查通过
  }

  char info[2][1000] = {};//检查轨道文件时输出的错误信息
  int isTrackLegal[2] = {};//两个轨道的检查结果
  int trackNum = 0;//有效轨道数量
  int isInvolved[2] = {};//两个账号是否参与
  int isPerformed[2] = {};//两个账号是否放卡

  //如果是识别任务或特殊任务，不检查轨道，直接通过
  if (IsAdvance(task))
  {
    int list = GetList(task);
    if (IsMission(*plot[list].playingAdvance) || IsNoBattle(*plot[list].playingAdvance))
    {
      for (int account = 0; account < 2; account++)
        isInvolved[account] = isPerformed[account] = plot[list].playingAdvance->deck[account] ? 1 : 0;
      return TaskReady(task, isInvolved, isPerformed);//准备启动任务
    }
  }

  //战斗任务需要检查轨道
  for (int account = 0; account < 2; account++)
    if (strlen(param[task].track[account]) > 0)//若轨道存在
    {
      isTrackLegal[account] = LoadTrack(task, account, info[account]);//载入并检查轨道
      isInvolved[account] = isPerformed[account] = 1;//账号参与+放卡
      trackNum++;
    }
    else //轨道不存在
      isTrackLegal[account] = -1;

  /*下列报错只在普通任务出现，因为高级任务开始前会一次性检查好*/
  if (trackNum == 0)//两个轨道都没填
    PopMessage(task, "请至少填写一个轨道");
  else if (isTrackLegal[0] == 0)//如果轨道1无法正常载入，弹窗提示
    PopMessage(task, info[0]);
  else if (isTrackLegal[1] == 0)//如果轨道2无法正常载入，弹窗提示
    PopMessage(task, info[1]);
  //如果能成功载入
  else if (trackNum == 2 && (param[task].tag[0].x == -1 || param[task].tag[1].x == -1))
    PopMessage(task, "双轨刷图需要抓取两个账号标签。\n如需单人刷图，请删除一个轨道。");
  //轨道数为2，但缺少一个账号标签时
  else if (strlen(param[task].track[0]) > 0 && param[task].tag[0].x == -1)
    PopMessage(task, "请抓取房主标签。");
  else if (strlen(param[task].track[1]) > 0 && param[task].tag[1].x == -1)
    PopMessage(task, "请抓取队友标签。");
  /*排除了上述错误后，双轨刷图必然已有两个标签，单轨刷图也至少有了对应账号的标签。
  但有可能使用单个双人轨道，而只抓取了一个标签*/
  else //没有错误才执行任务
  {
    //是否有账号卡组为×
    bool isDeckCanceled = IsAdvance(task) && (param[task].deck[0] == 0 || param[task].deck[1] == 0);
    //单轨且没有账号卡组为×时，检查是否为双人轨道
    if (trackNum == 1 && !isDeckCanceled)
      for (int account = 0; account < 2; account++)
        if (isPerformed[account] && GetTrackRoleNum(task, account) == 2) //若为双人轨道
        {
          //只有1个标签，询问是否按单人模式启动
          if (param[task].tag[0].x == -1 || param[task].tag[1].x == -1)
          {
            int option = PopMessageOK(task, "您使用的是双人轨道，\n但只抓取了一个账号标签，\n是否以单人模式启动？");
            if (option != IDOK)
              return 0;
          }
          else  //确认为双人刷图，则记录两个号都参与
            isInvolved[0] = isInvolved[1] = 1;
        }

    //普通任务魔塔轨道2P参与（高级任务work[task].tower[account]强制为0，不会触发此报错）
    if (work[task].tower[0] + work[task].tower[1] > 0 && isInvolved[1])
    {
      PopMessage(task, "魔塔轨道仅限房主单人使用。");
      return 0;
    }
    //刷技能模式
    if (work[task].isSkillMode)
    {
      if (!IsSingleTowerTask(task)) //如果不是单人魔塔
      {
        PopMessage(task, "刷技能轨道只能用于单塔或密室。");
        return 0;
      }
      //普通任务在这里提示，高级任务就不在这里提示了
      if (!IsAdvance(task))
      {
        char tip[300] = "刷技能要求:\n"
          "1. 所有卡片至少1星；\n"
          "2. 操作速度建议调至“快”。\n"
          "点击确认继续执行。";
        int Options = PopMessageOK(task, tip);
        if (Options != IDOK)
          return 0;
      }
    }
    return TaskReady(task, isInvolved, isPerformed);//准备启动任务
  }
  return 0;
}
//只在慢速模式（OperationSpeed==0）下等待
void SelectiveSleep(int task, int time)
{
  if (operationSpeed == 0)//慢速模式：全额等待
    CheckSleep(task, time);
  else if (operationSpeed == 1) //中速模式：等待一半时间
    CheckSleep(task, time / 2);
  else
    CheckSleep(task, 0);//快速模式：无需等待
}
//跳转到竞技岛
void OldJumpToContest(int task, int account)
{
  const POINT Jump = { 870, 560 };//跳转按钮
  const POINT ContestIsland = { 870, 384 };//跳转到竞技岛

  if (FindPicture(task, account, "竞技") == 0)//如果不在竞技岛
  {
    if (FindPicture(task, account, "跳转列表") == 0)
    {
      Click(task, account, Jump);//点击跳转按钮
      WaitPicture(APPEAR, task, account, "跳转列表");//等待打开跳转列表
    }
    Click(task, account, ContestIsland);//跳转到竞技岛
    WaitPicture(APPEAR, task, account, "竞技", nullptr, nullptr, 0, 0, WaitTime() * 2);//等待进入竞技岛界面
  }
  WaitColorDisappear(task, account, blueBox.x, blueBox.y, boxColor);//等待服务器蓝框消失
  CheckSleep(task, 200);
}
//点击活动图标name，进入活动。report：找不到图标是否报错
bool ClickActivity(int task, int account, char *name, bool report = true)
{
  int SwitchActivityX = 787, SwitchActivityY = 35;//切换活动
  int x, y;
  if (TryWaitPicture(APPEAR, task, account, name, 500, &x, &y) == 0)//如果没找到图标
  {
    Click(task, account, SwitchActivityX, SwitchActivityY);//点一下箭头
    if (report) //报错模式：强制等待图标出现
      WaitPicture(APPEAR, task, account, name, &x, &y);
    //非报错模式：尝试等待图标出现，等不到就返回false
    else if (!TryWaitPicture(APPEAR, task, account, name, 500, &x, &y))
      return false;
  }
  Click(task, account, x, y);//点击悬赏图标
  return true;
}
//领取悬赏奖励。level>=0为开局领奖（需检查第level关能否挑战），level==-1为打完领奖。
void ClaimBountyReward(int task, int account, int level = -1)
{
  int list = GetList(task);
  const POINT closeBounty = { 909, 36 };
  //4个关卡领奖按钮的位置
  const POINT claim[4] = { { 174, 411 }, { 344, 380 }, { 518, 376 }, { 690, 414 } };
  const char *claimButton[] = { "领取悬赏", "领取悬赏_亮" };//领取按钮图片名
  const int claimWidth = 107, claimHeight = 38;//领奖按钮尺寸
  if (level == -1)
  {
    ClickActivity(task, account, "悬赏图标");//点击悬赏图标
    WaitPicture(APPEAR, task, account, "悬赏界面");//等待打开悬赏界面
    CheckSleep(task, 2000);//等待奖励加载出来
  }
  DealInvitation(task, account, 0);//拒绝邀请
  if (level == -1) //如果是打完领奖，多等5秒，以免漏领
    TryWaitPictures(APPEAR, task, account, claimButton, WaitTime() / 2);//等待奖励出现
  int x = 0, y = 0;
  int counter = 0;
  //每次领奖等待1秒，防止识图失败
  while (TryWaitPictures(APPEAR, task, account, claimButton, 1000, &x, &y))
  {
    counter++;
    if (counter > 10)
      break;
    Click(task, account, x, y);//点击领取奖励
    CheckSleep(task, 1000);
    DealInvitation(task, account, 0);
  }
  if (counter > 10)//如果领取失败
  {
    if (level == -1)//打完后领奖失败：记录本关状态为“未领奖”
      plot[list].playingAdvance->result = 1;
    else //开局领奖失败：能建房就建房打，不能就跳过
    {
      //如果要打的关有“领取”字样，只能跳过任务
      if (FindPictures(task, account, claimButton, &x, &y, claim[level].x, claim[level].y,
        claim[level].x + claimWidth, claim[level].y + claimHeight))
        ReportError(task, account, "悬赏领奖失败", "悬赏领奖失败", SKIP_TASK);
      //如果要打的关没有“领取”字样则忽略，继续建房开打（打完领不了会记录）
    }
  }
  if (level == -1)
    Click(task, account, closeBounty);//点击关闭悬赏
}
//进入情侣任务列表
void OpenLoverMission(int task, int account)
{
  const POINT jump = { 870, 560 };//跳转按钮
  const POINT loverMission = { 870, 300 };//跳转到情侣任务
  const POINT uncheck = { 608, 401 };//拒绝邀请+取消选中情侣任务
  if (FindPicture(task, account, "跳转列表") == 0)
  {
    Click(task, account, jump);//点击跳转按钮
    WaitPicture(APPEAR, task, account, "跳转列表");//等待打开跳转列表
  }
  Click(task, account, loverMission);//跳转到公会任务
  WaitPicture(APPEAR, task, account, "情侣任务");//等待进入情侣任务界面
  WaitPicture(DISAPPEAR, task, account, "情侣任务未加载");//等待情侣任务加载完成
  CheckSleep(task, 500);
  Click(task, account, uncheck);//取消选中任务
  CheckSleep(task, 100);
  Click(task, account, uncheck);
  CheckSleep(task, 500);//等待任务取消选中，开始识图
}
//进入日常任务列表
void OpenDailyMission(int task, int account)
{
  const POINT dailyMission = { 640, 560 };//日常任务

  Click(task, account, dailyMission);//点击日常任务
  WaitPicture(APPEAR, task, account, "任务看板");//等待进入任务看板
  CheckSleep(task, 1000);//等待1000ms防卡
}
//进入公会任务列表
void OpenGuildMission(int task, int account)
{
  const POINT jump = { 870, 560 };//跳转按钮
  const POINT guildMission = { 870, 257 };//跳转到公会任务
  const POINT uncheck = { 608, 401 };//拒绝邀请+取消选中公会任务
  if (FindPicture(task, account, "跳转列表") == 0)
  {
    Click(task, account, jump);//点击跳转按钮
    WaitPicture(APPEAR, task, account, "跳转列表");//等待打开跳转列表
  }
  Click(task, account, guildMission);//跳转到公会任务
  WaitPicture(APPEAR, task, account, "公会任务");//等待进入公会任务界面
  WaitPicture(DISAPPEAR, task, account, "完成次数");//等待“完成次数”字样消失
  CheckSleep(task, 1000);//等待系统自动选中第一个任务
  Click(task, account, uncheck);//取消选中任务
  CheckSleep(task, 100);
  Click(task, account, uncheck);
  CheckSleep(task, 500);//等待任务取消选中，开始识图
}
const POINT closeDailyMission = { 854, 52 };//关闭日常任务界面
const POINT closeGuildMission = { 853, 56 };//关闭公会任务界面
const POINT closeLoverMission = { 845, 62 };//关闭情侣任务界面
const POINT closeContestMission = { 887, 51 };//关闭大赛任务界面
//领取情侣任务奖励
void ClaimLoverAward(int task, int account, bool isOpenRequired = true)
{
  int list = GetList(task);

  //如果已经领奖失败过，就不反复领了
  if (plot[list].isLoverClaimFailed)
    return;

  if (isOpenRequired)
    OpenLoverMission(task, account);//进入公会任务面板

  int x, y;
  int counter = 0;
  DealInvitation(task, account, 0);//先处理邀请再检验
  const char *claimButton[] = { "情侣任务可领取", "情侣任务可领取_亮" };
  while (FindPictures(task, account, claimButton, &x, &y))
  {
    counter++;
    if (counter > 15)
    {
      plot[list].isLoverClaimFailed = true;
      break;
    }
    Click(task, account, x + 5, y + 5);//点击已完成(+5是为了防止点边角无效）
    CheckSleep(task, 500);
    //等待领取按钮消失
    if (!WaitPictures(DISAPPEAR, task, account, claimButton, nullptr, nullptr, x, y, WaitTime() / 2, false))
    {
      plot[list].isLoverClaimFailed = true;
      break;
    }
    CheckSleep(task, 500);
    DealInvitation(task, account, 0);
  }
  if (isOpenRequired)
    Click(task, account, closeLoverMission);//关闭情侣任务
}
const int claimDaily = 0, claimGuild = 1;//0领取日常任务 1领取公会任务
//领取本页任务奖励（label：0日常任务 1公会任务）
void ClaimAwardInPage(int task, int account, int label)
{
  int list = GetList(task);
  bool &failed = label == claimDaily ? plot[list].isDailyClaimFailed : plot[list].isGuildClaimFailed;
  const char *questPanel = label == claimDaily ? "任务看板" : "公会任务";//任务面板名称

  POINT claimButton = { 642, 533 };//领取按钮

  int x, y;
  int counter = 0;
  while (true)
  {
    WaitPicture(APPEAR, task, account, questPanel);//等待任务看板重新出现（防止升级等横幅遮挡）
    DealInvitation(task, account, 0);//处理邀请
    if (!FindPicture(task, account, "任务完成", &x, &y)) //没有检测到"完成"则结束
      break;

    counter++;
    if (counter > 15)
    {
      failed = true;
      break;
    }
    Click(task, account, x + 5, y + 5);//点击已完成(+5是为了防止点边角无效）
    CheckSleep(task, 500);
    Click(task, account, claimButton);//点击领取按钮
    //等待"完成"变成"已领取"（升级时可能导致误判领取失败，但问题不大）
    if (!WaitPicture(APPEAR, task, account, "任务已领取", nullptr, nullptr, x, y, WaitTime() / 2, false))
    {
      failed = true;
      break;
    }
    CheckSleep(task, 500);
    DealInvitation(task, account, 0);
  }
}
//领取日常任务奖励
void ClaimDailyAward(int task, int account, bool isOpenRequired = true)
{
  int list = GetList(task);
  if (plot[list].isDailyClaimFailed) //已触发领奖失败则跳过
    return;

  if (isOpenRequired)
    OpenDailyMission(task, account);//进入任务面板

  //翻页并领取奖励
  const int dailyScrollX = 413, dailyScrollY = 145, dailyScrollHeight = 370;//滚动条位置
  COLORREF dailyScroll[dailyScrollHeight][1] = {};//滚动条
  for (int page = 0; page < 20; page++)
  {
    if (!PageScroll(task, account, page, dailyScrollX, dailyScrollY, dailyScroll))
      break;
    CheckSleep(task, 500);//由于每日任务滚动条变化和任务变化不同步，需要另行等待
    ForcedUpdateMap(task, account);//截图
    ClaimAwardInPage(task, account, claimDaily);//领取本页奖励
  }

  if (isOpenRequired)
    Click(task, account, closeDailyMission);//关闭任务面板
}
//领取公会任务奖励
void ClaimGuildAward(int task, int account, bool isOpenRequired = true)
{
  int list = GetList(task);
  if (plot[list].isGuildClaimFailed) //已触发领奖失败则跳过
    return;

  if (isOpenRequired)
    OpenGuildMission(task, account); //进入任务面板
  ClaimAwardInPage(task, account, claimGuild);//领取本页奖励
  if (isOpenRequired)
    Click(task, account, closeGuildMission); //关闭任务面板
}
const int contestX = 137, contestY = 357, contestWidth = 315, contestHeight = 206;//大赛识别区
//是否为大赛文字颜色
bool IsContestTextColor(COLORREF color)
{
  int brightness = Brightness(color);
  return brightness >= 100 && brightness <= 250;
}
//给定分界线位置divideY，判断y是否为某个大赛任务的文字区
bool IsContestRange(int y, int divideY)
{
  const double contestDistance = 55.3333;//大赛任务间距
  double diff = (double)y - (double)divideY;//位置y与判定线的浮点距离
  //把浮点距离调节到[0,55.3333)范围内
  while (diff < 0)
    diff += contestDistance;
  while (diff >= contestDistance)
    diff -= contestDistance;
  if (diff < 17 || diff > 49) //如果Y不在文字区范围，返回false
    return false;
  //在文字区的前提下，还要求该文字区的上下界不越界
  double beginY = (double)y - diff + 17;//文字区上界
  double endY = (double)y - diff + 49;//文字区下界
  return beginY >= contestY && endY <= contestY + contestHeight - 1;
}
//保存新文字图片，保存成功返回true，已有文字返回false
bool SaveNewText(COLORREF(&textImage)[textHeight][textWidth], int width)
{
  COLORREF image[textHeight][textWidth] = {};
  char path[maxPath] = {};
  //查重
  for (int i = 0; i < 999; i++)
  {
    sprintf_s(path, "附加程序\\新文字\\%d.png", i);
    //若路径不存在，直接保存到该路径
    if (!FileExist(path))
    {
      ColorToBitmap(textImage, path, 0, 0, width, textHeight);
      return true;
    }
    //若路径存在，检查是否与本图片重复，重复则无需保存
    int bmpWidth = 0, bmpHeight = 0;
    BitmapToColor(path, image, &bmpWidth, &bmpHeight);
    if (bmpWidth == width && IsBitmapEqual(image, textImage, width, textHeight))
      return false;
  }
  return false;
}
//图像是否全黑
template <int height, int width>
bool IsImageEmpty(COLORREF(&image)[height][width])
{
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      if (image[y][x])
        return false;
  return true;
}
//识别一个大赛文字，记录到text中
void ReadContestText(int task, int account, char(&text)[4], int &textX, int textY)
{
  //1. 生成文字黑白图像
  COLORREF textImage[textHeight][textWidth] = {}; //14x14文字图像
  for (int y = 0; y < textHeight; y++)
    for (int x = 0; x < textWidth; x++)
      textImage[y][x] = IsContestTextColor(work[task].map[textY + y][textX + x]) ? 0xffffff : 0;
  bool isImageEmpty = IsImageEmpty(textImage);//文字是否全空
  //2. 查询文字
  if (TrieSearch(text, textImage, 14)) //如果匹配14宽文字，textX右移15
    textX += 15;
  else if (TrieSearch(text, textImage, 7)) //如果匹配7宽文字，textX右移8
    textX += 8;
  else //如果识别失败
  {
    if (isImageEmpty) //如果文字是空的，说明到达行尾，直接右移15px 
      textX += 15;
    else //如果文字不是空的，说明是缺字
    {
      COLORREF textImage2[textHeight][textWidth] = {}; //右移8格后的文字图像
      char text2[4] = {};//右移8格后识别到的文字
      for (int y = 0; y < textHeight; y++)
        for (int x = 0; x < textWidth; x++)
          textImage2[y][x] = IsContestTextColor(work[task].map[textY + y][textX + 8 + x]) ? 0xffffff : 0;

      //如果右移8格后全空或可以识别，添加1字节通配符
      if (IsImageEmpty(textImage2) || TrieSearch(text2, textImage2, 14) || TrieSearch(text2, textImage2, 7))
      {
        strcpy_s(text, "*");//补上通配符
        SaveNewText(textImage, 7);//保存新字图片
        textX += 8;
      }
      else //否则添加2字节通配符
      {
        strcpy_s(text, "**");//补上通配符
        SaveNewText(textImage, 14);//保存新字图片
        textX += 15;
      }
    }
  }
}
//识别一行大赛文字，添加到lineText中
void ReadContestLine(int task, int account, char(&lineText)[200], int textY)
{
  int textX = contestX;
  char text[4] = {};
  while (textX < contestX + contestWidth - 6)
  {
    ReadContestText(task, account, text, textX, textY);
    strcat_s(lineText, text);
  }
}
//根据类型名称查找类型编号
int FindType(const char *typeString)
{
  for (int type = 0; type < maxTypeNum; type++)
    if (strcmp(typeName[type], typeString) == 0)
      return type;
  return -1;
}
//在指定类型下根据关卡名称查找关卡编号
int FindLevel(int type, const char *levelString)
{
  for (int level = 0; level < levelNum[type]; level++)
  {
    char levelName[100] = {};
    GetLevelName(levelName, type, level);
    if (strcmp(levelName, levelString) == 0)
      return level;
  }
  return -1;
}
//关卡全称对应表
struct LevelMap
{
  int code;//编号（编号小的优先做）
  char name[20];//关卡全称
  int type;//类型
  int level;//关卡
};
const int maxLevelMapNum = 200;
int levelMapNum;
LevelMap levelMap[maxLevelMapNum];
//读取关卡全称对应表
void LoadLevelMap()
{
  const char path[] = "附加程序\\文本\\关卡全称表.txt";
  FILE *f;
  if (fopen_s(&f, path, "r"))
    ReportMissingFile(path);
  //文本格式：全称\t类型\t关卡
  char s[1000] = {};
  levelMapNum = 0;
  fgets(f, s);//跳过标题行
  fgets(f, s);
  while (strlen(s) > 0)
  {
    memset(&levelMap[levelMapNum], 0, sizeof(levelMap[levelMapNum]));
    levelMap[levelMapNum].code = levelMapNum;
    char *context = nullptr;
    char *name = strtok_s(s, "\t", &context);//第一段：关卡全名，如"色拉岛（陆）"
    strcpy_s(levelMap[levelMapNum].name, name);
    char *typeString = strtok_s(nullptr, "\t", &context);//第二段：类型
    levelMap[levelMapNum].type = FindType(typeString);
    char *levelString = strtok_s(nullptr, "\t", &context);//第三段：关卡
    levelMap[levelMapNum].level = FindLevel(levelMap[levelMapNum].type, levelString);
    levelMapNum++;
    fgets(f, s);
  }
  fclose(f);
}
Contest guildSequence[maxAdvanceNum];//公会顺序表
Contest loverSequence[maxAdvanceNum];//情侣顺序表
int guildSequenceSize;
int loverSequenceSize;
//载入顺序表
void LoadSequence(const char *path, Contest(&sequence)[maxAdvanceNum], int &sequenceSize)
{
  FILE *f;
  if (fopen_s(&f, path, "r"))
    ReportMissingFile(path);

  char s[1000] = {};
  sequenceSize = 0;
  fgets(f, s);
  for (int order = 0; order < maxAdvanceNum && strlen(s) > 0; order++)
  {
    //序号 类型 关卡 人数 BOSS 名称 要求
    char *context = nullptr;
    char *idString = strtok_s(s, "\t", &context);//第一段：序号
    char *typeString = strtok_s(nullptr, "\t", &context);//第二段：类型
    char *levelString = strtok_s(nullptr, "\t", &context);//第三段：关卡
    char *roleString = strtok_s(nullptr, "\t", &context);//第四段：人数
    char *bossString = strtok_s(nullptr, "\t", &context);//第五段：BOSS
    sequence[order].type = FindType(typeString);
    sequence[order].level = FindLevel(sequence[order].type, levelString);
    sequence[order].roleNum = strcmp(roleString, "单人") == 0 ? 1 : 2;
    sequence[order].boss = strcmp(bossString, "不打BOSS") == 0 ? 1 : 2;
    for (int code = 0; code < levelMapNum; code++)
      if (levelMap[code].type == sequence[order].type && levelMap[code].level == sequence[order].level)
      {
        sequence[order].code = code;
        break;
      }
    sequenceSize++;
    fgets(f, s);
  }
  fclose(f);
}
//载入公会和情侣任务顺序表
void LoadSequence()
{
  LoadSequence("附加程序\\文本\\公会任务表.txt", guildSequence, guildSequenceSize);
  LoadSequence("附加程序\\文本\\情侣任务表.txt", loverSequence, loverSequenceSize);
}
//向文件f中写入大赛任务contest的信息（columnAndSize：是否记录列数和大小）
void AddContestLog(FILE *f, Contest &contest, bool columnAndSize)
{
  if (contest.code == 0)
    fprintf(f, "非战斗\t\t\t\t\t\t\t%s\n", contest.text);
  else
  {
    const char roleNumString[3][20] = { "均可", "单人", "组队" };//人数对应的文本
    const char bossString[3][20] = { "均可", "不打BOSS", "打BOSS" };//BOSS对应的文本
    char levelName[100] = {};
    GetLevelName(levelName, contest.type, contest.level);
    //编号 类型 关卡 人数
    fprintf(f, "%d\t%s\t%s\t%s\t%s", contest.code, typeName[contest.type], levelName,
      roleNumString[contest.roleNum], bossString[contest.boss]);

    //写入调用轨道需要考虑的条件：卡槽 禁卡 放卡 静置 限放
    char allRequirement[100] = {};
    char requirement[100] = {};
    //静置
    if (contest.restTime)
    {
      sprintf_s(requirement, "\t静置%d", contest.restTime);
      strcat_s(allRequirement, requirement);
    }
    //限放
    if (contest.cardNum)
    {
      sprintf_s(requirement, "\t限放%d", contest.cardNum);
      strcat_s(allRequirement, requirement);
    }
    //卡槽
    if (contest.slotNum)
    {
      sprintf_s(requirement, "\t%d槽", contest.slotNum);
      strcat_s(allRequirement, requirement);
    }
    //禁卡
    for (int i = 0; i < contestSlotNum; i++)
      if (contest.slotBanned[i])
      {
        sprintf_s(requirement, "\t禁%s", contestSlot[i]);
        strcat_s(allRequirement, requirement);
      }
    //放卡
    for (int i = 0; i < contestSlotNum; i++)
      if (contest.slotPlanted[i])
      {
        sprintf_s(requirement, "放%s ", contestSlot[i]);
        strcat_s(allRequirement, requirement);
      }
    //如果tab少于3个，补齐到3个
    int length = strlen(allRequirement);
    int tabCount = 0;
    for (int i = 0; i < length; i++)
      if (allRequirement[i] == '\t')
        tabCount++;
    if (tabCount < 3)
      for (int i = 0; i < 3 - tabCount; i++)
        strcat_s(allRequirement, "\t");

    fprintf(f, "%s\t", allRequirement);

    //列数和本列剩余任务数
    if (columnAndSize)
      fprintf_s(f, "%d-%d(%d)", contest.column / 3 + 1, contest.column % 3 + 1, contest.size);
    fprintf(f, "\t");

    //任务文本
    fprintf(f, "%s", contest.text);
    if (contest.failedTimes > 0)
      fprintf(f, "（失败%d次）", contest.failedTimes);
    fprintf(f, "\n");
  }
}
//评分等级
const char gradeString[6][4] = { "", "A", "A+", "S", "SS", "SSS" };
//根据contest.text确定大赛信息（关卡、人数、要求），未匹配到关卡返回false
//调用前各项要求的数值一定是0
bool GetContestInfo(Contest &contest)
{
  char message[1000] = {};
  for (int code = 1; code < levelMapNum; code++)
    if (strstr(contest.text, levelMap[code].name))
    {
      contest.code = code;//编号
      contest.type = levelMap[code].type;//类型
      contest.level = levelMap[code].level;//关卡
      //人数
      if (strstr(contest.text, "单人"))
        contest.roleNum = 1;
      else if (strstr(contest.text, "组队"))
        contest.roleNum = 2;
      else if (strstr(contest.text, "疯狂爱丽丝1只")) //爱丽丝暂时只能双人打
        contest.roleNum = 2;

      //火苗
      char *energyString = strstr(contest.text, "火苗");
      if (energyString) //存在"火苗"则记录火苗要求
      {
        while (energyString > contest.text)
        {
          energyString--;//指针左移1位
          if (*energyString < '0' || *energyString > '9') //移动到不是数字
          {
            contest.energy = atoi(energyString + 1);
            break;
          }
        }
      }
      //评分
      for (int i = 5; i >= 1; i--)
        if (strstr(contest.text, gradeString[i]))
        {
          contest.grade = i;
          break;
        }

      //杀敌
      char *killString = strstr(contest.text, "击杀");//击杀关键词
      if (!killString)
        killString = strstr(contest.text, "清剿");
      if (!killString)
        killString = strstr(contest.text, "清除");
      if (killString) //记录要击杀的内容
      {
        strcpy_s(contest.enemy[0], killString);
        contest.enemySize = 1;
      }

      //1. 卡槽数
      GetKeyValue(contest.slotNum, contest.text, "携带卡片不超过");
      GetKeyValue(contest.slotNum, contest.text, "携带少于");

      //2. 禁卡
      int banLength = 6;
      char *banString = strstr(contest.text, "不使用");
      if (!banString)
      {
        banLength = 6;
        banString = strstr(contest.text, "不携带");
        //“不携带”的可能是主武器，不过和卡片一样处理没有问题
      }
      if (!banString)
      {
        banLength = 8;
        banString = strstr(contest.text, "不能使用");
      }
      if (banString) //如果存在禁卡关键词
      {
        bool isSlotBanned = false;
        for (int i = 0; i < contestSlotNum; i++)
          if (strstr(banString + banLength, contestSlotFull[i]))
          {
            contest.slotBanned[i] = true;
            isSlotBanned = true;
          }
      }

      //3. 放卡
      char *plantString = strstr(contest.text, "使用");
      //使用6次酒瓶的特例
      char *bottleString = strstr(contest.text, "【酒瓶炸弹】或【可乐炸弹】不少于");
      //如果不是禁卡、不是使用使用6次酒瓶的特例，且存在放卡关键词
      if (!banString && !bottleString && plantString)
      {
        bool isSlotPlanted = false;
        for (int i = 0; i < contestSlotNum; i++)
          if (strstr(plantString + 4, contestSlotFull[i]))
          {
            contest.slotPlanted[i] = true;
            isSlotPlanted = true;
          }
      }

      //4. 静置时间
      GetKeyValue(contest.restTime, contest.text, "进入战斗后");

      //5. 放卡数限制
      GetKeyValue(contest.cardNum, contest.text, "存在不超过");
      GetKeyValue(contest.cardNum, contest.text, "放置不超过");

      //6. 星级限制
      GetKeyValue(contest.star, contest.text, "等级均不超过");

      //7. 生产要求
      GetKeyValue(contest.product, contest.text, "产出超过");

      //8. 存活的火炉
      GetKeyValue(contest.aliveStove, contest.text, "至少存活");

      //9. 平均星级
      GetKeyValue(contest.averageStar, contest.text, "平均星级不高于");
      GetKeyValue(contest.averageStar, contest.text, "平均等级不高于");

      //10. 通关时间
      GetKeyValue(contest.gameTime, contest.text, "通关时长少于");

      //11. 酒瓶放置次数
      GetKeyValue(contest.bottleTimes, contest.text, "【酒瓶炸弹】或【可乐炸弹】不少于");

      //12. 免费卡放置次数
      GetKeyValue(contest.freeTimes, contest.text, "累计放置超过");

      //13. 耗能限制
      GetKeyValue(contest.cost, contest.text, "耗能不超过");
      GetKeyValue(contest.cost, contest.text, "耗能最高不超过");

      //火苗、评分、杀敌、产出要打BOSS才能完成
      if (contest.energy || contest.grade || contest.enemySize || contest.product)
        contest.boss = 2;
      else if (contest.star || contest.cardNum >= 200) //星级、限放没法打BOSS
        contest.boss = 1;

      //区分信息
      if (strstr(contest.text, "涡"))
        contest.maelstrom = true;
      if (strstr(contest.text, "-"))
        contest.hyphen = true;

      return true;
    }
  return false;
  //如果任务描述不包含关卡名，则code==0
}
//两个击杀描述是否相同（考虑通配符）
bool isEmemyEqual(const char *s1, const char *s2)
{
  int length = strlen(s1);
  if (length != strlen(s2))
    return false;
  for (int i = 0; i < length; i++)
    if (s1[i] != '*' && s2[i] != '*' && s1[i] != s2[i])
      return false;
  return true;
}
//两项大赛任务是否完全相同
bool IsContestEqual(Contest &c1, Contest &c2)
{
  if (c1.code != c2.code)
    return false;
  if (c1.roleNum != c2.roleNum)
    return false;
  if (c1.slotNum != c2.slotNum)
    return false;
  if (memcmp(c1.slotBanned, c2.slotBanned, sizeof(c1.slotBanned)) != 0)
    return false;
  if (memcmp(c1.slotPlanted, c2.slotPlanted, sizeof(c1.slotPlanted)) != 0)
    return false;
  if (c1.energy != c2.energy)
    return false;
  if (c1.grade != c2.grade)
    return false;
  if (c1.restTime != c2.restTime)
    return false;
  if (c1.cardNum != c2.cardNum)
    return false;
  if (c1.star != c2.star)
    return false;
  if (c1.product != c2.product)
    return false;
  if (c1.enemySize != c2.enemySize)
    return false;
  for (int i = 0; i < c1.enemySize; i++)
    if (!isEmemyEqual(c1.enemy[i], c2.enemy[i]))
      return false;

  if (c1.aliveStove != c2.aliveStove)
    return false;
  if (c1.averageStar != c2.averageStar)
    return false;
  if (c1.gameTime != c2.gameTime)
    return false;
  if (c1.bottleTimes != c2.bottleTimes)
    return false;
  if (c1.freeTimes != c2.freeTimes)
    return false;
  if (c1.cost != c2.cost)
    return false;

  //if (c1.maelstrom != c2.maelstrom)
  //  return false;
  if (c1.hyphen != c2.hyphen)
    return false;
  return true;
}
//各期大赛12列任务的初始深度
int maxDepths[tournamentNum][12] = {};

//填写各期大赛各列任务数
void FillMaxDepth()
{
  const int normalDepth[12] = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 7, 7, 7 };//普通大赛各列任务数
  const int compositeDepth[12] = { 9, 6, 9, 9, 6, 9, 9, 6, 9, 6, 6, 6 };//综合大赛各列任务数
  for (int issue = 0; issue < 12; issue++)
    memcpy(maxDepths[issue], normalDepth, sizeof(normalDepth));
  for (int issue = 12; issue < tournamentNum; issue++)
    memcpy(maxDepths[issue], compositeDepth, sizeof(compositeDepth));
}
//一期大赛的全任务和最优解
struct Tournament
{
  Contest contest[12][9];//全任务
  Contest solution[70];//最优路径（最多61步）
  int minStep;//最优路径步数
  int solutionSize[70][12];//最优路径第i步完成后的进度
};
Tournament tournament[tournamentNum];//各期大赛

//【最严检查】检查本期是否存在任意两个任务相同
bool FindRepeatedContest(int issue)
{
  char message[1000] = {};
  int contestNum = 0;
  Contest contest[102] = {};//102项任务
  int column0[102] = {};//102项任务所属的列

  //依次遍历每个任务
  for (int column = 0; column < 12; column++)
    for (int depth = 0; depth < maxDepths[issue][column]; depth++)
    {
      //记录当前任务
      contest[contestNum] = tournament[issue].contest[column][depth];
      column0[contestNum] = column;
      for (int i = 0; i < contestNum; i++)
        if (IsContestEqual(contest[contestNum], contest[i]))
        {
          sprintf_s(message, "%s期 两任务重复：\n%d列 %s\n%d列 %s", contestName[issue],
            column0[i], contest[i].text, column, contest[contestNum].text);
          PopMessage(nullptr, message);
        }
      contestNum++;
    }
  return false;
}
//检查本期是否存在两个底任务相同
bool FindRepeatedContestInBottom(int issue)
{
  char message[1000] = {};
  for (int column1 = 0; column1 < 12; column1++)
  {
    Contest &contest1 = tournament[issue].contest[column1][0];
    for (int column2 = 0; column2 < column1; column2++)
    {
      Contest &contest2 = tournament[issue].contest[column2][0];
      if (IsContestEqual(contest1, contest2))
      {
        sprintf_s(message, "%s期 两列底任务重复：\n%d列 %s\n%d列 %s", contestName[issue],
          column1, contest1.text, column2, contest2.text);
        PopMessage(nullptr, message);
      }
    }
  }
  return false;
}

//判断第column列任务contest下方还有几个任务
int GetContestSize(int list, int column, Contest &contest)
{
  for (int depth = 0; depth < maxDepths[plot[list].issue][column]; depth++)
    if (IsContestEqual(contest, tournament[plot[list].issue].contest[column][depth]))
      return depth + 1;
  return 0; //匹配失败返回0（理论上不应该出现）
}
//判断底任务contest处于哪一列，定位失败返回-1
int GetBottomColumn(int list, Contest &bottom)
{
  for (int column = 0; column < 12; column++)
    if (IsContestEqual(bottom, tournament[plot[list].issue].contest[column][0]))
      return column;
  return -1;//定位失败
}
//在本期大赛中根据表面任务定位当前进度。无法定位返回false
bool LocateSurface(int list, int account)
{
  //进度预设为0
  memset(plot[list].contestSize[account], 0, sizeof(plot[list].contestSize[account]));

  //1. 未刷出的任务全部标记为已完成
  bool isFinished[12] = {};//12列是否已完成或未刷出
  for (int column = plot[list].surfaceSize[account]; column < 12; column++)
    isFinished[column] = true;

  //2. 识别到已完成的任务标记为已完成
  for (int order = 0; order < plot[list].surfaceSize[account]; order++)
    if (plot[list].surface[account][order].isFinished) //对于每个已完成的表面任务
    {
      //判断该任务是哪一列的底任务（理论上能匹配到唯一解）
      int column = GetBottomColumn(list, plot[list].surface[account][order]);
      if (column == -1) //定位失败（理论上不应该出现）
        return false;
      if (column == 3 && isFinished[3]) //如果判断为3列底任务，而3列已经被标记完成，说明是6列
        column = 6;
      isFinished[column] = true;//否则标记这一列任务已完成
      plot[list].surface[account][order].size = 0;
      plot[list].surface[account][order].column = column;
    }
  //已完成的列数进度为0，不用修改

  //3. 未完成的表面任务依次匹配未完成的列数（第1个任务匹配最大的列数）
  int unfinishedColumn = 12;//未完成的列数（预设为12）
  for (int order = 0; order < plot[list].surfaceSize[account]; order++)
    if (!plot[list].surface[account][order].isFinished) //对于每个未完成的表面任务
    {
      //寻找下一个未完成的列数
      do
        unfinishedColumn--;
      while (unfinishedColumn >= 0 && isFinished[unfinishedColumn]);
      if (unfinishedColumn == -1) //定位失败（理论上不应该出现）
        return false;
      //此时unfinishedColumn为这个表面任务所在的列数，直接列内定位
      int size = GetContestSize(list, unfinishedColumn, plot[list].surface[account][order]);
      if (size == 0) //定位失败（理论上不应该出现）
        return false;

      //若未完成次数达到3，该列任务数设为0，但日志中依然记录原始任务数
      if (plot[list].surface[account][order].failedTimes >= 3)
        plot[list].contestSize[account][unfinishedColumn] = 0;
      else
        plot[list].contestSize[account][unfinishedColumn] = size;

      plot[list].surface[account][order].size = size;
      plot[list].surface[account][order].column = unfinishedColumn;
    }
  return true;
}
//在12期大赛中根据表面任务定位当前进度，返回期数，定位失败返回-1
int GlobalLocateSurface(int list)
{
  for (int issue = 0; issue < tournamentNum; issue++)
  {
    plot[list].issue = issue;//设置期数
    //如果两个号都定位成功，确定为本期大赛
    if (LocateSurface(list, 0) && LocateSurface(list, 1))
      return issue;
  }
  return -1;
}
//如果本期大赛能定位两个号的进度，选择本期定位，否则全局定位
bool SmartLocateSurface(int list)
{
  if (LocateSurface(list, 0) && LocateSurface(list, 1))
    return true;
  return GlobalLocateSurface(list) >= 0;
}
//重写大赛文档
void RewriteContest(int issue)
{
  const char columnName[3][100] = { "击杀小怪", "击杀BOSS", "战斗挑战" };
  char oldPath[maxPath] = {}, newPath[maxPath] = {};
  sprintf_s(oldPath, "旧大赛\\%s期.txt", contestName[issue]);
  sprintf_s(newPath, "新大赛\\%s期.txt", contestName[issue]);

  Contest(&contest)[12][9] = tournament[issue].contest;
  memset(contest, 0, sizeof(contest)); //清空大赛信息

  //从文件读取12列任务
  FILE *fin, *fout;
  if (fopen_s(&fin, oldPath, "r"))
    ReportMissingFile(oldPath);

  char text[1000] = {};
  for (int column = 0; column < 12; column++)
  {
    fgets(fin, text);//跳过1行标题
    //读取一列大赛任务文本
    for (int depth = maxDepths[issue][column] - 1; depth >= 0; depth--)
      fgets(fin, contest[column][depth].text);
  }
  fclose(fin);

  //按新格式书写
  fopen_s(&fout, newPath, "w");
  for (int column = 0; column < 12; column++)
    for (int depth = maxDepths[issue][column] - 1; depth >= 0; depth--)
      fprintf(fout, "[%s第%d周]%s\n", columnName[column % 3], column / 3 + 1,
        contest[column][depth].text);
  fclose(fout);
}
char missingWord[1000][3];//字库中缺少的字
int missingWordNum;//已发现的缺字数
//检查字库中是否缺少文字word，缺少则记入数组
void CheckMissingWord(const char *word)
{
  //如果这个字在字库中，不予记录
  for (int code = 0; code < textNum; code++)
    if (strcmp(word, text[code].name) == 0)
      return;
  //如果这个字在已被记录，不予记录
  for (int code = 0; code < missingWordNum; code++)
    if (strcmp(word, missingWord[code]) == 0)
      return;
  strcpy_s(missingWord[missingWordNum++], word);
}
//检查并记录大赛任务文本中缺少的字
void CheckMissingText(const char *text)
{
  char word[100][3] = {};
  int wordSize = 0;
  int length = strlen(text);
  int i = 0;
  while (i < length)
  {
    if (text[i] < 0) //如果遇到汉字首字节
    {
      strncpy_s(word[wordSize++], text + i, 2);
      i += 2;
    }
    else //非汉字
    {
      strncpy_s(word[wordSize++], text + i, 1);
      i += 1;
    }
  }
  for (int j = 0; j < wordSize; j++)
    CheckMissingWord(word[j]);
}
//读取一期大赛全任务
void LoadContest(int issue, const char *contestPath)
{
  Contest(&contest)[12][9] = tournament[issue].contest;
  memset(contest, 0, sizeof(contest)); //清空大赛信息

  //从文件读取12列任务
  FILE *f;
  if (fopen_s(&f, contestPath, "r"))
    ReportMissingFile(contestPath);

  char text[1000] = {};
  for (int column = 0; column < 12; column++)
    for (int depth = maxDepths[issue][column] - 1; depth >= 0; depth--)
    {
      fgets(f, text);//读取一行文本
      strcpy_s(contest[column][depth].text, strchr(text, ']') + 1);
      //（缺字检查1/2）
      //if (issue == tournamentNum - 1)
      //  CheckMissingText(contest[column][depth].text);
    }
  fclose(f);

  //获取每个任务的关卡、人数和要求
  for (int column = 0; column < 12; column++)
    for (int depth = maxDepths[issue][column] - 1; depth >= 0; depth--)
      if (!GetContestInfo(contest[column][depth])) //匹配失败则报错
      {
        char message[1000] = {};
        sprintf_s(message, "column=%d depth=%d\n%s", column, depth, contest[column][depth].text);
        PopMessage(nullptr, message);
        exit(0);
      }
}
//读取最优顺序表
void LoadSolution(int issue, const char *solutionPath)
{
  tournament[issue].minStep = 0;
  memset(tournament[issue].solution, 0, sizeof(tournament[issue].solution));

  FILE *f;
  if (fopen_s(&f, solutionPath, "r"))
    ReportMissingFile(solutionPath);

  char s[1000] = {};
  fgets(f, s); //读取第1行文本（据此判断是数字顺序表还是文字顺序表）
  int param1 = 0;//第1行第1个参数
  char param2[1000] = {};//第1行第2个参数
  sscanf_s(s, "%d\t%s", &param1, param2, sizeof(param2));
  bool isDigitalSolution = param2[0] >= '0' && param2[0] <= '9';//是否为数字顺序表

  while (strlen(s) > 0)
  {
    Contest &solution = tournament[issue].solution[tournament[issue].minStep];

    //数字顺序表：关卡编号 人数 BOSS 
    if (isDigitalSolution)
    {
      sscanf_s(s, "%d\t%d\t%d", &solution.code, &solution.roleNum, &solution.boss);
      solution.type = levelMap[solution.code].type;//类型
      solution.level = levelMap[solution.code].level;//关卡
    }
    //文字顺序表：序号 类型 关卡 人数 BOSS 名称 要求
    else
    {
      char *context = nullptr;
      char *idString = strtok_s(s, "\t", &context);//第一段：序号
      char *typeString = strtok_s(nullptr, "\t", &context);//第二段：类型
      char *levelString = strtok_s(nullptr, "\t", &context);//第三段：关卡
      char *roleString = strtok_s(nullptr, "\t", &context);//第四段：人数
      char *bossString = strtok_s(nullptr, "\t", &context);//第五段：BOSS
      solution.type = FindType(typeString);
      solution.level = FindLevel(solution.type, levelString);
      solution.roleNum = strcmp(roleString, "单人") == 0 ? 1 : 2;
      solution.boss = strcmp(bossString, "不打BOSS") == 0 ? 1 : 2;
      for (int code = 0; code < levelMapNum; code++)
        if (levelMap[code].type == solution.type && levelMap[code].level == solution.level)
        {
          solution.code = code;
          break;
        }
    }

    tournament[issue].minStep++;
    fgets(f, s);
  }
  fclose(f);
}
//重置大赛进度
void ResetContestSize(int issue, int(&contestSize)[12])
{
  for (int column = 0; column < 12; column++)
    contestSize[column] = maxDepths[issue][column];
}
//两个大赛任务人数是否匹配
bool IsRoleNumMatched(int roleNum1, int roleNum2)
{
  if (roleNum1 == 0 || roleNum2 == 0)
    return true;
  return roleNum1 == roleNum2;
}
//两个大赛任务人数是否匹配
bool IsBossMatched(int boss1, int boss2)
{
  if (boss1 == 0 || boss2 == 0)
    return true;
  return boss1 == boss2;
}
//两个大赛任务的关卡、人数、BOSS是否匹配
bool IsContestMatched(Contest &contest1, Contest &contest2)
{
  return contest1.code == contest2.code && IsRoleNumMatched(contest1.roleNum, contest2.roleNum)
    && IsBossMatched(contest1.boss, contest2.boss);
}
//使用最优解模拟通关，记录每一步完成后的进度
void SimulateSolution(int issue)
{
  int contestSize[12] = {};//每一列的剩余任务数
  ResetContestSize(issue, contestSize);//重置任务进度

  //模拟每个步骤
  for (int step = 0; step < tournament[issue].minStep; step++)
  {
    //模拟执行当前步骤，更新进度
    for (int column = 0; column < 12; column++)
      if (contestSize[column] > 0)
      {
        Contest &surface = tournament[issue].contest[column][contestSize[column] - 1];//本列表面任务
        Contest &solution = tournament[issue].solution[step];//最优任务
        //如果最优任务与本列表面任务匹配，本列完成进度+1
        if (IsContestMatched(surface, solution))
          contestSize[column]--;
      }
    //记录每一步完成后的进度
    memcpy(tournament[issue].solutionSize[step], contestSize, sizeof(contestSize));
  }

  //检查能否完成任务
  for (int column = 0; column < 12; column++)
    if (contestSize[column] > 0)
      return PopMessage(nullptr, "最优解无法完成任务");
}
//读取各期大赛全任务和最优解，记录最优解每一步完成后的进度
void InitContest()
{
  FillMaxDepth();
  char contestPath[maxPath] = {}, solutionPath[maxPath] = {};
  for (int issue = 0; issue < tournamentNum; issue++)
  {
    sprintf_s(contestPath, "附加程序\\文本\\大赛任务\\%s期.txt", contestName[issue]);
    sprintf_s(solutionPath, "附加程序\\文本\\大赛最优解\\%s期.txt", contestName[issue]);
    LoadContest(issue, contestPath);//读取全任务

    LoadSolution(issue, solutionPath);//读取最优解

    SimulateSolution(issue);//使用最优解模拟通关，记录每一步完成后的进度
  }

  //（缺字检查2/2）
  //FILE *f;
  //fopen_s(&f, "缺字表.txt", "w");
  //for (int i = 0; i < missingWordNum; i++)
  //  fprintf(f, "%s\n", missingWord[i]);
  //fclose(f);
  //exit(0);
}
//contest是否为应完成的任务
bool IsRequiredContest(int list, Contest &contest)
{
  for (int i = 0; i < plot[list].requiredSize; i++)
    if (IsContestEqual(plot[list].requiredContest[i], contest))
      return true;
  return false;
}
//新增应完成的大赛任务
void AddRequiredContest(int list, Contest &contest)
{
  if (!IsRequiredContest(list, contest))
    plot[list].requiredContest[plot[list].requiredSize++] = contest;
}
//contest未完成的次数
int GetFailedTimes(int list, Contest &contest)
{
  //该任务已记录，则未完成次数+1
  for (int i = 0; i < plot[list].totalFailedSize; i++)
    if (IsContestEqual(plot[list].totalFailedContest[i], contest))
      return plot[list].totalFailedContest[i].failedTimes;
  return 0;
}
//contest是否为本轮未完成的任务
bool IsFailedContest(int list, Contest &contest)
{
  for (int i = 0; i < plot[list].failedSize; i++)
    if (IsContestEqual(plot[list].failedContest[i], contest))
      return true;
  return false;
}
//新增本轮未完成的大赛任务
void AddFailedContest(int list, Contest &contest)
{
  if (!IsFailedContest(list, contest))
    plot[list].failedContest[plot[list].failedSize++] = contest;
}
//新增累计未完成的大赛任务（同一轮不得重复记录）
void AddTotalFailedContest(int list, Contest &contest, int times)
{
  if (IsFailedContest(list, contest)) //本轮已记录过则退出
    return;
  AddFailedContest(list, contest); //记录本轮失败任务

  //记录累计失败任务
  for (int i = 0; i < plot[list].totalFailedSize; i++)
    if (IsContestEqual(plot[list].totalFailedContest[i], contest))
    {
      //该任务已记录，则未完成次数+1
      plot[list].totalFailedContest[i].failedTimes += times;
      return;
    }
  //该任务无记录，则添加记录
  plot[list].totalFailedContest[plot[list].totalFailedSize] = contest;
  plot[list].totalFailedContest[plot[list].totalFailedSize].failedTimes = times;
  plot[list].totalFailedSize++;
}
//获取第一个勾选的识别任务的编号，找不到返回-1
int GetMissionOrder(int list, int level)
{
  for (int order = 0; order < plot[list].advanceNum; order++)
    if (plot[list].advance[order].isSelected && plot[list].advance[order].type == 5
      && plot[list].advance[order].level == level)
      return order;
  return -1;
}
//获取最后一个大赛子任务的序号
int GetLastSubcontestOrder(int list, int contestOrder)
{
  //查找最后一个大赛子任务位置
  int lastSubcontestOrder = contestOrder;
  for (int order = plot[list].advanceNum - 1; order > contestOrder; order--)
    if (plot[list].advance[order].missionStyle == CONTEST)
    {
      lastSubcontestOrder = order;
      break;
    }
  return lastSubcontestOrder;
}
//记录表面任务中预期上轮完成而现在未完成的任务，填写每个表面任务的未完成次数
void RecordFailedContest(int list)
{
  int contestOrder = GetMissionOrder(list, CONTEST - 1);//识别-大赛位置
  int lastSubcontestOrder = GetLastSubcontestOrder(list, contestOrder);//最后一个子任务位置
  //如果没有子任务，说明是第一次识别，因此不存在预期完成的任务
  if (lastSubcontestOrder == contestOrder)
    return;

  //刚才执行的子任务
  Advance &advance = plot[list].advance[lastSubcontestOrder];
  //如果执行失败，给未完成的大赛任务添加3层标记（直接跳），否则加1层
  int times = advance.gamesFinished < advance.maxGames ? 3 : 1;

  //记录未完成的任务
  for (int account = 0; account < 2; account++)
    for (int i = 0; i < plot[list].surfaceSize[account]; i++)
    {
      Contest &contest = plot[list].surface[account][i];
      if (!contest.isFinished && IsRequiredContest(list, contest))
        AddTotalFailedContest(list, contest, times);
    }
  //填写每个表面任务的未完成次数
  for (int account = 0; account < 2; account++)
    for (int i = 0; i < plot[list].surfaceSize[account]; i++)
    {
      Contest &contest = plot[list].surface[account][i];
      contest.failedTimes = GetFailedTimes(list, contest);
    }
}
//增加杀敌要求
void AddEnemy(Contest &contest, const char *enemy)
{
  //杀敌要求已存在则退出
  for (int i = 0; i < contest.enemySize; i++)
    if (strcmp(contest.enemy[i], enemy) == 0)
      return;
  strcpy_s(contest.enemy[contest.enemySize++], enemy);
}
//选择最大值
void SelectMax(int &currentMax, int value)
{
  if (value > currentMax)
    currentMax = value;
}
//选择最小值
void SelectMin(int &currentMin, int value)
{
  if (value > 0 && value < currentMin)
    currentMin = value;
}
//根据两个号的表面任务确定最优任务的要求，未获取最优任务则返回false
bool FillBestContestFromSurface(int list, bool log)
{
  Contest &bestContest = plot[list].bestContest;//最优任务
  if (bestContest.code > 0)
  {
    bestContest.type = levelMap[bestContest.code].type;//类型
    bestContest.level = levelMap[bestContest.code].level;//等级
    //几个需要取最小值的要求
    bestContest.slotNum = INT_MAX;//卡槽数限制
    bestContest.cardNum = INT_MAX;//放卡数限制
    bestContest.star = INT_MAX;//星级限制
    bestContest.averageStar = INT_MAX;//平均星级限制
    bestContest.gameTime = INT_MAX;//通关时间限制
    bestContest.cost = INT_MAX;//卡片耗能限制

    //考虑编号、人数均匹配的未完成任务的附加要求
    for (int account = 0; account < 2; account++)
      for (int order = 0; order < plot[list].surfaceSize[account]; order++)
      {
        Contest &contest = plot[list].surface[account][order];
        if (!contest.isFinished && contest.failedTimes < 3 && IsContestMatched(contest, bestContest))
        {
          //0. 记录这个任务是应完成的
          AddRequiredContest(list, contest);

          //重要的要求

          //1. 卡槽数：取最小值
          SelectMin(bestContest.slotNum, contest.slotNum);
          //2. 禁卡：取并集
          for (int i = 0; i < contestSlotNum; i++)
            if (contest.slotBanned[i])
              bestContest.slotBanned[i] = true;
          //3. 放卡：取并集
          for (int i = 0; i < contestSlotNum; i++)
            if (contest.slotPlanted[i])
              bestContest.slotPlanted[i] = true;
          //4. 静置时间：取最大值
          SelectMax(bestContest.restTime, contest.restTime);
          //5. 放卡数：取最小值
          SelectMin(bestContest.cardNum, contest.cardNum);
          //6. 星级：取最小值
          SelectMin(bestContest.star, contest.star);
          //7. 产出：取最大值
          SelectMax(bestContest.product, contest.product);

          //8. 存活火炉：取最大值
          SelectMax(bestContest.aliveStove, contest.aliveStove);
          //9. 平均星级：取最小值
          SelectMin(bestContest.averageStar, contest.averageStar);
          //10. 通关时间：取最小值
          SelectMin(bestContest.gameTime, contest.gameTime);
          //11. 酒瓶使用次数：取最大值
          SelectMax(bestContest.bottleTimes, contest.bottleTimes);
          //12. 免费卡放置次数：取最大值
          SelectMax(bestContest.freeTimes, contest.freeTimes);
          //13. 耗能限制：取最小值
          SelectMin(bestContest.cost, contest.cost);

          //不重要的要求：火苗，评分，杀敌
          if (contest.energy > bestContest.energy)
            bestContest.energy = contest.energy;
          if (contest.grade > bestContest.grade)
            bestContest.grade = contest.grade;
          if (contest.enemySize == 1)
            AddEnemy(bestContest, contest.enemy[0]);
        }
      }
    if (bestContest.slotNum == INT_MAX) //无卡槽数限制则设为0
      bestContest.slotNum = 0;
    if (bestContest.cardNum == INT_MAX) //无放卡数限制则设为0
      bestContest.cardNum = 0;
    if (bestContest.star == INT_MAX) //无星级限制则设为0
      bestContest.star = 0;
    if (bestContest.averageStar == INT_MAX) //无平均星级限制则设为0
      bestContest.averageStar = 0;
    if (bestContest.gameTime == INT_MAX) //无时间限制则设为0
      bestContest.gameTime = 0;
    if (bestContest.cost == INT_MAX) //无耗能限制则设为0
      bestContest.cost = 0;
  }

  //记录选择结果到大赛日志
  if (log)
  {
    FILE *f;
    fopen_s(&f, plot[list].contestLog, "a");
    if (plot[list].isLocated)
    {
      if (bestContest.code == 0)
        fprintf(f, "%s期 已完成\n", contestName[plot[list].issue]);
      else
        fprintf(f, "%s期 第%d步\n", contestName[plot[list].issue], plot[list].step + 1);
    }
    else
      fprintf(f, "定位失败\n");
    for (int account = 0; account < 2; account++)
    {
      fprintf(f, "[%dP]\n", account + 1);
      for (int order = 0; order < plot[list].surfaceSize[account]; order++)
        AddContestLog(f, plot[list].surface[account][order], plot[list].isLocated);
    }
    fprintf(f, "选择结果：\n");
    if (bestContest.code == 0)
      fprintf(f, "无\n");
    else
      AddContestLog(f, bestContest, false);
    fclose(f);
  }

  return bestContest.code > 0;
}
//进度size1是否达到或超越进度size2
bool IsProgressReached(int(&size1)[12], int(&size2)[12])
{
  for (int column = 0; column < 12; column++)
    if (size1[column] > size2[column])
      return false;
  return true;
}
//根据双号进度判断接下来要执行哪一步
int GetNextStep(int list)
{
  //如果有一个号的进度没达到第step步完成后的进度，说明需要执行第step步
  for (int step = 0; step < tournament[plot[list].issue].minStep; step++)
    if (!IsProgressReached(plot[list].contestSize[0], tournament[plot[list].issue].solutionSize[step])
      || !IsProgressReached(plot[list].contestSize[1], tournament[plot[list].issue].solutionSize[step]))
      return step;
  return tournament[plot[list].issue].minStep;//全部完成，返回最大步数
}
//根据最优解选择任务
bool SelectBestSurface(int list)
{
  Contest &bestContest = plot[list].bestContest;//最优任务
  memset(&bestContest, 0, sizeof(bestContest));//清空最优任务

  plot[list].step = GetNextStep(list); //根据进度确定需要执行最优解中的哪一步
  if (plot[list].step == tournament[plot[list].issue].minStep) //任务已完成则返回false
    return false;

  bestContest = tournament[plot[list].issue].solution[plot[list].step];
  return true;
}
//从两个号的表面任务中选择编号最小的任务（定位失败时触发）
bool SelectMinSurface(int list)
{
  Contest &bestContest = plot[list].bestContest;//最优任务
  memset(&bestContest, 0, sizeof(bestContest));//清空最优任务

  //1. 确定最优任务的编号（取最小值）
  int minCode = INT_MAX;
  for (int account = 0; account < 2; account++)
    for (int order = 0; order < plot[list].surfaceSize[account]; order++)
    {
      Contest &surface = plot[list].surface[account][order];
      if (!surface.isFinished && surface.failedTimes < 3 && surface.code < minCode)
        minCode = surface.code;
    }
  if (minCode == INT_MAX) //没有能选择的任务则返回false
    return false;
  bestContest.code = minCode;

  //2. 考虑人数要求
  bool singleExist = false;//同一关卡是否存在强制单人任务
  bool teamExist = false;//同一关卡是否存在强制双人任务
  for (int account = 0; account < 2; account++)
    for (int order = 0; order < plot[list].surfaceSize[account]; order++)
    {
      Contest &surface = plot[list].surface[account][order];
      if (!surface.isFinished && surface.failedTimes < 3 && surface.code == bestContest.code)
      {
        if (surface.roleNum == 1)
          singleExist = true;
        else if (surface.roleNum == 2)
          teamExist = true;
      }
    }
  if (singleExist && !teamExist) //有单无双做单人
    bestContest.roleNum = 1;
  else //其他情况做双人
    bestContest.roleNum = 2;
  return true;
}
//从大赛面板识别大赛任务
void ReadContest(int task, int account)
{
  int list = GetList(task);

  //分7页截图
  for (int page = 0; page < 7; page++)
  {
    //1. 翻页并截图
    ScrollContest(task, account, page);//点击第i段大赛任务（获得无邀请的截图）

    //2. 寻找任务分界线
    const int divideX = contestX + contestWidth;//竖直判定线X
    int divideY = -1;//第一条任务分界线Y

    //注：无亮度遮挡的Y范围为364-560 所以要从371开始找，才能确保上7无亮度遮挡
    for (int y = 371; y < contestY + contestHeight - 10; y++)
    {
      int bright = Brightness(work[task].map[y][divideX]);//本Y亮度
      int brightUp1 = Brightness(work[task].map[y - 1][divideX]);//上方1亮度
      int brightDown1 = Brightness(work[task].map[y + 1][divideX]);//下方1亮度
      int brightUp7 = Brightness(work[task].map[y - 7][divideX]);//上方7亮度
      int brightDown7 = Brightness(work[task].map[y + 7][divideX]);//下方7亮度
      //任务分界线：比上面暗，比下面暗，比上7格亮度小30，比下7格亮度小30
      if (bright < brightUp1 && bright < brightDown1 && brightUp7 - bright >= 30 && brightDown7 - bright >= 30)
      {
        divideY = y;
        break;
      }
    }
    if (divideY == -1) //如果找不到任务分界线，说明本页没有任务
      continue;

    //3. 标记任务文字区
    bool isContestRangeY[gameHeight] = {};//高度y是否为任务文字区
    int contestTextY[4] = {};//本页第i个任务文字区Y
    int contestTextNum = 0;//本页任务数量
    for (int y = contestY; y < contestY + contestHeight; y++)
      if (IsContestRange(y, divideY))
      {
        isContestRangeY[y] = true;
        if (!isContestRangeY[y - 1]) //如果y-1不是文字区，则记录文字区起点
          contestTextY[contestTextNum++] = y;
      }

    //4. 对文字区内的任务进行识别
    for (int order = 0; order < contestTextNum; order++)
    {
      //4.1 查找首行文字Y
      int textY = -1;
      for (int y = contestTextY[order]; y < contestTextY[order] + 10; y++)
      {
        for (int x = contestX; x < contestX + contestWidth; x++)
          if (IsContestTextColor(work[task].map[y][x]))
          {
            textY = y;
            break;
          }
        if (textY != -1) //找到了首行文字Y则退出循环
          break;
      }
      if (textY == -1) //找不到首行文字Y则跳过本任务
        continue;

      //首行文字Y上一行亮度小于400，说明是已完成的任务
      bool isFinished = Brightness(work[task].map[textY - 1][contestX]) < 400;

      //4.2 识别两行大赛文本
      char contestText[200] = {};
      ReadContestLine(task, account, contestText, textY);
      ReadContestLine(task, account, contestText, textY + 16);
      //4.3 比较并记录
      bool repeat = false; //是否与已记录的任务重复
      for (int i = 0; i < plot[list].surfaceSize[account]; i++)
        if (strcmp(plot[list].surface[account][i].text, contestText) == 0)
          repeat = true;
      if (!repeat)  //记录不重复的任务
        strcpy_s(plot[list].surface[account][plot[list].surfaceSize[account]].text, contestText);
      //记录任务是否已完成
      plot[list].surface[account][plot[list].surfaceSize[account]].isFinished = isFinished;
      //根据文本填写任务信息，是战斗任务则记录数量+1
      if (GetContestInfo(plot[list].surface[account][plot[list].surfaceSize[account]]))
        plot[list].surfaceSize[account]++;
    }
  }
}
//打开大赛面板。report：没有大赛图标是否报错
bool OpenContestMission(int task, int account, bool report = true)
{
  //点击大赛图标，没有图标则返回false
  if (!ClickActivity(task, account, "大赛图标", report))
    return false;
  WaitPicture(APPEAR, task, account, "大赛界面");//等待打开大赛界面
  CheckSleep(task, 1000);//等待奖励加载出来
  int counter = 0;
  while (FindPicture(task, account, "大赛背景")) //如果10秒还没加载出来，很可能是网卡了
  {
    counter++;
    if (counter > 10) //10秒还加载不出来，中断任务
      ReportError(task, account, "大赛无法加载", "大赛面板加载失败");
    CheckSleep(task, 1000);
  }
  return true;
}
//领取营地任务奖励（只能在探险营地执行）
void ClaimCampAward(int task, int account)
{
  int x = 0, y = 0;
  int counter = 0;
  while (FindPicture(task, account, "营地领取按钮", &x, &y))
  {
    counter++;
    if (counter > 15)
      ReportError(task, account, "营地领奖失败", "无法领取营地奖励");
    Click(task, account, x + 5, y + 5);//点击领取按钮(+5是为了防止点边角无效）
    CheckSleep(task, 2000);//额外等待2秒
    DealInvitation(task, account, 0);
  }
}
//领取美食大赛奖励。report：没有大赛图标是否报错
void ClaimContestAward(int task, int account, bool isOpenRequired = true, bool report = true)
{
  //打开大赛面板，没有大赛图标则退出
  if (isOpenRequired)
    if (!OpenContestMission(task, account, report))
      return;

  //分7页领取奖励
  int x = 0, y = 0;
  for (int page = 0; page < 7; page++)
  {
    ScrollContest(task, account, page);//点击第i段大赛任务（获得无邀请截图）
    int counter = 0;
    while (FindPicture(task, account, "大赛领取按钮", &x, &y))
    {
      counter++;
      if (counter > 15)
        ReportError(task, account, "大赛领奖失败", "无法领取大赛奖励");
      Click(task, account, x + 5, y + 5);//点击领取按钮(+5是为了防止点边角无效）
      WaitPicture(DISAPPEAR, task, account, "大赛领取按钮", nullptr, nullptr, x, y);//等待该位置领取按钮消失
      CheckSleep(task, 4000);//额外等待4秒
      DealInvitation(task, account, 0);
    }
  }

  //关闭大赛面板
  if (isOpenRequired)
    Click(task, account, closeContestMission);
}
//关闭健康提示和充值面板
void CloseHealthAndPayment(int task, int account)
{
  const POINT closeHealthPrompt = { 588, 212 };//关闭健康提示
  const POINT closePayment = { 836, 48 };//关闭充值面板
  int counter = 0;
  while (FindPicture(task, account, "健康提示")) //关闭健康提示
  {
    counter++;
    if (counter > 10)
      ReportError(task, account, "无法关闭提示", "无法关闭健康提示");
    Click(task, account, closeHealthPrompt);
    CheckSleep(task, 500);
  }
  counter = 0;
  while (FindPicture(task, account, "假期特惠")) //关闭假期特惠
  {
    counter++;
    if (counter > 10)
      ReportError(task, account, "无法关闭特惠", "无法关闭假期特惠");
    Click(task, account, closeHealthPrompt);
    Click(task, account, closePayment);
    CheckSleep(task, 500);
  }
}
//生成随机换区顺序
void ShuffleRandomZone(int list)
{
  for (int i = 0; i < 7; i++)
    plot[list].randomZone[i] = i;
  srand((unsigned)time(0));
  for (int i = 0; i < 6; i++)
  {
    int j = i + rand() % (7 - i);
    Swap(&plot[list].randomZone[i], &plot[list].randomZone[j]);
  }
}
//判断任务类型和要跳转的岛屿是否相同
bool IsTypeAndZoneEqual(int type, int zoneOrder)
{
  if (type == 8 && zoneOrder == 6) //遗迹
    return true;
  if (type == 10 && (zoneOrder == 4 || zoneOrder == 5)) //海底
    return true;
  if (type == 11 && (zoneOrder == 2 || zoneOrder == 3)) //星际
    return true;
  if (type == 12 && (zoneOrder == 0 || zoneOrder == 1)) //营地
    return true;
  return false;
}
//可跳转的各区域名称
const char zoneName[8][13] = { "营地一区", "营地二区", "星际一区", "星际二区", "海洋一区", "海洋二区", "遗迹一区", "竞技一区" };
//跳转到海洋一二区，星际一二区，营地一二区，遗迹一区，竞技一区
void RandomJump(int task, int account)
{
  int list = GetList(task);
  const POINT closeWorldMap = { 828, 61 };
  const POINT changeZone = { 819, 84 };//换线按钮
  const POINT zoneLocation[8] = { { 780, 84 }, { 780, 110 }, { 780, 84 }, { 780, 110 }, { 780, 84 }, { 780, 110 }, { 780, 84 }, { 780, 84 } };
  const int islandOrder[7] = { 6, 6, 5, 5, 4, 4, 2 };
  int zoneOrder = 7;//目标小区编号
  const char *targetIslandName = "竞技";//目标岛屿名称
  POINT targetIslandLocation = { 201, 346 };//目标岛屿位置
  if (!plot[list].isLevelInsufficient) //如果等级足够，执行随机跳转
  {
    zoneOrder = plot[list].randomZone[plot[list].zoneCounter % 7];//跳转到的小区编号
    targetIslandName = typeName[islandOrder[zoneOrder] + basicTypeNum];
    targetIslandLocation = islandLocation[islandOrder[zoneOrder]];
  }

  if (FindPicture(task, account, targetIslandName) == 0)//如果不在目标岛屿，换到目标岛屿
  {
    Click(task, account, worldMap);//打开世界地图
    WaitPicture(APPEAR, task, account, "世界地图");//等待打开世界地图
    DealInvitation(task, account, 0);
    Click(task, account, targetIslandLocation);//点击岛屿

    //等待世界地图消失。如果无法跳转至指定岛屿，判定等级不足，重新跳转
    if (!TryWaitPicture(DISAPPEAR, task, account, "世界地图", WaitTime() * 2))
    {
      //if (plot[list].isLevelInsufficient) //如果竞技岛都跳不了
      //  RecordError(task, account, "等级不足", "等级不足8级，无法使用高级任务", END_TASK);
      //如果是普通岛屿跳不了
      plot[list].isLevelInsufficient = true;//记录等级不足，下次跳竞技岛
      ReportError(task, account, "等级不足", "等级不足，尝试跳转至竞技岛");
    }
    WaitPicture(APPEAR, task, account, targetIslandName);//等待进入岛屿
    WaitColorDisappear(task, account, blueBox.x, blueBox.y, boxColor);//等待服务器蓝框消失
    CheckSleep(task, 200);
  }
  //换到目标区域
  int counter = 0;
  while (FindPicture(task, account, zoneName[zoneOrder]) == 0) //不断换区，换成为止
  {
    counter++;
    if (counter > 5)
    {
      char message[100];
      sprintf_s(message, "无法换到[%s]", zoneName[zoneOrder]);
      ReportError(task, account, "换区失败", message);
    }
    Click(task, account, changeZone);//点击换区按钮
    CheckSleep(task, 200);
    SelectiveSleep(task, 300);
    Click(task, account, zoneLocation[zoneOrder]);//点击区名
    CheckSleep(task, 200);
    SelectiveSleep(task, 300);
    WaitColorDisappear(task, account, blueBox.x, blueBox.y, boxColor);//等待服务器蓝框消失
  }
  CheckSleep(task, 500);//多等一会儿，免得不能操作

  //已经在正确的区，也要等蓝框消失
  WaitColorDisappear(task, account, blueBox.x, blueBox.y, boxColor);
}
//高级任务完成后的退出操作。
void Retreat(int task)
{
  int list = GetList(task);
  const POINT closeChallenge = { 845, 57 };//关闭勇士界面
  const POINT closeTower = { 927, 34 };//关闭魔塔界面
  const POINT closePet = { 793, 34 };//关闭宠塔界面
  const POINT closePeak = { 931, 17 };//关闭巅峰对决界面
  const POINT closeCrossServer = { 916, 34 };//关闭跨服界面

  //非定时任务等待500ms防止出错
  if (!IsTimer(*plot[list].playingAdvance))
    CheckSleep(task, 500);
  //无轨任务直接退
  if (IsNoTrack(*plot[list].playingAdvance))
    return;
  for (int account = 0; account < 2; account++)
    if (work[task].isInvolved[account])
    {
      if (!work[task].isQuitUsed) //如果没有退出房间
      {
        Click(task, account, bagCancelX, bagCancelY);//取消掉背包满的弹窗
        CheckSleep(task, 500);
        if (plot[list].playingAdvance->type == 3)//跨服：直接跳竞技岛
          OldJumpToContest(task, account);
        else //其他模式
        {
          //是否为魔塔（不含宠塔）
          bool isTower = plot[list].playingAdvance->type == 2 && plot[list].playingAdvance->level / 1000 != 3;
          if (!isTower)
            Click(task, account, retreat);//除魔塔外，翻牌完成后要退出房间
          CheckSleep(task, 1000);//等待1秒

          if (plot[list].playingAdvance->type == 0) //悬赏：退出后领取奖励
            ClaimBountyReward(task, account);//领取悬赏奖励
          //对于勇士任务，房主后要关闭勇士界面
          else if (plot[list].playingAdvance->type == 1 && account == work[task].host)
            Click(task, account, closeChallenge);
          else if (plot[list].playingAdvance->type == 2)//魔塔：退出后关闭魔塔界面
          {
            if (plot[list].playingAdvance->level / 1000 == 3)
              Click(task, account, closePet);
            else
              Click(task, account, closeTower);
          }
          else if (IsPeak(*plot[list].playingAdvance)) //巅峰对决：退出后关闭巅峰对决界面
            Click(task, account, closePeak);
        }
      }
      else //如果已退出房间
      {
        //悬赏：都退出了肯定没奖励，啥也不用干；
        //勇士：房主关闭界面即可
        if (plot[list].playingAdvance->type == 1 && account == work[task].host)
          Click(task, account, closeChallenge);
        else if (plot[list].playingAdvance->type == 2)//魔塔：关闭魔塔界面
        {
          if (plot[list].playingAdvance->level / 1000 == 3)
            Click(task, account, closePet);
          else
            Click(task, account, closeTower);
        }
        else if (plot[list].playingAdvance->type == 3)//跨服：关闭跨服界面后随机跳转
        {
          Click(task, account, closeCrossServer);
          const char *jumpButton[] = { "跳转按钮", "跳转按钮_高亮" };
          WaitPictures(APPEAR, task, account, jumpButton);
          CheckSleep(task, 500);
          RandomJump(task, account);
        }
        else if (IsPeak(*plot[list].playingAdvance)) //巅峰对决：退出后关闭巅峰对决界面
          Click(task, account, closePeak);
        //假期、公会、普通图：啥也不用干
      }
      if (listStyle || plot[list].playingAdvance->missionStyle)
      {
        CheckSleep(task, 1000);
        if (listStyle == CONTEST) //大赛领取
          ClaimContestAward(task, account);
        else if (listStyle == CAMP) //营地领取
          ClaimCampAward(task, account);
        else if (listStyle == DAILY) //营地领取
          ClaimDailyAward(task, account);
        else if (plot[list].playingAdvance->missionStyle == GUILD) //公会领取
          ClaimGuildAward(task, account);
        else if (plot[list].playingAdvance->missionStyle == LOVER) //情侣领取
          ClaimLoverAward(task, account);
      }
    }
  CheckSleep(task, 1000);//退出后等待1秒，任务线程结束
}
//完成任务并退出任务线程
void FinishTask(int task)
{
  RecordLevelTime(task, 1);//记录每局用时
  if (IsAdvance(task))//高级任务执行退出程序
  {
    int list = GetList(task);
    Retreat(task);//退出房间并完成领奖、关闭窗口等操作
    reserve[task].arriveFinish = true;
    if (plot[list].playingAdvance->host == 2) //如果是同步任务，要等另一个任务一起完成
    {
      int anotherTask = IsDual2P(task) ? task - 1 : task + 1;//另一个任务编号
      while (reserve[anotherTask].arriveFinish == false) //等另一个任务也到达
        CheckSleep(task, 10);
    }
    ReturnState(task, "已完成");//进度MaxGames+1/MaxGames，第Wave波第0秒，平均用时刷新，累计时间刷新
  }
  else //普通任务直接弹窗退出
  {
    char message[30];
    ReturnState(task, "已完成");//进度MaxGames+1/MaxGames，第Wave波第0秒，平均用时刷新，累计时间刷新
    sprintf_s(message, "普通任务%d完成，共刷%d局", task, work[task].maxGames);
    isTaskStarted[task] = 0;//让按钮显示“启动”
    isRepaintRequired = 1;//发送重绘指令
    PopMessage(task, message);
  }
  ExitTask(task);
}
/*高级任务进入房间相关函数*/
//等待进入房间（检测到选卡界面），返回房间号（无房间号返回0，进房失败返回-1）
//最长等待waitTime毫秒
int WaitIndoor(int task, int account, bool noError = false, int timeout = 0)
{
  if (timeout == 0)
    timeout = WaitTime();
  int list = GetList(task);
  int digit = -1;
  int counter = 0;
  while (digit == -1)//等待房主进入房间
  {
    counter++;
    if (counter >= timeout / 100)
    {
      if (noError == false)
      {
        if (account == work[task].host)
          ReportError(task, account, "创建房间失败", "创建房间失败");
        else
          ReportError(task, account, "进入房间失败", "进入房间失败");
      }
      return -1;
    }
    CheckSleep(task, 100);

    const char *indoor[] = { "选卡界面", "选卡界面_实验室" };
    if (FindPictures(task, account, indoor))//如果检测到进入房间
    {
      digit = GetIndoorDigit(task);//读取一次房间号
      if (digit == -1) //进了房间没有房间号，视为0
        digit = 0;
    }
    else //截图失败或未进入房间，房间号为-1
      digit = -1;
  }
  CheckSleep(task, 100);
  return digit;
}
//进入第一个房间，验证房间号是否为roomDigit，是返回1，否返回0
bool EnterFirstRoom(int task, int account, int roomDigit)
{
  const int confirmPasswordX = 475, confirmPasswordY = 335;//进入房间确认密码

  DealInvitation(task, account, 0);//拒绝邀请
  Click(task, account, ListX, ListY + 20);//点击房间号
  //输入密码
  CheckSleep(task, 300);
  for (int i = 1; i <= 3; i++)//连续3下退格
    Press(task, account, 8);
  CheckSleep(task, 300);
  for (int i = 1; i <= 3; i++)//输入222
    Press(task, account, 98);
  CheckSleep(task, 300);
  Click(task, account, confirmPasswordX, confirmPasswordY);//确认密码，进入房间
  return WaitIndoor(task, account) == roomDigit;
}
//进入指定房间号的密码房（含输入密码）
int EnterRoom(int task, int account, int roomDigit)
{
  const int confirmPasswordX = 475, confirmPasswordY = 335;//进入房间确认密码
  const int roomScrollX = 723, roomScrollY = 143, roomScrollHeight = 195;//房间列表滚动条
  COLORREF roomScroll[roomScrollHeight][1] = {};

  ForcedUpdateMap(task, account);
  for (int page = 0; page <= 20; page++)
  {
    //翻到第page页并截图，已经翻到底则返回0
    if (!PageScroll(task, account, page, roomScrollX, roomScrollY, roomScroll))
      return 0;

    int height[5] = {}, room[5] = {};
    int num = GetRoomList(task, height, room);//获取房间列表本页的房间数、各个房间的高度和编号
    for (int order = 0; order < num; order++)
      if (room[order] == roomDigit)
      {
        DealInvitation(task, account, 0);//拒绝邀请
        Click(task, account, ListX, height[order]);//点击房间号
        //输入密码
        CheckSleep(task, 300);
        for (int i = 1; i <= 3; i++)//连续3下退格
          Press(task, account, 8);
        CheckSleep(task, 300);
        for (int i = 1; i <= 3; i++)//输入222
          Press(task, account, 98);
        CheckSleep(task, 300);
        Click(task, account, confirmPasswordX, confirmPasswordY);//确认密码，进入房间
        WaitIndoor(task, account);
        return 1;
      }
  }
  return 0;
}
//等待进入房间，返回房间号
int WaitCrossIndoor(int task, int account)
{
  int list = GetList(task);
  int roomDigit = -1;
  int iColor = 0;
  while (roomDigit == -1)//等待房主进入房间
  {
    iColor++;
    if (iColor >= 100)
    {
      if (account == work[task].host)
        ReportError(task, account, "跨服创房失败", "跨服创建房间失败");
      else
        ReportError(task, account, "跨服进房失败", "跨服进入房间失败");
    }
    CheckSleep(task, 100);
    if (UpdateMap(task, account))
      roomDigit = GetCrossIndoorDigit(task);
    else
      roomDigit = -1;
  }
  return roomDigit;
}
//在房间列表创建密码房，返回房间号。isMutielementRoom：是否为含有多元奇遇的宽房间
int CreatePasswordRoom(int task, int account, bool isMutielementRoom)
{
  const POINT tickPasswords[2] = { { 490, 452 }, { 511, 452 } };//勾选密码
  const POINT inputPasswords[2] = { { 686, 454 }, { 708, 454 } };//输入密码
  const POINT create = { 585, 495 };//创建房间

  CheckSleep(task, 300);//必要的停顿
  Click(task, account, tickPasswords[isMutielementRoom]);//勾选密码
  CheckSleep(task, 500);
  Click(task, account, inputPasswords[isMutielementRoom]);//点击密码输入框
  CheckSleep(task, 300);//必要的停顿
  Click(task, account, inputPasswords[isMutielementRoom]);//点击密码输入框
  CheckSleep(task, 300);//必要的停顿
  for (int i = 1; i <= 3; i++)//连续3下退格
    Press(task, account, 8);
  CheckSleep(task, 300);//必要的停顿
  for (int i = 1; i <= 3; i++)//输入222
    Press(task, account, 98);
  CheckSleep(task, 300);//必要的停顿
  Click(task, account, create);
  return WaitIndoor(task, account);//等待房主进入房间
}
//进入跨服界面并切换到2区
void EnterCrossInterface(int task, int account)
{
  int x, y;
  const POINT cross = { 352, 333 };//地图“跨服远征”点击位置
  const POINT changeZone = { 793, 32 };
  const POINT zone2 = { 814, 86 };

  if (FindPicture(task, account, "跨服界面") == 0) //如果不在跨服界面，则打开跨服界面并换到二区
  {
    Click(task, account, worldMap);//打开世界地图
    WaitPicture(APPEAR, task, account, "世界地图", &x, &y);//等待打开世界地图
    Click(task, account, cross);//打开勇士挑战
    WaitPicture(APPEAR, task, account, "跨服界面", &x, &y);//等待进入跨服界面
    SelectiveSleep(task, 500);
    WaitColorDisappear(task, account, blueBox.x, blueBox.y, boxColor);//等待服务器蓝框消失
    CheckSleep(task, 500); //蓝框消失后有个刷新过程，还是等500ms比较好
    SelectiveSleep(task, 500);
    int counter = 0;
    while (FindPicture(task, account, "跨服二区") == 0) //不断换区，换成为止
    {
      counter++;
      if (counter > 10)
        ReportError(task, account, "跨服换区失败", "跨服换区失败");
      Click(task, account, changeZone);//点击换区
      CheckSleep(task, 200);
      SelectiveSleep(task, 300);
      Click(task, account, zone2);//换到2区
      CheckSleep(task, 200);
      SelectiveSleep(task, 300);
      WaitColorDisappear(task, account, blueBox.x, blueBox.y, boxColor);//等待服务器蓝框消失
    }
  }
}
//筛选跨服关卡
void FilterCrossLevel(int task, int account, int level)
{
  int type = level / 8;//关卡类型：古堡=0，冰封=5
  int grade = level % 8;//关卡等级，8星=0，15星=7
  const int ChooseTypeX = 640, ChooseTypeY = 77;//选择关卡类型
  const int TypeX = 640, TypeY = 135, TypeHeight = 27;//关卡类型
  const int ChooseGradeX = 838, ChooseGradeY = 79;//选择关卡等级
  const int GradeX = 838, GradeY = 135, GradeHeight = 27;//关卡等级
  const POINT star15 = { 801, 323 };
  const COLORREF star15Color = 0xff9700;

  Click(task, account, ChooseTypeX, ChooseTypeY);//点开类型列表
  CheckSleep(task, 200);
  SelectiveSleep(task, 300);
  Click(task, account, TypeX, TypeY + type * TypeHeight);//选择类型
  CheckSleep(task, 200);
  SelectiveSleep(task, 300);
  //识图确认点开等级列表
  int counter = 0;
  while (GetColor(task, account, star15) != star15Color)
  {
    counter++;
    if (counter > 25) //5秒点不开报错
      ReportError(task, account, "无法点开列表", "无法点开等级筛选列表");
    if (counter % 5 == 1) //每秒再点一次
      Click(task, account, ChooseGradeX, ChooseGradeY);//点开等级列表
    CheckSleep(task, 200);
  }
  Click(task, account, GradeX, GradeY + grade * GradeHeight);//选择等级
}
//创建跨服房间
int CreateCrossRoom(int task, int account, int level)
{
  int x, y;
  int CreateX = 854, CreateY = 552;
  int ClassX = 62, ClassY = 70, ClassWidth = 102;//古堡位置和类型之间的宽度
  int LevelWidth = 226, LevelHeight = 220;
  int TickPasswordX = 123, TickPasswordY = 244;
  int type = level / 8;//关卡类型：古堡=0，冰封=5
  int grade = level % 8;//关卡等级，8星=0，15星=7
  int row = grade / 4;
  int column = grade % 4;

  Click(task, account, CreateX, CreateY);//点击创建房间
  WaitPicture(APPEAR, task, account, "跨服创房界面");//等待进入创建房间界面
  SelectiveSleep(task, 1000);
  Click(task, account, ClassX + ClassWidth * type, ClassY);//点击关卡类型
  CheckSleep(task, 300);
  SelectiveSleep(task, 700);
  const char *createButton[] = { "跨服创房按钮", "跨服创房按钮亮" };
  WaitPictures(APPEAR, task, account, createButton, &x, &y);//等待第一个关卡创房按钮亮起
  CheckSleep(task, 300);
  SelectiveSleep(task, 700);
  Click(task, account, TickPasswordX + column * LevelWidth, TickPasswordY + row * LevelHeight);//勾选密码
  CheckSleep(task, 200);
  SelectiveSleep(task, 300);
  for (int i = 1; i <= 3; i++)//输入222
    Press(task, 0, 98);
  CheckSleep(task, 200);
  SelectiveSleep(task, 300);
  Click(task, account, x + column * LevelWidth, y + row * LevelHeight);//点击该关卡的创建房间按钮
  return WaitCrossIndoor(task, account);//等待进入房间并返回房间号
}
//【跨服】进入指定房间号的密码房（含输入密码）
int EnterCrossRoom(int task, int account, int roomDigit)
{
  const POINT NextPage = { 698, 486 };//下一页
  const POINT RefreshRoom = { 892, 74 };//刷新按钮
  int ConfirmPasswordX = 490, ConfirmPasswordY = 351;//进入房间确认密码
  int x[6], y[6], room[6];
  for (int page = 0; page <= 10; page++)//最多翻页10次，翻5页就刷新
  {
    if (!UpdateMap(task, account))
      return 0;
    int roomNum = GetCrossList(task, x, y, room);//获取房间列表本页的房间数、各个房间的高度和编号
    for (int n = 0; n < roomNum; n++)
      if (room[n] == roomDigit) //如果找到了房间
      {
        Click(task, account, x[n], y[n]);//点击房间号
        CheckSleep(task, 500);
        /*输入密码*/
        for (int i = 1; i <= 3; i++)//输入222
          Press(task, account, 98);
        CheckSleep(task, 500);
        Click(task, account, ConfirmPasswordX, ConfirmPasswordY);//确认密码，进入房间
        WaitCrossIndoor(task, account);//等待进入房间
        return 1;
      }
    if (page % 5 == 0)
      Click(task, account, RefreshRoom);
    else
      Click(task, account, NextPage);
    CheckSleep(task, 1000);
  }
  return 0;
}
//进入跨服关卡
void EnterCross(int task, int level)
{
  EnterCrossInterface(task, work[task].host);//房主进入跨服2区
  CheckSleep(task, 500);//必要的停顿
  int roomDigit = CreateCrossRoom(task, work[task].host, level);//房主创建跨服房间

  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    EnterCrossInterface(task, 1 - work[task].host);//队友进入跨服2区
    CheckSleep(task, 500);//必要的停顿
    FilterCrossLevel(task, 1 - work[task].host, level);//筛选跨服关卡
    CheckSleep(task, 500);//必要的停顿
    if (EnterCrossRoom(task, 1 - work[task].host, roomDigit) == 0)//按房间号进入房主的关卡（找不到则翻页）
    {
      char message[200] = {};
      sprintf_s(message, "未找到房间号%d", roomDigit);
      ReportError(task, 1 - work[task].host, "未找到房间号", message);
    }
  }
}
//搜索账号account的名字
void SearchInviteeName(int task, int account)
{
  POINT Input = { 692, 143 };
  POINT Search = { 727, 142 };

  Click(task, 1 - account, Input);
  CheckSleep(task, 100);
  for (int i = 1; i <= 10; i++)//连续10下退格
    Press(task, 1 - account, 8);
  PressCtrl(task, 1 - account, 'V');//粘贴
  Click(task, 1 - account, Search);//搜索
  CheckSleep(task, 100);//必要的停顿
  const char *addFriend[] = { "加为好友", "加为好友_亮" };
  WaitPictures(APPEAR, task, 1 - account, addFriend);
}
//细角色名和邀请列表第line行的名字是否匹配
bool IsLightNameEqual(int task, int account, int line)
{
  const COLORREF inviteeNameColor = 0x993300;//邀请列表细角色名颜色
  const COLORREF baseColor[2] = { 0xf4e9ba, 0xece2a3 };//细角色名的两种底色
  int baseNum[2] = {};//细角色名区域中两种底色的数量
  int inviteeX = 460, inviteeY = 150 + line * 30;//第line个细角色名位置

  //判断底色
  for (int y = 0; y < lightNameHeight; y++)
    for (int x = 0; x < lightNameWidth; x++)
      for (int i = 0; i < 2; i++)
        if (work[task].map[inviteeY + y][inviteeX + x] == baseColor[i])
          baseNum[i]++;
  int baseType = baseNum[0] > baseNum[1] ? 0 : 1;//哪种底色数量多，哪种就是底色
  //逐像素匹配
  for (int y = 0; y < lightNameHeight; y++)
    for (int x = 0; x < lightNameWidth; x++)
    {
      int fileColor = 2;//文件中细角色名本像素颜色类型，默认为杂色（2）
      if (work[task].lightName[account][y][x] == inviteeNameColor) //文字颜色（1）
        fileColor = 1;
      if (work[task].lightName[account][y][x] == 0xffffff) //底色（0）
        fileColor = 0;

      int inviteeColor = 2;//邀请列表中细角色名本像素颜色类型，默认为杂色（2）
      if (work[task].map[inviteeY + y][inviteeX + x] == inviteeNameColor)  //文字颜色（1）
        inviteeColor = 1;
      if (work[task].map[inviteeY + y][inviteeX + x] == baseColor[baseType]) //底色（0）
        inviteeColor = 0;

      if (fileColor != inviteeColor)
        return false;
    }
  return true;
}
//等待出现邀请界面并接受，成功接受邀请返回1，没有被邀请返回0
int WaitInvitationAndAccept(int task, int account)
{
  POINT accept = { 449, 403 };
  if (TryWaitPicture(APPEAR, task, account, "被邀请界面", WaitTime() / 2))//如果3秒内出现了被邀请界面
  {
    Click(task, account, accept);//接受邀请
    return 1;
  }
  return 0;
}
//账号1-account邀请account加入room房间（room==0则不验证房间号）
//邀请成功返回1，否则返回0
void Invite(int task, int account, int room = 0)
{
  const int pageNum = 8;
  //翻页按钮位置（大厅5页，好友3页）
  const POINT pageButton[pageNum] = {
    { 458, 130 }, { 562, 459 }, { 562, 459 }, { 562, 459 }, { 562, 459 },
    { 527, 130 }, { 562, 459 }, { 562, 459 } };
  POINT openInvitation = { 410, 546 };
  POINT sendInvitation = { 600, 156 };
  POINT quitInvitation = { 587, 491 };

  int inviteeOrder = -1;//被邀请者序号
  //邀请结果：0=成功，1=找不到角色名（默认），2=未进入正确房间，3=队友未收到邀请
  int invitationResult = 1;

  Click(task, 1 - account, openInvitation);//房主打开邀请界面
  CheckSleep(task, 500);//必要的停顿
  for (int page = 0; page < pageNum; page++)
  {
    Click(task, 1 - account, pageButton[page]);//房主打开大厅/好友界面
    CheckSleep(task, 500);//必要的停顿
    if (!UpdateMap(task, 1 - account))//房主截图
      break;
    //在邀请列表中寻找队友名字
    for (int line = 0; line < 10; line++)
      //如果队友细角色名和邀请列表第line行名字匹配，进行记录
      if (IsLightNameEqual(task, account, line))
      {
        inviteeOrder = line;
        break;
      }
    if (inviteeOrder >= 0) //如果找到了被邀请者的名字
    {
      DealInvitation(task, account, 0);//如果队友已被邀请，拒绝邀请
      Click(task, 1 - account, sendInvitation.x, sendInvitation.y + 30 * inviteeOrder);//房主发送邀请
      if (WaitInvitationAndAccept(task, account) == 1)//如果队友成功接受邀请
      {
        if (room == WaitIndoor(task, account))//检验房间号是否正确
          invitationResult = 0; //正确说明邀请成功
        else
          invitationResult = 2; //否则记录“未进入正确房间”
      }
      else
        invitationResult = 3; //虽然成功点击了邀请按钮，但队友没有收到邀请
      break;
    }
  }

  if (invitationResult == 1)
    ReportError(task, account, "未找到队友名字", "邀请列表中未找到队友名字");
  else if (invitationResult == 2)
    ReportError(task, account, "未进入正确房间", "未进入正确房间");
  else if (invitationResult == 3)
    ReportError(task, account, "队友未收到邀请", "队友未收到邀请");

  Click(task, 1 - account, quitInvitation);//房主退出邀请界面
}
//保存新角色名到文件
void SaveRoleName(int task, int account)
{
  WaitForSingleObject(hMutexUserImage, INFINITE);

  char path[maxPath];
  //找一个未使用的图片名称
  for (int code = 0; code < 999; code++)
  {
    sprintf_s(path, "用户参数\\角色名\\%d.png", code);
    if (!FileExist(path))
    {
      sprintf_s(roleName[roleNameNum].name, "%d", code);//填写图片名称
      //拷贝粗角色名到角色名数组
      CopyMap(roleName[roleNameNum].image, 0, 0,
        heavyNameWidth, heavyNameHeight, work[task].heavyName[account]);
      //拷贝细角色名到角色名数组
      CopyMap(roleName[roleNameNum].image, heavyNameWidth, 0,
        lightNameWidth, lightNameHeight, work[task].lightName[account]);
      //保存角色名到文件
      ColorToBitmap(roleName[roleNameNum].image, path);
      //角色名数量+1
      roleNameNum++;
      break;
    }
  }

  ReleaseMutex(hMutexUserImage);
}
//从好友列表截取细角色名
void CatchLightName(int task, int account)
{
  const COLORREF friendNameColor = 0xa45815;//好友列表细角色名颜色
  const COLORREF inviteeNameColor = 0x993300;//邀请列表细角色名颜色
  const COLORREF baseColor[2] = { 0xf4e9bb, 0xece2a4 };//细角色名的两种底色
  int baseNum[2] = {};//细角色名区域中两种底色的数量

  //拷贝细角色名原始图像
  CopyMap(work[task].lightName[account], work[task].map, lightNameX, lightNameY);

  //判断底色
  for (int y = 0; y < lightNameHeight; y++)
    for (int x = 0; x < lightNameWidth; x++)
      for (int i = 0; i < 2; i++)
        if (work[task].lightName[account][y][x] == baseColor[i])
          baseNum[i]++;
  int baseType = baseNum[0] > baseNum[1] ? 0 : 1;//哪种底色数量多，哪种就是底色

  //把底色变白
  for (int y = 0; y < lightNameHeight; y++)
    for (int x = 0; x < lightNameWidth; x++)
    {
      if (work[task].lightName[account][y][x] == baseColor[baseType])
        work[task].lightName[account][y][x] = 0xffffff;
      else if (work[task].lightName[account][y][x] == friendNameColor)
        work[task].lightName[account][y][x] = inviteeNameColor;
    }
}
//通过好友列表截取被邀请者account的角色名
int CatchInviteeName(int task, int account)
{
  POINT friendButton = { 40, 106 };//好友按钮
  POINT closeFriend = { 757, 102 };//关闭好友界面

  for (int i = 0; i < 3; i++)
    Click(task, account, 152, 44);//三击角色名（全选）
  CheckSleep(task, 500);//必要的停顿
  PressCtrl(task, account, 'C');//复制
  Click(task, 1 - account, friendButton);//房主打开好友界面
  WaitPicture(APPEAR, task, 1 - account, "好友界面");
  CheckSleep(task, 1000);
  SearchInviteeName(task, account);//搜索队友名字
  CatchLightName(task, account);//获取细角色名
  Click(task, 1 - account, closeFriend);//关闭好友界面
  SaveRoleName(task, account);//保存新的角色名
  return 1;
}
//创建假期房间
int CreateVacationRoom(int task, int account)
{
  const POINT startChallenge = { 655, 198 };
  ClickActivity(task, account, "假期图标");//点击假期图标
  WaitPicture(APPEAR, task, account, "假期界面");//等待打开假期界面
  SelectiveSleep(task, 500);
  Click(task, account, startChallenge);//进入房间
  return WaitIndoor(task, account);
}
//进入假期关卡
void EnterVacation(int task)
{
  int room = CreateVacationRoom(task, work[task].host);//房主创建假期房间
  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    SelectiveSleep(task, 500);
    Invite(task, 1 - work[task].host, room);//邀请队友加入房间，邀请失败会报错
  }
}
//进入勇士挑战房间列表
void EnterChallengeRoomList(int task, int account)
{
  if (FindPicture(task, account, "勇士界面"))//如果已经打开勇士界面，则无需操作
    return;
  //打开世界地图，点击勇士挑战。房主：切换BOSS，创建房间；队友：进入房间
  const POINT challenge = { 523, 437 };
  Click(task, account, worldMap);//打开世界地图
  WaitPicture(APPEAR, task, account, "世界地图");//等待打开世界地图
  Click(task, account, challenge);//打开勇士挑战
  if (!TryWaitPicture(APPEAR, task, account, "勇士界面", WaitTime())) //如果没等到就再打开一次
  {
    Click(task, account, worldMap);//打开世界地图
    WaitPicture(APPEAR, task, account, "世界地图");//等待打开世界地图
    Click(task, account, challenge);//打开勇士挑战
    WaitPicture(APPEAR, task, account, "勇士界面");//等待进入勇士挑战界面
  }
}
//读取当前选择的勇士BOSS编号
int GetChallengeLevel(int task, int account)
{
  int x, y;
  UpdateMap(task, account);
  for (int level = 0; level < challengeNum; level++)
    if (FindPictureInMap(task, challenge[level], &x, &y) == 1)//如果找到BOSS
      return level;
  return -1;
}
//获取勇士挑战列表数量
int GetChallengeListNum(int task, int account)
{
  int ListX = 755, ListY = 149, ListHeight = 44;
  POINT point[10] = { ListX, ListY };
  COLORREF color[10];
  COLORREF ListColor = 0x6d9d37;
  for (int i = 1; i <= 9; i++)
  {
    point[i].x = point[i - 1].x;
    point[i].y = point[i - 1].y + ListHeight;
  }
  GetMultiColor(task, account, 10, color, point);
  for (int i = 0; i < 10; i++)//检查10个点的颜色是不是列表颜色
    if (color[i] != ListColor)
      return i;
  return 10;
}
//切换到指定BOSS
void SwitchToChallengeLevel(int task, int account, int level)
{
  int LastBossX = 154, LastBossY = 176;//上一个BOSS
  int NextBossX = 496, NextBossY = 176;//下一个BOSS
  int x, y;
  int CurrentLevel = GetChallengeLevel(task, account);//获取当前勇士BOSS编号

  //由于CurrentLevel一定会超过level，所以不需要counter
  while (CurrentLevel < level)//当前关卡小于目标关卡，向后切换
  {
    Click(task, account, NextBossX, NextBossY);
    if (WaitPicture(APPEAR, task, account, challenge[CurrentLevel + 1], &x, &y))
      CurrentLevel++;
    else
      ReportError(task, account, "BOSS切换失败", "勇士BOSS向后切换失败");
  }
  while (CurrentLevel > level)//当前关卡大于目标关卡，向前切换
  {
    Click(task, account, LastBossX, LastBossY);
    if (WaitPicture(APPEAR, task, account, challenge[CurrentLevel - 1], &x, &y))
      CurrentLevel--;
    else
      ReportError(task, account, "BOSS切换失败", "勇士BOSS向前切换失败");
  }
}
//进入指定房间号，进入失败返回0
int EnterChallengeRoom(int task, int account, int roomDigit)
{
  int ListX = 755, ListY = 149, ListHeight = 44;
  int num = GetChallengeListNum(task, account);//获取列表数量
  for (int i = 0; i < num; i++)
  {
    Click(task, account, ListX, ListY + i * ListHeight);//点击第i个房间
    if (WaitIndoor(task, account, true) == roomDigit) //如果房间号匹配则结束
      return 1;
    Click(task, account, retreat);//不匹配则退出房间
    WaitPicture(APPEAR, task, account, "勇士界面");//等待进入勇士挑战界面
    SelectiveSleep(task, 1000);
  }
  return 0; //进入失败返回0
}
//创建勇士房间
int CreateChallengeRoom(int task, int account, int level)
{
  int CreateX = 450, CreateY = 533;//创建房间
  EnterChallengeRoomList(task, account);//房主进入勇士挑战界面
  SwitchToChallengeLevel(task, account, level);//房主切换到指定BOSS
  SelectiveSleep(task, 1000);
  Click(task, account, CreateX, CreateY);//房主创建房间
  return WaitIndoor(task, account);//等待房主进入房间，读取房主房间号
}
//双号进入勇士挑战房间
void EnterChallenge(int task, int level)
{
  int room = CreateChallengeRoom(task, work[task].host, level);//房主创建假期房间
  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    SelectiveSleep(task, 500);
    Invite(task, 1 - work[task].host, room);//邀请队友加入房间，邀请失败会报错
  }
}
//进入魔塔界面(tag：1单人 2双人 3密室 4宠塔)
void EnterTower(int task, int account, int tag)
{
  const int towerX = 319, towerY = 456;
  //单人 双人 占位 密室
  const POINT tagLocation[4] = { { 43, 67 }, { 114, 67 }, { 0, 0 }, { 189, 67 } };

  if (tag == 3) //宠塔
  {
    ClickActivity(task, account, "萌宠图标");
    WaitPicture(APPEAR, task, account, "萌宠神殿");
    CheckSleep(task, 500);
    WaitColorDisappear(task, account, 416, 557, 0x4f6e87, "宠塔加载超时");//等待层数正常加载
  }
  else //单塔 双塔 密室
  {
    Click(task, account, worldMap);//打开世界地图
    WaitPicture(APPEAR, task, account, "世界地图");//等待打开世界地图
    Click(task, account, towerX, towerY);//进入魔塔蛋糕
    WaitPicture(APPEAR, task, account, "魔塔界面");//等待进入魔塔选层界面
    CheckSleep(task, 500);
    WaitColorDisappear(task, account, 273, 557, 0x4f6e87, "魔塔加载超时");//等待层数正常加载
    CheckSleep(task, 500);
    Click(task, account, tagLocation[tag - 1]);//选择标签（单人 双人 密室）
  }
  CheckSleep(task, 500);
}
//公会任务是否正在进行中
bool isGuildInProgress(int task, int guildY)
{
  for (int x = 0; x < guildStateWidth; x++)
    for (int y = 0; y < guildStateHeight; y++)
    {
      char isWhite = work[task].map[guildY + y][guildStateX + x] == 0xffffff;
      if (isWhite != guildInProgress[x][y]) //有一个颜色不匹配就返回false
        return false;
    }
  return true;
}
//添加高级任务
void AddAdvance(int list, int type, int level, int maxGames, int deck0, int deck1)
{
  Advance &advance = plot[list].advance[plot[list].advanceNum];
  memset(&advance, 0, sizeof(advance));
  advance.isSelected = 1;
  advance.type = type;
  advance.level = level;
  advance.maxGames = maxGames;
  advance.isProceed = 1;
  strcpy_s(advance.track[0], "Track.txt");
  advance.deck[0] = deck0;
  advance.deck[1] = deck1;
  plot[list].advanceNum++;
}
//为第order条任务填写通用轨道
void MatchCommonTrack(int list, int order)
{
  char levelName[100];//关卡名
  char track[2][maxPath] = {};//房主和队友的轨道路径
  Advance &advance = plot[list].advance[order];
  if (advance.isSelected == 0) //跳过未勾选的任务
    return;
  if (IsNoTrack(advance)) //跳过无轨关卡
    return;
  for (int account = 0; account < 2; account++)
    strcpy_s(advance.track[account], "");//轨道先留空
  GetLevelName(levelName, advance.type, advance.level);//获取关卡名称 
  char levelProfix[10] = {};//关卡编号前缀
  if (advance.type != 2) //非魔塔轨道文件带关卡编号前缀
    sprintf_s(levelProfix, "%d", advance.level);
  //获取房主和队友轨道路径
  sprintf_s(track[0], "预制轨道\\通用轨道\\%d%s\\%s%s.txt",
    advance.type, typeName[advance.type], levelProfix, levelName);
  sprintf_s(track[1], "预制轨道\\通用轨道\\%d%s\\%s%s+.txt",
    advance.type, typeName[advance.type], levelProfix, levelName);
  if (advance.host == 2)//同步任务：激活两个卡组，轨道都填房主轨道
  {
    if (FileExist(track[0]))
      for (int account = 0; account < 2; account++)
        strcpy_s(advance.track[account], track[0]);
  }
  else //单线程任务：分别填写房主和队友轨道
  {
    if (FileExist(track[0]))
      strcpy_s(advance.track[advance.host], track[0]);
    if (FileExist(track[1]))
      strcpy_s(advance.track[1 - advance.host], track[1]);
  }
}
//一键配轨：为所有任务填写通用轨道
void MatchCommonTrack(int list)
{
  for (int order = 0; order < plot[list].advanceNum; order++)
    MatchCommonTrack(list, order);
}
//带弹窗选择的一键配轨
void MatchCommonTrackWithChoice(int list)
{
  const char *message = "将为勾选的任务填写通用轨道，\n是否继续？";
  if (PopMessageOK(GetTask(list), message) == IDOK)
    MatchCommonTrack(list);
}
//创建默认高级任务列表
void CreateInitialList(int list)
{
  plot[list].advanceNum = 0;
  AddAdvance(list, 17, 0, 1, 1, 1);//签到
  AddAdvance(list, 17, 1, 1, 1, 1);//施肥
  AddAdvance(list, 0, 0, 1, 1, 1);//美味悬赏
  AddAdvance(list, 0, 1, 1, 1, 1);//火山悬赏
  AddAdvance(list, 0, 2, 1, 1, 1);//浮空悬赏
  AddAdvance(list, 1, 17, 10, 1, 1);//勇士
  AddAdvance(list, 2, 1151, 10, 1, 1);//魔塔
  AddAdvance(list, 3, 35, 10, 1, 1);//跨服
  AddAdvance(list, 5, 0, 1, 1, 1);//公会任务
  AddAdvance(list, 6, 7, 45, 1, 1);//神殿
  MatchCommonTrack(list);
  for (int order = 2; order <= 4; order++)
    strcpy_s(plot[list].advance[order].track[0], "Track.txt");
}
//获取顺序表任务条数
int GetSequenceSize(int target, int issue)
{
  if (target == GUILD)
    return guildSequenceSize;
  if (target == LOVER)
    return loverSequenceSize;
  if (target == CONTEST)
    return tournament[issue].minStep;
  return 0;
}
//获取顺序表指针
Contest *GetSequence(int target, int issue)
{
  if (target == GUILD)
    return guildSequence;
  if (target == LOVER)
    return loverSequence;
  if (target == CONTEST)
    return tournament[issue].solution;
  return nullptr;
}
//获取任务列表指针
Advance *GetAdvancePointer(int list, int target, int issue)
{
  if (target == ADVANCE)
    return plot[list].advance;
  if (target == GUILD)
    return plot[list].guildAdvance;
  if (target == LOVER)
    return plot[list].loverAdvance;
  if (target == CONTEST)
    return plot[list].contestAdvance[issue];
  return nullptr;
}
//获取任务数量引用
int &GetAdvanceNumRef(int list, int target, int issue)
{
  if (target == ADVANCE)
    return plot[list].advanceNum;
  if (target == GUILD)
    return plot[list].guildAdvanceNum;
  if (target == LOVER)
    return plot[list].loverAdvanceNum;
  if (target == CONTEST)
    return plot[list].contestAdvanceNum[issue];
  return plot[list].advanceNum;
}
//获取任务锁定引用
bool &GetLockRef(int list, int target, int issue)
{
  if (target == ADVANCE)
    return plot[list].isAdvanceLocked;
  if (target == GUILD)
    return plot[list].isGuildAdvanceLocked;
  if (target == LOVER)
    return plot[list].isLoverAdvanceLocked;
  if (target == CONTEST)
    return plot[list].isContestAdvanceLocked[issue];
  return plot[list].isAdvanceLocked;
}
//从path读取任务列表。target：0=Advance，1=GuildAdvance
int LoadListFrom(int list, char *path, int target, int issue)
{
  Advance *advance = GetAdvancePointer(list, target, issue);
  int &advanceNum = GetAdvanceNumRef(list, target, issue);
  bool &isLocked = GetLockRef(list, target, issue);

  char s[1000];//列表一行信息
  FILE *f;

  WaitForSingleObject(hMutexLoadList, INFINITE);

  if (fopen_s(&f, path, "r")) //读取失败
  {
    if (target == ADVANCE) //如果是读取当前列表失败，则创建初始列表
      CreateInitialList(list);
    ReleaseMutex(hMutexLoadList);
    return 0;
  }
  if (target == ADVANCE)
    memset(plot[list].harmony, 0, sizeof(plot[list].harmony));
  memset(advance, 0, maxAdvanceNum * sizeof(Advance));
  advanceNum = 0;
  isLocked = false;

  int totalVersion = 0;//版本号
  while (!feof(f))
  {
    fgets(f, s);//读取一行
    if (strstr(s, "列表版本号")) //新版
    {
      int mainVersion = 0, secondVersion = 0;//主版本号和次版本号
      sscanf_s(s, "列表版本号=v%d.%d", &mainVersion, &secondVersion);
      if (secondVersion >= 10) //以前会把3.9.1写成3.91，这个要视为3.9
        secondVersion = secondVersion / 10;
      totalVersion = mainVersion * 10 + secondVersion;
      fgets(f, s);
      //4.8版列表中有温馨id
      if (totalVersion >= 48 && target == ADVANCE)
      {
        int harmonyNum = 0;
        while (strstr(s, "openid=") == s)
        {
          if (harmonyNum < maxHarmonySize)
            strcpy_s(plot[list].harmony[harmonyNum], s + 7);
          harmonyNum++;
          fgets(f, s);
        }
      }
      if (strstr(s, "锁定=") == s)
      {
        isLocked = atoi(s + 5) == 1;
        fgets(f, s);
      }
    }
    if (strlen(s) > 0)//非空才读取信息
    {
      int itemsScanned = sscanf_s(s, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &advance[advanceNum].isSelected,
        &advance[advanceNum].type, &advance[advanceNum].level, &advance[advanceNum].maxGames,
        &advance[advanceNum].deck[0], &advance[advanceNum].deck[1], &advance[advanceNum].host,
        &advance[advanceNum].missionStyle, &advance[advanceNum].isProceed, &advance[advanceNum].timer);

      if (advance[advanceNum].type == 2 && advance[advanceNum].level < 1000)//魔塔类型小于1000的关卡（单塔）增加1000
        advance[advanceNum].level += 1000;
      //远古版本转化为3.8
      if (version == 0 && advance[advanceNum].type >= 5) //远古版本没有公会副本，岛屿类型+1
        advance[advanceNum].type++;
      //3.8转化为4.5
      if (totalVersion <= 38 && advance[advanceNum].type == 5) //3.8之前的版本没有公会任务，公会副本关卡+1
        advance[advanceNum].level++;
      //4.5转化为4.6
      if (totalVersion <= 45 && advance[advanceNum].type == 5 && advance[advanceNum].level >= 1)//公会副本类型-1
        advance[advanceNum].type--;

      //默认设置
      if (itemsScanned < 7)
        advance[advanceNum].host = 0;//房主
      if (itemsScanned < 8)
        advance[advanceNum].missionStyle = 0;//识别任务标记
      if (itemsScanned < 9)
        advance[advanceNum].isProceed = 1;//是否继续作战
      if (itemsScanned < 10)
        advance[advanceNum].timer = 0;//定时时刻

      for (int account = 0; account < 2; account++)
        fgets(f, advance[advanceNum].track[account]);//读取一行
      advanceNum++;
    }
  }
  fclose(f);
  ReleaseMutex(hMutexLoadList);
  return 1;
}
//从列表文件读取任务列表
int LoadList(int list)
{
  return LoadListFrom(list, plot[list].advancePath, ADVANCE, 0);
}
//保存高级任务列表
void SaveListTo(int list, char *path, int target, int issue)
{
  WaitForSingleObject(hMutexSaveList, INFINITE);

  Advance *advance = GetAdvancePointer(list, target, issue);
  int &advanceNum = GetAdvanceNumRef(list, target, issue);
  bool &isLocked = GetLockRef(list, target, issue);

  FILE *f;
  if (!fopen_s(&f, path, "w"))
  {
    fprintf(f, "列表版本号=%s\n", version);
    if (target == ADVANCE)
      for (int i = 0; i < maxHarmonySize; i++)
      {
        if (strlen(plot[list].harmony[i]) == 0)
          break;
        fprintf_s(f, "openid=%s\n", plot[list].harmony[i]);
      }
    fprintf(f, "锁定=%d\n", (int)isLocked);
    for (int order = 0; order < advanceNum; order++)
    {
      //有假同步标记的，房主一律保存2
      int host = advance[order].dual ? 2 : advance[order].host;
      fprintf(f, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n%s\n%s\n", advance[order].isSelected, advance[order].type,
        advance[order].level, advance[order].maxGames, advance[order].deck[0], advance[order].deck[1],
        host, advance[order].missionStyle, advance[order].isProceed, advance[order].timer,
        advance[order].track[0], advance[order].track[1]);
    }
    fclose(f);
  }

  ReleaseMutex(hMutexSaveList);
}
//任务列表是否改变
int IsAdvanceChanged(int list)
{
  char path[50];
  sprintf_s(path, "自动备份\\任务列表\\%s\\%d.txt", plot[list].backupFolder, plot[list].currentBackup);
  return (int)!IsFileEqual(plot[list].advancePath, path);//如果当前任务列表与最新备份不同，则视为改变
}
//保存至某一个备份
void SaveBackup(int list, int order)
{
  char path[50];
  sprintf_s(path, "自动备份\\任务列表\\%s\\%d.txt", plot[list].backupFolder, order);
  SaveListTo(list, path, ADVANCE, 0);
}
//保存高级任务并备份
void SaveList(int list)
{
  SaveListTo(list, plot[list].advancePath, ADVANCE, 0);
  if (IsAdvanceChanged(list))//如果列表有变，进行备份
  {
    plot[list].currentBackup++;
    plot[list].backupNum = plot[list].currentBackup;
    SaveBackup(list, plot[list].currentBackup);
  }
}
//是否情侣任务文字颜色
bool IsLoverColor(COLORREF color)
{
  const COLORREF loverColor[] = { 0xd1e2f4, 0x6cfb4b, 0xffbbff, 0x00ffff, 0xb4a89f, 0x7bb840 };
  for (int i = 0; i < sizeof(loverColor) / sizeof(COLORREF); i++)
    if (color == loverColor[i])
      return true;
  return false;
}
//识别情侣任务
void ReadLover(int task, int account, bool(&loverExist)[maxAdvanceNum])
{
  //截取情侣任务面板
  int counter = 0;
  while (!UpdateMap(task, account))
  {
    counter++;
    if (counter > 10)
      ReportError(task, account, "任务截图失败", "情侣任务截图失败");
    CheckSleep(task, 100);
  }
  const int loverTextDistance = 15;
  const int findTextX[3] = { 130, 380, 630 };//文字起始查找位置
  const int findButtonX[3] = { 170, 420, 670 };//领取按钮查找范围
  const int findButtonY = 410, findButtonWidth = 110, findButtonHeight = 35;//领取按钮查找范围
  //识别3个情侣任务
  char curLoverName[100] = {};//当前情侣任务关卡全名
  for (int column = 0; column < 3; column++)
  {
    //0. 判断情侣任务是否已完成，已完成的不予识别
    const char *loverFinished[] = { "情侣任务可领取", "情侣任务可领取_亮", "情侣任务已领取" };
    if (FindPicturesInMap(task, loverFinished, nullptr, nullptr, findButtonX[column], findButtonY,
      findButtonX[column] + findButtonWidth, findButtonY + findButtonHeight))
      continue;

    //1. 查找文字起始位置X和结束位置X
    int loverBeginX = -1;//文字起始X
    const int loverTextY = 135;//文字Y
    int loverEndX = -1;//文字结束X
    int loverTextNum = 0;//已读完的字数
    int loverTextWidth[20] = {};//第i个字的宽度
    int loverTextX[20] = {};//第i个字的X
    for (int x = findTextX[column]; x < findTextX[column] + 186; x++)
    {
      //判断x是否处于文字区域
      bool isTextAreaX = false;
      for (int y = loverTextY; y < loverTextY + textHeight; y++)
        if (IsLoverColor(work[task].map[y][x]))
          isTextAreaX = true;

      if (isTextAreaX) //如果x处于文字区
      {
        loverEndX = x;//更新文字结束X
        if (loverTextWidth[loverTextNum] == 0) //记录这个字的起始X
          loverTextX[loverTextNum] = x;
        loverTextWidth[loverTextNum]++;//更新这个字的宽度
      }
      else //如果x不处于文字区
      {
        if (loverTextWidth[loverTextNum] > 0)//如果刚才还处于文字区
          loverTextNum++;//记录字数+1
      }
    }
    //判断文字起始X
    if (loverTextWidth[0] == 14) //第1个字满宽：直接确定文字起始X
      loverBeginX = loverTextX[0];
    else if (loverTextWidth[0] == 13) //第1个字宽度13，要看第二个字
    {
      if (loverTextWidth[1] == 14) //第2个字满宽（可能是神殿）：用第2个字确定文字起始X
        loverBeginX = loverTextX[1] - loverTextDistance;
      else //不是神殿
        loverBeginX = loverTextX[0] - 1;
    }
    else //第1个字宽度12，直接确定文字起始X
      loverBeginX = loverTextX[0] - 1;

    //2. 文字识别
    strcpy_s(curLoverName, "");
    while (loverBeginX <= loverEndX)
    {
      //2.1 生成文字图像
      COLORREF curText[textHeight][textWidth] = {};//文字图像
      for (int y = 0; y < textHeight; y++)
        for (int x = 0; x < textWidth; x++)
          if (IsLoverColor(work[task].map[loverTextY + y][loverBeginX + x]))
            curText[y][x] = 0xffffff;
      //2.2 识别当前文字
      char word[4] = {};
      if (TrieSearch(word, curText, 14))
        strcat_s(curLoverName, word);
      loverBeginX += loverTextDistance;//调至下一个文字
    }

    //3.查找任务名对应的预制列表任务ID并记入loverExist数组
    for (int order = 0; order < loverSequenceSize; order++)
      if (strstr(curLoverName, levelMap[loverSequence[order].code].name))
      {
        loverExist[order] = true;
        break;
      }
  }
}
/*
//测试情侣任务识别
void TestReadLover(int task, int account)
{
  char lover[3][100] = {};
  ReadLover(task, account, lover);
  char message[100] = {};
  sprintf_s(message, "%s\n%s\n%s", lover[0], lover[1], lover[2]);
  RecordError(task, account, "情侣任务识别", message, END_TASK);
}
*/
//识别公会任务，将“进行中”的战斗任务编号在guildExist中记为1；发现新公会任务返回true
bool ReadGuild(int task, int account, bool(&guildExist)[maxAdvanceNum])
{
  bool isNewGuildPrompted = false;//是否已经提示过新公会任务
  int list = GetList(task);
  //截取公会任务面板
  int counter = 0;
  while (!UpdateMap(task, account))
  {
    counter++;
    if (counter > 10)
      ReportError(task, account, "任务截图失败", "公会任务截图失败");
    CheckSleep(task, 100);
  }
  //识别任务存入数组
  int streak = 0; //连续颜色数量
  for (int y = 162; y <= 536; y++) //在高度范围内查找公会任务并保存文件
  {
    if (work[task].map[y][guildX - 3] == guildBarColor)
    {
      streak++;
      if (streak == guildHeight + 1) //找到一条公会任务，进行记录
      {
        int guildY = y - guildHeight;
        GetCurGuildImage(task, guildY);
        //与已有的每个任务比较
        for (int order = 0; order < guildNum; order++)
        {
          if (guildEqual(task, order)) //与任务order图像一致则记录编号
          {
            if (isGuildInProgress(task, guildY)) //如果状态为“进行中”
            {
              if (order == 30 && work[task].map[guildY + 2][guildX + 3] != 0x00ffff)
                guildExist[31] = true;//薄荷海滩夜晚
              else
              {
                guildExist[order] = true;//记录任务编号存在
                if (order == 78) //78号任务“鼠族难题”对应两个任务
                  guildExist[79] = true;
              }
            }
            break;
          }
          if (order == guildNum - 1)//如果匹配失败，截图并记录
          {
            char newGuildPath[maxPath] = {};
            char newTrackPath[maxPath] = {};
            sprintf_s(newGuildPath, "附加程序\\公会任务\\%d.png", guildNum + 1);
            sprintf_s(newTrackPath, "预制轨道\\公会任务\\%d_未命名.txt", guildNum + 1);
            ColorToBitmap(work[task].map, newGuildPath, guildX, guildY, guildWidth, guildHeight);

            LoadGuildImage();//更新公会任务截图
            LoadListFrom(list, "预制列表\\公会任务.txt", GUILD, 0);
            plot[list].guildAdvance[plot[list].guildAdvanceNum].isSelected = 0;
            plot[list].guildAdvance[plot[list].guildAdvanceNum].type = 6;
            plot[list].guildAdvance[plot[list].guildAdvanceNum].level = 2;
            plot[list].guildAdvance[plot[list].guildAdvanceNum].maxGames = 1;
            plot[list].guildAdvance[plot[list].guildAdvanceNum].host = 0;
            strcpy_s(plot[list].guildAdvance[plot[list].guildAdvanceNum].track[0], newTrackPath);
            strcpy_s(plot[list].guildAdvance[plot[list].guildAdvanceNum].track[1], "");
            plot[list].guildAdvance[plot[list].guildAdvanceNum].deck[0] = 1;
            plot[list].guildAdvance[plot[list].guildAdvanceNum].deck[1] = 1;
            plot[list].guildAdvance[plot[list].guildAdvanceNum].missionStyle = 0;
            plot[list].guildAdvanceNum++;
            SaveListTo(list, "预制列表\\公会任务.txt", GUILD, 0);
          }
        }
      }
    }
    else
      streak = 0;
  }
  return isNewGuildPrompted;
}
//当前map是否处于房间列表界面
bool InRoomList(int task, int account)
{
  //房间列表判定条颜色
  const COLORREF roomListBarColor = 0x56a1d0;
  //房间列表判定条（2个）位置
  const int roomListBarX[2] = { 210, 413 }, roomListBarY = 102,
    roomListBarWidth = 100, roomListBarHeight = 20;

  for (int i = 0; i < 2; i++)
    for (int y = roomListBarY; y < roomListBarY + roomListBarHeight; y++)
      for (int x = roomListBarX[i]; x < roomListBarX[i] + roomListBarWidth; x++)
        if (work[task].map[y][x] != roomListBarColor)
          return false;
  return true;
}
//等待进入房间列表
void WaitRoomList(int task, int account)
{
  int timeout = WaitTime();
  int counter = 0;
  ForcedUpdateMap(task, account);
  while (!InRoomList(task, account))
  {
    counter++;
    if (counter >= timeout / 100)//timeout ms还未进入预期界面
      ReportError(task, account, "未进入房间列表", "未进入房间列表");
    CheckSleep(task, 100);
    ForcedUpdateMap(task, account);
  }
}
//【全程识图】进入公会副本房间列表，并跳转到无人区
void EnterGuildRoomList(int task, int account, int level)
{
  int GuildLevelX = 154, GuildLevelY = 415, GuildLevelWidth = 211;//选择公会关卡
  const POINT Jump = { 870, 560 };//跳转按钮
  const POINT Guild = { 870, 222 };//跳转到公会副本

  if (FindPicture(task, account, "跳转列表") == 0)
  {
    Click(task, account, Jump);//点击跳转按钮
    WaitPicture(APPEAR, task, account, "跳转列表");//等待打开跳转列表
  }
  Click(task, account, Guild);//跳转到公会副本
  WaitPicture(APPEAR, task, account, "公会副本进入地图");//等待进入公会副本界面，且“进入地图”按钮亮起
  Click(task, account, GuildLevelX + GuildLevelWidth * (level - 1), GuildLevelY);//选关进入房间列表
  WaitRoomList(task, account);
  RandomJump(task, account);

  //房主尽量在无人区建房
  if (account == work[task].host)
  {
    int list = GetList(task);
    for (int i = 0; i < 7; i++)
    {
      //如果到了无人区则退出循环
      if (TryWaitPicture(APPEAR, task, account, "房间列表无人", 500))
        break;
      //否则尝试下一个区
      plot[list].zoneCounter++;
      RandomJump(task, account);
    }
  }
}
//【全程识图】进入悬赏房间列表，并切换到竞技一区
void EnterBoutyRoomList(int task, int account, int level)
{
  //悬赏关卡
  const POINT bountyLevel[4] = { { 230, 430 }, { 405, 400 }, { 577, 400 }, { 750, 430 } };
  ClickActivity(task, account, "悬赏图标");//点击悬赏图标
  WaitPicture(APPEAR, task, account, "悬赏界面");//等待打开悬赏界面
  ClaimBountyReward(task, account, level);//尝试领取悬赏奖励
  Click(task, account, bountyLevel[level]);//选关进入房间列表
  WaitRoomList(task, account);
  RandomJump(task, account);
}
//每日签到
int DailySign(int task, int account)
{
  int list = GetList(task);
  const POINT closePanel = { 854, 55 };
  ClickActivity(task, account, "每日签到图标");//点击签到图标
  WaitPicture(APPEAR, task, account, "每日签到");//等待打开每日签到界面
  WaitColorDisappear(task, account, 215, 370, 0x174684, "签到界面加载失败");

  int x = 0, y = 0;
  DealInvitation(task, account, 0);//先处理邀请再检验
  if (FindPicture(task, account, "签到按钮", &x, &y))
  {
    Click(task, account, x + 5, y + 5);//点击领取按钮(+5是为了防止点边角无效）
    CheckSleep(task, 500);
    //如果无法领取签到奖励，记录签到失败
    if (!WaitPicture(DISAPPEAR, task, account, "签到按钮", nullptr, nullptr, x, y, WaitTime() / 2, false))
      plot[list].playingAdvance->result = 2;
  }
  Click(task, account, closePanel);//点击关闭签到面板
  WaitPicture(DISAPPEAR, task, account, "每日签到");//等待每日签到面板消失
  CheckSleep(task, 100);
  return 1;
}
//VIP签到
int VipSign(int task, int account)
{
  int list = GetList(task);
  const POINT closePanel = { 821, 55 };
  const POINT sign = { 738, 186 };
  const POINT wage = { 225, 280 };
  ClickActivity(task, account, "VIP签到图标");//点击签到图标
  WaitPicture(APPEAR, task, account, "VIP签到");//等待打开VIP签到界面
  int isSignRequired = TryWaitPicture(APPEAR, task, account, "VIP签到按钮", WaitTime() / 2);//先等VIP签到按钮亮
  int isWageRequired = TryWaitPicture(APPEAR, task, account, "工资领取按钮", 1000);//再等待工资领取按钮亮
  //点击VIP签到
  if (isSignRequired)
  {
    Click(task, account, sign);
    if (!TryWaitPicture(APPEAR, task, account, "VIP签到按钮_灰", WaitTime() / 2))
      plot[list].playingAdvance->result = 2;
    CheckSleep(task, 500);
  }
  //点击工资领取
  if (isWageRequired)
  {
    Click(task, account, wage);
    if (!TryWaitPicture(APPEAR, task, account, "工资领取按钮_灰", WaitTime() / 2))
      plot[list].playingAdvance->result = 2;
    CheckSleep(task, 500);
  }
  Click(task, account, closePanel);//点击关闭签到面板
  CheckSleep(task, 100);
  return 1;
}
//访问特定openid，返回领取结果（0=成功 1=已领 2=未绑定角色 3=领取失败 4=访问失败）
int RequestOpenid(int task, int idOrder)
{
  int list = GetList(task);

  char resultPath[maxPath] = {};
  sprintf_s(resultPath, "附加程序\\温馨礼包%d_%d.txt", task, idOrder);
  remove(resultPath);

  char link[300] = "http://meishi.wechat.123u.com/meishi/gift?openid=";
  strcat_s(link, plot[list].harmony[idOrder]);

  DeleteUrlCacheEntryA(link);
  URLDownloadToFileA(nullptr, link, resultPath, 0, 0);//访问openid并获取结果
  if (!FileExist(resultPath)) //未成功下载文件返回4
    return 4;
  FILE *f;
  int result = 0;
  if (fopen_s(&f, resultPath, "r")) //文件打不开返回4
    result = 4;
  else
  {
    char s[1000] = {};
    fgets(f, s);
    fclose(f);
    if (strstr(s, "你已成功领取温馨礼包，请到背包检查"))
      result = 0;
    else if (strstr(s, "你已经领过了，请明天再来"))
      result = 1;
    else if (strstr(s, "请先绑定游戏角色"))
      result = 2;
    else if (strstr(s, "领取失败，请联系客服"))
      result = 3;
  }
  remove(resultPath);
  return result;
}
//多次访问openid，直到返回确定的结果（0=成功 1=已领 2=未绑定角色 3=领取失败 4=访问失败）
int MutiRequestOpenid(int task, int idOrder)
{
  int result[10] = {};
  for (int i = 0; i < 10; i++) //最多访问10次
  {
    result[i] = RequestOpenid(task, idOrder);
    if (result[i] <= 2) //结果为0-2直接返回，否则重复领取
    {
      //本次反馈"今日已领取"，而上次反馈"领取失败"，判定为"背包已满，礼包被吞"
      if (i > 0 && result[i] == 1 && result[i - 1] == 3)
        return 5;
      return result[i];
    }
  }
  return result[9];
}
//逐条领取温馨礼包并记录日志
bool ClaimHarmony(int task)
{
  bool totalResult = true;//是否全部领取成功
  const char resultString[10][30] = { "领取成功", "今日已领取", "未绑定账号", "领取失败", "无法访问", "背包已满，礼包被吞" };
  int list = GetList(task);

  //逐条领取温馨礼包并记录日志
  plot[list].harmonyLogSize = 0;
  for (int order = 0; order < maxHarmonySize; order++)
  {
    if (strlen(plot[list].harmony[order]) == 0)
      break;
    int result = MutiRequestOpenid(task, order);
    if (result >= 2)
      totalResult = false;
    sprintf_s(plot[list].harmonyLog[order], "%d. openid=%s %s", order,
      plot[list].harmony[order], resultString[result]);
    plot[list].harmonyLogSize++;
  }
  return totalResult;
}
//签到（每日签到和VIP签到）
void Sign(int task, int account)
{
  DailySign(task, account);
  VipSign(task, account);
  ClaimDailyAward(task, account);
  ClaimContestAward(task, account, true, false);
}
//判定本页的4颗树是否可以选择
void BasicJudgeTree(int task, int account, char(&avaliable)[4])
{
  const int height = 28;
  const int textHeight[4] = { 0, 28, 56, 85 };//4行文字的高度坐标
  const COLORREF matureColor = 0xfcf9dc;//"成熟了"文字颜色
  const COLORREF dismissColor = 0x1ae641;//"已解散"文字颜色

  avaliable[0] = avaliable[1] = avaliable[2] = avaliable[3] = -1;//预设为-1
  if (!UpdateMap(task, account))//截图
    return;

  //如果有前往，没有"成熟了"，没有"已解散"，说明这是一棵可以选择的公会树（avaliable==1)
  for (int i = 0; i < 4; i++)
  {
    if (FindPictureInMap(task, "已解散", nullptr, nullptr, 0, textHeight[i], 0, 0, dismissColor))
      avaliable[i] = -1;
    else if (FindPictureInMap(task, "前往", nullptr, nullptr, 0, height * i)
      && !FindPictureInMap(task, "成熟了", nullptr, nullptr, 0, textHeight[i], 0, 0, matureColor))
      avaliable[i] = 1;
    else
      avaliable[i] = 0;
  }
}
const int gardenTitleX = 349, gardenTitleY = 46, gardenTitleWidth = 126, gardenTitleHeight = 20;
COLORREF gardenTitle[gardenTitleHeight][gardenTitleWidth];//公会标题
const int guildListX = 612, guildListY = 209, guildListWidth = 83, guildListHeight = 97;
COLORREF guildList[guildListHeight][guildListWidth];//公会列表
const COLORREF guildListColor = 0x1ae641;
//等待区域颜色变化，成功变化返回1，超时不变返回0
template <size_t width, size_t height>
int WaitAreaChange(int task, int account, COLORREF(&origin)[height][width],
  int x0, int y0, COLORREF specificColor = 0, int timeout = 0)
{
  if (timeout == 0)
    timeout = WaitTime();
  int counter = 0;
  while (true)
  {
    counter++;
    if (counter > timeout / 200)
      return 0;
    CheckSleep(task, 200);
    if (UpdateMap(task, account))
      if (!IsBitmapEqual(work[task].map, origin, width, height, x0, y0, 0, 0, specificColor))
      {
        CheckSleep(task, 100);
        return 1;
      }
  }
  return 0;
}
//判定本页的4颗树是否可以选择，如果是4个解散公会则翻到变化为止，如果无法变化则返回0
int JudgeTree(int task, int account, char(&avaliable)[4])
{
  const POINT pageDown = { 843, 292 };

  BasicJudgeTree(task, account, avaliable);//先判定一次
  //如果是4个解散公会，翻到变化为止
  if (avaliable[0] == -1 && avaliable[1] == -1 && avaliable[2] == -1 && avaliable[3] == -1)
  {
    int counter = 0;
    while (true)
    {
      counter++;
      if (counter > 15)//最多翻15页解散公会
        return 0;
      CopyMap(guildList, work[task].map, guildListX, guildListY);//记录原公会列表
      Click(task, account, pageDown);//翻页
      // 等待公会列表变化（解散公会翻页1次只等3秒）
      if (WaitAreaChange(task, account, guildList, guildListX, guildListY, guildListColor, WaitTime() / 2))
        break;
    }
    BasicJudgeTree(task, account, avaliable);//再判定一次
  }
  return 1;
}
//换一棵公会树，最多翻页30次
int ChooseAnotherTree(int task, int account, char(&avaliable)[4], int &treeCode)
{
  const POINT allGuild = { 799, 119 };
  const POINT enter = { 794, 203 };
  const POINT pageDown = { 843, 292 };
  const int height = 28;

  //首次换树：点击全部公会
  if (treeCode == -1)
  {
    CopyMap(guildList, work[task].map, guildListX, guildListY);//记录原公会列表
    Click(task, account, allGuild);//点击全部公会
    if (!WaitAreaChange(task, account, guildList, guildListX, guildListY, guildListColor))//等待公会列表变化
      ReportError(task, account, "公会列表打不开", "打开公会列表失败");
    if (!JudgeTree(task, account, avaliable))//判定本页公会树是否可用
      return 0;
  }

  //查找下一颗可以点击的树
  while (true)
  {
    if (treeCode == 3)//如果本页已经没有树可以换了，向上翻页
    {
      CopyMap(guildList, work[task].map, guildListX, guildListY);//记录原公会列表
      Click(task, account, pageDown);//翻页
      if (!WaitAreaChange(task, account, guildList, guildListX, guildListY, guildListColor))//等待公会列表变化
        return 0;//如果已经翻不动了，返回0
      if (!JudgeTree(task, account, avaliable))//重新判定
        return 0;
      treeCode = -1;
    }
    treeCode++;//树的编号+1，只能是0/1/2/3
    if (avaliable[treeCode] == 1) //如果找到了可选的树，退出循环
      break;
  }

  CheckSleep(task, 200);
  CopyMap(gardenTitle, work[task].map, gardenTitleX, gardenTitleY);//记录原公会标题
  Click(task, account, enter.x + 5, enter.y + 5 + height * treeCode);//点击前往
  if (!WaitAreaChange(task, account, gardenTitle, gardenTitleX, gardenTitleY))//等待公会标题变化
    ReportError(task, account, "前往公会树失败", "前往公会树失败");
  WaitColorDisappear(task, account, 258, 105, 0x8bf4fe);//等待白色消失
  UpdateMap(task, account);//记录当前图像，便于接下来检查成长值
  return 1;
}
//浇水/施肥操作times次
int Water(int task, int account, POINT operation, POINT confirm, int times, char(&avaliable)[4], int &treeCode)
{
  char growthString[10] = {};
  while (times > 0)
  {
    //如果treeCode==-1，即自家公会树，则除了顶部成长值还要验证底部成长值
    if (treeCode == -1)
      if (isFull(task) || isTotalFull(task))
        if (!ChooseAnotherTree(task, account, avaliable, treeCode))
          return 0;

    //如果成长值满，就换一棵树，直到成长值未满；如果翻页翻到头了，则返回0
    while (isFull(task))
      if (!ChooseAnotherTree(task, account, avaliable, treeCode))
        return 0;

    Click(task, account, operation);//浇水施肥按钮
    Click(task, account, confirm);//确定按钮
    CheckSleep(task, 500);//等待500ms，期间可能出现【公会任务完成】

    //重新检测到成长值视为浇水施肥成功
    int counter = 0;
    while (true)
    {
      counter++;
      if (counter > 100) //如果10秒无成长值
        ReportError(task, account, "浇水施肥失败", "浇水施肥失败");
      CheckSleep(task, 100);
      //如果成长值获取成功，成长值不为空字符串，视为浇水成功
      if (UpdateMap(task, account) && GetGrowth(task, growthString))
      {
        times--;
        break;
      }
    }
  }
  return 1;
}
//浇水施肥
int WaterAndManure(int task, int account)
{
  int list = GetList(task);
  const POINT guild = { 778, 557 };//公会图标
  const POINT guildActivity = { 747, 412 };//公会活动
  const POINT guildGarden = { 839, 331 };//公会花园
  const POINT closeGarden = { 853, 56 };//关闭公会花园
  const POINT closeGuild = { 904, 57 };//关闭公会

  //打开公会
  Click(task, account, guild);
  WaitPicture(APPEAR, task, account, "公会");
  CheckSleep(task, 500);

  //点击公会花园，直到公会界面消失
  int counter = 0;
  while (true)
  {
    Click(task, account, guildActivity);//点击公会活动
    Click(task, account, guildGarden);//点击公会花园
    if (TryWaitPicture(DISAPPEAR, task, account, "公会", 1000))
      break;
    counter++;
    if (counter > 5)
      ReportError(task, account, "无法进入花园", "无法进入公会花园");
  }

  //等待公会花园加载完成
  counter = 0;
  char growth[10];
  while (true)
  {
    counter++;
    if (counter > 10)
      ReportError(task, account, "花园加载失败", "公会花园加载失败");
    CheckSleep(task, 500);
    if (UpdateMap(task, account))
    {
      GetGrowth(task, growth);
      //第一条件：成长值不为"0/0"且标题为“我的公会花园”
      if (strcmp(growth, "0/0") != 0 && FindPictureInMap(task, "我的公会花园"))
      {
        //达成第一条件后，继续等待第二条件
        int counter1 = 0;
        while (true)
        {
          counter1++;
          if (counter1 > 10)
            ReportError(task, account, "公会树加载失败", "公会树加载失败");
          CheckSleep(task, 500);
          if (UpdateMap(task, account))
          {
            GetTotalGrowth(task, growth);
            //第二条件：总成长值不为"0/0"
            if (strcmp(growth, "0/0") != 0)
              break;
          }
        }
        break;
      }
    }
  }

  //开始浇水施肥
  const POINT water = { 782, 357 };//浇水按钮
  const POINT manure = { 782, 418 };//施肥按钮
  const POINT confirm = { 424, 348 };//确定按钮
  char avaliable[4] = {};
  int treeCode = -1;

  int isWaterFinished[2] = {};//浇水和施肥是否正常完成
  bool isPageFinished = false;//页数是否已经用尽
  if (plot[list].guildExist[0]) //浇水任务
  {
    isWaterFinished[0] = Water(task, account, water, confirm, 1, avaliable, treeCode);
    if (isWaterFinished[0] == 0)
      isPageFinished = true;
  }
  if (plot[list].guildExist[1] && !isPageFinished) //施肥任务
    isWaterFinished[1] = Water(task, account, manure, confirm, 3, avaliable, treeCode);

  Click(task, account, closeGarden);
  CheckSleep(task, 100);
  Click(task, account, closeGuild);
  CheckSleep(task, 100);

  OpenGuildMission(task, account);//进入公会任务面板
  plot[list].isGuildClaimFailed = false;
  ClaimGuildAward(task, account, false);//领奖

  if (plot[list].isGuildClaimFailed) //施肥如果领奖失败，再来一次
  {
    //防止横幅影响操作
    WaitPicture(APPEAR, task, account, "公会任务");//等待进入公会任务界面
    WaitPicture(DISAPPEAR, task, account, "完成次数");//等待“完成次数”字样消失
    Click(task, account, closeGuildMission);//关闭公会任务
    OpenGuildMission(task, account);//进入公会任务面板
    plot[list].isGuildClaimFailed = false;
    ClaimGuildAward(task, account, false);//领奖
  }

  //检查施肥是否完成（领奖失败视为已完成）
  memset(plot[list].guildExist, 0, sizeof(plot[list].guildExist));//所有公会任务预设为不存在
  ReadGuild(task, account, plot[list].guildExist);//识别公会任务存入数组
  //如果施肥任务未完成，记录任务失败并刷新
  if (plot[list].guildExist[1])
  {
    plot[list].playingAdvance->result = 2;//记录施肥未完成
    ReportError(task, account, "施肥任务未完成", "施肥任务未完成");
  }
  else //否则记录任务成功
    plot[list].playingAdvance->result = 0;
  Click(task, account, closeGuildMission);//关闭公会任务
  return 1;
}
//判断(x,y)位置的道具是否为特定物品
bool IsProp(int task, int x, int y, COLORREF(&propImage)[propHeight][propWidth])
{
  return IsBitmapEqual(work[task].map, propImage, lootCoreWidth, lootCoreHeight,
    x + lootCoreX, y + lootCoreY, lootCoreX, lootCoreY);
}
//判断(x,y)位置的道具是否为可删物品
bool IsTrash(int task, int x, int y)
{
  for (int code = 0; code < trashNum; code++)
    if (IsProp(task, x, y, trash[code].image))
      return true;
  return false;
}
//判断(x,y)位置的道具是否为双倍卡
template <int cardType>
bool IsDoubleCard(int task, int x, int y)
{
  return IsProp(task, x, y, doubleCard[cardType]);
}
//寻找本页第一个特定物品位置（IsProp为判定函数）。物品区左上角(x0,y0)，列数columnsNum
bool FindProp(bool IsProp(int, int, int), int task, int account, int x0, int y0, int columnsNum, int *pRow, int *pColumn)
{
  ForcedUpdateMap(task, account);//强制截图
  WaitForSingleObject(hMutexUserImage, INFINITE);
  bool result = false;
  //遍历每一个位置（道具背包和宝石分解背包都是63格）
  for (int order = 0; order < 63; order++)
  {
    int row = order / columnsNum;
    int column = order % columnsNum;
    int x = x0 + column * lootWidth;
    int y = y0 + row * lootHeight;
    if (IsProp(task, x, y))
    {
      *pRow = row;
      *pColumn = column;
      result = true;
      break;
    }
  }
  ReleaseMutex(hMutexUserImage);
  return result;
}
//寻找本页第一个双倍卡
bool FindDoubleCard(int task, int account, int cardType, int *pRow, int *pColumn)
{
  if (cardType == 0)
    return FindProp(IsDoubleCard<0>, task, account, propX, reserve[task].propY, 9, pRow, pColumn);
  else
    return FindProp(IsDoubleCard<1>, task, account, propX, reserve[task].propY, 9, pRow, pColumn);
}
//使用本页第一个双倍卡
bool UseDoubleCardInPage(int task, int account, int cardType)
{
  //1. 确定道具位置（翻页时已经截图）
  LocatePropY(task, account);

  //2. 等待本页道具加载完成
  int counter = 0;
  while (!isPropLoaded(task, account))
  {
    counter++;
    if (counter > 30)
      ReportError(task, account, "道具加载失败", "道具加载失败");
    CheckSleep(task, 1000);
  }

  //3. 查找本页第一个双倍卡，查找成功则使用
  int row = 0, column = 0;
  const char *useProp[] = { "使用物品", "使用物品_亮" };
  if (FindDoubleCard(task, account, cardType, &row, &column))
  {
    Click(task, account, propX + (2 * column + 1) * propWidth / 2 - 20,
      reserve[task].propY + (2 * row + 1) * propHeight / 2);//点击双倍卡左侧位置
    int useX = 0, useY = 0;
    WaitPictures(APPEAR, task, account, useProp, &useX, &useY);//等待"使用物品"菜单出现
    CheckSleep(task, 500);
    Click(task, account, useX + 5, useY + 5); //点击使用
    WaitPictures(DISAPPEAR, task, account, useProp, &useX, &useY);//等待"使用物品"菜单消失
    CheckSleep(task, 500);
    return true;
  }
  return false;
}
//使用双倍卡
void UseDoubleCard(int task, int account, int cardType, int cardNum)
{
  const POINT bag = { 592, 559 };//背包
  const POINT arrange = { 902, 474 };//整理
  const POINT closeBag = { 919, 58 };//关闭背包

  //1. 打开装备背包
  Click(task, account, bag);//打开背包
  WaitPicture(APPEAR, task, account, "装备背包");
  CheckSleep(task, 500);
  Click(task, account, arrange);//点击整理，默认2秒整理好
  CheckSleep(task, 2000);

  //2. 翻页查找双倍卡
  const int propScrollX = 917, propScrollY = 107, propScrollHeight = 305;
  COLORREF propScroll[propScrollHeight][1] = {};//背包滚动条
  ForcedUpdateMap(task, account);
  int usedNum = 0;
  for (int page = 0; page <= 20; page++)
  {
    //翻页并截图
    if (!PageScroll(task, account, page, propScrollX, propScrollY, propScroll))
      break;
    //在本页范围内循环使用双爆卡
    while (usedNum < cardNum)
    {
      //如果本页没有双爆卡可用，则退出while循环
      if (!UseDoubleCardInPage(task, account, cardType))
        break;
      usedNum++;
    }
    //如果已使用目标数量，退出循环
    if (usedNum >= cardNum)
      break;
  }

  //3. 关闭背包
  Click(task, account, closeBag);
  CheckSleep(task, 100);
}
//解除二级密码。若需要解除则返回true
bool ReleasePassword(int task, int account)
{
  int list = GetList(task);
  const POINT confirmPassword = { 438, 387 };//确认二级密码
  const POINT closeMall = { 919, 27 };//关闭商城

  reserve[task].passwordReleased = true;
  if (TryWaitPicture(APPEAR, task, account, "二级密码确认", 1000))
  {
    CheckSleep(task, 500);
    StringToWindow(plot[list].password[account], work[task].hWnd[account]);//输入二级密码
    CheckSleep(task, 1000);
    Click(task, account, confirmPassword);//确认二级密码
    WaitPicture(DISAPPEAR, task, account, "二级密码确认");//等待二级密码确认消失
    CheckSleep(task, 500);
    return true;
  }
  return false;
}
void BuyTower(int task, int account, int times)
{
  POINT buy = { 643, 518 };//购买
  POINT tickGiftTicket = { 446, 335 };//勾选礼券
  POINT confirm = { 427, 379 };//确认购买
  POINT closeTower = { 927, 34 };//关闭魔塔界面

  EnterTower(task, account, 1);//进入单人魔塔
  for (int i = 0; i < times; i++)
  {
    Click(task, account, buy);
    WaitPicture(APPEAR, task, account, "购买魔塔次数");
    Click(task, account, tickGiftTicket);
    WaitPicture(APPEAR, task, account, "购买魔塔次数_礼券");
    Click(task, account, confirm);
    //若未解除过二级密码则尝试解除，成功解除则本次购买无效
    if (!reserve[task].passwordReleased)
      if (ReleasePassword(task, account))
        i--;
    WaitPicture(DISAPPEAR, task, account, "购买魔塔次数");
  }
  Click(task, account, closeTower);
  WaitPicture(DISAPPEAR, task, account, "魔塔界面");//等待关闭魔塔选层界面
}
//寻找第一个可删道具
bool FindTrashProp(int task, int account, int *pRow, int *pColumn)
{
  return FindProp(IsTrash, task, account, propX, reserve[task].propY, 9, pRow, pColumn);
}
//寻找第一个可删宝石
bool FindTrashJewel(int task, int account, int *pRow, int *pColumn)
{
  return FindProp(IsTrash, task, account, jewelX, jewelY, 7, pRow, pColumn);
}
//检查二级密码是否存在
//void CheckSecondPassword(int task, int account)
//{
//  const POINT set = { 210, 84 };//打开游戏设置
//  const POINT closeSet = { 609, 140 };//关闭游戏设置
//  const char *passwordLoaded[] = { "设置密码", "设置密码_亮", "删除密码", "删除密码_亮" };
//  const char *deletePassword[] = { "删除密码", "删除密码_亮" };
//
//  Click(task, account, set);//打开游戏设置
//  WaitPicture(APPEAR, task, account, "游戏设置");
//  WaitPictures(APPEAR, task, account, passwordLoaded);//等待二级密码面板加载
//  //如果存在"删除密码"字样，说明有二级密码
//  reserve[task].passwordExist = TryWaitPictures(APPEAR, task, account, deletePassword, 1000) != 0;
//  reserve[task].passwordReleased = false;
//  Click(task, account, closeSet);//关闭游戏设置
//  CheckSleep(task, 500);
//}
//删除本页无用道具
void DeletePropInPage(int task, int account)
{
  //1. 确定道具位置（翻页时已经截图）
  LocatePropY(task, account);

  //2. 等待本页道具加载完成
  int counter = 0;
  while (!isPropLoaded(task, account))
  {
    counter++;
    if (counter > 30)
      ReportError(task, account, "道具加载失败", "道具加载失败");
    CheckSleep(task, 1000);
  }

  //3. 删除本页道具
  const POINT deleteButton = { 845, 474 };//删除按钮
  const POINT confirmDelete = { 425, 350 };//确认删除

  int row = 0, column = 0;//可删物品位置
  bool isDeletePicked = false;
  //查找第一个可删物品，直到没有可删物品为止
  while (FindTrashProp(task, account, &row, &column))
  {
    if (!isDeletePicked)
    {
      Click(task, account, deleteButton);//拿起删除按钮
      CheckSleep(task, 500);
      isDeletePicked = true;
    }
    Click(task, account, propX + (2 * column + 1) * propWidth / 2,
      reserve[task].propY + (2 * row + 1) * propHeight / 2);//点击可删物品
    WaitPicture(APPEAR, task, account, "删除物品提示");//等待删除提示出现
    Click(task, account, confirmDelete);//确认删除
    WaitPicture(DISAPPEAR, task, account, "删除物品提示");//等待删除提示消失
    //若二级密码存在且未解除，解除二级密码
    if (!reserve[task].passwordReleased)
      ReleasePassword(task, account);
    CheckSleep(task, 500);//等待500ms
  }
  if (isDeletePicked)
  {
    Click(task, account, deleteButton);//放下删除按钮
    CheckSleep(task, 500);
  }
}
//删除无用道具
void DeleteProp(int task, int account)
{
  const POINT bag = { 592, 559 };//背包
  const POINT propBag = { 781, 68 };//道具背包
  const POINT arrange = { 902, 474 };//整理
  const POINT closeBag = { 919, 58 };//关闭背包

  //1. 打开背包，选择道具背包
  Click(task, account, bag);//打开背包
  WaitPicture(APPEAR, task, account, "装备背包");
  CheckSleep(task, 500);
  Click(task, account, propBag);//打开道具背包
  WaitPicture(APPEAR, task, account, "道具背包");
  CheckSleep(task, 500);
  Click(task, account, arrange);//点击整理，默认2秒整理好
  CheckSleep(task, 2000);

  //2. 翻页并删除物品
  const int propScrollX = 917, propScrollY = 107, propScrollHeight = 305;
  COLORREF propScroll[propScrollHeight][1] = {};//背包滚动条
  ForcedUpdateMap(task, account);
  for (int page = 0; page <= 20; page++)
  {
    //翻页并截图
    if (!PageScroll(task, account, page, propScrollX, propScrollY, propScroll))
      break;
    DeletePropInPage(task, account);
  }

  Click(task, account, arrange);//删完再点一次整理
  CheckSleep(task, 2000);
  Click(task, account, closeBag);//关闭背包
  CheckSleep(task, 100);
}
//分解宝石
void DecomposeJewel(int task, int account)
{
  const POINT workshop = { 686, 561 };//合成屋
  const POINT jewelDecomposition = { 458, 435 };//宝石分解
  const POINT decompose = { 286, 377 };//分解按钮
  const POINT closeWorkshop = { 910, 42 };//关闭合成屋
  //执行这一步时，二级密码已经解锁
  Click(task, account, workshop);//打开合成屋
  WaitPicture(APPEAR, task, account, "合成屋");
  Click(task, account, jewelDecomposition);//打开宝石分解
  WaitPicture(APPEAR, task, account, "宝石分解");

  //宝石：9行7列
  int row = 0, column = 0;//可分解宝石位置
  //查找第一个可分解宝石，直到没有可分解宝石为止
  while (FindTrashJewel(task, account, &row, &column))
  {
    Click(task, account, jewelX + (2 * column + 1) * jewelWidth / 2,
      jewelY + (2 * row + 1) * jewelHeight / 2);//点击可删宝石
    WaitPicture(DISAPPEAR, task, account, "宝石分解槽");//等待宝石放入分解槽
    Click(task, account, decompose);//点击分解
    //若二级密码存在且未解除，解除二级密码并再次点击分解
    if (!reserve[task].passwordReleased)
    {
      ReleasePassword(task, account);
      Click(task, account, decompose);//点击分解
    }
    WaitPicture(APPEAR, task, account, "宝石分解槽");//等待宝石消失
    CheckSleep(task, 500);//等待500ms
  }
  Click(task, account, closeWorkshop);//关闭背包
  CheckSleep(task, 100);
}
//在防御卡背包中检查本页防御卡
void CheckSlotInPage(int task, int account)
{
  int list = GetList(task);

  //1. 确定防御卡位置（翻页时已经截图）
  LocateSlotY(task, account);

  //2. 等待本页防御卡加载完成
  int counter = 0;
  while (!isSlotLoaded(task, account))
  {
    counter++;
    if (counter > 30)
      ReportError(task, account, "防御卡加载失败", "防御卡加载失败");
    CheckSleep(task, 1000);
  }

  //3. 检查所有自定卡槽在本页的最佳星级、优先级
  for (int order = 0; order < plot[list].usedSlotNum[account]; order++)
  {
    Candidate &candidate = plot[list].candidate[account][order];
    //搜索这个卡槽的所有候选卡（优先级从高到低）
    for (int i = 0; i < candidate.num; i++)
    {
      //如果此候选卡的优先级小于已找到的最高优先级，则无需查找
      if (candidate.priority[i] < plot[list].bestPriority[account][order])
        break;
      //查找此候选卡在本页的最高星级
      int row = 0, column = 0;//最高星级候选卡位置
      int star = FindSlotInPage(task, candidate.code[i], &row, &column);
      if (star >= 0) //如果查找成功
        //如果此卡优先级更高，或优先级相同但星级更高，更新这张卡已找到的最高优先级和最高星级
        if (candidate.priority[i] > plot[list].bestPriority[account][order]
          || star > plot[list].bestStar[account][order])
        {
          plot[list].bestPriority[account][order] = candidate.priority[i];
          plot[list].bestStar[account][order] = star;
          plot[list].bestCode[account][order] = candidate.code[i];
          //拷贝左上角星级图像
          int x = propX + column * customWidth + starX;
          int y = reserve[task].slotY + row * customHeight + starY;
          CopyMap(plot[list].bestStarImage[account][order], work[task].map, x, y);
        }
    }
  }
}
//检查背包中各种卡槽是否存在
void CheckSlotInBag(int task, int account)
{
  int list = GetList(task);

  //初始化所需卡片的最佳星级、优先级、编号
  for (int i = 0; i < plot[list].usedSlotNum[account]; i++)
  {
    plot[list].bestStar[account][i] = -1;
    plot[list].bestPriority[account][i] = -99;
  }

  const POINT bag = { 592, 559 };//背包
  const POINT customBag = { 652, 68 };//防御卡背包
  const POINT closeBag = { 919, 58 };//关闭背包

  //1. 打开防御卡背包
  Click(task, account, bag);//打开背包
  WaitPicture(APPEAR, task, account, "装备背包");
  CheckSleep(task, 500);
  Click(task, account, customBag);//打开道具背包
  WaitPicture(APPEAR, task, account, "防御卡背包");
  CheckSleep(task, 500);

  //2. 翻页并记录防御卡
  const int propScrollX = 917, propScrollY = 107, propScrollHeight = 305;
  COLORREF propScroll[propScrollHeight][1] = {};//背包滚动条
  ForcedUpdateMap(task, account);
  for (int page = 0; page <= 20; page++)
  {
    //翻页并截图
    if (!PageScroll(task, account, page, propScrollX, propScrollY, propScroll))
      break;
    CheckSlotInPage(task, account);//检查本页卡槽
  }

  CheckSleep(task, 500);
  //Click(task, account, closeBag);//关闭背包
  //CheckSleep(task, 100);
}
//在本页中查找门票code，查找成功则返回数量，失败则返回-1
int FindTicketInPage(int task, int code)
{
  for (int row = 0; row < 7; row++)
    for (int column = 0; column < 9; column++)
    {
      //查找row行column列门票是否匹配，匹配则记录数量
      int count = GetEqualTicketCount(task, code, row, column);
      if (count > 0)
        return count;
    }
  return -1;
}
//检查本页道具数量
void CheckTicketInPage(int task, int account)
{
  int list = GetList(task);

  //1. 确定道具位置（翻页时已经截图）
  LocatePropY(task, account);

  //2. 等待本页道具加载完成
  int counter = 0;
  while (!isPropLoaded(task, account))
  {
    counter++;
    if (counter > 30)
      ReportError(task, account, "道具加载失败", "道具加载失败");
    CheckSleep(task, 1000);
  }

  //3. 检查各种门票在本页中的数量
  for (int code = 0; code < ticketNum; code++)
    //只检查需要使用且还未发现的门票
    if (plot[list].usedTicketCount[account][code] > 0 && plot[list].ticketCount[account][code] == 0)
    {
      //在本页中查找这种门票
      int count = FindTicketInPage(task, code);
      //如果门票存在，记录门票数量
      if (count > 0)
        plot[list].ticketCount[account][code] = count;
    }
}
//检查背包中各种门票的数量
void CheckTicketInBag(int task, int account)
{
  int list = GetList(task);
  //初始化门票数量为0
  memset(plot[list].ticketCount[account], 0, sizeof(plot[list].ticketCount[account]));

  const POINT bag = { 592, 559 };//背包
  const POINT propBag = { 781, 68 };//道具背包
  const POINT arrange = { 902, 474 };//整理
  const POINT closeBag = { 919, 58 };//关闭背包

  //1. 打开背包，选择道具背包
  //Click(task, account, bag);//打开背包
  //WaitPicture(APPEAR, task, account, "装备背包");
  //CheckSleep(task, 500);
  Click(task, account, propBag);//打开道具背包
  WaitPicture(APPEAR, task, account, "道具背包");
  CheckSleep(task, 500);
  Click(task, account, arrange);//点击整理，默认2秒整理好
  CheckSleep(task, 2000);

  //2. 翻页并删除物品
  const int propScrollX = 917, propScrollY = 107, propScrollHeight = 305;
  COLORREF propScroll[propScrollHeight][1] = {};//背包滚动条
  ForcedUpdateMap(task, account);
  for (int page = 0; page <= 20; page++)
  {
    //翻页并截图
    if (!PageScroll(task, account, page, propScrollX, propScrollY, propScroll))
      break;
    CheckTicketInPage(task, account);//检查本页门票数量
  }

  CheckSleep(task, 500);
  Click(task, account, closeBag);//关闭背包
  CheckSleep(task, 100);
}

const int panelTitleWidth = 50, panelTitleHeight = 30;//统计面板标题行宽高
const int areaRow = 4, areaColumn = 9;//每个绘图区的行列数
const int areaBlank = 10;//绘图区边缘留空
const int areaTextHeight = 20;//绘图区文字高度
const int areaWidth = areaColumn * propWidth + 2 * areaBlank;
const int ticketAreaHeight = areaRow * (propHeight + areaTextHeight) + 2 * areaBlank;
const int slotAreaHeight = areaRow * (customHeight + areaTextHeight) + 2 * areaBlank;
const int panelWidth = panelTitleWidth + 2 * areaWidth;
const int panelHeight = panelTitleHeight + ticketAreaHeight + slotAreaHeight;
int usedPanelHeight;//已使用的面板高度
int usedPanelHeightDPI;//已使用的面板高度（缩放后的值）
typedef COLORREF Count[panelHeight][panelWidth];//统计面板类型

//原始大小的统计面板
HDC hDCPanel;//统计面板HDC
HBITMAP hBmpPanel;//统计面板HBITMAP
Count &panel = *(Count *)MallocColor(panelWidth, panelHeight, &hDCPanel, &hBmpPanel);//统计面板

//放大的统计面板（确定DPI后才能初始化）
IMAGE *pPanelDPI;//放大统计面板的IMAGE指针
HDC hDCPanelDPI;//放大统计面板的HDC
int panelWidthDPI, panelHeightDPI;//放大统计面板的尺寸

//窗口截图缩放核心类
class WindowScaler
{
private:
  Gdiplus::Graphics graphics;//绑定绘图窗口，用于绘制缩放后的图像
public:
  //初始化graphics设置
  explicit WindowScaler(HDC hDCImage) : graphics(hDCImage)
  {
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
    graphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);
  }
  //绘制缩放后的图像
  void PaintZoomedImage()
  {
    Gdiplus::Bitmap srcBitmap(hBmpPanel, nullptr);//绑定原始统计面板
    graphics.DrawImage(&srcBitmap, Gdiplus::Rect(0, 0, panelWidthDPI, panelHeightDPI),
      0, 0, panelWidth, panelHeight, Gdiplus::UnitPixel);//缩放后绘制到放大统计面板
  }
};
WindowScaler *pScaler;

//获取账号account使用的门票种数
int GetUsedTicketNum(int list, int account)
{
  int usedTicketNum = 0;
  for (int code = 0; code < maxTicketNum; code++)
    if (plot[list].usedTicketCount[account][code] > 0)
      usedTicketNum++;
  return usedTicketNum > maxUsedTicketNum ? maxUsedTicketNum : usedTicketNum;
}
//计算门票区末尾Y坐标
int GetTicketEndY(int list)
{
  int ticketTypeCount0 = GetUsedTicketNum(list, 0);
  int ticketTypeCount1 = GetUsedTicketNum(list, 1);
  int ticketTypeCount = max(ticketTypeCount0, ticketTypeCount1);
  //门票占用行数（最少为1）
  int ticketRow = ticketTypeCount == 0 ? 1 : (ticketTypeCount + areaColumn - 1) / areaColumn;
  return panelTitleHeight + 2 * areaBlank + ticketRow * (propHeight + areaTextHeight);
}
//计算卡槽区末尾Y坐标
int GetSlotEndY(int list, int ticketEndY)
{
  int slotCount = max(plot[list].usedSlotNum[0], plot[list].usedSlotNum[1]);
  //卡槽占用行数（最少为1）
  int slotRow = slotCount == 0 ? 1 : (slotCount + areaColumn - 1) / areaColumn;
  return ticketEndY + 2 * areaBlank + slotRow * (customHeight + areaTextHeight);
}
//将彩色color转化为灰色
COLORREF Gray(COLORREF color)
{
  byte r = bgrRValue(color);
  byte g = bgrGValue(color);
  byte b = bgrBValue(color);
  byte gray = (byte)(0.299 * r + 0.587 * g + 0.114 * b);
  return bgr(gray, gray, gray);
}
//将彩色图像转化为灰色图像
template <int height, int width>
void GrayImage(COLORREF(&gray)[height][width], COLORREF(&old)[height][width])
{
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++)
      gray[y][x] = Gray(old[y][x]);
}
//将卡组绘制到原始统计面板。bagChecked：是否已经检查过背包
void PaintSlot(int list, int account, int ticketEndY, bool bagChecked)
{
  //绘图区左上角位置
  int areaX = panelTitleWidth + areaBlank + account * areaWidth;
  int areaY = ticketEndY + areaBlank;
  for (int i = 0; i < plot[list].usedSlotNum[account]; i++)
  {
    //当前卡槽行列
    int row = i / areaColumn, column = i % areaColumn;
    //当前卡槽绘制位置
    int paintX = areaX + column * customWidth;
    int paintY = areaY + row * (customHeight + areaTextHeight);

    //自定卡图像（有卡显示最高优先级截图，无卡显示“缺图”）
    COLORREF(*pImage)[customHeight][customWidth] = &missingCustom;

    //该卡槽存在至少1张自定卡截图时，显示最高优先级
    if (plot[list].candidate[account][i].num > 0)
    {
      int code = plot[list].candidate[account][i].code[0];
      pImage = &custom[code].image;
    }
    //用于修改自定卡图像的临时数组
    COLORREF image[customHeight][customWidth];

    //如果已经检查过背包
    if (bagChecked)
    {
      //如果背包中没有找到该卡，显示灰色的自定卡图像
      if (plot[list].bestPriority[account][i] == -99)
        GrayImage(image, *pImage);
      else //找到了则显示最高优先级最高星级图像
      {
        int code = plot[list].bestCode[account][i];
        CopyMap(image, custom[code].image);//拷贝最高优先级自定卡图像
        //拷贝最高星级图像
        CopyMap(image, starX, starY, starWidth, starHeight, plot[list].bestStarImage[account][i]);
      }
      pImage = &image;
    }
    CopyMap(panel, paintX, paintY, customWidth, customHeight, *pImage);
  }
}
//将卡槽名称显示到放大统计面板
void ViewSlotName(int list, int account, int ticketEndY)
{
  //绘图区左上角位置
  int areaX = panelTitleWidth + areaBlank + account * areaWidth;
  int areaY = ticketEndY + areaBlank;
  for (int i = 0; i < plot[list].usedSlotNum[account]; i++)
  {
    //当前卡槽行列
    int row = i / areaColumn, column = i % areaColumn;
    //当前卡槽绘制位置
    int paintX = areaX + column * customWidth;
    int paintY = areaY + row * (customHeight + areaTextHeight);
    //添加文字
    CenterView(plot[list].usedSlot[account][i], paintX + customWidth / 2,
      paintY + customHeight + areaTextHeight / 2);
  }
}
//在物品图像高度propDigitY处用颜色color显示数值quantity
void WriteValueInProp(COLORREF(&image)[propHeight][propWidth], int value, int propDigitY,
  COLORREF color = 0xffffff)
{
  //把战利品数量保存到总战利品截图中
  int digits[9] = {};//各位数字
  int digitsNum = GetDigits(value, digits);//位数
  int quantityWidth = digitsNum * lootDigitDistance + 1;//数量区宽度
  const int quantityHeight = lootDigitHeight + 2;
  int quantityTop = propDigitY - 1;//数量区上方Y
  int quantityRight = lootDigitX + lootDigitWidth;//数量区右侧X
  //把数量区涂黑
  for (int y = quantityTop; y < quantityTop + quantityHeight; y++)
    for (int x = quantityRight - quantityWidth + 1; x <= quantityRight; x++)
      image[y][x] = 0;
  //填写各位数字
  for (int i = 0; i < digitsNum; i++)
  {
    int digitX = quantityRight - (i + 1) * lootDigitDistance + 1;
    int digitY = quantityTop + 1;
    CopyMap(image, digitX, digitY, lootDigitWidth, lootDigitHeight, lootDigit[digits[i]], 0, 0);
  }
  if (color != 0xffffff)
    for (int y = quantityTop; y < quantityTop + quantityHeight; y++)
      for (int x = quantityRight - quantityWidth + 1; x <= quantityRight; x++)
        if (image[y][x] == 0xffffff)
          image[y][x] = color;
}
//在物品图像右下角显示数量
void WritePropCount(COLORREF(&image)[propHeight][propWidth], int count)
{
  WriteValueInProp(image, count, lootDigitY);
}
//将门票绘制到原始统计面板。bagChecked：是否已经检查过背包
void PaintTicket(int list, int account, bool bagChecked)
{
  //绘图区左上角位置
  int areaX = panelTitleWidth + areaBlank + account * areaWidth;
  int areaY = panelTitleHeight + areaBlank;
  int ticketTypeCount = 0;
  for (int code = 0; code < maxTicketNum; code++)
  {
    if (plot[list].usedTicketCount[account][code] > 0)
    {
      //当前门票行列
      int row = ticketTypeCount / areaColumn, column = ticketTypeCount % areaColumn;
      //当前门票绘制位置
      int ticketX = areaX + column * propWidth;
      int ticketY = areaY + row * (propHeight + areaTextHeight);
      //门票图像
      COLORREF image[propHeight][propWidth];
      //如果已经检查过背包
      if (bagChecked)
      {
        //门票盈余数量
        int surplus = plot[list].ticketCount[account][code] - plot[list].usedTicketCount[account][code];
        const int surplusY = 6;
        //如果门票数量不足，对门票进行灰色处理，显示不足数量
        if (surplus < 0)
        {
          GrayImage(image, ticket[code].image);
          WriteValueInProp(image, -surplus, surplusY, RGB(64, 64, 255));
        }
        else //如果数量充足，拷贝门票图像，显示盈余数量
        {
          CopyMap(image, ticket[code].image);
          WriteValueInProp(image, surplus, surplusY, RGB(0, 255, 0));
        }
      }
      else //未检查背包时，直接拷贝门票图像
        CopyMap(image, ticket[code].image);

      WritePropCount(image, plot[list].usedTicketCount[account][code]);//标记需求数量
      CopyMap(panel, ticketX, ticketY, propWidth, propHeight, image);//绘制到面板
      ticketTypeCount++;
      if (ticketTypeCount >= maxUsedTicketNum)
        break;
    }
  }
}
//将门票名称显示到放大统计面板
void ViewTicketName(int list, int account)
{
  //const char ticketStyleName[3][2] = { "", "A", "B" };//钥匙、徽章后缀
  const char ticketStyleName[3][2] = { "", "", "" };//钥匙、徽章后缀
  //绘图区左上角位置
  int areaX = panelTitleWidth + areaBlank + account * areaWidth;
  int areaY = panelTitleHeight + areaBlank;
  int ticketTypeCount = 0;
  for (int code = 0; code < maxTicketNum; code++)
  {
    if (plot[list].usedTicketCount[account][code] > 0)
    {
      //当前门票行列
      int row = ticketTypeCount / areaColumn, column = ticketTypeCount % areaColumn;
      //当前门票绘制位置
      int ticketX = areaX + column * propWidth;
      int ticketY = areaY + row * (propHeight + areaTextHeight);
      //添加文字
      char ticketName[10] = {};
      sprintf_s(ticketName, "%s%s", ticket[code].name, ticketStyleName[ticket[code].style]);
      CenterView(ticketName, ticketX + propWidth / 2, ticketY + propHeight + areaTextHeight / 2);
      ticketTypeCount++;
      if (ticketTypeCount >= maxUsedTicketNum)
        break;
    }
  }
}
//设置统计面板字体
void SetPanelFont()
{
  setbkmode(TRANSPARENT);    //透明字体
  LOGFONT font;
  gettextstyle(&font);       //获取当前字体设置
  font.lfHeight = 16;        //设置字体高度
  font.lfWeight = 1000;      //设置字体粗细
  strcpy_s(font.lfFaceName, "等线");   //设置字体为“等线”
  font.lfQuality = ANTIALIASED_QUALITY;//设置输出效果为抗锯齿
  settextstyle(&font);       //设置字体样式
}
const int panelTipHeight = 30;//统计面板提示文字高度
//统计结果显示对话框
INT_PTR CALLBACK CountDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    int list = (int)lParam;
    const int panelBlank = 10;//黑色面板四周留白10像素
    RECT rectDlg;
    GetWindowRect(hDlg, &rectDlg);
    RECT client;
    GetClientRect(hDlg, &client);
    POINT leftTop = {}, rightBottom = { client.right, client.bottom };
    ClientToScreen(hDlg, &leftTop);
    ClientToScreen(hDlg, &rightBottom);
    //客户区在窗口中的位置
    int clientX = leftTop.x - rectDlg.left;
    int clientY = leftTop.y - rectDlg.top;
    int clientBottomCut = rectDlg.bottom - rightBottom.y;
    //填写提示文本
    HWND hWndText = GetDlgItem(hDlg, IDC_TEXT);
    if (plot[list].isDeepCheckingSuccessful)
      SetWindowTextA(hWndText, "已显示实际拥有的门票和卡槽。灰色表示缺少，绿色数字为盈余数量，"
        "红色数字为缺少数量。");
    else
      SetWindowTextA(hWndText, "所需门票、卡槽如图（星级转职不代表配置要求）。"
        "单击【检查背包】可检查是否有缺少。");
    //确定按钮文本和尺寸
    HWND hWndOK = GetDlgItem(hDlg, IDOK);
    if (plot[list].isDeepCheckingSuccessful)
      SetWindowTextA(hWndOK, "确定");
    else
      SetWindowTextA(hWndOK, "检查背包");
    RECT rectOK;
    GetWindowRect(hWndOK, &rectOK);
    int OKWidth = rectOK.right - rectOK.left;
    int OKHeight = rectOK.bottom - rectOK.top;
    //调整对话框尺寸并居中
    const int dlgWidth = panelWidthDPI + 2 * clientX + 2 * panelBlank;
    const int dlgHeight = usedPanelHeightDPI + clientY + clientBottomCut + 3 * panelBlank
      + OKHeight + tipHeight;
    RECT rectDesktop;
    GetWindowRect(GetDesktopWindow(), &rectDesktop);
    int xPos = (rectDesktop.left + rectDesktop.right - dlgWidth) / 2;
    int yPos = (rectDesktop.top + rectDesktop.bottom - dlgHeight) / 2;
    SetWindowPos(hDlg, HWND_TOP, xPos, yPos, dlgWidth, dlgHeight, 0);

    //调整确定按钮位置
    GetClientRect(hDlg, &client);
    SetWindowPos(hWndOK, nullptr, (client.right - OKWidth) / 2,
      client.bottom - OKHeight - panelBlank, 0, 0, SWP_NOSIZE);

    //设置对话框标题
    if (plot[list].isDeepCheckingSuccessful)
      SetWindowText(hDlg, "背包检查结果");
    else
      SetWindowText(hDlg, "任务检查结果");
  }
  return TRUE;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    BeginPaint(hDlg, &ps);
    int usedPanelHeightDPI = usedPanelHeight * DPI / 96;
    RECT rect;
    GetClientRect(hDlg, &rect);
    POINT point = { rect.left, rect.top };
    ClientToScreen(hDlg, &point);
    GetWindowRect(hDlg, &rect);
    BitBlt(GetDC(hDlg), 10, 10 + panelTipHeight, panelWidthDPI, usedPanelHeightDPI,
      hDCPanelDPI, 0, 0, SRCCOPY);
    EndPaint(hDlg, &ps);
    break;
  }
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK) // 点击了“确定”按钮
    {
      EndDialog(hDlg, IDOK);// 结束对话框
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL) // 点击了“取消”按钮
    {
      EndDialog(hDlg, IDCANCEL);
      return TRUE;
    }
    break;
  }
  return FALSE;
}
//弹出统计结果对话框
int PopCountDialog(int list)
{
  return DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_COUNT),
    hWndActuator, CountDialogProc, (LPARAM)list);
}
//弹窗显示深度检查结果
void PopDeepResult(int list)
{
  int task = GetTask(list);

  memset(panel, 0, sizeof(panel));
  int ticketEndY = GetTicketEndY(list);
  usedPanelHeight = GetSlotEndY(list, ticketEndY);
  usedPanelHeightDPI = usedPanelHeight * DPI / 96;
  //3.1 在panel中绘制门票和卡组
  for (int account = 0; account < 2; account++)
  {
    PaintTicket(list, account, true);
    PaintSlot(list, account, ticketEndY, true);
  }
  //3.2 将图片放大并绘制到pPanelDPI
  pScaler->PaintZoomedImage();
  //3.3 在pPanelDPI中标注文字和绘制框线
  SetWorkingImage(pPanelDPI);
  setaspectratio((float)DPI / 96, (float)DPI / 96);
  SetPanelFont();//设置标注字体
  //3.3.1 标注门票和卡槽名称
  for (int account = 0; account < 2; account++)
  {
    ViewTicketName(list, account);
    ViewSlotName(list, account, ticketEndY);
  }
  //3.3.2 标注标题
  CenterView("1P", panelTitleWidth + areaWidth / 2, panelTitleHeight / 2);
  CenterView("2P", panelTitleWidth + 3 * areaWidth / 2, panelTitleHeight / 2);
  CenterView("门票", panelTitleWidth / 2, (panelTitleHeight + ticketEndY) / 2);
  CenterView("卡槽", panelTitleWidth / 2, (ticketEndY + usedPanelHeight) / 2);
  //3.3.3绘制框线
  //2条竖线
  for (int account = 0; account < 2; account++)
    line(panelTitleWidth + account * areaWidth, 0,
      panelTitleWidth + account * areaWidth, usedPanelHeight);
  //2条横线
  line(0, panelTitleHeight, panelWidth, panelTitleHeight);
  line(0, ticketEndY, panelWidth, ticketEndY);

  setaspectratio(1, 1);
  SetWorkingImage(nullptr);

  //4 弹窗展示
  PopCountDialog(list);
}
//发布会长任务
void AssignGuildMission(int task, int account)
{
  //1. 打开公会任务面板
  OpenGuildMission(task, account);
  //2. 如果2秒内发布按钮没有变灰，发布公会任务
  if (!TryWaitPicture(APPEAR, task, account, "发布任务_灰", 2000))
  {
    const POINT assign = { 122, 56 };//发布任务
    const POINT draw = { 481, 440 };//抽取并发布
    const POINT closeDraw = { 636, 196 };//关闭“抽取并发布”界面
    Click(task, account, assign);//单击“发布任务”
    WaitPicture(APPEAR, task, account, "发布任务");//等待进入发布任务界面
    CheckSleep(task, 500);
    Click(task, account, draw);//单击“抽取并发布”
    WaitPicture(APPEAR, task, account, "抽取并发布_灰");
    CheckSleep(task, 3000);
    Click(task, account, closeDraw);//关闭“抽取并发布”界面
  }
  CheckSleep(task, 500);
  Click(task, account, closeGuildMission);//关闭公会任务面板
  CheckSleep(task, 500);
}
//将custom拷贝至work数组的customCore
void LoadCustomCore(int task)
{
  WaitForSingleObject(hMutexUserImage, INFINITE);

  for (int code = 0; code < customNum; code++)
  {
    strcpy_s(work[task].custom[code].name, custom[code].name);
    work[task].custom[code].priority = custom[code].priority;
    CopyMap(work[task].custom[code].image, custom[code].image,
      customX, customY);
  }
  work[task].customNum = customNum;

  ReleaseMutex(hMutexUserImage);
}
//执行特殊任务
int EnterSpecial(int task, int level)
{
  int list = GetList(task);
  if (level == 0)//签到
  {
    plot[list].playingAdvance->result = 0;
    if (!ClaimHarmony(task))//领取温馨礼包
      plot[list].playingAdvance->result = 2;
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
        Sign(task, account);
  }
  else if (level == 1)//浇水施肥
  {
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
      {
        OpenGuildMission(task, account);//进入公会任务面板
        memset(plot[list].guildExist, 0, sizeof(plot[list].guildExist));//所有公会任务预设为不存在
        ReadGuild(task, account, plot[list].guildExist);//识别公会任务存入数组
        ClaimGuildAward(task, account, false);//领取奖励
        Click(task, account, closeGuildMission);//关闭公会任务界面
        CheckSleep(task, 500);
        //如果不存在施肥任务，则删除施肥未完成的标记
        if (!plot[list].guildExist[1])
          plot[list].playingAdvance->result = 0;
        //如果存在浇水或施肥任务，进行浇水施肥
        if (plot[list].guildExist[0] || plot[list].guildExist[1])
          WaterAndManure(task, account);
      }
  }
  else if (level == 2)//清理背包
  {
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
      {
        reserve[task].passwordReleased = false;//二级密码预设为“未解除”
        DeleteProp(task, account);//删除道具
        DecomposeJewel(task, account);//分解宝石
      }
  }
  else if (level == 3 || level == 4)//使用双经卡/双爆卡
  {
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
        UseDoubleCard(task, account, level - 3, param[task].maxGames);
  }
  else if (level == 5) //购买魔塔次数
  {
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
      {
        reserve[task].passwordReleased = false;//二级密码预设为“未解除”
        BuyTower(task, account, param[task].maxGames);
      }
  }
  else if (level == 6) //发布会长任务
  {
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
        AssignGuildMission(task, account);
  }
  else if (level == 7) //检查防御卡和门票数量
  {
    LoadCustomCore(task);//载入自定卡槽核心
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
      {
        CheckSlotInBag(task, account);
        CheckTicketInBag(task, account);
      }
    //运行到这里则标记检查成功
    plot[list].isDeepCheckingSuccessful = true;
    //展示检查结果
    PopDeepResult(list);
  }
  FinishTask(task);
  return 1;
}
//获取定时秒数
int GetTimerSecond(int timer)
{
  time_t stdTime = time(NULL);//获取当前系统时间（世界时）
  tm localTime;
  localtime_s(&localTime, &stdTime);//转化为本地时间
  int nowSecond = localTime.tm_hour * 3600 + localTime.tm_min * 60 + localTime.tm_sec;//当前时间秒数
  if (timer < nowSecond)  //如果定时时刻小于当前时刻，定时时刻+24小时
    return timer + 86400 - nowSecond;
  else
    return timer - nowSecond;
}
//退出服务器
void ExitServer(int task, int account)
{
  //1. 获取游戏窗口，游戏窗口不存在则无需退服
  HWND hWndHall = Hall(param[task].hallName[account]);
  HWND hWndOrigin = GetActiveGameWindow(hWndHall);
  if (!hWndOrigin)
    return;

  //2. 点击刷新按钮，直到游戏窗口关闭或更改
  int refreshX = 228, refreshY = 44;
  LeftClickDPI(hWndHall, refreshX, refreshY); //点击刷新按钮
  int counter = 0;
  do
  {
    counter++;
    if (counter > 100)
      ReportError(task, account, "刷新失败", "刷新按钮点击无效");
    CheckSleep(task, 100);
  } while (GetActiveGameWindow(hWndHall) == hWndOrigin);

  //3. 检测游戏窗口是否重新出现
  counter = 0;
  do
  {
    counter++;
    if (counter > 5) //5秒内没有出现游戏窗口，说明已经成功退服
      return;
    CheckSleep(task, 1000);
    param[task].hWnd[account] = GetActiveGameWindow(Hall(param[task].hallName[account]));
  } while (!param[task].hWnd[account]);

  //如果出现了游戏窗口，说明是微端网址，补点一次后退
  int backX = 142, backY = 44;
  LeftClickDPI(hWndHall, backX, backY); //点击后退按钮
  counter = 0;
  do
  {
    counter++;
    if (counter > 100)
      ReportError(task, account, "退服失败", "无法退出服务器");
    CheckSleep(task, 100);
  } while (GetActiveGameWindow(hWndHall));
}
//执行控制任务
int EnterControl(int task, int level)
{
  if (level == 0) //定时
  {
    time_t now = time(nullptr);//当前时刻
    //当前时刻到定时时刻的秒数
    int timerSecond = param[task].realTimer <= now ? 0 : (int)(param[task].realTimer - now);
    DWORD timerEndTick = GetTickCount() + timerSecond * 1000;//定时时刻

    int list = GetList(task);
    FILE *f;
    char path[maxPath];
    sprintf_s(path, "高级任务%d定时日志.txt", list);
    fopen_s(&f, path, "w");
    fclose(f);

    int counter = 0;
    while (GetTickCount() < timerEndTick) //等待到达定时时刻
    {
      CheckSleep(task, 100);
      if (counter % 100 == 0)
      {
        counter = 0;
        //写入内容：当前时间、param定时器、plot定时器及转化为本地时间的输出
        fopen_s(&f, path, "a");
        tm local;
        time_t now = time(NULL);
        localtime_s(&local, &now);
        fprintf_s(f, "now=%lld %02d/%02d %02d:%02d:%02d\t", now,
          local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
        localtime_s(&local, &param[task].realTimer);
        fprintf_s(f, "param=%lld %02d/%02d %02d:%02d:%02d\t", param[task].realTimer,
          local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);
        localtime_s(&local, &plot[list].playingAdvance->realTimer);
        fprintf_s(f, "plot=%lld %02d/%02d %02d:%02d:%02d\t", plot[list].playingAdvance->realTimer,
          local.tm_mon + 1, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec);

        // 打印时区信息
        TIME_ZONE_INFORMATION tzinfo;
        DWORD tzid = GetTimeZoneInformation(&tzinfo);
        fprintf(f, "bias=%d ", tzinfo.Bias);

        if (tzid == TIME_ZONE_ID_DAYLIGHT)
          fprintf(f, "DaylightBias=%d\n", tzinfo.DaylightBias);
        else if (tzid == TIME_ZONE_ID_STANDARD)
          fprintf(f, "StandardBias=%d\n", tzinfo.StandardBias);
        else
          fprintf(f, "UNKNOWN\n");
        fclose(f);
      }
      counter++;
    }
  }
  else if (level == 2) //退服
  {
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
        ExitServer(task, account);
  }
  else if (level == 3) //关机
  {
    const char tip[] = "警告：系统将在一分钟内关机。\n"
      "如需取消关机，请手动停止高级任务。";
    PopMessageThread(task, tip);
    CheckSleep(task, 60000);
    Execute("shutdown -p", true, true);
    CheckSleep(task, 1000000);
  }
  FinishTask(task);
  return 1;
}
//进入公会副本，进入成功返回1
void EnterGuildInstance(int task, int level)
{
  //房主进入房间列表，并尽量切换到无人区
  EnterGuildRoomList(task, work[task].host, level);
  SelectiveSleep(task, 500);
  int room = CreatePasswordRoom(task, work[task].host, false);//房主创建密码房并获取房间号

  if (work[task].isInvolved[1 - work[task].host]) //如果有队友
  {
    EnterGuildRoomList(task, 1 - work[task].host, level);//队友进入房间列表
    SelectiveSleep(task, 1000);
    //队友进入第一个房间
    if (!EnterFirstRoom(task, 1 - work[task].host, room))
    {
      char message[200] = {};
      sprintf_s(message, "未进入%d房间", room);
      ReportError(task, 1 - work[task].host, "未进入指定房间", message);
    }
  }
}
//跳转到美味镇
void EnterTown(int task, int account)
{
  const POINT town = { 500, 187 };//美味镇
  Click(task, account, worldMap);//打开世界地图
  WaitPicture(APPEAR, task, account, "世界地图");//等待打开世界地图
  DealInvitation(task, account, 0);
  Click(task, account, town);//点击美味镇
  WaitPicture(DISAPPEAR, task, account, "世界地图");//等待世界地图消失
  WaitColorDisappear(task, account, blueBox.x, blueBox.y, boxColor);//等待服务器蓝框消失
  CheckSleep(task, 500);
}
//从文件读取表面任务
void LoadSurface(int list)
{
  FILE *f;
  fopen_s(&f, "表面任务0.txt", "r");
  char s[1000] = {};
  for (int account = 0; account < 2; account++)
  {
    fgets(f, s);
    for (int i = 0; i < 9; i++)
    {
      fgets(f, s);//读取一条表面任务
      char *text = strchr(s, '\t');
      for (int times = 0; times < 8; times++) //查找第9个tab
        text = strchr(text + 1, '\t');
      strcpy_s(plot[list].surface[account][i].text, text + 1);//记录任务文本
      GetContestInfo(plot[list].surface[account][i]);//获取任务信息
      if (i >= 6)
        plot[list].surface[account][i].isFinished = true;
    }
    plot[list].surfaceSize[account] = 9;
  }
  fclose(f);
}
//执行识别任务
void EnterMission(int task, int level)
{
  //跳到美味镇，防止邀请干扰
  //for (int account = 0; account < 2; account++)
  //  if (param[task].deck[account])
  //    EnterTown(task, account);

  int list = GetList(task);
  if (level == 0) //任务-公会
  {
    memset(plot[list].guildExist, 0, sizeof(plot[list].guildExist));//所有公会任务预设为不存在
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
      {
        OpenGuildMission(task, account);//打开公会任务面板
        ReadGuild(task, account, plot[list].guildExist);//识别公会任务存入数组
        ClaimGuildAward(task, account, false);//领取奖励
        Click(task, account, closeGuildMission);
      }
    FinishTask(task);//直接宣告任务完成（完成后根据数组替换任务）
  }
  else if (level == 1) //任务-情侣
  {
    memset(plot[list].loverExist, 0, sizeof(plot[list].loverExist));//所有情侣任务预设为不存在
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
      {
        OpenLoverMission(task, account);//打开情侣任务面板
        ReadLover(task, account, plot[list].loverExist);//识别情侣任务存入数组
        ClaimLoverAward(task, account, false);//领取奖励
        Click(task, account, closeLoverMission);
      }
    FinishTask(task);//直接宣告任务完成（完成后根据数组替换任务）
  }
  else if (level == 2) //任务-大赛
  {
    //清空表面任务和最佳任务
    memset(&plot[list].bestContest, 0, sizeof(plot[list].bestContest));
    for (int account = 0; account < 2; account++)
    {
      plot[list].surfaceSize[account] = 0;
      memset(plot[list].surface[account], 0, sizeof(plot[list].surface[account]));
    }

    //领取奖励和识别表面任务
    for (int account = 0; account < 2; account++)
    {
      OpenContestMission(task, account);//打开大赛任务面板
      ClaimContestAward(task, account, false);//领取大赛任务奖励
      //识别大赛任务文本（同步大赛两个号都识别，否则只识别房主）
      if (plot[list].playingAdvance->dual || account == plot[list].playingAdvance->host)
        ReadContest(task, account);//识别战斗任务并填写完整信息
      Click(task, account, closeContestMission);//关闭大赛
    }

    //测试段：文件输入两个号的表面任务进行定位
    //LoadSurface(list);

    //清空未完成的大赛任务
    plot[list].failedSize = 0;
    memset(plot[list].failedContest, 0, sizeof(plot[list].failedContest));
    //记录未完成的大赛任务，填写每个表面任务的未完成次数
    RecordFailedContest(list);
    //清空应完成的大赛任务
    plot[list].requiredSize = 0;
    memset(plot[list].requiredContest, 0, sizeof(plot[list].requiredContest));

    //定位期数和进度（失败3次的任务所在列进度设为0）
    plot[list].isLocated = SmartLocateSurface(list);
    if (plot[list].isLocated) //定位成功则选择最优解
      SelectBestSurface(list);
    else //定位失败则选择最小编号任务（忽略失败3次的任务）
      SelectMinSurface(list);
    //填写最优任务的完整信息，记录应完成的任务（记录日志）
    FillBestContestFromSurface(list, true);
    FinishTask(task);//直接宣告任务完成（完成后插入编号最小的任务）
  }
}
//进入巅峰对决房间
void EnterPeakLevel(int task)
{
  int account = work[task].host;
  const POINT embark = { 761, 564 };//前往挑战

  WaitPicture(DISAPPEAR, task, account, "巅峰对决_次数背景");
  CheckSleep(task, 500);
  Click(task, account, embark);//选关进入房间列表
  WaitIndoor(task, account);
  CheckSleep(task, 4000);
}
//进入巅峰对决界面
void EnterPeak(int task)
{
  int account = work[task].host;
  ClickActivity(task, account, "巅峰图标");
  WaitPicture(APPEAR, task, account, "巅峰对决");
  WaitPicture(DISAPPEAR, task, account, "巅峰对决_BOSS背景");
  WaitPicture(DISAPPEAR, task, account, "巅峰对决_次数背景");
  CheckSleep(task, 1000);
}
//进入副本关卡（含假期和公会副本）
void EnterInstance(int task, int level)
{
  if (level == 0)
    EnterVacation(task);//假期
  else if (level >= 1 && level <= 3)
    EnterGuildInstance(task, level);//公会副本
  else if (level == 4)
    EnterPeak(task);//巅峰对决
}
//双号进入悬赏房间，进入成功返回1
void EnterBounty(int task, int level)
{
  EnterBoutyRoomList(task, work[task].host, level);//房主进入房间列表
  SelectiveSleep(task, 500);
  int room = CreatePasswordRoom(task, work[task].host, false);//房主创建密码房并获取房间号

  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    SelectiveSleep(task, 500);
    Invite(task, 1 - work[task].host, room);//邀请队友加入房间，邀请失败会报错
  }
}
//跳转到指定岛屿
void EnterIsland(int task, int account, int islandOrder)
{
  const POINT Adventure = { 620, 366 };//探险港口
  const POINT SwitchActivity = { 787, 35 };//切换活动

  //0=美味，1=火山，2=遗迹，3=浮空，4=海底，5=星际，6=营地，7以上先进营地
  Click(task, account, worldMap);//打开世界地图
  WaitPicture(APPEAR, task, account, "世界地图");//等待打开世界地图
  DealInvitation(task, account, 0);
  Click(task, account, islandLocation[min(islandOrder, 6)]);//点击岛屿
  WaitPicture(APPEAR, task, account, typeName[min(islandOrder, 6) + basicTypeNum],
    nullptr, nullptr, 0, 0, WaitTime() * 2);//等待进入岛屿
  WaitColorDisappear(task, account, blueBox.x, blueBox.y, boxColor);//等待服务器蓝框消失

  if (islandOrder >= 7) //7=沙漠 8=雪山 9=雷城 10=奇境
  {
    Click(task, account, Adventure);//点击探险港口
    WaitPicture(APPEAR, task, account, "探险港口界面");//等待进入探险港口界面
    Click(task, account, islandLocation[islandOrder]);//点击副本
    WaitPicture(APPEAR, task, account, typeName[basicTypeNum + islandOrder]);//等待进入副本
    WaitColorDisappear(task, account, blueBox.x, blueBox.y, boxColor);//等待服务器蓝框消失
  }

  char AboveName[13];
  sprintf_s(AboveName, "%s上", typeName[basicTypeNum + islandOrder]);
  int counter = 0;
  while (FindPicture(task, account, AboveName) == 0)//如果没找到岛屿上方界面图像，说明被活动图标遮挡
  {
    counter++;
    if (counter > 50)
    {
      char errorString[100];
      sprintf_s(errorString, "未能进入[%s]界面", AboveName);
      ReportError(task, account, "未进入指定界面", errorString);
    }
    if (counter % 10 == 1)
      Click(task, account, SwitchActivity);//点一下箭头
    CheckSleep(task, 100);
  }
}
//任务是否为多元奇遇关卡
bool IsMutielement(int type, int level)
{
  return (type == basicTypeNum || type == basicTypeNum + 1) && level >= 17;
}
//任务是否需要创建含有多元奇遇的房间
bool IsMutielementRoom(int type, int level)
{
  return (type == basicTypeNum && (level == 7 || level == 14 || level >= 17))
    || (type == basicTypeNum + 1 && (level == 5 || level == 10 || level >= 17));
}
//进入普通关卡房间列表，并切换到竞技一区
void EnterNormalRoomList(int task, int account, int islandOrder, int level)
{
  const POINT multielement = { 109, 253 };//多元奇遇

  EnterIsland(task, account, islandOrder);//跳转至指定岛屿
  SelectiveSleep(task, 500);
  DealInvitation(task, account, 0);
  Click(task, account, islandLevelButton[islandOrder][level]);
  WaitRoomList(task, account);
  if (IsMutielement(basicTypeNum + islandOrder, level))
  {
    CheckSleep(task, 100);
    Click(task, account, multielement);
    CheckSleep(task, 500);
  }
  RandomJump(task, account);
}
//进入普通关卡
void EnterNormal(int task, int islandOrder, int level)
{
  EnterNormalRoomList(task, work[task].host, islandOrder, level);//房主进入房间列表
  SelectiveSleep(task, 500);
  //房主创建密码房并获取房间号
  int room = CreatePasswordRoom(task, work[task].host, IsMutielementRoom(basicTypeNum + islandOrder, level));

  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    SelectiveSleep(task, 500);
    Invite(task, 1 - work[task].host, room);//邀请队友加入房间，邀请失败会报错
  }
}
//点击num张牌的“免费”按钮
void ClickFree(int task, int account, int num)
{
  //可以识图就读取波数，不是翻牌界面不翻牌
  if (IsGameWindowVisible(work[task].hWnd[account]))
  {
    int wave = 0;
    GetGameState(task, account, nullptr, &wave, nullptr);
    if (wave != waveFlip)
      return;
  }
  for (int i = 0; i < num; i++)
    Click(task, account, flip[i]);
}
//确认关卡完成已经回到选卡界面（只检测显示的号，因此高级任务没问题）
void ReturnStart(int task)
{
  int wave[2] = { 0, 0 };
  int counter = 0;
  bool isAnyAccountQuitted = false; //是否有账号退出房间

  //等待手动退出的账号回到选卡、结算、翻牌、魔塔界面或关卡外（最多5秒）
  for (int account = 0; account < 2; account++)
    if (work[task].isInvolved[account] && work[task].isQuitted[account])
    {
      work[task].isQuitted[account] = false;//账号恢复未退出状态（这样才能进行Click点击）
      isAnyAccountQuitted = true;
      GetGameState(task, account, NULL, &wave[account], NULL);
      counter = 0;
      while (!IsOutOfLevel(wave[account]) && wave[account] != waveGrade && wave[account] != waveFlip)
      {
        counter++;
        if (counter > 30) //点退出按钮后3秒不能结束关卡，报错“退出关卡失败”
          ReportError(task, account, "退出关卡失败", "退出关卡失败");
        CheckSleep(task, 100);
        GetGameState(task, account, NULL, &wave[account], NULL);
      }
    }

  //双号翻牌，直到进入选卡或魔塔界面
  bool isReturned[2] = { true, true };//两个号是否已经返回开始界面
  bool isFilpRequired[2] = { false, false };//两个号是否需要翻牌
  counter = 0;
  while (true)
  {
    counter++;
    if (counter > 30)
      ReportError0(task, "翻牌超时", "翻牌30秒未结束");
    for (int account = 0; account < 2; account++)
      if (work[task].isInvolved[account] == 1) //读取参与账号的波数，判断是否需要翻牌，是否已返回选卡界面
      {
        if (GetGameState(task, account, NULL, &wave[account], NULL) == 1) //如果识图成功
        {
          //没有回到选卡/魔塔界面
          if (!IsOutOfLevel(wave[account]))
          {
            isFilpRequired[account] = true; //需要翻牌
            isReturned[account] = false; //没有返回选卡界面
          }
          else //回到了选卡/魔塔界面
          {
            isFilpRequired[account] = false;//不需要翻牌
            isReturned[account] = true;//回到了选卡界面
          }
        }
        else //如果识图不成功
        {
          isFilpRequired[account] = true;//需要翻牌
          isReturned[account] = true;//回到了选卡界面
        }
      }
    if (isReturned[0] && isReturned[1]) //两个号都回到了选卡界面，则退出循环
      break;

    ReturnState(task, "运行中");//进度games/MaxGames，第Wave波第CurrentTime秒，平均用时不变，累计时间刷新
    for (int account = 0; account < 2; account++)
      if (isFilpRequired[account])
        ClickFree(task, account, flipNum);//点“免费”
    CheckSleep(task, 500);
    for (int account = 0; account < 2; account++)
      if (isFilpRequired[account])
        Click(task, account, finish);//点“完成”
    CheckSleep(task, 500);
    work[task].currentTime++;
  }

  const char *indoor[] = { "选卡界面", "选卡界面_实验室" };
  //如果回到的是waveOutOfLevel（选卡界面、岛屿主界面、勇士界面、跨服界面），且执行过手动退出，则所有在房间内的账号都要退出
  for (int account = 0; account < 2; account++)
    if (work[task].isInvolved[account] == 1 && IsGameWindowVisible(work[task].hWnd[account]))
      if (wave[account] == waveOutOfLevel && isAnyAccountQuitted)
        if (FindPictures(task, account, indoor))
          Click(task, account, retreat);

  //如果退出了房间
  if (isAnyAccountQuitted)
  {
    //不是最后一局，则需要重回房间
    if (work[task].games < work[task].maxGames)
    {
      if (!work[task].isSkillMode) //非刷技能模式等100ms，以免太快了
        CheckSleep(task, 100);
      SelectiveSleep(task, 400);

      //普通任务只有魔塔能触发退出
      if (IsNormal(task) && work[task].tower[work[task].host]) //魔塔（普通任务）：啥也不做
        return;

      //到这里就只可能是高级任务了
      int list = GetList(task);
      if (plot[list].playingAdvance->type == 2) //魔塔（高级任务）：什么也不做
        return;
      else if (plot[list].playingAdvance->type == 0) //悬赏：直接重走进房程序
        EnterBounty(task, plot[list].playingAdvance->level);
      else if (plot[list].playingAdvance->type == 1) //勇士：重走进房程序（勇士界面已打开不影响）
        EnterChallenge(task, plot[list].playingAdvance->level);
      //魔塔：啥也不需要做
      else if (plot[list].playingAdvance->type == 3) //跨服：重走进房程序（省去了进跨服界面这一步）
        EnterCross(task, plot[list].playingAdvance->level);
      else if (plot[list].playingAdvance->type == 4) //副本
      {
        if (plot[list].playingAdvance->level != 4) //假期和公会副本：重走进房程序
          EnterInstance(task, plot[list].playingAdvance->level);
      }
      else if (plot[list].playingAdvance->type >= 6)//普通关卡：重走进房程序
        EnterNormal(task, plot[list].playingAdvance->type - basicTypeNum, plot[list].playingAdvance->level);
      CheckSleep(task, 100);//进房后等0.1秒
    }
    else //最后一局无需重回房间，只需记录已退出房间，调整Retreat操作
      work[task].isQuitUsed = true;
  }
}
//退出关卡，退出成功返回1
int QuitLevel(int task, int account)
{
  POINT quit = { 923, 580 };
  POINT confirmQuit = { 473, 383 };
  const char *quitButton[] = { "退出按钮", "退出按钮亮" };
  //非刷技能模式：无退出按钮时不退出并返回0，等待下次退出。刷技能模式强制退出，但是退完要判断
  if (!work[task].isSkillMode && FindPictures(task, account, quitButton) == 0)
    return 0;
  Click(task, account, quit);
  Click(task, account, confirmQuit);
  work[task].isQuitted[account] = true;//记录关卡已退出
  return 1;
}
const int verifyX = 339, verifyY = 311, verifyWidth = 210, verifyHeight = 60;//验证码区域
const int verifyDigitWidth = 27, verifyDigitHeight = 40;//验证码数字尺寸
const int questionX = 369, questionY = 229, questionWidth = 218, questionHeight = 20;//验证问题区域
//验证码6个数字的位置
const POINT verifyDigitLoc[6] = { { 15, 15 }, { 45, 5 }, { 75, 15 }, { 105, 5 }, { 135, 15 }, { 165, 5 } };
COLORREF verifyDigit[10][verifyDigitHeight][verifyDigitWidth];//验证码数字模板
COLORREF question[10][questionHeight][questionWidth];//验证问题模板
//载入动态验证数字和问题
void LoadVerify()
{
  char path[maxPath] = {};
  for (int digit = 0; digit < 10; digit++)
  {
    sprintf_s(path, "附加程序\\数字\\动态验证\\%d.png", digit);
    BitmapToColor(path, verifyDigit[digit]);
  }
  for (int order = 0; order < 10; order++)
  {
    sprintf_s(path, "附加程序\\图片\\动态验证问题\\%d.png", order);
    BitmapToColor(path, question[order]);
  }
}
//比较两个验证码数字的相似度
int GetVerifySimilarity(COLORREF(&digit1)[verifyDigitHeight][verifyDigitWidth],
  COLORREF(&digit2)[verifyDigitHeight][verifyDigitWidth])
{
  int similarity = 0;
  for (int y = 0; y < verifyDigitHeight; y++)
    for (int x = 0; x < verifyDigitWidth; x++)
      if (digit1[y][x] == digit2[y][x])
        similarity++;
  return similarity;
}
//判断验证码数字图片中的数字
int GetVerifyDigit(COLORREF(&digit)[verifyDigitHeight][verifyDigitWidth])
{
  int maxSimilarity = -1;
  int bestNum = -1;
  for (int num = 0; num < 10; num++)
  {
    int similarity = GetVerifySimilarity(digit, verifyDigit[num]);
    if (similarity > maxSimilarity)
    {
      maxSimilarity = similarity;
      bestNum = num;
    }
  }
  return bestNum;
}
//根据截图获取问题编号
int GetQuestion(int task)
{
  for (int order = 0; order < 10; order++)
    if (IsBitmapEqual(work[task].map, question[order], questionWidth, questionHeight, questionX, questionY))
      return order;
  return -1;
}
//根据6位数字nums和问题编号question获取问题的答案
void SolveQuestion(char(&answer)[5], int(&nums)[6], int question)
{
  memset(answer, 0, sizeof(answer));
  if (question == 0)
    for (int i = 0; i < 4; i++)
      answer[i] = '0' + nums[i + 1];
  else
  {
    int minDigit = INT_MAX;
    for (int i = 1; i < 5; i++)
      if (nums[i] < minDigit)
        minDigit = nums[i];
    sprintf_s(answer, "%d", minDigit + question);
  }
}
//进行动态验证（截图已获取）
void ReleaseVerify(int task, int account)
{
  if (!TryWaitPicture(APPEAR, task, account, "动态验证", 500))
    return;

  //提取验证码图片
  COLORREF verify[verifyHeight][verifyWidth] = {};
  for (int y = 0; y < verifyHeight; y++)
    for (int x = 0; x < verifyWidth; x++)
      verify[y][x] = work[task].map[verifyY + y][verifyX + x] == 0xbbbbbb ? 0xffffff : 0;

  //识别问题
  int question = GetQuestion(task);

  //识别其中的数字
  int nums[6] = {};
  COLORREF digit[verifyDigitHeight][verifyDigitWidth] = {};
  for (int order = 0; order < 6; order++)
  {
    for (int y = 0; y < verifyDigitHeight; y++)
      for (int x = 0; x < verifyDigitWidth; x++)
        digit[y][x] = verify[verifyDigitLoc[order].y + y][verifyDigitLoc[order].x + x];
    nums[order] = GetVerifyDigit(digit);
  }

  //输入答案  
  const POINT input = { 392, 278 };
  const POINT confirm = { 475, 440 };
  char answer[5] = {};
  SolveQuestion(answer, nums, question);
  Click(task, account, input);
  CheckSleep(task, 500);
  StringToWindow(answer, work[task].hWnd[account]);
  CheckSleep(task, 1000);
  Click(task, account, confirm);
}
//进入关卡
void EnterLevel(int task)
{
  int list = GetList(task);

  //关闭健康提示和充值面板，解除验证
  for (int account = 0; account < 2; account++)
    if (work[task].isInvolved[account])
    {
      CloseHealthAndPayment(task, account);
      ReleaseVerify(task, account);
    }

  //如果为双人高级任务，需要检查两个号是否已匹配细角色名，否则自动截取
  if (work[task].isInvolved[0] && work[task].isInvolved[1])
    for (int account = 0; account < 2; account++)
      if (!work[task].IsLightNameMatched[account])
      {
        //截图
        ForcedUpdateMap(task, account);
        //从截图中提取粗角色名
        CopyMap(work[task].heavyName[account], 0, 0, heavyNameWidth, heavyNameHeight,
          work[task].map, heavyNameX, heavyNameY);
        //不是白色都变成黑色
        for (int y = 0; y < heavyNameHeight; y++)
          for (int x = 0; x < heavyNameWidth; x++)
            if (work[task].heavyName[account][y][x] != 0xffffff)
              work[task].heavyName[account][y][x] = 0;

        WaitForSingleObject(hMutexUserImage, INFINITE);
        //在已有角色名截图中匹配粗角色名
        for (int i = 0; i < roleNameNum; i++)
          //如果匹配成功
          if (IsBitmapEqual(roleName[i].image, work[task].heavyName[account], heavyNameWidth, heavyNameHeight))
          {
            //拷贝细角色名
            CopyMap(work[task].lightName[account], 0, 0, lightNameWidth, lightNameHeight,
              roleName[i].image, heavyNameWidth, 0);
            //记录细角色名已匹配
            work[task].IsLightNameMatched[account] = true;
            break;
          }
        ReleaseMutex(hMutexUserImage);

        //如果没有匹配细角色名，利用好友列表截取细角色名
        if (!work[task].IsLightNameMatched[account])
          CatchInviteeName(task, account);
      }

  //部分关卡打之前先换区：勇士/假期/双塔（需要邀请），巅峰/宠塔（需要出海）
  int type = plot[list].playingAdvance->type;
  int level = plot[list].playingAdvance->level;

  int inviteType = 0;//邀请类型
  //1类（可直接邀请）：勇士、双塔、假期
  if (type == 1 || (type == 2 && level / 1000 == 2) || (type == 4 && level == 0))
    inviteType = 1;
  //2类（需要去指定区建房再回来邀请）：悬赏、岛屿
  if (type == 0 || (type >= basicTypeNum && type < basicTypeNum + islandTypeNum))
    inviteType = 2;
  //3类（需要先出海）：巅峰、宠塔
  if ((type == 4 && level == 4) || (type == 2 && level / 1000 == 3))
    inviteType = 3;

  //1类和2类：如果是双人打，队友先换区，准备接受邀请
  if (inviteType == 1 || inviteType == 2)
    if (work[task].isInvolved[0] && work[task].isInvolved[1])
      RandomJump(task, 1 - work[task].host);

  //1类和3类：房主在此处换区
  if (inviteType == 1 || inviteType == 3)
    RandomJump(task, work[task].host);

  CheckSleep(task, 100);
  if (plot[list].playingAdvance->type == 0)//悬赏
    EnterBounty(task, plot[list].playingAdvance->level);
  else if (plot[list].playingAdvance->type == 1)//勇士
    EnterChallenge(task, plot[list].playingAdvance->level);
  else if (plot[list].playingAdvance->type == 2)//魔塔
    EnterTower(task, work[task].host, plot[list].playingAdvance->level / 1000);//房主进入魔塔界面
  else if (plot[list].playingAdvance->type == 3)//跨服
    EnterCross(task, plot[list].playingAdvance->level);
  else if (plot[list].playingAdvance->type == 4)//副本（假期和公会副本）
    EnterInstance(task, plot[list].playingAdvance->level);
  else if (plot[list].playingAdvance->type == 5)//公会副本
    EnterMission(task, plot[list].playingAdvance->level);
  else if (plot[list].playingAdvance->type == 17)//签到施肥清包
    EnterSpecial(task, plot[list].playingAdvance->level);
  else if (plot[list].playingAdvance->type == 18)//控制
    EnterControl(task, plot[list].playingAdvance->level);
  else
    EnterNormal(task, plot[list].playingAdvance->type - basicTypeNum, plot[list].playingAdvance->level);
  CheckSleep(task, 100);
}
//进入魔塔层数
void EnterTowerFloor(int task, int account, int tag, int floor)
{
  int targetWave = tag == 3 ? wavePet : waveTower;//目标波次
  int targetPage = (floor + 14) / 15;//目标层所在页数
  int towerThisPage = (floor - 1) % 15 + 1;//本层高度
  const POINT secret[5] = { { 60, 135 }, { 60, 230 }, { 60, 325 }, { 60, 420 }, { 60, 515 } };//密室

  bool isFirstGame = work[task].games == reserve[task].gamesFinished + 1;//是否为第1局
  //第一局时检查是否处于魔塔选层界面，并翻到指定页数
  if (isFirstGame)
  {
    int wave = 0;
    int counter = 0;//识图次数
    GetGameState(task, account, NULL, &wave, NULL);//第一次检查波数，第18波是魔塔界面
    while (wave != targetWave)
    {
      counter++;
      if (counter >= 3) //检查三次还没到就报错
        ReportError(task, account, "未进入选层界面", "请进入魔塔选层界面启动");
      CheckSleep(task, 50);
      GetGameState(task, account, NULL, &wave, NULL);
    }
    if ((tag == 1 || tag == 2 || tag == 3) && floor > 0) //单塔 双塔 宠塔
      PageTower(task, account, tag, targetPage);//翻到目标页
  }

  bool isQuitUsed = false;
  for (int iAccount = 0; iAccount < 2; iAccount++)
    if (work[task].isPerformed[iAccount] && work[task].quitTime[iAccount] > 0)
      isQuitUsed = true;

  //跳过选层：宠塔、非第1局、无退出
  bool skipEnterRoom = tag == 3 && !isFirstGame && !isQuitUsed;
  if (!skipEnterRoom)
  {
    const int petX = 257, towerX = 113, towerY = 540, towerHeight = 31;
    //选择层数
    if ((tag == 1 || tag == 2) && floor > 0) //单塔 双塔 宠塔
      Click(task, account, towerX, towerY - (towerThisPage - 1) * towerHeight);
    else if (tag == 3 && floor > 0) //单塔 双塔 宠塔
      Click(task, account, petX, towerY - (towerThisPage - 1) * towerHeight);
    else if (tag == 4)//密室
      Click(task, account, secret[floor]);
    if (!work[task].isSkillMode)
      CheckSleep(task, 100);
    SelectiveSleep(task, 100);

    //开始挑战
    const POINT towerEmbark = { 618, 562 };//魔塔“开始挑战”按钮
    const POINT petEmbark = { 738, 562 };//宠塔“开始挑战”按钮
    if (tag == 3)
      Click(task, account, petEmbark);
    else
      Click(task, account, towerEmbark);
    if (!work[task].isSkillMode)
      CheckSleep(task, 100);
    SelectiveSleep(task, 100);
  }

  //等待进入房间
  int counter = 0;
  int wave = 0;
  GetGameState(task, account, NULL, &wave, NULL);
  while (wave != waveOutOfLevel)
  {
    counter++;
    if (counter >= 20)//如果超过10秒没进入房间，报错退出
      ReportError(task, account, "进入魔塔失败", "进入魔塔失败");
    CheckSleep(task, 500);//每个循环等0.5秒
    GetGameState(task, account, NULL, &wave, NULL);
  }
}
//进入魔塔关卡
void EnterTowerLevel(int task)
{
  //单塔和密室直接房主进层；双塔在EnterTower前截图队友名字，先队友退房，然后房主进房，然后邀请
  int level = 0; //魔塔关卡
  if (IsAdvance(task)) //高级任务：从列表获取
  {
    int list = GetList(task);
    level = plot[list].playingAdvance->level;
  }
  else //普通任务：轨道中的魔塔层数+1000
  {
    int tower = work[task].tower[work[task].host];
    level = tower == 0 ? 0 : 1000 + tower;
  }
  int tag = level / 1000;//魔塔种类
  int floor = level % 1000;//层数

  if (tag == 2) //双塔模式：队友先退房
  {
    const char *indoor[] = { "选卡界面", "选卡界面_实验室" };
    if (FindPictures(task, 1 - work[task].host, indoor))
      Click(task, 1 - work[task].host, retreat);
  }

  EnterTowerFloor(task, work[task].host, tag, floor);//房主进入层数

  if (tag == 2) //双塔模式：邀请队友
  {
    SelectiveSleep(task, 500);
    Invite(task, 1 - work[task].host);//邀请队友加入房间
  }
}
//准备程序，超过MaxWaitTime秒未准备则报错
void Ready(int task, int MaxWaitTime, int &Entry)
{
  int Wave = 0, Box = 0;
  bool isBagConfirmed = false;//是否已经点了背包满确认，点了就不用操作了
  /*进入准备程序（识图模式无图像状态无法完成准备）*/
  if (work[task].gameTime == zero)//识图模式：识图判定准备
  {
    for (int iColor = 1; iColor <= 300; iColor++)//最多等30秒
    {
      GetGameStateFromAnyAccount(task, &Entry, &Wave, &Box);//读取关卡进入状态
      //几种退出的情形
      if (Entry == 1)//Entry==1说明队友擅自进入了关卡
        ReportError0(task, "房主可能已掉线", "房主未进入关卡，可能已掉线");
      else if (Entry == 2)//Entry==2说明双人已经进入了关卡
        break;
      else if (Entry == -2)//Entry==-2说明队友已准备
      {
        CheckSleep(task, 2000);//等2秒以免房主有延迟
        break;
      }
      else if (iColor >= MaxWaitTime * 10)//超时没准备上
        ReportError0(task, "队友无法准备", "队友无法准备");//弹窗报错
      //准备操作：只在未确认背包满时进行点击，确认后就准备完成了
      if (isBagConfirmed == false)
      {
        for (int account = 0; account < 2; account++) //无图像或检测到处于翻牌状态则翻牌
          if (work[task].isInvolved[account] == 1)
          {
            if (iColor % 10 == 4)
            {
              if (GetGameState(task, account, NULL, &Wave, &Box) == 0 || Wave == waveFlip)
                ClickFree(task, account, flipNum);
            }
            else if (iColor % 10 == 7)
            {
              if (GetGameState(task, account, NULL, &Wave, &Box) == 0 || Wave == waveFlip)
                Click(task, account, finish);
            }
          }
        if (iColor % 20 == 1) //先点准备再点确认背包满，以免取消准备
          Click(task, 1 - work[task].host, startX, startY);//队友准备
        if (iColor % 10 == 1)//经验满和背包满
        {
          if (GetGameState(task, 1 - work[task].host, NULL, &Wave, &Box)) //如果识别到了图像
          {
            if (Wave == waveOutOfLevel) //仅在选卡界面确认
            {
              if (Box == 3) //经验满弹窗
              {
                if (expFullOperation == 0) //如果设置经验满时停止
                  ReportError(task, 1 - work[task].host, "经验已达上限", "经验已达上限", END_TASK);
                else //如果设置经验满时继续
                {
                  Click(task, 1 - work[task].host, expNoMoreTip);//勾选不再提示
                  Click(task, 1 - work[task].host, bagConfirmX, bagConfirmY);//确认
                }
              }
              else if (Box == 2) //背包满弹窗
              {
                if (bagFullOperation == 0) //如果设置背包满时停止
                  ReportError(task, 1 - work[task].host, "背包满或未带卡",
                    "检测到背包满或未带卡的弹窗，\n如需跳过，请将“背包满时”设为继续。", END_TASK);
                else //如果设置背包满时继续
                {
                  Click(task, 1 - work[task].host, bagConfirmX, bagConfirmY);//确认准备
                  isBagConfirmed = true;//记录已经点击确认
                }
              }
            }
          }
          else
          {
            if (expFullOperation == 1) //如果设置经验满时继续
            {
              Click(task, 1 - work[task].host, expNoMoreTip);//勾选不再提示
              Click(task, 1 - work[task].host, bagConfirmX, bagConfirmY);//确认
            }
            if (bagFullOperation == 1) //如果设置背包满时继续
              Click(task, 1 - work[task].host, bagConfirmX, bagConfirmY);//确认准备
          }
        }
      }
      CheckSleep(task, 100);//每0.1秒识图一次
    }
    if (work[task].games == reserve[task].gamesFinished + 1)
      strcat_s(work[task].embarkString, "队友已准备\n");
  }
  else //非识图模式：点完准备等2秒直接进入开始程序
  {
    Click(task, 1 - work[task].host, startX, startY);//只点一次准备
    for (int i = 1; i <= 4; i++)//退出准备程序再等2秒
    {
      CheckSleep(task, 500);//一共等待2000ms
      GetGameStateFromAnyAccount(task, &Entry, &Wave, NULL);//读取关卡进入状态
      if (Entry == -2 || Entry == 1 || Entry == 2)//已准备或进入关卡则跳出循环
        break;
      if (expFullOperation == 1) //如果设置经验满时继续
      {
        Click(task, 1 - work[task].host, expNoMoreTip);//勾选不再提示
        Click(task, 1 - work[task].host, bagConfirmX, bagConfirmY);//确认
      }
      if (bagFullOperation == 1) //如果设置背包满时继续
        Click(task, 1 - work[task].host, bagConfirmX, bagConfirmY);//确认准备
    }
  }
}
//开始程序
void Start(int task, int MaxWaitTime, int &Entry)
{
  int Wave = 0, Box = 0;
  bool isBagConfirmed = false;//是否已经点了背包满确认，点了就不用操作了
  Click(task, work[task].host, startX, startY);//房主开始
  for (int iColor = 1; iColor <= 300; iColor++)//最多等30秒
  {
    GetGameStateFromAnyAccount(task, &Entry, &Wave, NULL);//读取关卡进入状态，Box=2意味着弹出提示
    if (Entry > zero || Entry == -1)//已经进入关卡或加载程序，则跳出循环
      break;
    else if (iColor >= MaxWaitTime * 10)//房主翻牌完成10秒没开始，或队友翻牌完成30秒没开始，则报错退出
      ReportError0(task, "进入关卡失败", "进入关卡失败");//弹窗报错

    //开始操作：每秒1次
    if (isBagConfirmed == false && iColor % 10 == 0)
    {
      //翻牌：有可能点到格子，只能放在点开始前。每3秒1次
      if (iColor % 30 == 0)
      {
        if (GetGameState(task, work[task].host, NULL, &Wave, &Box) == 0 || Wave == waveFlip)
        {
          ClickFree(task, work[task].host, flipNum);
          Click(task, work[task].host, finish);
        }
      }

      if (GetGameState(task, work[task].host, NULL, &Wave, &Box)) //如果识别到了图像
      {
        if (Wave == waveOutOfLevel) //仅在选卡界面确认
        {
          if (Box == 3) //经验满弹窗
          {
            if (expFullOperation == 0) //如果设置经验满时停止
              ReportError(task, work[task].host, "经验已达上限", "经验已达上限", END_TASK);
            else //如果设置经验满时继续
            {
              Click(task, work[task].host, expNoMoreTip);//勾选不再提示
              Click(task, work[task].host, bagConfirmX, bagConfirmY);//确认
            }
          }
          else if (Box == 2) //背包满弹窗
          {
            if (bagFullOperation == 0) //如果设置背包满时停止
              ReportError(task, work[task].host, "背包满或未带卡",
                "检测到背包满或未带卡的弹窗，\n如需跳过，请将“背包满时”设为继续。", END_TASK);
            else //如果设置背包满时继续
            {
              Click(task, work[task].host, bagConfirmX, bagConfirmY);//确认准备
              isBagConfirmed = true;//记录已经点击确认
            }
          }
        }
      }
      else //没有识别到图像：每3秒在点开始按钮前的瞬间点确认
      {
        if (iColor % 30 == 0)
        {
          if (expFullOperation == 1) //如果设置经验满时继续
          {
            Click(task, work[task].host, expNoMoreTip);//勾选不再提示
            Click(task, work[task].host, bagConfirmX, bagConfirmY);//确认
          }
          if (bagFullOperation == 1) //如果设置背包满时继续
            Click(task, work[task].host, bagConfirmX, bagConfirmY);//确认准备
        }
      }

      if (isBagConfirmed == false && iColor % 30 == 0) //如果已经识别到蓝框并点击，就不用再点开始按钮了
        Click(task, work[task].host, startX, startY);//房主开始
    }
    CheckSleep(task, 100);//每0.1秒识图
  }
  if (work[task].games == reserve[task].gamesFinished + 1)
    strcat_s(work[task].embarkString, "房主已开始\n");
}
int outSkillNum = 0;
//统计截图成功率
void CountUpdateSuccessRate(int &updateNum, int &successfulUpdateNum, int &iconicNum)
{
  for (int task = 0; task < maxTaskNum; task++)
  {
    updateNum += reserve[task].updateNum;
    successfulUpdateNum += reserve[task].successfulUpdateNum;
    iconicNum += reserve[task].iconicNum;
  }
}
//将任务0的调试输出信息写入字符串tracePrint（重绘界面时会显示这个字符串）
//内容：显示所有任务的截图数和成功次数，debug模式下显示其他信息
void WriteTracePrint(int task)
{
  int updateNum = 0, successfulUpdateNum = 0, iconicNum = 0;
  CountUpdateSuccessRate(updateNum, successfulUpdateNum, iconicNum);
  char message[1000] = {}, trace[1000] = {};//提示消息
  sprintf_s(message, "识图%d帧, 成功%d帧", updateNum, successfulUpdateNum);
  strcat_s(trace, message);
  if (iconicNum > 0)
  {
    sprintf_s(message, ", 最小化%d帧", iconicNum);
    strcat_s(trace, message);
  }
#ifdef _DEBUG
  /*
  strcat_s(Trace, "出怪");
  for (int i = 0; i < Work[task].ratRowNum; i++)
  {
    sprintf_s(message, "%d", Work[task].ratRow[i]);
    strcat_s(Trace, message);
  }
  */
  /*
  sprintf_s(message, ", 图数%d，", imageNumGotton);
  strcat_s(Trace, message);
  */
  /*
  for (int i = 0; i < imageNumGotton; i++)
  {
    sprintf_s(message, "%d%d", imageLocation[i].x, imageLocation[i].y);
    strcat_s(Trace, message);
    if (i < imageNumGotton - 1)
      strcat_s(Trace, ",");
  }
  strcat_s(Trace, "）");
  */
  sprintf_s(message, ",1槽成功次数=%d", work[task].slot[0][1].successfulTimes);
  strcat_s(trace, message);

  for (int account = 0; account < 2; account++)
  {
    sprintf_s(message, "[%dP]收集%d, 火苗%d, 卡槽%d, 冷却",
      account + 1, work[task].isCollect[account], work[task].energy[account],
      work[task].realSlotNum[account]);
    strcat_s(trace, message);
    for (int i = 1; i <= work[task].realSlotNum[account]; i++)
    {
      sprintf_s(message, "%d", work[task].isCold[account][i]);
      strcat_s(trace, message);
    }
    strcat_s(trace, " ");
  }

  sprintf_s(message, " | 读CD次数%d", getColdNum);
  strcat_s(trace, message);
  /*
  sprintf_s(message, " | 海星=%d", Work[task].starfishOrder[0]);
  strcat_s(Trace, message);
  */
#endif
  strcpy_s(tracePrint, trace);
}
//准备和开始
void ReadyAndStart(int task, int &entry)
{
  int maxWaitTime = 30; //最大等待时间
  if (work[task].games == reserve[task].gamesFinished + 1)//第一局只等10秒，方便查错
    maxWaitTime = 10;

  //准备
  if (work[task].isInvolved[1 - work[task].host] == 1) //如果队友参与，则执行准备程序
    Ready(task, maxWaitTime, entry);

  //开始
  if (work[task].isInvolved[work[task].host] == 1) //如果房主参与，则执行开始程序
    Start(task, maxWaitTime, entry);

  //等待加载完成进入关卡
  int counter = 0;
  while (entry <= 0)
  {
    counter++;
    if (counter > 600) //最多允许加载60秒
      ReportError0(task, "加载超时", "加载超时");
    CheckSleep(task, 100);//每0.1秒识图
    GetGameStateFromAnyAccount(task, &entry, NULL, NULL);//读取关卡进入状态
  }
  if (work[task].games == reserve[task].gamesFinished + 1)
    strcat_s(work[task].embarkString, "已进入关卡\n");

  //双人刷图只进了一个人，提示另一个号已掉线
  if (work[task].isInvolved[0] && work[task].isInvolved[1] && entry == 1)
    ReportError0(task, "队友可能已掉线", "队友未进入关卡，可能已掉线");
}
//抵达卡槽末端，end：0=头部，1=尾部
void goToSlotEnd(int task, int account, int end, bool isConfirmRequired = false)
{
  const POINT DeckPageUp = { 930, 57 }, DeckPageDown = { 930, 86 };//卡组向左翻、向右翻
  POINT button = end == 0 ? DeckPageUp : DeckPageDown;

  //点击5下按钮
  for (int i = 0; i < 5; i++)
  {
    Click(task, account, button);
    CheckSleep(task, 100);
  }
  //如果开启了确认，则继续点到按钮变灰为止，但最多点5次
  if (isConfirmRequired)
  {
    int counter = 0;
    while (GetColor(task, account, button) != 0xa0a0a0)
    {
      counter++;
      if (counter > 5)
        ReportError(task, account, "卡组无法翻页", "卡组无法翻页");
      Click(task, account, button);
      CheckSleep(task, 100);
    }
    CheckSleep(task, 100);//截完图等100 ms，否则下次截图可能会黑
  }
}
//检查空卡槽，白天分出咖啡粉前后的卡槽，夜晚所有卡槽都是咖啡粉前卡槽。
void CheckEmptySlot(int task, int account, bool &isCoffeeExist, bool(&isBeforeCoffee)[maxSlotNum + 1],
  bool &isAnySlotBeforeCoffee, bool(&isAfterCoffee)[maxSlotNum + 1], bool &isAnySlotAfterCoffee)
{
  const COLORREF emptySlotColor = 0x1a4263;
  const POINT DeckSlot = { 395, 91 };
  const int DeckSlotWidth = 48;
  bool isDay = false;//是否白天关卡
  isCoffeeExist = false;//咖啡是否需要携带（夜晚固定为false）
  isAnySlotBeforeCoffee = false;
  isAnySlotAfterCoffee = false;
  memset(isBeforeCoffee, 0, sizeof(isBeforeCoffee));
  memset(isAfterCoffee, 0, sizeof(isAfterCoffee));
  for (int order = 1; order <= work[task].originalSlotNum[account]; order++)
  {
    if (order == 1) //翻到1-11槽
    {
      goToSlotEnd(task, account, 0, true);
      if (FindPictureInMap(task, "白天关卡", nullptr, nullptr, 0, 0, 0, 0, 0xffffff))
        isDay = true;
    }
    else if (order == 12)//翻到12-21槽
      goToSlotEnd(task, account, 1, true);
    int orderInPage = order <= 11 ? order - 1 : order - 11;//卡槽order在画面中的位置
    //如果本卡槽是空卡槽
    if (work[task].map[DeckSlot.y][DeckSlot.x + orderInPage * DeckSlotWidth] == emptySlotColor)
    {
      //如果是白天关卡，本卡槽为咖啡粉，记录咖啡粉存在
      if (isDay && strcmp(work[task].originalSlot[account][order].name, "咖啡粉") == 0)
        isCoffeeExist = true;
      else if (isCoffeeExist) //对于非咖啡粉，如果咖啡粉已存在，记入咖啡后卡槽
      {
        isAfterCoffee[order] = true;
        isAnySlotAfterCoffee = true;
      }
      else //如果咖啡粉尚未存在，记入咖啡前卡槽
      {
        isBeforeCoffee[order] = true;
        isAnySlotBeforeCoffee = true;
      }
    }
  }
  CheckSleep(task, 100);//截完图等待100 ms
}
//携带一组自定卡。isSlotRequired：每张卡是否需要携带
void TakeCustomGroup(int task, int account, bool(&isSlotRequired)[maxSlotNum + 1], int maxStar)
{
  char message[100] = {};
  int bestCode[maxSlotNum + 1] = {};//每个卡槽匹配到的最佳候选卡的编号
  int bestStar[maxSlotNum + 1] = {};//每个卡槽匹配到的最佳候选卡的星级

  //第一轮扫描：检查每个空卡槽所能匹配的最高优先级，记录其编号
  ScanCustomzz(task, account, isSlotRequired, bestCode, bestStar, maxStar);

  if (customOption == 1) //如果带卡选择为“位置”，则最优星级设为-1（表示不选择星级）
    for (int order = 0; order <= maxSlotNum; order++)
      bestStar[order] = -1;

  //如果某张空卡槽最优编号为-1，宣告order号卡不存在
  for (int order = 1; order <= work[task].originalSlotNum[account]; order++)
    if (isSlotRequired[order] && bestCode[order] == -1)
    {
      if (strcmp(work[task].originalSlot[account][order].name, "咖啡粉") == 0)
        strcpy_s(message, "未找到[木塞子]替咖啡粉");
      else
        sprintf_s(message, "未找到绑定卡[%s]", work[task].originalSlot[account][order].name);
      ReportError(task, account, "找卡失败", message);
    }

  //依次查找并携带每个空卡槽的最高优先级卡片
  for (int order = 1; order <= work[task].originalSlotNum[account]; order++)
    if (isSlotRequired[order])
    {
      int option = bestStar[order] == -1 ? 0 : 1;//选项：0=任意星级 1=指定星级
      int star = bestStar[order];//星级
      if (!TakeCustom(task, account, bestCode[order], option, star))
      {
        sprintf_s(message, "未找到绑定卡[%s]", work[task].originalSlot[account][order].name);
        ReportError(task, account, "找卡失败2", message);
      }
    }

}
//携带指定名称的自定卡的最高优先级，不限星级（带卡失败不报错）
void TakeCertainCustom(int task, int account, const char *customName)
{
  char message[100] = {};
  for (int code = work[task].customNum - 1; code >= 0; code--)
    if (strcmp(work[task].custom[code].name, customName) == 0)
    {
      if (!TakeCustom(task, account, code, 0, -1))
      {
        sprintf_s(message, "未找到绑定卡[%s]", customName);
        ReportError(task, account, "找卡失败3", message);
      }
      return;
    }
}
//如果卡组全空，带木塞子
void TakePlugIfEmpty(int task, int account)
{
  const COLORREF emptySlotColor = 0x1a4263;
  const POINT DeckSlot = { 395, 91 };
  const int DeckSlotWidth = 48;
  for (int order = 1; order <= 21; order++)
  {
    if (order == 1) //翻到1-11槽
      goToSlotEnd(task, account, 0, true);
    else if (order == 12)//翻到12-21槽
      goToSlotEnd(task, account, 1, true);
    int orderInPage = order <= 11 ? order - 1 : order - 11;//卡槽order在画面中的位置
    //有一个卡槽非空就无需带卡
    if (work[task].map[DeckSlot.y][DeckSlot.x + orderInPage * DeckSlotWidth] != emptySlotColor)
      return;
  }
  CheckSleep(task, 100);//截完图等待100 ms
  //如果全是空的，带木塞子
  TakeCertainCustom(task, account, "木塞子");
}
//若banned为真，卸除第order个卡槽
void BanSlot(int task, int account, int order, bool banned)
{
  const POINT deckSlot = { 411, 73 }; //第1个卡槽位置
  const int deckSlotWidth = 48;//卡槽宽度

  if (order == 1) //翻到1-11槽
    goToSlotEnd(task, account, 0, true);
  else if (order == 12)//翻到12-21槽
    goToSlotEnd(task, account, 1, true);
  if (banned) //卡片被ban
  {
    if (order <= 11)
      Click(task, account, deckSlot.x + (order - 1) * deckSlotWidth, deckSlot.y);//点掉卡槽
    else
      Click(task, account, deckSlot.x + (order - 11) * deckSlotWidth, deckSlot.y);//点掉卡槽
    CheckSleep(task, 100);
  }
}
//选择卡组，并根据ban卡列表取消携带某些卡组
void ChooseDeck(int task)
{
  bool isDeckRequired = false;//是否需要携带卡组
  //第1局需要带卡
  if (work[task].games == reserve[task].gamesFinished + 1)
    isDeckRequired = true;
  else
  {
    //魔塔关卡需要带卡
    if (IsTowerTask(task) || IsPeakTask(task))
      isDeckRequired = true;
    //某账号设置了退出时间需要带卡
    for (int account = 0; account < 2; account++)
      if (work[task].isPerformed[account] && work[task].quitTime[account] > 0)
        isDeckRequired = true;
  }
  if (!isDeckRequired)
    return;

  for (int account = 0; account < 2; account++)//选择卡组
    if (work[task].isInvolved[account] == 1)//如果小号1参与
    {
      if (work[task].deck[account] > zero) //如果设置了卡组，点击卡组
      {
        if (!work[task].isSkillMode)
        {
          Click(task, account, deckX, deckY);//选择卡组1
          SelectiveSleep(task, 300);
        }
        Click(task, account, deckX + (work[task].deck[account] - 1) * deckWidth, deckY);//选择目标卡组
      }
      if (work[task].isSkillMode)
        return;

      //如果有卸卡指令，先把需要替换的卡槽卸了
      if (work[task].isBanEnabled[account])
        for (int order = 1; order <= maxSlotNum; order++)
          BanSlot(task, account, order, strlen(work[task].originalSlot[account][order].alternate) > 0);

      //如果设置了自定卡，查找并选择自定卡（条件：账号参与放卡且可见）
      if (work[task].customNum > 0 && IsGameWindowVisible(work[task].hWnd[account]))
      {
        if (work[task].isPerformed[account])//放卡账号：按轨道带卡
        {
          //1-21槽是否为咖啡粉前（后）的空卡槽
          bool isBeforeCoffee[maxSlotNum + 1] = {}, isAfterCoffee[maxSlotNum + 1] = {};
          //是否存在咖啡粉前（后）的空卡槽
          bool isAnySlotBeforeCoffee = false, isAnySlotAfterCoffee = false;
          //是否存在咖啡粉空卡槽
          bool isCoffeeExist = false;

          //非刷技能模式，检查空卡槽，并按照咖啡前、咖啡、咖啡后分类
          if (!work[task].isSkillMode)
            CheckEmptySlot(task, account, isCoffeeExist, isBeforeCoffee, isAnySlotBeforeCoffee,
              isAfterCoffee, isAnySlotAfterCoffee);

          //如果有咖啡粉前的空卡槽，检查并携带
          if (isAnySlotBeforeCoffee)
            TakeCustomGroup(task, account, isBeforeCoffee, work[task].star[account]);
          //如果有咖啡粉，携带咖啡粉
          if (isCoffeeExist)
            TakeCertainCustom(task, account, "咖啡粉");
          //如果有咖啡粉后的卡槽，检查并携带
          if (isAnySlotAfterCoffee)
            TakeCustomGroup(task, account, isAfterCoffee, work[task].star[account]);
        }
        else //不放卡账号：如果卡组全空，带木塞子
          TakePlugIfEmpty(task, account);
      }

      //如果需要卸卡，执行卸卡
      if (work[task].isBanEnabled[account])
        for (int order = 1; order <= maxSlotNum; order++)
          BanSlot(task, account, order, work[task].originalSlot[account][order].banned &&
            strlen(work[task].originalSlot[account][order].alternate) == 0);
    }
  SelectiveSleep(task, 500);
}
//所有放卡账号是否已经显示
bool isPerformedAccountVisible(int task)
{
  for (int account = 0; account < 2; account++)
    if (work[task].gameTime == 0 && work[task].isPerformed[account] == 1 && !IsGameWindowVisible(work[task].hWnd[account]))
      return false;
  return true;
}
//设置波次为wave，小波和进度清零
void ResetWave(int task, int wave)
{
  work[task].wave = wave;
  work[task].smallWave = 0;//小波设为0
  work[task].progress = 0;//进度设为0
}
const int dragonX = 320, dragonY = 104, dragonWidth = 66, dragonHeight = 83;
COLORREF dragon[dragonHeight][dragonWidth];
byte dragonTable[16777216 / 8];
//载入龙图
void LoadDragon()
{
  const char path[] = "附加程序\\文本\\龙图.txt";
  FILE *f;
  if (fopen_s(&f, path, "rb"))
    ReportMissingFile(path);
  while (!feof(f))
  {
    byte b = fgetc(f);
    byte g = fgetc(f);
    byte r = fgetc(f);
    COLORREF color = r * 65536 + g * 256 + b;
    RecordColor(dragonTable, color);
  }
  fclose(f);
}
//保存龙图（弃用）
void SaveDragon()
{
  FILE *f;
  fopen_s(&f, "附加程序\\文本\\新龙图.txt", "wb");
  for (COLORREF color = 0; color <= 0xffffff; color++)
    if (IsColorExist(dragonTable, color))
    {
      fputc(bgrBValue(color), f);
      fputc(bgrGValue(color), f);
      fputc(bgrRValue(color), f);
    }
  fclose(f);
}
/*
//初始地图类型
struct InnateType
{
  char name[100];//地图名称
  COLORREF judge[GridHeight][10];//判定区，用于判定地图种类
};
*/
COLORREF musicInnateGrid[7 * gridHeight][7 * gridWidth];
//载入音乐节夜的初始地图
void LoadMusicInnateGrid()
{
  BitmapToColor("附加程序\\图片\\内置地图\\音乐夜.png", musicInnateGrid);
}
//载入初始地图格子
void LoadInnateGrid(int task)
{
  //棉麦放置次数清零
  memset(reserve[task].maltoseTimes, 0, sizeof(reserve[task].maltoseTimes));
  //先判断一下是不是音乐节夜
  int x1 = (3 - 1) * gridWidth;
  int y1 = (7 - 1) * gridHeight;
  int x0 = innateGridX + x1;
  int y0 = innateGridY + y1;
  //是否为音乐节夜
  bool isMusic = IsBitmapEqual(work[task].map, musicInnateGrid, gridWidth, gridHeight, x0, y0, x1, y1);
  int startColumn = 1;//起始拷贝列
  //如果是音乐节夜，从无卡地图拷贝前7列地图，只从map中获取最后两列地图，同时标记所有初始格子
  if (isMusic)
  {
    for (int row = 1; row <= 7; row++)
      for (int column = 1; column <= 7; column++)
        CopyMap(reserve[task].innateGrid[row - 1][column - 1], musicInnateGrid,
          (column - 1) * gridWidth, (row - 1) * gridHeight);
    //第1,5,6,7列标记为放置过1次棉麦
    for (int row = 1; row <= 7; row++)
    {
      reserve[task].maltoseTimes[row - 1][1 - 1] = 1;
      reserve[task].maltoseTimes[row - 1][5 - 1] = 1;
      reserve[task].maltoseTimes[row - 1][6 - 1] = 1;
      reserve[task].maltoseTimes[row - 1][7 - 1] = 1;
    }
    reserve[task].isMaltoseClearRequired = true;//需要重铺棉麦
    startColumn = 8;
  }
  for (int row = 1; row <= 7; row++)
    for (int column = startColumn; column <= 9; column++)
    {
      int y0 = innateGridY + (row - 1) * gridHeight;
      int x0 = innateGridX + (column - 1) * gridWidth;
      for (int y = 0; y < gridHeight; y++)
        for (int x = 0; x < gridWidth; x++)
          if (IsColorExist(dragonTable, work[task].map[y0 + y][x0 + x]))
            reserve[task].innateGrid[row - 1][column - 1][y][x] = 0;
          else
            reserve[task].innateGrid[row - 1][column - 1][y][x] = work[task].map[y0 + y][x0 + x];
    }
}
COLORREF vipImage[15][vipHeight][vipWidth];//各级vip图片
//载入VIP图片
void LoadVip()
{
  char folder[maxPath] = "附加程序\\图片\\VIP";
  if (!FileExist(folder))
  {
    PopMessage(nullptr, "没有VIP图片。");
    exit(0);
  }
  char path[maxPath];
  for (int vip = 0; vip <= 14; vip++)
  {
    sprintf_s(path, "%s\\%d.png", folder, vip);
    BitmapToColor(path, vipImage[vip]);
  }
}
//获取VIP等级
int GetVip(int task, int account)
{
  int location = account == work[task].host ? 0 : 1;//房主在左，队友在右
  for (int vip = 0; vip <= 14; vip++)
    if (IsBitmapEqual(work[task].map, vipImage[vip], vipWidth, vipHeight,
      vipX + location * vipDistance, vipY))
      return vip;
  return -1;
}
//检测是否处于房间内（房间内1，关卡内2，其他界面0），并校准偏移值
int CheckOffsetIndoor(int task, int account)
{
  const char *indoor[] = { "选卡界面", "选卡界面_实验室" };
  const int headX = 100;//头像特征颜色判定位置X
  const COLORREF levelColor = 0xfcf3ca; //关卡内头像特征颜色
  const COLORREF proceedColor = 0x4c4a3d; //继续作战弹窗时头像特征颜色

  //对于偏移0和偏移-2，分别检查选卡界面和关卡内
  for (int offset = 0; offset >= -2; offset -= 2)
  {
    //判断是否处于选卡界面
    if (FindPicturesInMap(task, indoor, nullptr, nullptr, 0, offset))
    {
      work[task].mapOffsetY[account] = offset;
      return 1;
    }
    //判断是否处于关卡内
    if (CheckHeadColor(task, 0, offset))
    {
      work[task].mapOffsetY[account] = offset;
      return 2;
    }
    //判断是否处于继续作战弹窗状态
    if (CheckHeadColor(task, 1, offset))
    {
      work[task].mapOffsetY[account] = offset;
      return 2;
    }
  }
  return 0;
}
//【任务线程】开始任务task
DWORD __stdcall StartTask(void *vpTask)
{
  int task = (int)vpTask;//获取任务序号
  int entry = 0, box = 0;//关卡进入状态（1=关卡内，2=加载中，0=其他）；弹窗（1=继续作战，2=掉线，0=无）
  char message[100] = {};
  if (reserve[task].gamesFinished == 0) //如果是从第1局开始，则清空战利品数量
    memset(reserve[task].lootNum, 0, sizeof(reserve[task].lootNum));
  RecordStartTime(task);//记录任务启动时间
  work[task].wave = waveOutOfLevel;//每局开始前，波数为“未进入关卡”
  ReturnState(task, "运行中");//进度games/MaxGames，第waveOutOfLevel波第0秒，平均用时刷新，累计时间刷新
  if (IsAdvance(task)) //高级任务首次启动需要绑定Param.hWnd窗口（此时会检查窗口是否显示）
  {
    int list = GetList(task);
    //定时、退服、关机：不用刷新和绑定游戏窗口
    if (IsTimer(*plot[list].playingAdvance) || IsExitServer(*plot[list].playingAdvance) || IsShutdown(*plot[list].playingAdvance))
      EnterControl(task, plot[list].playingAdvance->level);
    else
    {
      //刷新任务
      if (IsRefresh(*plot[list].playingAdvance))
        plot[list].isRefreshRequired = 1;
      if (plot[list].isRefreshRequired == 1) //如果高级任务需要刷新（上轮中断时触发）
      {
        reserve[task].arriveRefresh = true;
        if (plot[list].playingAdvance->host == 2) //如果是同步任务，要等另一个任务一起刷新
        {
          int anotherTask = IsDual2P(task) ? task - 1 : task + 1;//另一个任务编号
          while (reserve[anotherTask].arriveRefresh == false) //等另一个任务也到达
            CheckSleep(task, 10);
        }
        plot[list].isRefreshRequired = 0;//取消“需要刷新”标记
        Refresh(task);//刷新游戏并更新param.hWnd和work.hWnd
      }
      AdvanceBindWindow(task);//首次执行时绑定param.hWnd。绑定失败将刷新游戏并获取param.hWnd。保证在可跳转界面。
    }
  }
  LoadTrackAndBindWindow(task);//读取轨道、绑定work.hWnd和DPI修正（此后才能用Click进行点击）、切换到放卡账号

  //启动前检验一下放卡账号有没有显示
  if (!isPerformedAccountVisible(task))
  {
    PopMessage(task, "请分窗口显示每个放卡账号。\n显示后任务将继续进行。");
    while (!isPerformedAccountVisible(task))
      CheckSleep(task, 500);
  }

  //AutoSaveProp(task, 0);

  //UpdateMap(task, 0);
  //SaveBanner(task);

  if (IsAdvance(task)) //高级任务：进入关卡（如果不在竞技岛，会跳转至竞技岛）
    EnterLevel(task);//非轨道关卡会直接在这里结束

  /*开始循环刷图*/
  int skillSlotNum = 0;//刷技能模式的卡槽数，只在第一局读取
  //从已完成局数+1开始刷
  for (work[task].games = reserve[task].gamesFinished + 1; work[task].games < INT_MAX; work[task].games++)
  {
    if (IsAdvance(task)) //更新已完成局数
      plot[GetList(task)].playingAdvance->gamesFinished = work[task].games - 1;
    ResetData(task);//重置读第0波轨道，重置种植次数、索敌卡位置
    ResetWave(task, waveOutOfLevel);//每局开始前，波数为“未进入关卡”
    if (work[task].games > work[task].maxGames)//刷完指定局数，结束任务
      FinishTask(task);
    RecordLevelTime(task, 0);//记录每局用时
    ReturnState(task, "运行中");//进度games/MaxGames，第waveOutOfLevel波第0秒，平均用时刷新，累计时间刷新

    if (IsTowerTask(task)) //魔塔选层
      EnterTowerLevel(task);
    else if (IsPeakTask(task))
      EnterPeakLevel(task);

    int wave = -1;
    bool isEnterRequired = true;//是否需要进入关卡

    //校准画面偏移
    int indoorState[2] = { -1, -1 };
    bool gameStateGotten = false;
    for (int account = 0; account < 2; account++)
      if (work[task].isInvolved[account] && IsGameWindowVisible(work[task].hWnd[account]))
      {
        work[task].mapOffsetY[account] = 0;//偏移预设为0
        ForcedUpdateMap(task, account);
        indoorState[account] = CheckOffsetIndoor(task, account);//校准偏移
        //获取关卡状态（只从一个号获取）
        if (!gameStateGotten)
          if (GetGameState(task, account, &entry, &wave, &box))
            gameStateGotten = true;
      }

    //根据房间状态执行操作
    for (int account = 0; account < 2; account++)
      if (indoorState[account] != -1)
      {
        //已进入关卡（包括继续作战弹窗时）：跳过选卡、准备、开始阶段
        if (indoorState[account] == 2)
          isEnterRequired = false;
        //处于选卡界面：需要走带卡、准备开始程序
        else if (indoorState[account] == 1)
        {
          isEnterRequired = true;
          work[task].banner = GetBanner(task);//记录漫游类型
        }
        else //其他情况：报错
          ReportError(task, account, "未进入房间", "未进入房间");
        break;
      }

    for (int account = 0; account < 2; account++)
      if (work[task].isPerformed[account])
        ReloadWave(task, account, 0);//重新读取第0波轨道

    if (isEnterRequired) //正常进入关卡，需要执行选卡、准备、开始、截取初始地图和填写强制棉麦格子
    {
      if (IsAdvance(task)) //高级任务根据VIP等级设置自动收集
        for (int account = 0; account < 2; account++)
          if (work[task].isInvolved[account])
          {
            ForcedUpdateMap(task, account);
            int vip = GetVip(task, account);//获取本账号的VIP等级
            if (vip >= 7)
              work[task].isCollect[account] = 0;//V7以上无需自动收集
            else if (vip >= 4)
              work[task].isCollect[account] = 1;//V4以上只需收集物品
            else
              work[task].isCollect[account] = 2;//否则全部都要收集
          }
      LoadCustomCore(task);//载入自定卡槽核心
      ChooseDeck(task);//选择卡组
      //清除联合体中的自定卡槽核心（此后用于储存颜色哈希表）
      memset(work[task].custom, 0, sizeof(work[task].custom));
      ReadyAndStart(task, entry);//准备和开始（第2局开始，另一个号可能还没翻牌完）
      work[task].isGridCatched = 0;
    }
    else //中途进入关卡，沿用初始地图和强制棉麦格子
    {
      work[task].isGridCatched = 1;
      //清除联合体中的战利品（此后用于储存颜色哈希表）
      memset(work[task].loot, 0, sizeof(work[task].loot));
    }

    work[task].totalWaveTick[0] = GetTickCount();//记录进入关卡时刻（第0波时刻）
    for (int account = 0; account < 2; account++)//两个账号的计时起点均设为进入关卡时刻
      for (int order = 0; order <= maxSlotNum + 1; order++)
      {
        work[task].slot[account][order].startTick = work[task].totalWaveTick[0];
        work[task].slot[account][order].avaliableTick = work[task].totalWaveTick[0] - 1000;
      }

    //刷技能模式 brushSkill
    if (work[task].isSkillMode)
    {
      outSkillNum = skillSlotNum;
      WriteTracePrint(task);
      ResetWave(task, 0);
      work[task].currentTime = 0;
      ReturnState(task, "运行中");//每局第一次记录：进度games/MaxGames，第0波第1秒，第1局平均用时0，累计时间已有

      int &host = work[task].host;
      if (skillSlotNum == 0)
      {
        UpdateMap(task, host);
        GetSlotNum(task, host);
        skillSlotNum = min(work[task].realSlotNum[host], work[task].slotNum[host]);
      }

      CheckSleep(task, 200);//开局等200ms
      SelectiveSleep(task, 200);
      LayRole(task, host);//放人物
      bool isUpgradeExist = false;//是否存在升级层卡片
      for (int order = 1; order <= skillSlotNum; order++)
      {
        Slot &slot = work[task].slot[host][order];
        if (slot.level == 4)//如果是升级层，进行记录，稍后放置
          isUpgradeExist = true;
        else if (slot.row[1] > 0) //如果有安排位置
          PlantToGrid(task, host, order, slot.row[1], slot.column[1], 1);//立即放置
      }

      if (isUpgradeExist) //如果有升级卡
      {
        CheckSleep(task, 200);//等0.3秒
        SelectiveSleep(task, 200);
        for (int order = 1; order <= skillSlotNum; order++)
        {
          Slot &slot = work[task].slot[host][order];
          if (slot.level == 4 && slot.row[1] > 0)
            PlantToGrid(task, host, order, slot.row[1], slot.column[1], 1);//立即放置
        }
      }
      QuitLevel(task, host);//放完退出
      ReturnStart(task);//回到开始界面
      continue;//本局结束
    }

    int firstPerformedAccount = GetFirstPerformedAccount(task);//第1个放卡账号
    //普通模式等1秒进关放人物
    for (int account = 0; account < 2; account++)//人物挪到1行1列
      if (work[task].isInvolved[account] == 1)
        Touch(task, account, 1, -1);
    CheckSleep(task, 300);
    //截图，获取卡槽数、火苗、卡片模板背景、初始前线
    for (int account = 0; account < 2; account++)
      if (work[task].isPerformed[account] == 1)
      {
        //棉麦开局标记“模板已截取”
        for (int order = 1; order <= work[task].slotNum[account]; order++)
          if (work[task].slot[account][order].isMaltose)
            work[task].isTemplateCatched[account][order] = 1;
        if (UpdateMap(task, account)) //从放卡账号截图
        {
          //记录初始地图格子（用于棉麦补阵）
          if (account == firstPerformedAccount)
          {
            if (!work[task].isGridCatched)
              LoadInnateGrid(task);
            work[task].isGridCatched = 1;
          }
          //记录卡片模板背景
          CopyMap(work[task].background[account], work[task].map, wideTemplateX, wideTemplateY);
          work[task].isBackgroundCatched[account] = 1;
        }
      }
    for (int account = 0; account < 2; account++)//放置参与的账号的人物
      if (work[task].isInvolved[account] == 1)
        LayRole(task, account);

    //如果是从房间进入关卡，所有参与账号加速，加速时间为两号设置的加速时间的最大值
    if (isEnterRequired)
    {
      int accelerationTime = 0;
      for (int account = 0; account < 2; account++)
        if (work[task].isPerformed[account])
          accelerationTime = max(accelerationTime, work[task].accelerationTime[account]);
      if (accelerationTime)
      {
        Accelerate(task);//开启加速
        CheckSleep(task, accelerationTime);
        Accelerate(task);//关闭加速
      }
    }

    CheckSleep(task, 1000 - (GetTickCount() - work[task].totalWaveTick[0]));//等足1秒
    work[task].currentTime = 1;
    ResetWave(task, 0);

    //开局静置
    for (int i = 0; i < work[task].restTime; i++)
    {
      ReturnState(task, "运行中");
      CheckSleep(task, 1000);
      work[task].currentTime++;
    }

    /*---------------------开始放卡---------------------*/

    while (true) //每秒1个循环
    {
      if (work[task].gameTime == zero)//识图模式下读取波数并按波数操作
      {
        bool isImagePicked = false;//是否识图成功
        bool isProceedPopped = false;//是否弹出继续作战
        for (int account = 0; account < 2; account++)
          if (work[task].isPerformed[account] && !work[task].isQuitted[account])
          {
            //读取关卡状态（不修改波次），读取成功则记录识图成功
            if (GetGameState(task, account, &entry, NULL, &box))
            {
              isImagePicked = true;
              if (box == 1) //记录弹出了继续作战框
                isProceedPopped = true;
            }
            if (box == 2) //如果检测到掉线蓝框
            {
              if (account == work[task].host) //如果是用房主识的图，说明掉线的是房主
                ReportError(task, account, "房主已掉线", "房主已掉线");
              else
                ReportError(task, account, "队友已掉线", "队友已掉线");
            }
          }
        if (isProceedPopped) //如果出现继续作战框，点击继续作战或领取奖励
          for (int account = 0; account < 2; account++)
          {
            if (work[task].isProceed[account]) //继续作战
              Click(task, account, proceed);
            else if (work[task].currentTime >= 92)//领取奖励（需要时间达到1分30秒）
              Click(task, account, claim);
          }
        if (!isImagePicked) //如果没有一个号能识别到图像，则进行无图像处理（非识图模式不会处理）
          NoImageProcess(task);

        WriteTracePrint(task);//调试输出卡组冷却状态和出怪行信息

        //如果所有参与账号都已检测到结束波次，则执行结束程序
        if (IsLevelEnded(task))
        {
          //战利品或结算界面：停止放卡，等待进入翻牌或开始界面。
          if (work[task].wave == waveLoot || work[task].wave == waveGrade)
          {
            bool isAccelerationRequired = work[task].isAccelerationRequired;
            for (int account = 0; account < 2; account++)
              if (work[task].isInvolved[account] && !IsGameWindowVisible(work[task].hWnd[account]))
                isAccelerationRequired = false; //所有参与账号都可见才加速

            if (isAccelerationRequired) //开启加速
              Accelerate(task);

            //等待回到翻牌界面或关卡外
            int counter = 0;
            const int secondCount = 5;//每秒计数
            while (work[task].wave != waveFlip && !IsOutOfLevel(work[task].wave))
            {
              ReturnState(task, "运行中");//进度games/MaxGames，第Wave波第CurrentTime秒，平均用时刷新，累计时间刷新
              counter++;
              if (counter > 20 * secondCount)//20秒未完成结算算超时
                ReportError0(task, "结算超时", "结算超时");
              CheckSleep(task, 1000 / secondCount);
              if (counter % secondCount == 0)
                work[task].currentTime++;
              ReadLevelInfo(task);
            }

            if (isAccelerationRequired) //关闭加速
              Accelerate(task);
          }

          //选卡或魔塔选层界面：进入ReturnStart函数
          if (IsOutOfLevel(work[task].wave))
            break;

          //翻牌界面：显示翻牌状态，进入ReturnStart函数
          if (work[task].wave == waveFlip)
          {
            ReturnState(task, "运行中");//进度games/MaxGames，第Wave波第CurrentTime秒，平均用时不变，累计时间刷新
            break;//进入ReturnStart函数：双号翻牌直到回到选卡界面
          }
        }
        else //如果关卡未结束
        {
          //如果距离一个账号结束超过30秒则报错
          if (work[task].oneAccountEndTick && GetTickCount() - work[task].oneAccountEndTick > 30000)
            ReportError0(task, "两号结束不同步", "两号结束时间相差超过30秒");
        }
      }
      else//如果不是识图模式
      {
        //每一秒都要点击继续作战或领取奖励
        for (int account = 0; account < 2; account++)
        {
          if (work[task].isProceed[account])
            Click(task, account, proceed);
          else
            Click(task, account, claim);
        }
        if (work[task].currentTime >= work[task].gameTime)//运行时间超过GameTime则进入下一局
          break;
      }
      ReturnState(task, "运行中");//进度games/MaxGames，第Wave波第CurrentTime秒，平均用时不变，累计时间刷新

      /*物品火苗收集程序*/
      int ColumnsCollected[2] = { 0, 0 };//收集列数：不收集为0，收集物品为9，收集火苗为11
      for (int account = 0; account < 2; account++)
      {
        if (work[task].isInvolved[account] && !work[task].isQuitted[account]) //仅参与的账号执行收集
        {
          if (work[task].isCollect[account] == 2)
            ColumnsCollected[account] = 11; //收集火苗：每秒执行一次，收集1-11列
          else if (work[task].isCollect[account] == 1 && work[task].currentTime % 3 == zero)
            ColumnsCollected[account] = 9; //收集物品：每3秒执行一次，收集1-9列
        }
      }

      //顺序：账号1收集第1行->账号2收集第1行->等待1 ms
      for (int AutoRow = 1; AutoRow <= 7; AutoRow++)
      {
        for (int account = 0; account < 2; account++)
          for (int AutoColumn = 1; AutoColumn <= ColumnsCollected[account]; AutoColumn++)
            Touch(task, account, AutoRow, AutoColumn);
        if (ColumnsCollected[0] > 0 || ColumnsCollected[1] > 0) //有账号执行收集，才等待1 ms
          Sleep(1);
      }

      //1000ms结束前持续放卡
      while (GetTickCount() - work[task].totalWaveTick[0] < DWORD(work[task].currentTime + 1) * 1000)
      {
        CheckPlant(task);//检查并放置防御卡
        CheckSleep(task, 1);
      }

      if (work[task].gameTime != zero)//如果不是识图模式，进行翻牌
      {
        if (work[task].currentTime % 2 == 0)
        {
          ClickFree(task, 1, flipNum);
          ClickFree(task, 0, flipNum);
        }
        else
        {
          Click(task, 1, finish);
          Click(task, 0, finish);
        }
      }
      work[task].currentTime++;//计时器+1
      if (IsAdvance(task) && plot[GetList(task)].playingAdvance->type == 3)
      {
        if (work[task].currentTime >= 360) //计时超过6分钟
        {
          sprintf_s(message, "跨服6分钟未结束（第%d.%d波）", work[task].wave, work[task].smallWave);
          ReportError0(task, "跨服超时", message);
        }
      }
      else
      {
        if (work[task].currentTime >= 1800) //计时超过30分钟
        {
          sprintf_s(message, "关卡30分钟未结束（第%d.%d波）", work[task].wave, work[task].smallWave);
          ReportError0(task, "关卡超时", message);
        }
      }

      //高级任务设置了退出时间时，执行退出
      if (IsAdvance(task))
        for (int account = 0; account < 2; account++)
          if (work[task].isInvolved[account] && !work[task].isQuitted[account])
            if (work[task].quitTime[account] > 0 && work[task].currentTime >= work[task].quitTime[account])
              QuitLevel(task, account);

      bool isAllAccountQuitted = true;
      for (int account = 0; account < 2; account++)
        if (work[task].isInvolved[account] && !work[task].isQuitted[account])
          isAllAccountQuitted = false;
      if (isAllAccountQuitted) //都退出了，则进入ReturnStart过程
        break;
    }
    ReturnStart(task);//显示的号返回选卡界面（这样两个号都显示就会零延迟）
  }
  return 0;
}
//删除高级任务
void DeleteAdvance(int list, int order)
{
  for (int i = order; i < plot[list].advanceNum; i++)
    plot[list].advance[i] = plot[list].advance[i + 1];
  memset(&plot[list].advance[plot[list].advanceNum], 0, sizeof(plot[list].advance[plot[list].advanceNum]));
  plot[list].advanceNum--;
}
//将高级任务order复制到最后
void CopyAdvance(int list, int order)
{
  plot[list].advance[plot[list].advanceNum++] = plot[list].advance[order];
}
//把任务参数保存到文件
void SaveParameter()
{
  WaitForSingleObject(hMutexSaveParameter, INFINITE);

  FILE *f;
  if (!fopen_s(&f, "用户参数\\执行器参数.txt", "w"))//打开文件
  {
    //写入公共参数
    fprintf(f, "缩放比例=%d\n", zoom);
    fprintf(f, "放卡延迟=%d\n", plantDelay);
    fprintf(f, "带卡选择=%d\n", customOption);
    fprintf(f, "翻牌数量=%d\n", flipNum);
    fprintf(f, "背包满时=%d\n", bagFullOperation);
    fprintf(f, "经验满时=%d\n", expFullOperation);
    fprintf(f, "补阵强度=%d\n", repairLevel);
    fprintf(f, "自动还原=%d\n", missionRecovery);
    fprintf(f, "自动刷新=%d\n", maxRefreshTimes);
    fprintf(f, "操作速度=%d\n", operationSpeed);
    fprintf(f, "加速时间=%d\n", globalAccelarationTime);

    //写入高级任务参数
    for (int list = 0; list < listNum; list++)
    {
      fprintf(f, "--高级任务%d--\n", list);
      fprintf(f, "1P服务器=%d\n", plot[list].server[0]);
      fprintf(f, "1P二级密码=%s\n", plot[list].password[0]);
      fprintf(f, "2P服务器=%d\n", plot[list].server[1]);
      fprintf(f, "2P二级密码=%s\n", plot[list].password[1]);
    }
    //写入普通任务参数
    for (int task = 0; task < maxTaskNum; task++)
    {
      fprintf(f, "--普通任务%d--\n", task);
      fprintf(f, "任务标题=%s\n", param[task].title);
      for (int account = 0; account < 2; account++)
        fprintf(f, "%dP轨道=%s\n", account + 1, param[task].track[account]);
      fprintf(f, "执行局数=%d\n", param[task].maxGames);
      fprintf(f, "每局用时=%d\n", param[task].gameTime);
      fprintf(f, "继续作战=%d,%d\n", param[task].isProceed[0], param[task].isProceed[1]);
      fprintf(f, "自动收集=%d,%d\n", param[task].isCollect[0], param[task].isCollect[1]);
      fprintf(f, "使用卡组=%d,%d\n", param[task].deck[0], param[task].deck[1]);
      for (int account = 0; account < 2; account++)
        fprintf(f, "%dP标签=%d,%d\n", account + 1, param[task].tag[account].x, param[task].tag[account].y);
      for (int account = 0; account < 2; account++)
        fprintf(f, "%dP大厅=%s\n", account + 1, param[task].hallName[account]);
      fprintf(f, "初始大厅=%s\n", param[task].mainName);
      for (int account = 0; account < 2; account++)
      {
        HideInfo &hideInfo = param[task].hideInfo[account];
        fprintf(f, "%dP隐藏信息=%d,%d,%d,%d,%d,%d\n", account + 1,
          hideInfo.hidden ? 1 : 0, hideInfo.maximized ? 1 : 0,
          hideInfo.rect.left, hideInfo.rect.top, hideInfo.rect.right, hideInfo.rect.bottom);
      }
    }
    fclose(f);
  }

  ReleaseMutex(hMutexSaveParameter);
}
const int maxBackupNum = 256;
char backupList[maxBackupNum][maxPath];
//清理所有自动备份
void ClearBackup()
{
  int directNum = GetFileList("自动备份\\任务列表\\*", backupList, maxBackupNum);
  for (int order = 0; order < directNum; order++)
  {
    char direct[maxPath];
    sprintf_s(direct, "自动备份\\任务列表\\%s\\", backupList[order]);
    DeleteFolder(direct);
  }
}
//清理本次执行的自动备份
void ClearThisBackup()
{
  char direct[maxPath];
  for (int list = 0; list < listNum; list++)
  {
    sprintf_s(direct, "自动备份\\任务列表\\%s\\", plot[list].backupFolder);
    DeleteFolder(direct);
  }
}
const int maxLogNum = 256;
char logList[maxLogNum][maxPath];
//清理执行记录
void ClearLog()
{
  int logNum = GetFileList("执行记录\\*", logList, maxLogNum);

  time_t currentTime = time(nullptr);//当前时间
  for (int order = 0; order < logNum; order++)
  {
    tm tmRecordTime = {};//本条记录时间结构体
    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    sscanf_s(logList[order], "%d.%d.%d-%d.%d.%d", &year, &month, &day, &hour, &minute, &second);
    tmRecordTime.tm_year = year - 1900;  // 年份从1900年开始
    tmRecordTime.tm_mon = month - 1;     // 月份从1月开始
    tmRecordTime.tm_mday = day;          // 日
    tmRecordTime.tm_hour = hour;         // 时
    tmRecordTime.tm_min = minute;        // 分
    tmRecordTime.tm_sec = second;        // 秒
    tmRecordTime.tm_isdst = -1;          // 是否夏令时，设为-1让系统自动判断
    time_t recordTime = mktime(&tmRecordTime);//本条记录时间转化为timt_t

    if (currentTime - recordTime >= time_t(3 * 86400)) //相差3天以上则删除
    {
      char logFolder[maxPath];
      sprintf_s(logFolder, "执行记录\\%s\\", logList[order]);
      DeleteFolder(logFolder);
    }
  }
}
//清理备份和执行记录
void ClearBackupAndLog()
{
  ClearBackup();
  ClearLog();
}
//载入某一个备份
void LoadBackup(int list, int offset)
{
  char path[50];
  plot[list].currentBackup = plot[list].currentBackup + offset;
  sprintf_s(path, "自动备份\\任务列表\\%s\\%d.txt", plot[list].backupFolder, plot[list].currentBackup);
  LoadListFrom(list, path, ADVANCE, 0);
  SaveListTo(list, plot[list].advancePath, ADVANCE, 0);
  int TotalAdvancePage = plot[list].advanceNum / advanceNumPerPage + 1;
  if (plot[list].advancePage > TotalAdvancePage)
    plot[list].advancePage = TotalAdvancePage;
}
int IsUndoAllowed(int list)
{
  return plot[list].currentBackup > 1;
}
int IsRepeatAllowed(int list)
{
  return plot[list].currentBackup < plot[list].backupNum;
}
//撤销操作
void Undo(int list)
{
  LoadBackup(list, -1);
}
//恢复操作
void Repeat(int list)
{
  LoadBackup(list, 1);
}
//传递高级任务advance的参数(房主、局数、轨道、卡组、钥匙)到param[task]，每局用时填0
void GetParamFromAdvance(int task, Advance &advance)
{
  if (advance.host < 2) //单线程任务
  {
    //同步任务2P无需使用，把maxGames设为0阻止任务启动即可
    if (IsDual2P(task))
    {
      param[task].maxGames = 0;
      return;
    }
    param[task].host = advance.host;
    for (int account = 0; account < 2; account++)
    {
      if (advance.deck[account] == 0) //如果卡组为0，代表这个账号不启用
        strcpy_s(param[task].track[account], "");
      else
        strcpy_s(param[task].track[account], advance.track[account]);
      param[task].deck[account] = advance.deck[account];
    }
  }
  else //同步任务中的一支
  {
    param[task].host = IsDual2P(task) ? 1 : 0;//同步任务2P房主为2P，同步任务1P房主为1P
    if (IsDual1P(task)) //同步任务1P：1P填写advance的1P轨道和卡组，2P留空
    {
      strcpy_s(param[task].track[0], advance.track[0]);
      param[task].deck[0] = advance.deck[0];
      strcpy_s(param[task].track[1], "");
      param[task].deck[1] = 0;
    }
    else //同步任务2P：1P留空，2P填写advance的2P轨道和卡组
    {
      strcpy_s(param[task].track[0], "");
      param[task].deck[0] = 0;
      strcpy_s(param[task].track[1], advance.track[1]);
      param[task].deck[1] = advance.deck[1];
    }
  }
  //两类任务共同填写的参数
  for (int account = 0; account < 2; account++)
    param[task].isProceed[account] = advance.isProceed;
  //不需要轨道的任务（公会任务、签到施肥）局数为1
  param[task].maxGames = IsNoGames(advance) ? 1 : advance.maxGames;
  param[task].gameTime = 0;
  param[task].realTimer = advance.realTimer;
}
//检查某条任务advance是否合格，不合格返回0写入checkInfo。
//1P参与返回1，2P参与返回2，都参与返回3（返回值用于确定高级任务参与人数）
int CheckQuest(int list, Advance &advance, char(&checkInfo)[1000], bool *pIsSkillMode = nullptr)
{
  int task = GetTask(list);
  if (pIsSkillMode)
    *pIsSkillMode = false;
  GetParamFromAdvance(task, advance);
  GetParamFromAdvance(task + 1, advance);

  char info[2][1000] = {};//检查轨道文件时输出的错误信息
  int checkResult[2] = {};//轨道检查结果
  int trackNum = 0;//有效轨道数量
  int isInvolved[2] = {};//两个账号是否参与
  int isPerformed[2] = {};//两个账号是否放卡

  int workingTask[2] = { task, task };//1P轨道和2P轨道所属任务
  if (advance.host == 2) //同步任务2P属于任务6，单线程任务1P和2P都属于任务5
    workingTask[1] = task + 1;

  //检查填写的轨道
  work[task].targetNum = 0;
  for (int account = 0; account < 2; account++)
    if (strlen(advance.track[account]) > 0 && advance.deck[account] > 0)
    {
      checkResult[account] = LoadTrack(workingTask[account], account, info[account]);//载入并检查轨道
      isInvolved[account] = isPerformed[account] = 1;//该账号参与且放卡
      trackNum++;
    }
    else //轨道不存在，检查结果记为-1
      checkResult[account] = -1;

  //两类任务共同机制：轨道出错则提示
  if (trackNum == 0)//两个轨道都没填
    strcpy_s(checkInfo, "请至少填写一个轨道");
  else if (checkResult[0] == 0)//如果轨道1无法正常载入，弹窗提示
    strcpy_s(checkInfo, info[0]);
  else if (checkResult[1] == 0)//如果轨道2无法正常载入，弹窗提示
    strcpy_s(checkInfo, info[1]);
  else //如果轨道无误，则根据是否为同步任务分类讨论
  {
    if (advance.host == 2) //同步任务：双轨双卡组，且不为双人魔塔则通过
    {
      bool hasTwoTracks = true; //advance是否为双轨双卡组
      for (int account = 0; account < 2; account++)
        if (strlen(advance.track[account]) == 0 || advance.deck[account] == 0)
          hasTwoTracks = false;

      if (!hasTwoTracks) //不是双轨双卡组
        strcpy_s(checkInfo, "同步任务1P和2P都要填写轨道。");
      else if (advance.type == 2 && advance.level / 1000 == 2) //双塔
        strcpy_s(checkInfo, "双人魔塔不能设置为同步任务。");
      else if (IsMission(advance)) //识别任务
        sprintf_s(checkInfo, "%s任务不能设置为同步任务。", missionName[advance.level]);
      else if (work[task].isSkillMode && !IsSingleTowerTask(task)) //刷技能轨道不在单塔执行
        strcpy_s(checkInfo, "刷技能轨道只能用于单塔或密室。");
      else
        return 3;//检验通过，为双人参与
    }
    else //非同步任务
    {
      //是否有账号卡组为×
      bool isDeckCanceled = advance.deck[0] == 0 || advance.deck[1] == 0;
      //单轨且没有账号卡组为×时，检查是否为双人轨道
      if (trackNum == 1 && !isDeckCanceled)
        for (int account = 0; account < 2; account++)
          if (isPerformed[account] && GetTrackRoleNum(task, account) == 2)
            isInvolved[0] = isInvolved[1] = 1;

      //房主不参与而队友参与是不允许的
      if (isInvolved[advance.host] == 0 && isInvolved[1 - advance.host] == 1)
      {
        if (advance.deck[advance.host] == 0) //如果是把房主卡组设为了×
          strcpy_s(checkInfo, "房主必须参与任务");
        else //如果没有设×，而是轨道为单人
          strcpy_s(checkInfo, "队友使用了单人轨道，房主未参与。");
      }
      else if (advance.type == 4 && advance.level == 4 && isInvolved[1 - advance.host])
        strcpy_s(checkInfo, "巅峰对决必须单人参与。");
      else if (advance.type == 2 && advance.level / 1000 == 1 && isInvolved[1 - advance.host])
        strcpy_s(checkInfo, "单塔必须单人参与。");
      else if (advance.type == 2 && advance.level / 1000 == 2 && !isInvolved[1 - advance.host])
        strcpy_s(checkInfo, "双塔必须双人参与。");
      else if (advance.type == 2 && advance.level / 1000 == 3 && isInvolved[1 - advance.host])
        strcpy_s(checkInfo, "宠塔必须单人参与。");
      else if (advance.type == 2 && advance.level / 1000 == 4 && isInvolved[1 - advance.host])
        strcpy_s(checkInfo, "密室必须单人参与。");
      else if (work[task].isSkillMode && !IsSingleTowerTask(task)) //刷技能轨道不在单塔执行
        strcpy_s(checkInfo, "刷技能轨道只能用于单塔或密室。");
      else //轨道合格
      {
        if (isInvolved[1 - advance.host] == 1) //如果队友参与
          return 3;//轨道检查合格，为双人参与
        else //如果仅房主参与
        {
          if (work[task].isSkillMode) //记录刷技能模式
            if (pIsSkillMode)
              *pIsSkillMode = true;
          return advance.host + 1;//轨道检查合格，仅房主单人参与
        }
      }
    }
  }
  return 0;//轨道检查不合格
}
//列表检查信息
struct ListInfo
{
  int involved[2];
};
//检查高级任务列表，合格返回-1，不合格返回出错的order并填写错误信息
//记录参与情况和三大识别任务的编号
int CheckList(int list, char(&checkInfo)[1000], ListInfo(&listInfo), int target, int issue,
  bool &skillExist, bool &shutdownExist)
{
  Advance *advance = GetAdvancePointer(list, target, issue);//任务列表
  int &advanceNum = GetAdvanceNumRef(list, target, issue);//任务数量

  skillExist = false;
  shutdownExist = false;
  listInfo.involved[0] = listInfo.involved[1] = 0;
  int missionCount[3] = {};//3种识别任务出现的次数
  int checkResult[maxAdvanceNum] = {};
  bool questExist = false;
  bool cycleExist = false;
  for (int order = 0; order < advanceNum; order++)
    if (advance[order].isSelected)//遍历所有勾选的高级任务
    {
      if (cycleExist)
      {
        sprintf_s(checkInfo, "“循环”必须是最后一项任务。");
        return order;
      }
      if (IsCycle(advance[order])) //循环任务
      {
        if (!questExist) //前面没有任务报错
        {
          sprintf_s(checkInfo, "“循环”不能是第一项任务。");
          return order;
        }
        cycleExist = true;
      }
      if (IsShutdown(advance[order]))
        shutdownExist = true;
      questExist = true;
      //双倍卡任务：局数不能超过6
      if (IsDoubleCardQuest(advance[order]))
      {
        if (advance[order].maxGames > 6)
        {
          sprintf_s(checkInfo, "双倍卡使用数量不能超过6。");
          return order;
        }
      }

      //如果是识别任务
      if (IsMission(advance[order]))
      {
        //1. 预制列表中不能嵌套识别任务
        if (target != ADVANCE)
        {
          sprintf_s(checkInfo, "%s任务列表中不允许嵌套%s任务。", missionName[target - 1],
            missionName[advance[order].level]);
          return order;
        }
        //2. 同种识别任务只允许出现一次
        missionCount[advance[order].level]++;//该识别任务出现的次数+1
        if (missionCount[advance[order].level] > 1)
        {
          sprintf_s(checkInfo, "%s任务只允许出现一次。", missionName[advance[order].level]);
          return order;
        }
        //3. 识别任务不允许设置为“同步”（大赛伪同步除外）
        if (advance[order].host == 2) //如果房主设置为“同步”
        {
          sprintf_s(checkInfo, "%s任务不能设置为同步任务。", missionName[advance[order].level]);
          return order;
        }
        //4. 大赛任务必须双号参与
        if (IsContest(advance[order]) && (advance[order].deck[0] == 0 || advance[order].deck[1] == 0))
        {
          strcpy_s(checkInfo, "大赛需要双人参与，因为有双人任务。");
          return order;
        }
        //5. 识别任务房主必须参与（非识别任务会在CheckQuest中检查）
        if (advance[order].deck[advance[order].host] == 0) //如果把房主卡组设为了×
        {
          strcpy_s(checkInfo, "房主必须参与任务。");
          return order;
        }
      }

      //无轨任务
      if (IsNoTrack(advance[order]))
      {
        //对于填写了卡组的号
        for (int account = 0; account < 2; account++)
          if (advance[order].deck[account])
          {
            if (!IsNoDeck(advance[order])) //需要卡组的号才计算参与
              listInfo.involved[account] = 1;
          }
        continue; //不再进行其他检查
      }

      plot[list].playingOrder = order;//更新plot[list].playingOrder，检查承载方案时需要使用
      plot[list].playingAdvance = &advance[order];//更新PlayingAdvance

      bool isSkillMode = false;
      checkResult[order] = CheckQuest(list, advance[order], checkInfo, &isSkillMode);//检查任务
      if (checkResult[order] == 0)//不合格则返回出错的order
        return order;
      if (checkResult[order] == 1 || checkResult[order] == 3)
        listInfo.involved[0] = 1;
      if (checkResult[order] == 2 || checkResult[order] == 3)
        listInfo.involved[1] = 1;
      if (isSkillMode)
        skillExist = true;
    }
  return -1;
}
//删除识别任务level的所有子任务
void ReturnMission(int list, int level)
{
  for (int order = plot[list].advanceNum - 1; order >= 0; order--)
    if (plot[list].advance[order].missionStyle == level + 1)
      DeleteAdvance(list, order);
}
//如果列表中勾选了某项识别任务，则删除它的所有子任务
void ReturnCheckedMission(int list)
{
  for (int level = 0; level < 3; level++)
    if (GetMissionOrder(list, level) != -1)
      ReturnMission(list, level);
}
//删除所有子任务
void ReturnMission(int list)
{
  for (int level = 0; level < 3; level++)
    ReturnMission(list, level);
}
//识别任务真同步转假同步
void SetFalseDualContest(int list)
{
  for (int order = 0; order < plot[list].advanceNum; order++)
    if (plot[list].advance[order].isSelected && IsContest(plot[list].advance[order])
      && plot[list].advance[order].host == 2)
    {
      plot[list].advance[order].host = 0;
      plot[list].advance[order].dual = true;
    }
}
//识别任务假同步转真同步
void SetTrueDualContest(int list)
{
  for (int order = 0; order < plot[list].advanceNum; order++)
    if (plot[list].advance[order].isSelected && IsContest(plot[list].advance[order])
      && plot[list].advance[order].dual)
    {
      plot[list].advance[order].host = 2;
      plot[list].advance[order].dual = false;
    }
}
//结束高级任务并提示message（exitList：是否弹窗并结束线程）
void StopAdvance(int list, const char *message = nullptr, bool exitList = true)
{
  int task = GetTask(list);
  if (exitList)
  {
    plot[list].isAdvanceStarted = 0;//高级任务停止
    plot[list].isAdvanceStarted_Confirm = 0;//确认高级任务停止
  }
  //将高级任务对应的两个普通任务都刹停
  for (int iTask = task; iTask <= task + 1; iTask++)
  {
    isTaskStarted[iTask] = 0;
    isTaskStarted_Confirm[iTask] = 0;
  }
  SetTrueDualContest(list);//识别任务假同步转真同步
  if (missionRecovery == 1)
    ReturnCheckedMission(list);//还原识别任务
  SaveList(list);//保存以便撤销
  SaveParameter();
  isRepaintRequired = 1;//发送重绘指令

  if (exitList && message)
    PopMessage(task, message);//弹窗提示
  plot[list].isChecking = false;//检查结束，取消标记
  if (exitList)
    ExitThread(0);//高级任务结束
}
//检查列表，合格返回-1，有刷技能模式返回-2，不合格报错停止
int CheckListAndReport(int list, char(&checkInfo)[1000], ListInfo(&listInfo), int target, int issue, bool &skillExist, bool &shutdownExist)
{
  Advance *advance = GetAdvancePointer(list, target, issue);//任务列表
  int &advanceNum = GetAdvanceNumRef(list, target, issue);

  char listName[100] = {};//预制列表名称

  char message[1000] = {};
  //如果是预制列表，先检查各任务
  if (target != ADVANCE)
  {
    if (target == CONTEST)
      sprintf_s(listName, "美食大赛_%s期", contestName[issue]);
    else
      strcpy_s(listName, fullMissionName[target - 1]);

    int sequenceSize = GetSequenceSize(target, issue);//顺序表任务数量
    Contest *sequence = GetSequence(target, issue);
    //检查任务数量
    if (sequenceSize != advanceNum)
    {
      sprintf_s(message, "预制列表出错：%s\n任务数量不匹配。\n请重新安装以修复问题。", listName);
      StopAdvance(list, message);
    }
    //逐条检查任务
    for (int order = 0; order < advanceNum; order++)
    {
      if (target == CONTEST && !advance[order].isSelected)
      {
        sprintf_s(message, "预制列表出错：%s\nID%d 任务未勾选。\n大赛任务不能取消勾选。", listName, order + 1);
        StopAdvance(list, message);
      }
      if (sequence[order].type != advance[order].type || sequence[order].level != advance[order].level)
      {
        sprintf_s(message, "预制列表出错：%s\nID%d 关卡不匹配。\n请重新安装以修复问题。", listName, order + 1);
        StopAdvance(list, message);
      }
    }
  }

  //检查列表全部任务，返回出错编号
  int errorOrder = CheckList(list, checkInfo, listInfo, target, issue, skillExist, shutdownExist);
  if (errorOrder >= 0)//如果检查出错
  {
    char levelString[100];
    GetLevelName(levelString, advance[errorOrder].type, advance[errorOrder].level);
    if (target == ADVANCE)
      sprintf_s(message, "[%d] %s(%s)\n%s", errorOrder + 1, typeName[advance[errorOrder].type],
        levelString, checkInfo);//填写提示信息
    else
      sprintf_s(message, "%s [%d] %s(%s)\n%s", listName, errorOrder + 1, typeName[advance[errorOrder].type],
        levelString, checkInfo);//填写提示信息
    StopAdvance(list, message);
  }
  return errorOrder;
}
//任务是否完成（未开始算作完成）
bool IsTaskFinished(int task)
{
  if (isTaskStarted_Confirm[task] == 1) //任务正在运行，返回false
    return false;
  return strcmp(state[task].tip, "未开始") == 0 || strcmp(state[task].tip, "已完成") == 0;
}
//任务是否中断
bool IsTaskBroken(int task)
{
  if (isTaskStarted_Confirm[task] == 1) //任务正在运行，返回false
    return false;
  return !IsTaskFinished(task);//任务不在运行，不是完成就是中断
}
//执行列表list的任务order，等到StartTask线程结束才会返回
int StartQuest(int list, int order)
{
  int task = GetTask(list);
  plot[list].playingOrder = order; //记录正在执行的任务编号
  plot[list].playingAdvance = &plot[list].advance[order];//更新PlayingAdvance

  //将高级任务参数（局数、房主、轨道、卡组）填写到param[task]
  for (int iTask = task; iTask <= task + 1; iTask++)
  {
    GetParamFromAdvance(iTask, plot[list].advance[order]);//获取param[task]
    reserve[iTask].gamesFinished = 0;//已完成局数设为0
    ResetState(iTask);//任务状态重置为“未开始”
  }

  char questString[100] = {};//任务字符串（如"[2] 美味(神殿)"）
  char message[100] = {};//输出信息
  int refreshLevel = 0;//刷新等级
  char errorString[100] = {};//刷新原因

  while (true)//循环执行任务，如果意外中断则继续循环，正常中断/完成则跳出循环
  {
    //检查param，若能启动则传递参数到work，不能则报错退出（实际上不会触发）
    for (int iTask = task; iTask <= task + 1; iTask++)
      if (IsParamRunnable(iTask, plot[list].advance[order].host == 2) == 0)
        StopAdvance(list, "任务检查不通过。");

    //两个任务均已刷满则结束
    if (work[task].games > work[task].maxGames && work[task + 1].games > work[task + 1].maxGames)
      return 1;
    //否则视为任务正在进行

    //新增输出信息：进行中
    GetQuestString(questString, list, order);
    sprintf_s(message, "%s 进行中...", questString);
    sprintf_s(plot[list].logQuestString, "%s 进行中 %d/%d", questString,
      work[task].games - 1, work[task].maxGames);
    AddOutput(list, message);

    plot[list].zoneCounter++;//每次执行高级任务前，换区计数器+1
    if (listStyle == CAMP) //营地模式必须去营地
      while (plot[list].randomZone[plot[list].zoneCounter % 7] > 1)
        plot[list].zoneCounter++;
    else //常规列表避开关卡所在区（打星际不能去星际区）
      while (IsTypeAndZoneEqual(plot[list].advance[order].type, plot[list].randomZone[plot[list].zoneCounter % 7]))
        plot[list].zoneCounter++;

    //为未刷满的任务创建任务线程
    for (int iTask = task; iTask <= task + 1; iTask++)
      if (work[iTask].games <= work[iTask].maxGames)
      {
        reserve[iTask].arriveRefresh = false;//任务还未到达刷新判定处
        reserve[iTask].arriveFinish = false;//任务还未到达FinishTask
        isTaskStarted[iTask] = 1;//记录任务开始
        isTaskStarted_Confirm[iTask] = 1;
        CreateThread(NULL, 0, StartTask, (void *)iTask, 0, NULL);//启动任务线程
      }
      else //不运行的任务arrive设为true，另一个任务不用等待
      {
        reserve[iTask].arriveRefresh = true;
        reserve[iTask].arriveFinish = true;
      }

    //有任务运行时，继续等待（若其中一个任务中断则刹停）
    while (isTaskStarted_Confirm[task] == 1 || isTaskStarted_Confirm[task + 1] == 1)
    {
      int brokenTask = -1;//中断的任务编号（-1表示无中断）
      for (int iTask = task; iTask <= task + 1; iTask++) //记录中断的任务
        if (IsTaskBroken(iTask))
          brokenTask = iTask;
      if (brokenTask >= 0) //若一个任务中断，刹停另一个任务
      {
        int runningTask = brokenTask == task ? task + 1 : task;//另一个正在运行的任务
        isTaskStarted[runningTask] = 0; //刹停任务
        while (isTaskStarted_Confirm[runningTask] == 1)//等待任务结束
          Sleep(50);
        break;//结束等待
      }
      Sleep(50);
    }
    Sleep(50);//退出while后再等50ms

    //两个任务是否均已完成
    bool isQuestFinished = IsTaskFinished(task) && IsTaskFinished(task + 1);

    //从刷新等级最高的任务记录刷新等级和刷新原因
    if (state[task].refreshLevel >= state[task + 1].refreshLevel)
    {
      refreshLevel = state[task].refreshLevel;
      strcpy_s(errorString, state[task].errorString);
    }
    else
    {
      refreshLevel = state[task + 1].refreshLevel;
      strcpy_s(errorString, state[task + 1].errorString);
    }

    //手动刹停/任务完成/END等级中断：结束本条任务且不需要刷新
    if (plot[list].isAdvanceStarted == 0 || isQuestFinished || refreshLevel == END_TASK)
      break;

    //否则属于RETRY或SKIP等级中断，在下一次StartTask开头刷新
    plot[list].isRefreshRequired = 1;
    //修改输出信息：仅完成
    if (IsMission(plot[list].advance[order]) || IsControl(plot[list].advance[order]))
      sprintf_s(plot[list].logQuestString, "%s 未完成", questString);
    else
      sprintf_s(plot[list].logQuestString, "%s 仅完成 %d/%d", questString,
        work[task].games - 1, work[task].maxGames);
    strcpy_s(plot[list].output[plot[list].outputNum - 1], plot[list].logQuestString);
    RecordError(task, false);//更新中断日志
    //新增输出信息：刷新原因
    Truncate(message, errorString, titleWidth + parameterWidth - 12);
    AddOutput(list, message);

    //跳过模式（2）或刷新次数已满：结束本条任务
    if (refreshLevel == SKIP_TASK || plot[list].timesRefreshed[order] >= maxRefreshTimes)
      break;

    //否则继续完成本条任务剩余局数，记录已刷新次数+1
    GetLogFinished(list);//记录已完成的任务日志
    for (int iTask = task; iTask <= task + 1; iTask++)
      reserve[iTask].gamesFinished = work[iTask].games - 1;//已完成的局数
    plot[list].timesRefreshed[order]++;//已刷新次数+1
  }

  //结束本条任务的几种情况：正常完成/手动停止/END/SKIP
  if (strcmp(state[task].tip, "已完成") == 0) //1. 正常完成（局数打满且不触发中断）
  {
    if (plot[list].advance[order].result == 2) //未完成：仅限签到施肥
      sprintf_s(plot[list].logQuestString, "%s 仅完成 0/1", questString);
    else if (plot[list].advance[order].result == 1) //未领奖：仅限悬赏
      sprintf_s(plot[list].logQuestString, "%s 未领奖 %d/%d", questString,
        work[task].maxGames, work[task].maxGames);
    else //已完成
    {
      if (IsMission(plot[list].advance[order]) || IsControl(plot[list].advance[order]))
        sprintf_s(plot[list].logQuestString, "%s 已完成", questString);
      else
        sprintf_s(plot[list].logQuestString, "%s 已完成 %d/%d", questString,
          work[task].maxGames, work[task].maxGames);//修改输出信息：已完成
    }
    strcpy_s(plot[list].output[plot[list].outputNum - 1], plot[list].logQuestString);
    RecordLevelTime(task, 2);//更新已完成日志
  }
  else if (plot[list].isAdvanceStarted == 0) //2. 手动停止
  {
    //修改输出信息：已停止
    if (IsMission(plot[list].advance[order]) || IsControl(plot[list].advance[order]))
      sprintf_s(plot[list].logQuestString, "%s 已停止", questString);
    else
      sprintf_s(plot[list].logQuestString, "%s 已停止 %d/%d",
        questString, min(work[task].games, work[task].maxGames), work[task].maxGames);
    strcpy_s(plot[list].output[plot[list].outputNum - 1], plot[list].logQuestString);
    //不用更新日志
  }
  else if (refreshLevel == END_TASK) //3. END_TASK
  {
    //修改输出信息：已中断
    if (IsMission(plot[list].advance[order]) || IsControl(plot[list].advance[order]))
      sprintf_s(plot[list].logQuestString, "%s 已中断", questString);
    else
      sprintf_s(plot[list].logQuestString, "%s 已中断 %d/%d", questString,
        min(work[task].games, work[task].maxGames), work[task].maxGames);
    strcpy_s(plot[list].output[plot[list].outputNum - 1], plot[list].logQuestString);
    RecordError(task, false);//更新中断日志
    plot[list].isAdvanceStarted = 0; //主动刹停高级任务
  }
  //4. SKIP_TASK（上面已经改过输出信息了）

  GetLogFinished(list);//记录已完成的任务日志
  isRepaintRequired = 1;//发送重绘指令
  return 1;//如果没有刹停高级任务，则继续执行下一条任务
}
//高级任务结束，报告未完成的任务。exitList：是否退出线程（仅循环模式为false）
void FinishAdvance(int list, bool exitList)
{
  char failureTip[2000] = "高级任务结束，以下任务未完成：";
  const char resultString[3][100] = { "已完成", "未领奖", "未完成" };
  char questString[100] = {};//任务字符串
  char questFailureTip[100] = {};//单项任务未完成提示
  bool isAnyAdvanceFailed = false;//是否有未完成的任务
  //逐条检查高级任务
  for (int order = 0; order < plot[list].advanceNum; order++)
    if (plot[list].advance[order].isSelected)
    {
      GetQuestString(questString, list, order);
      //非轨道任务，根据result提示（目前只有result==2 仅完成0/1 这一种情况）
      if (IsNoTrack(plot[list].advance[order]))
      {
        if (plot[list].advance[order].result == 2)
        {
          sprintf_s(questFailureTip, "\n%s 仅完成 0/1", questString);
          strcat_s(failureTip, questFailureTip);
          isAnyAdvanceFailed = true;
        }
      }
      //轨道任务，如果局数没打满，提示仅完成；否则只可能result==1，提示未领奖
      else
      {
        if (plot[list].advance[order].gamesFinished < plot[list].advance[order].maxGames)
        {
          sprintf_s(questFailureTip, "\n%s 仅完成 %d/%d", questString,
            plot[list].advance[order].gamesFinished, plot[list].advance[order].maxGames);
          strcat_s(failureTip, questFailureTip);
          isAnyAdvanceFailed = true;
        }
        else if (plot[list].advance[order].result == 1)
        {
          sprintf_s(questFailureTip, "\n%s 未领奖", questString);
          strcat_s(failureTip, questFailureTip);
          isAnyAdvanceFailed = true;
        }
      }
    }

  //检查公会、情侣、日常任务是否领奖失败
  if (plot[list].isGuildClaimFailed || plot[list].isLoverClaimFailed || plot[list].isDailyClaimFailed)
  {
    strcpy_s(questFailureTip, "\n");

    if (plot[list].isGuildClaimFailed)
      strcat_s(questFailureTip, "公会、");
    else if (plot[list].isLoverClaimFailed)
      strcat_s(questFailureTip, "情侣、");
    else if (plot[list].isDailyClaimFailed)
      strcat_s(questFailureTip, "日常、");
    int length = strlen(questFailureTip);
    questFailureTip[length - 2] = 0; //删去一个顿号
    strcat_s(questFailureTip, "任务 未领奖");
    strcat_s(failureTip, questFailureTip);
    isAnyAdvanceFailed = true;
  }

  char message[2000] = {}; //高级任务结束提示
  if (isAnyAdvanceFailed)
    strcpy_s(message, failureTip);
  else
    strcpy_s(message, "高级任务已全部完成。");

  //记录日志
  FILE *f;
  int task = GetTask(list);
  sprintf_s(work[task].logPath, "%s\\任务日志.txt", plot[list].advanceFolderPath);//填写日志文件路径
  if (CreateLogFile(task, &f, "高级任务完成", false)) ///创建日志文件
  {
    fprintf(f, "%s", plot[list].logFinished);//填写已执行完毕的任务日志
    fprintf(f, "----------------------------------------\n");
    fprintf(f, "%s\n", message);//填写高级任务结束提示
    fclose(f);
  }

  StopAdvance(list, message, exitList);
}
void ExchangeAdvance(int list, int order1, int order2)
{
  if (order1 == order2)
    return;
  Advance Temp = plot[list].advance[order1];
  plot[list].advance[order1] = plot[list].advance[order2];
  plot[list].advance[order2] = Temp;
}
//将高级任务order1插入到order2位置，其他任务顺序保持
void InsertAdvance(int list, int order1, int order2)
{
  if (order1 == order2)
    return;
  Advance Temp = plot[list].advance[order1];
  if (order1 > order2) //向前插入
    for (int i = order1; i >= order2 + 1; i--)
      plot[list].advance[i] = plot[list].advance[i - 1];
  else if (order1 < order2) //向后插入
    for (int i = order1; i <= order2 - 1; i++)
      plot[list].advance[i] = plot[list].advance[i + 1];
  plot[list].advance[order2] = Temp;
}
//向ban卡前缀添加ban卡项
void AddBanItem(char(&banPrefix)[maxPath], const char *banItem)
{
  //如果banPrefix已经有内容了，则要加逗号
  if (strlen(banPrefix) > 1)
    strcat_s(banPrefix, ",");
  strcat_s(banPrefix, banItem);
}
//根据contest中的卡槽、禁卡、放卡要求添加<>前缀
//返回值：0检查通过 -1无轨道或打不开 1-7要求不满足
int FillBannedTrack(int task, int account, int host, Contest &contest)
{
  //0. 读取轨道，获得卡槽信息（包括哪些卡未使用）
  if (strlen(param[task].track[account]) == 0) //空轨道视为合格。2P空代表单人任务，1P空已提前报错
    return 0;
  char trackInfo[1000] = {};
  if (LoadTrack(task, account, trackInfo) == 0) //轨道无法打开，返回-1
    return -1;
  //单人大赛的2P轨道能打开就够了，无需添加<>指令，也无需检查能否完成要求
  if (host != 2 && account == 1)
  {
    //如果为空轨道，直接删除
    if (IsEmptyTrack(param[task].track[account]))
      strcpy_s(param[task].track[account], "");
    return 0;
  }

  //1. 把要求禁用的卡ban了
  bool isBanRequired = false;//是否需要使用ban卡前缀
  bool isCupRequired = false;//是否需要小火换酒杯
  bool isSlotBanned[maxSlotNum + 1] = {};
  for (int i = 0; i < contestSlotNum; i++)
    if (contest.slotBanned[i])
    {
      int order = GetOrder(task, account, contestSlot[i]);
      if (order > 0) //如果有要禁的卡，前7张直接ban，其他卡必须无轨才能ban
        if (i < 7 || !work[task].slot[account][order].used)
        {
          if (strcmp(contestSlot[i], "小火炉") == 0) //禁小火要换上酒杯灯
            isCupRequired = true;
          isBanRequired = true;
          isSlotBanned[order] = true;
        }
    }

  //2. 如果有卡槽数限制，把除咖啡粉外的所有空卡槽ban了
  if (contest.slotNum > 0)
  {
    //咖啡粉是否可以卸除：夜晚关卡或金枪鱼_3槽这个特例可以卸除
    bool coffeeBanned = strstr(param[task].track[account], "夜") || strstr(param[task].track[account], "0金枪鱼_槽3");
    isBanRequired = true;
    for (int order = 1; order <= work[task].slotNum[account]; order++)
      if (!work[task].slot[account][order].used &&
        (coffeeBanned || strcmp(work[task].slot[account][order].name, "咖啡粉") != 0))
        isSlotBanned[order] = true;
  }

  //3. 如果需放卡或静置，也要使用ban卡前缀
  for (int i = 0; i < contestSlotNum; i++)
    if (contest.slotPlanted[i])
      isBanRequired = true;
  if (contest.restTime)
    isBanRequired = true;

  char bannedTrack[maxPath] = {};//带<>指令的轨道

  //4. 如果需要使用ban卡前缀，填写ban卡前缀
  if (isBanRequired)
  {
    strcpy_s(bannedTrack, "<");//完整路径以"<"开头
    char banItem[10] = {};//一个ban卡项，如"2酒杯灯"

    //加入要ban的卡
    int bannedNum = 0;//已ban数量
    for (int order = 1; order <= work[task].slotNum[account]; order++)
      if (isSlotBanned[order])
      {
        if (isCupRequired && strcmp(work[task].slot[account][order].name, "小火炉") == 0)
          sprintf_s(banItem, "%d酒杯灯", order); //小火换酒杯
        else
          sprintf_s(banItem, "%d", order); //其他直接ban
        AddBanItem(bannedTrack, banItem);
        bannedNum++;
      }
    //加入要放的卡
    for (int i = 0; i < contestSlotNum; i++)
      if (contest.slotPlanted[i])
        AddBanItem(bannedTrack, contestSlot[i]);
    //加入静置时间
    if (contest.restTime)
    {
      sprintf_s(banItem, "-%d", contest.restTime);
      AddBanItem(bannedTrack, banItem);
    }

    strcat_s(bannedTrack, ">");
  }
  strcat_s(bannedTrack, param[task].track[account]);//填写完整轨道

  //5. 把带指令的轨道路径写入param，再检查一遍能否正常打开
  strcpy_s(param[task].track[account], bannedTrack);
  if (LoadTrack(task, account, trackInfo) == 0) //轨道无法打开，直接返回空串
    return -1;

  //如果轨道为空轨道且不含<>指令，直接删除轨道
  if (!isBanRequired && IsEmptyTrack(param[task].track[account]))
    strcpy_s(param[task].track[account], "");

  //6. 条件检查：1星级 2平均星级 3限放 4放置酒瓶（检查词条）5卡槽 6禁卡（检查轨道）
  char requirement[100] = {};
  //6.1 星级
  if (contest.star)
  {
    sprintf_s(requirement, "_星%d", contest.star);
    if (!strstr(param[task].track[account], requirement))
      return 1;
  }
  //6.2 平均星级
  if (contest.averageStar)
  {
    sprintf_s(requirement, "_均%d", contest.averageStar);
    if (!strstr(param[task].track[account], requirement))
      return 2;
  }
  //6.3 限放
  if (contest.cardNum)
  {
    sprintf_s(requirement, "_限%d", contest.cardNum);
    if (!strstr(param[task].track[account], requirement))
      return 3;
  }
  //6.4 放置酒瓶
  if (contest.bottleTimes)
  {
    sprintf_s(requirement, "_瓶%d", contest.bottleTimes);
    if (!strstr(param[task].track[account], requirement))
      return 4;
  }
  //6.5 卡槽数
  if (contest.slotNum > 0)
  {
    int realSlotNum = 0;//ban完后的卡槽数
    //没ban的卡都要统计
    for (int order = 1; order <= work[task].originalSlotNum[account]; order++)
      if (!isSlotBanned[order])
        realSlotNum++;
    //如果有小火换酒杯的设定，实际卡槽数还要+1
    if (isCupRequired)
      realSlotNum++;
    //超出卡槽限制，返回错误编号3
    if (realSlotNum > contest.slotNum)
      return 5;
  }
  //6.6 禁卡
  for (int i = 0; i < contestSlotNum; i++)
    if (contest.slotBanned[i])
    {
      int order = GetOrder(task, account, contestSlot[i]);
      //要求禁的卡在轨道中且没禁，返回错误编号2
      if (order > 0 && !isSlotBanned[order])
        return 6;
    }
  return 0;
}
//轨道路径是否有效
bool IsTrackEffective(const char *pureTrack)
{
  //1. 空轨道视为有效
  if (strlen(pureTrack) == 0 || IsEmptyTrack(pureTrack))
    return true;
  //2. 轨道存在视为有效
  if (FileExist(pureTrack))
    return true;
  //3. 轨道不存在时，判断是否为可转化的++轨道
  char trackP[maxPath] = {};//"+.txt"轨道
  return IsTrackPPEffective(pureTrack, trackP);
}
//根据Contest生成一条高级任务（log：是否记录轨道到大赛日志）
//返回值：0=成功，-1=无法添加或轨道无效，1=无法完成任务要求
int ContestToAdvance(int list, Contest &contest, Advance &advance, int host, int(&deck)[2], bool log)
{
  memset(&advance, 0, sizeof(advance));
  advance.isSelected = 1;
  advance.type = contest.type;
  advance.level = contest.level;
  advance.maxGames = 1;
  advance.isProceed = contest.boss == 1 ? 0 : 1;
  advance.missionStyle = CONTEST;//设为大赛风格
  memcpy(advance.deck, deck, sizeof(deck));

  //轨道选择规则：特制＞通用＞空
  char specialTrack[3][maxPath] = {};//特制轨道（不能用<>实现的要求才会有特制轨道）
  char commonTrack[3][maxPath] = {};//通用轨道
  const char emptyTrack[3][maxPath] = { "", "+", "++" };//空轨道
  char chosenTrack[3][maxPath] = {};//选择的轨道
  char levelName[100];//关卡名称
  GetLevelName(levelName, contest.type, contest.level);//获取关卡名称

  //1. 填写特殊轨道
  //1.1 填写所需词条（必须特制：星级 平均星级 限放 酒瓶；可能特制：卡槽 禁卡 放卡）
  char requirement[100] = {};//特殊要求词条
  if (contest.star) //星级
    sprintf_s(requirement, "星%d", contest.star);
  else if (contest.averageStar) //平均星级
    sprintf_s(requirement, "均%d", contest.averageStar);
  else if (contest.cardNum) //限放
    sprintf_s(requirement, "限%d", contest.cardNum);
  else if (contest.bottleTimes) //放置酒瓶
    sprintf_s(requirement, "瓶%d", contest.bottleTimes);
  else if (contest.slotNum)
    sprintf_s(requirement, "槽%d", contest.slotNum);
  else
  {
    for (int i = 0; i < contestSlotNum; i++)
      if (contest.slotBanned[i])
      {
        sprintf_s(requirement, "禁%s", contestSlot[i]);
        break;
      }
    if (strlen(requirement) == 0)
      for (int i = 0; i < contestSlotNum; i++)
        if (contest.slotPlanted[i])
        {
          sprintf_s(requirement, "放%s", contestSlot[i]);
          break;
        }
    if (strlen(requirement) == 0)
      if (contest.roleNum == 1)
        sprintf_s(requirement, "单");
  }

  //1.2 填写特殊轨道
  for (int account = 0; account < 3; account++)
    sprintf_s(specialTrack[account], "预制轨道\\美食大赛\\%d%s\\%d%s_%s%s.txt",
      contest.type, typeName[contest.type], contest.level, levelName, requirement, emptyTrack[account]);

  //2. 填写通用轨道
  for (int account = 0; account < 3; account++)
    sprintf_s(commonTrack[account], "预制轨道\\通用轨道\\%d%s\\%d%s%s.txt",
      contest.type, typeName[contest.type], contest.level, levelName, emptyTrack[account]);

  //3. 按优先级选择轨道：特制>通用>空
  for (int account = 0; account < 3; account++)
    if (IsTrackEffective(specialTrack[account]))
      strcpy_s(chosenTrack[account], specialTrack[account]);
    else if (IsTrackEffective(commonTrack[account]))
      strcpy_s(chosenTrack[account], commonTrack[account]);
    else
      strcpy_s(chosenTrack[account], emptyTrack[account]);
  //如果没有1P轨道，大赛无法执行，返回-1
  if (strlen(chosenTrack[0]) == 0)
    return -1;

  //4. 向param填入1P和2P轨道
  int task = GetTask(list);
  strcpy_s(param[task].track[0], chosenTrack[0]);//1P轨道填track[0]
  if (contest.roleNum == 1) //单人任务：2P轨道留空
    strcpy_s(param[task].track[1], "");
  else if (host == 2) //同步大赛中的双人任务：2P轨道填track[2]
    strcpy_s(param[task].track[1], chosenTrack[2]);
  else //单人大赛中的双人任务：2P轨道填track[1]
    strcpy_s(param[task].track[1], chosenTrack[1]);

  //5. 检查param[task].track并根据要求添加<>指令（单人大赛的2P轨道只检查能否打开）
  int trackResult[2] = {};//轨道检查结果
  int result = 0;//0=任务合格，-1=轨道不存在或无效，1=无法完成要求
  for (int account = 0; account < 2; account++)
  {
    trackResult[account] = FillBannedTrack(task, account, host, contest);
    if (trackResult[account] == -1) //轨道打不开，结果为-1
      result = -1;
    else if (trackResult[account] > 0) //轨道无法完成要求，结果为1
    {
      if (result != -1)
        result = 1;
    }
  }

  //6. 将轨道和检查结果写入日志
  if (log)
  {
    char output[4][3][maxPath] = {};//3特殊+3通用+3选择+2最终
    //6.1 填入特殊、通用、选择轨道
    for (int account = 0; account < 3; account++)
    {
      strcpy_s(output[0][account], specialTrack[account]);
      strcpy_s(output[1][account], commonTrack[account]);
      strcpy_s(output[2][account], chosenTrack[account]);
      for (int i = 0; i < 3; i++) //不存在的轨道添加"（无）"
      {
        if (strlen(output[i][account]) == 0 || !IsTrackEffective(output[i][account]))
          strcat_s(output[i][account], "（无）");
        //如果轨道路径以"预制轨道\"开头，删除"预制轨道\"
        if (strstr(output[i][account], "预制轨道\\") == output[i][account])
          DeleteString(output[i][account], 9);
      }
    }
    //6.2 填入最终轨道
    const char trackResultString[4][10] = { "（√）", "（放）", "（禁）", "（槽）" };//检查结果对应的符号
    for (int account = 0; account < 2; account++)
    {
      strcpy_s(output[3][account], param[task].track[account]);
      if (trackResult[account] >= 0)
        strcat_s(output[3][account], trackResultString[trackResult[account]]);
    }

    //6.3 向文件中写入4种轨道
    FILE *f;
    if (!fopen_s(&f, plot[list].contestLog, "a"))
    {
      const char trackStyle[4][10] = { "特殊", "通用", "选择", "生成" };//4种轨道类型
      for (int i = 0; i < 4; i++)
      {
        fprintf(f, "%s\t", trackStyle[i]);
        for (int account = 0; account < 3; account++)
          fprintf(f, "%s\t", output[i][account]);
        fprintf(f, "\n");
      }
      if (result == 0)
        fprintf(f, "轨道合格\n\n");
      else if (result == -1)
        fprintf(f, "轨道无效\n\n");
      else
        fprintf(f, "轨道不满足要求\n\n");
      fclose(f);
    }
  }

  //7. 根据大赛人数和本条任务人数分配房主和轨道
  if (host == 2) //同步大赛
  {
    //双人任务：分配房主轨道和同步队友轨道
    if (contest.roleNum == 2)
    {
      advance.host = 0; //房主为1P
      for (int account = 0; account < 2; account++)
        strcpy_s(advance.track[account], param[task].track[account]);
    }
    else //单人任务：分配同步轨道
    {
      advance.host = 2; //房主为同步
      for (int account = 0; account < 2; account++)
        strcpy_s(advance.track[account], param[task].track[0]);
    }
  }
  else //单人大赛
  {
    advance.host = host;
    strcpy_s(advance.track[advance.host], param[task].track[0]);
    strcpy_s(advance.track[1 - advance.host], param[task].track[1]);
    if (contest.roleNum == 1) //单人任务：队友卡组设为×
      advance.deck[1 - advance.host] = 0;
  }
  return result;
}
//切换放卡账号：交换房主、轨道、卡组
void ExchangePerformer(int list, int order)
{
  //交换轨道
  SwapStr(plot[list].advance[order].track[0], plot[list].advance[order].track[1]);
  //交换卡组
  Swap(&plot[list].advance[order].deck[0], &plot[list].advance[order].deck[1]);
  //交换房主（如果是同步则不交换）
  if (plot[list].advance[order].host < 2)
    plot[list].advance[order].host = 1 - plot[list].advance[order].host;
}
//根据Contest向列表中添加一项大赛任务（deck：大赛卡组；log：是否记录轨道到大赛日志）
//返回值：0成功 -1无法添加 1已添加但不合格
int AddContestQuest(int list, Contest &contest, int host, int(&deck)[2], bool log = false)
{
  AddAdvance(list, contest.type, contest.level, 1, deck[0], deck[1]);//添加任务到列表末尾
  Advance &advance = plot[list].advance[plot[list].advanceNum - 1];//最后一条任务
  //如果定位成功，直接拷贝预制列表中的方案，但标记为大赛
  if (plot[list].isLocated)
  {
    //拷贝预制列表中的任务
    memcpy(&advance, &plot[list].contestAdvance[plot[list].issue][plot[list].step], sizeof(advance));
    //如果房主是2P，将这条任务的房主变更，轨道和卡组交换
    if (host == 1)
      ExchangePerformer(list, plot[list].advanceNum - 1);
    //若本条大赛任务卡组不为0，则进行卡组运算
    for (int account = 0; account < 2; account++)
      if (advance.deck[account] > 0)
        advance.deck[account] = 1 + (advance.deck[account] + deck[account] - 2) % 6;
    advance.missionStyle = CONTEST;
    return 0;
  }
  //如果定位失败，根据任务要求填写轨道
  return ContestToAdvance(list, contest, advance, host, deck, log);
  //ContestToAdvance函数中已经考虑了2P单人大赛的情形，无需在此调整
}
//向列表中插入最优大赛任务，记录插入前最后一个大赛子任务位置
//返回值：0成功 -1无法插入任务 1已插入但不合格
int InsertContest(int list, int &lastSubcontestOrder, bool updateFile, bool log)
{
  //大赛位置
  int contestOrder = GetMissionOrder(list, CONTEST - 1);
  //最后一个大赛子任务位置
  lastSubcontestOrder = GetLastSubcontestOrder(list, contestOrder);

  if (plot[list].bestContest.code == 0)//无需插入则退出
    return 0;

  //将最优任务插入到insertOrder+1
  Contest &contest = plot[list].bestContest;
  int host = plot[list].advance[contestOrder].dual ? 2 : plot[list].advance[contestOrder].host;

  //在列表末尾添加任务（log决定是否记录日志）
  int result = AddContestQuest(list, contest, host, plot[list].advance[contestOrder].deck, log);
  InsertAdvance(list, plot[list].advanceNum - 1, lastSubcontestOrder + 1);//插入到正确位置

  if (updateFile)
  {
    SaveList(list);//由于列表已改动，需要保存一次并生成备份
    SaveParameter();
    isRepaintRequired = 1;//发送重绘指令
  }
  return result;
}
//向列表中插入识别到的任务
bool InsertMission(int list, int level)
{
  //1. 判断识别任务的位置
  int missionOrder = -1;
  for (int order = 0; order < plot[list].advanceNum; order++)
    if (plot[list].advance[order].isSelected && plot[list].advance[order].type == 5
      && plot[list].advance[order].level == level)
    {
      missionOrder = order;
      break;
    }
  if (missionOrder == -1) //没有识别任务则结束
    return false;
  //记录识别任务的房主和卡组
  int missionHost = plot[list].advance[missionOrder].host;
  int missionDeck[2] = {};
  memcpy(missionDeck, plot[list].advance[missionOrder].deck, sizeof(missionDeck));

  //2. 遍历missionExist数组，记录要插入的任务
  Advance *missionAdvance = level == 0 ? plot[list].guildAdvance : plot[list].loverAdvance;
  bool *missionExist = level == 0 ? plot[list].guildExist : plot[list].loverExist;
  int startOrder = level == 0 ? 2 : 0;//公会任务从ID2开始查（跳过浇水施肥）
  int todayMission[20] = {};//今日任务编号
  int todayMissionNum = 0;//今日任务数量
  //记录存在的任务编号
  for (int i = startOrder; i < maxAdvanceNum; i++)
    if (missionExist[i])
      todayMission[todayMissionNum++] = i;

  //3. 插入任务
  if (todayMissionNum > 0)
  {
    //3.1. missionOrder后面的任务后移todayMissionNum格子
    for (int i = plot[list].advanceNum - 1; i > missionOrder; i--)
      plot[list].advance[i + todayMissionNum] = plot[list].advance[i];

    //3.2. missionOrder + 1至missionOrder + todayGuildNum用预制列表中的任务替换
    for (int i = 0; i < todayMissionNum; i++)
    {
      Advance &advance = plot[list].advance[missionOrder + 1 + i];
      advance = missionAdvance[todayMission[i]];//拷贝任务
      advance.missionStyle = level + 1;//修改任务标记
      advance.host = missionHost;//修改房主
      //如果【任务-公会】为2P单人参与，则交换1P与2P轨道与卡组
      if (missionHost == 1 && missionDeck[0] == 0)
      {
        SwapStr(advance.track[0], advance.track[1]);
        Swap(&advance.deck[0], &advance.deck[1]);
      }
      //如果【任务-公会】为2P双人参与，则只把房主设为2P，轨道和卡组沿用

      //卡组执行加法运算
      for (int account = 0; account < 2; account++)
      {
        if (missionDeck[account] > 0)
          advance.deck[account] = 1 + (advance.deck[account] + missionDeck[account] - 2) % 6;
        else
          advance.deck[account] = 0;
      }
    }
    plot[list].advanceNum += todayMissionNum;//任务总数增加
  }
  SaveList(list);//由于列表已改动，需要保存一次并生成备份
  SaveParameter();
  isRepaintRequired = 1;//发送重绘指令
  return true;
}
//执行任务-大赛前的准备工作
void ResetContest(int list)
{
  plot[list].issue = 0;//大赛预设为第0期
  //清空累计未完成的大赛任务
  plot[list].totalFailedSize = 0;
  memset(plot[list].totalFailedContest, 0, sizeof(plot[list].totalFailedContest));
  //清空本轮未完成的大赛任务
  plot[list].failedSize = 0;
  memset(plot[list].failedContest, 0, sizeof(plot[list].failedContest));
  //清空本轮应完成的大赛任务
  plot[list].requiredSize = 0;
  memset(plot[list].requiredContest, 0, sizeof(plot[list].requiredContest));
}
//检查当前列表、公会列表和情侣列表，不合格报错停止
void CheckAllListAndReport(int list, char(&checkInfo)[1000], ListInfo &listInfo,
  bool &skillExist, bool &shutdownExist)
{
  //检查前的准备工作

  //1. 删除勾选的识别任务对应的子任务（需要保存和重绘）
  ReturnCheckedMission(list);
  SaveList(list);
  SaveParameter();
  isRepaintRequired = 1;

  //2. 大赛识别任务真同步转假同步（不做这步过不了检查）
  SetFalseDualContest(list);

  //检查高级任务列表
  plot[list].isCountingSlot = true;//标记正在检查当前列表（用于统计用到的防御卡）
  memset(plot[list].usedSlotNum, 0, sizeof(plot[list].usedSlotNum));
  CheckListAndReport(list, checkInfo, listInfo, ADVANCE, 0, skillExist, shutdownExist);
  plot[list].isCountingSlot = false;//取消检查标记

  for (int account = 0; account < 2; account++)
    plot[list].advanceInvolved[account] = listInfo.involved[account];

  ListInfo tempListInfo = {};
  bool tempSkillExist = false, tempShutdownExist = false;

  //如果存在公会任务，载入并检查公会任务列表
  if (GetMissionOrder(list, GUILD - 1) >= 0)
  {
    //载入公会任务列表，如果载入失败则中断任务
    if (LoadListFrom(list, "预制列表\\公会任务.txt", GUILD, 0) == 0)
      StopAdvance(list, "载入公会任务列表失败。");
    //检查公会任务列表
    CheckListAndReport(list, checkInfo, tempListInfo, GUILD, 0, tempSkillExist, tempShutdownExist);
  }

  //如果存在情侣任务，载入并检查情侣任务列表
  if (GetMissionOrder(list, LOVER - 1) >= 0)
  {
    //载入情侣任务列表，如果载入失败则中断任务
    if (LoadListFrom(list, "预制列表\\情侣任务.txt", LOVER, 0) == 0)
      StopAdvance(list, "载入情侣任务列表失败。");
    //检查情侣任务列表
    CheckListAndReport(list, checkInfo, tempListInfo, LOVER, 0, tempSkillExist, tempShutdownExist);
  }

  //如果存在大赛任务，载入并检查大赛任务列表
  int contestOrder = GetMissionOrder(list, CONTEST - 1);//任务-大赛编号
  if (contestOrder >= 0)
  {
    char contestListName[maxPath] = {};
    char contestListPath[maxPath] = {};
    for (int issue = 0; issue < tournamentNum; issue++)
    {
      sprintf_s(contestListName, "美食大赛_%s期_%s.txt", contestName[issue],
        plot[list].advance[contestOrder].dual ? "双人" : "单人");
      sprintf_s(contestListPath, "预制列表\\%s", contestListName);
      //载入美食大赛列表，如果载入失败则中断任务
      if (LoadListFrom(list, contestListPath, CONTEST, issue) == 0)
      {
        char message[100 + maxPath] = {};
        sprintf_s(message, "载入预制列表失败：\n%s", contestListName);
        StopAdvance(list, message);
      }
      //检查美食大赛列表
      CheckListAndReport(list, checkInfo, tempListInfo, CONTEST, issue, tempSkillExist, tempShutdownExist);
    }
  }
}
//【任务线程】开始高级任务序列
DWORD __stdcall StartList(void *vpList)
{
  int list = (int)vpList;
  int task = GetTask(list);
  int cycleTimes = 0;//循环次数（只有第1次有启动提示）

restart:
  cycleTimes++;
  plot[list].isGuildClaimFailed = false;
  plot[list].isLoverClaimFailed = false;
  plot[list].isDailyClaimFailed = false;
  plot[list].isLevelInsufficient = false;//等级不足预设为false

  time_t lastTimer = time(nullptr);//上一个定时时刻（最初设为启动时刻）
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    plot[list].advance[order].result = 0;
    plot[list].advance[order].gamesFinished = 0;
    plot[list].advance[order].realTimer = 0;
    //对每个勾选的定时任务，确认带日期的定时时刻
    if (plot[list].advance[order].isSelected && IsTimer(plot[list].advance[order]))
    {
      //将上一个时刻的时分秒设置为定时时刻的时分秒
      tm local;
      localtime_s(&local, &lastTimer);
      local.tm_hour = plot[list].advance[order].timer % 100000 / 3600;
      local.tm_min = plot[list].advance[order].timer % 100000 % 3600 / 60;
      local.tm_sec = plot[list].advance[order].timer % 100000 % 60;
      time_t realTimer = mktime(&local); //定时时刻预设为当天
      while (realTimer <= lastTimer) //如果定时时刻早于上一个时刻，将定时时刻推后1天
        realTimer += 86400;
      plot[list].advance[order].realTimer = realTimer;//记录带日期的定时时刻
      lastTimer = realTimer;
    }
  }
  memset(plot[list].timesRefreshed, 0, sizeof(plot[list].timesRefreshed));//已刷新次数清零
  plot[list].outputNum = 0;//清空输出信息
  plot[list].isRefreshRequired = 0;
  plot[list].isAdvanceWindowBound = 0;//Param[5].hWnd是否被绑定（同步任务是否需要特殊处理？）
  memset(plot[list].logFinished, 0, sizeof(plot[list].logFinished));//已完成任务日志清空
  plot[list].zoneCounter = 0;//已执行的任务数量设为0
  ShuffleRandomZone(list);//生成随机换区顺序

  ListInfo listInfo = {};//列表信息（参与情况，三大识别任务编号）
  char checkInfo[1000] = {};//列表检查信息
  bool skillExist = false;//是否存在刷技能任务
  bool shutdownExist = false;//是否存在关机任务

  //"任务-大赛"执行前的准备工作（期数预设为0，完成/未完成标记清空）
  ResetContest(list);

  //检查当前列表、公会列表和情侣列表
  CheckAllListAndReport(list, checkInfo, listInfo, skillExist, shutdownExist);

  //如果是任务检查模式，线程到此为止
  if (plot[list].isChecking)
  {
    plot[list].isCheckingSuccessful = true;
    StopAdvance(list);
  }

  bool isLabelMissing[2] = {};//是否缺少1P/2P标签
  for (int account = 0; account < 2; account++)
    isLabelMissing[account] = plot[list].advanceInvolved[account] && param[task].tag[account].x == -1;

  if (isLabelMissing[0] || isLabelMissing[1]) //如果未抓取标签
  {
    if (isLabelMissing[0] && isLabelMissing[1])
      StopAdvance(list, "请抓取1P和2P标签。");
    else if (isLabelMissing[0])
      StopAdvance(list, "请抓取1P标签。");
    else if (isLabelMissing[1])
      StopAdvance(list, "请抓取2P标签。");
    return 0;//任务结束
  }

  if (plot[list].advanceInvolved[0] && plot[list].advanceInvolved[1]
    && strcmp(param[task].hallName[0], param[task].hallName[1]) == 0)
    StopAdvance(list, "请分窗口显示两个账号。\n（大厅右上角“多窗口玩游戏”）");

  //检验通过，填写param[task + 1]的标签
  for (int account = 0; account < 2; account++)
  {
    param[task + 1].tag[account] = param[task].tag[account];
    param[task + 1].hWnd[account] = param[task].hWnd[account];
    strcpy_s(param[task + 1].hallName[account], param[task].hallName[account]);
  }
  strcpy_s(param[task + 1].mainName, param[task].mainName);

  //非深度检查模式需要提示大赛模式、关机、刷技能
  if (!plot[list].isDeepChecking)
  {
    if (strstr(plot[list].advancePath, "美食大赛"))
      listStyle = CONTEST;
    else if (strstr(plot[list].advancePath, "营地任务"))
      listStyle = CAMP;
    else if (strstr(plot[list].advancePath, "主线任务"))
      listStyle = DAILY;
    else
      listStyle = 0;
    if (listStyle)
      bagFullOperation = 1;

    if (cycleTimes == 1) //第1轮循环进行提示
    {
      //含有关机任务
      if (shutdownExist)
      {
        char tip[300] = "警告：列表中含有关机任务。\n"
          "点击确认继续执行。";
        int answer = PopMessageOK(task, tip);
        if (answer != IDOK)
          StopAdvance(list);
      }
      //刷技能
      if (skillExist)
      {
        char tip[300] = "刷技能要求:\n"
          "1. 所有卡片至少1星；\n"
          "2. 操作速度建议调至“快”。\n"
          "点击确认继续执行。";
        int answer = PopMessageOK(task, tip);
        if (answer != IDOK)
          StopAdvance(list);
      }
      //大赛、营地列表
      if (listStyle == CONTEST || listStyle == CAMP)
      {
        int answer = MessageBoxA(hWndActuator,
          "大赛/营地模式遇到任务无法完成将停止列表。\n请确保钥匙、徽章及背包空间充足。\n点击确认继续执行。",
          "提示", MB_OKCANCEL | MB_ICONINFORMATION | MB_SYSTEMMODAL);
        if (answer != IDOK) //如果未确认执行大赛
          StopAdvance(list);
      }
    }
  }

  char taskStartTime[100];//任务启动时间字符串，用于给日志文件夹命名
  GetTimeStringForFolder(taskStartTime, time(NULL));//记录本次启动的时间
  sprintf_s(plot[list].advanceFolderPath, "执行记录\\%s_高级任务%d", taskStartTime, list);//填写日志文件夹路径
  CreatePath(plot[list].advanceFolderPath);//创建日志文件夹

  sprintf_s(plot[list].contestLog, "%s\\大赛日志.txt", plot[list].advanceFolderPath);//填写大赛日志路径
  FILE *f;
  fopen_s(&f, plot[list].contestLog, "w"); //创建大赛日志
  fclose(f);

  //深度检查模式直接运行第n+1条
  if (plot[list].isDeepChecking)
  {
    //第n+1条的卡组按照高级任务参与情况设置
    for (int account = 0; account < 2; account++)
      if (plot[list].advanceInvolved[account])
        plot[list].advance[plot[list].advanceNum].deck[account] = 1;
    //执行第n+1条任务，如果顺利完成则判定检查成功
    StartQuest(list, plot[list].advanceNum);
    plot[list].isDeepChecking = false;
    StopAdvance(list);
  }

  //依次执行各项任务 StartListCore
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    //order跳转至时间已经到达的ID最大的killer定时器
    time_t now = time(nullptr);
    for (int iOrder = order + 1; iOrder < plot[list].advanceNum; iOrder++)
    {
      Advance &advance = plot[list].advance[iOrder];
      if (advance.isSelected && IsTimer(advance) && advance.timer < 100000)
        if (now >= advance.realTimer)
          order = iOrder;
    }

    if (plot[list].advance[order].isSelected)
    {
      //遇到循环任务，结束本轮高级任务，重新执行
      if (IsCycle(plot[list].advance[order]))
      {
        FinishAdvance(list, false);
        goto restart;
      }
      StartQuest(list, order);//执行单项任务
      if (IsMission(plot[list].advance[order])) //执行完识别任务后插入识别结果
      {
        //公会和情侣任务：插入全部任务
        if (plot[list].advance[order].level == 0 || plot[list].advance[order].level == 1)
          InsertMission(list, plot[list].advance[order].level);
        else if (plot[list].advance[order].level == 2) //大赛任务：插入1条任务
        {
          int lastSubcontestOrder = -1;//插入前最后一个子任务位置
          InsertContest(list, lastSubcontestOrder, true, true);
          //order设为插入前最后一个任务。若插入成功，接下来执行插入的任务，否则大赛结束
          order = lastSubcontestOrder;
        }
      }
      //执行完大赛子任务，把order调到大赛前一项，下一项继续执行大赛识别
      else if (plot[list].advance[order].missionStyle == CONTEST)
      {
        int contestOrder = GetMissionOrder(list, CONTEST - 1);
        if (contestOrder != -1)
          order = contestOrder - 1;
      }
    }
    if (plot[list].isAdvanceStarted == 0)//检测到刹车指令，立即结束线程
      StopAdvance(list);
  }
  FinishAdvance(list, true);//高级任务完成
  return 0;
}
template <size_t size>
void GetTimeStringMS(char(&s)[size], int Time)//从秒数转化为“mm:ss”字符串
{
  sprintf_s(s, "%02d:%02d", Time / 60, Time % 60);
}
template <size_t size>
void GetTimeStringHMS(char(&s)[size], int Time)//从秒数转化为“h:mm:ss”字符串
{
  sprintf_s(s, "%d:%02d:%02d", Time / 3600, Time % 3600 / 60, Time % 60);
}
/*执行器界面函数*/
//判断鼠标点击位置(x,y)在哪个区域
int GetArea(int originX, int originY)
{
  int x = (int)(originX / ratio + 0.5);
  int y = (int)(originY / ratio + 0.5);
  if (x < 0 || y < 0 || x >= wndWidth || y >= wndHeight)
    return 0;

  if (x >= modeX && x < modeX + modeWidth && y >= modeY && y < modeY + 2 * modeHeight)//切换模式
  {
    int n = (y - modeY) / modeHeight;
    return 1 + n;
  }

  if (y < modeY)//缩放按钮
  {
    if (x >= modeX - 20 && x < modeX + modeWidth / 2)
      return 314;
    else if (x > modeX + modeWidth / 2)
      return 414;
  }
  if (mode == 1)//普通任务模式
  {
    for (int task = 0; IsNormal(task); task++)
    {
      if (x >= parameterX + (2 * task + 1) * parameterWidth / 2 - 100 && x <= parameterX + (2 * task + 1) * parameterWidth / 2 - 40 &&
        y >= parameterY + 11 * parameterHeight + 5 && y < parameterY + 12 * parameterHeight - 4)
        return 10 + task;//点击启动按钮
      if (x >= parameterX + (2 * task + 1) * parameterWidth / 2 - 30 && x <= parameterX + (2 * task + 1) * parameterWidth / 2 + 30 &&
        y >= parameterY + 11 * parameterHeight + 5 && y < parameterY + 12 * parameterHeight - 4)
        return 20 + task;//点击隐藏1按钮
      if (x >= parameterX + (2 * task + 1) * parameterWidth / 2 + 40 && x <= parameterX + (2 * task + 1) * parameterWidth / 2 + 100 &&
        y >= parameterY + 11 * parameterHeight + 5 && y < parameterY + 12 * parameterHeight - 4)
        return 30 + task;//点击隐藏2按钮
    }
    return ButtonArea[0][x][y];
  }
  else if (mode == 2)//高级任务模式
  {
    if (x >= parameterX + parameterWidth / 2 - 100 && x <= parameterX + parameterWidth / 2 - 40 &&
      y >= parameterY + 11 * parameterHeight + 5 && y < parameterY + 12 * parameterHeight - 4)
      return 15;//点击启动按钮
    if (x >= parameterX + parameterWidth / 2 - 30 && x <= parameterX + parameterWidth / 2 + 30 &&
      y >= parameterY + 11 * parameterHeight + 5 && y < parameterY + 12 * parameterHeight - 4)
      return 25;//点击隐藏1按钮
    if (x >= parameterX + parameterWidth / 2 + 40 && x <= parameterX + parameterWidth / 2 + 100 &&
      y >= parameterY + 11 * parameterHeight + 5 && y < parameterY + 12 * parameterHeight - 4)
      return 35;//点击隐藏2按钮

    int area = ButtonArea[1][x][y];
    //500+特殊编号：500全选格子 501全选框 502锁定框 503列表路径 504-1P服务器二级 505-2P服务器二级
    if (area / 10000 == 1 && area % 10000 / 100 == 0)
      return 0;
    //10+~30+：隐藏与启动
    //100+：左侧选项名称
    //200+400+：右侧按钮
    //500：全选格子；501：全选框；502：锁定
    //1000+~7000+：高级任务参数调节
    //10000+~70000+：高级任务列表调节
    return area;
  }
  return 0;
}
//注册所有按钮的区域值
void RegisterAllButtons()
{
  for (int mode = 1; mode <= 2; mode++)
  {
    int zoomButton = RegisterButtonArea(mode, zoomX, zoomY, zoomWidth, zoomHeight, 214);//注册缩放按钮区
    RegisterHorizontalAdjust(mode, zoomButton, 314, 414);
  }

  //普通任务参数：1000+
  buttonParameter[0] = RegisterButtonsArea(1, 12, 5, parameterX, parameterY, parameterWidth, parameterHeight, 1000, 1, 100);
  //普通任务状态：1000+
  buttonState[0] = RegisterButtonsArea(1, 5, 5, parameterX, parameterY + 12 * parameterHeight + 2, parameterWidth, parameterHeight, 1012, 1, 100);
  //普通参数调节按钮：1000+&2000+：右调节；6000+&7000+：左调节；4000+：左点击；5000+：右点击
    //改为：本体1000+，左点击2000+，右点击3000+，左调节4000+&5000+：右调节6000+&7000+
  for (int task = 0; IsNormal(task); task++)
  {
    RegisterRightButton(1, FindButton(buttonParameter[0], 0, task), 36, 3000 + task * 100);
    for (int row = 5; row <= 7; row++)//注册调节按钮
    {
      RegisterLeftAdjust(1, FindButton(buttonParameter[0], row, task), 4000 + task * 100 + row, 5000 + task * 100 + row);
      RegisterRightAdjust(1, FindButton(buttonParameter[0], row, task), 6000 + task * 100 + row, 7000 + task * 100 + row);
    }
    for (int row = 1; row <= 2; row++)//注册轨道左边的交换按钮和右边的“×”
    {
      RegisterLeftButton(1, FindButton(buttonParameter[0], row, task), 18, 2000 + task * 100 + row);
      RegisterRightButton(1, FindButton(buttonParameter[0], row, task), 18, 3000 + task * 100 + row);
    }
    for (int row = 8; row <= 9; row++)//注册参数左边的抓取按钮和右边的“×”
    {
      RegisterLeftButton(1, FindButton(buttonParameter[0], row, task), 22, 2000 + task * 100 + row);
      RegisterRightButton(1, FindButton(buttonParameter[0], row, task), 18, 3000 + task * 100 + row);
    }
    for (int row = 0; row <= 4; row++)//清空状态按钮
      RegisterRightButton(1, FindButton(buttonState[0], row, task), 18, 3012 + task * 100 + row);
  }
  //高级任务参数

  //参数标题
  buttonParameterTitle[1] = RegisterButtonsArea(2, 5, 1, titleX, titleY + 7 * titleHeight,
    titleWidth, titleHeight, 107, 1, 0);
  //状态标题
  buttonStateTitle[1] = RegisterButtonsArea(2, 5, 1, titleX, titleY + 12 * titleHeight + 2,
    titleWidth, titleHeight, 112, 1, 0);
  //参数内容
  buttonParameter[1] = RegisterButtonsArea(2, 5, 1, parameterX, parameterY + 7 * parameterHeight,
    parameterWidth, parameterHeight, 1507, 1, 0);
  //状态内容
  buttonState[1] = RegisterButtonsArea(2, 5, 1, parameterX, parameterY + 12 * parameterHeight + 2,
    parameterWidth, parameterHeight, 1512, 1, 0);
  //高级任务切换按钮
  RegisterHorizontalAdjust(2, FindButton(buttonParameter[1], 0, 0), 2507, 3507);
  for (int row = 1; row <= 2; row++)//1P/2P标签左边的抓取按钮和右边的“×”
  {
    RegisterLeftButton(2, FindButton(buttonParameter[1], row, 0), 22, 2507 + row);
    RegisterRightButton(2, FindButton(buttonParameter[1], row, 0), 18, 3507 + row);
  }
  //删除状态按钮
  RegisterRightButton(2, FindButton(buttonState[1], 0, 0), 18, 3512);

  //高级任务输出信息
  buttonOutputPage = RegisterButtonsArea(2, 1, 1, titleX + (titleWidth + parameterWidth) / 2 - 55, titleY + 6 * titleHeight,
    110, titleHeight, 45, 0, 0);
  RegisterLeftButton(2, FindButton(buttonOutputPage, 0, 0), 25, 55);//向左翻页
  RegisterRightButton(2, FindButton(buttonOutputPage, 0, 0), 25, 65);//向右翻页
  //清空信息按钮
  RegisterButtonArea(2, titleX, titleY + 6 * titleHeight, 20, titleHeight, 75);
  //info按钮
  RegisterButtonArea(2, titleX + titleWidth + parameterWidth - 35, titleY + 6 * titleHeight, 35, titleHeight, 85);

  //高级任务列表
  buttonAdvance = RegisterButtonsArea(2, advanceNumPerPage + 1, advanceTitleNum, advanceX, advanceY, advanceWidth, advanceHeight, 10000, 100, 1);

  //注册列表区域
  for (int order = 0; order < advanceNumPerPage; order++)//注册调节按钮和勾选框
  {
    for (int i = 1; i < advanceTitleNum; i++)
    {
      if (i == TYPE || i == LEVEL || i == DECK_0 || i == DECK_1)
        RegisterRightAdjust(2, FindButton(buttonAdvance, order + 1, i), 20000 + i + 100 * (order + 1), 30000 + i + 100 * (order + 1));
      if (i == TRACK_0 || i == TRACK_1)
      {
        RegisterLeftButton(2, FindButton(buttonAdvance, order + 1, i), 18, 60000 + i + 100 * (order + 1));
        RegisterRightButton(2, FindButton(buttonAdvance, order + 1, i), 18, 70000 + i + 100 * (order + 1));
      }
    }
    RegisterLeftAdjust(2, FindButton(buttonAdvance, order + 1, 2), 40002 + 100 * (order + 1), 50002 + 100 * (order + 1));//“关卡”的左调节
    RegisterLeftButton(2, FindButton(buttonAdvance, order + 1, 0), 30, 10000 + CHECK + (order + 1) * 100);//勾选框
  }
  RegisterCenterButton(2, FindButton(buttonAdvance, 0, 0), 0, 0, advanceWidth[0], advanceHeight[0], 500);//"ID"
  RegisterLeftButton(2, FindButton(buttonAdvance, 0, 0), 30, 501);//"ID"左侧的全选框
  buttonLock = RegisterButtonArea(2, lockX, lockY, lockWidth, lockHeight, 502);//锁定框
  RegisterButtonArea(2, titleX, 0, playerX - titleX, titleY, 503);//列表路径
  RegisterButtonArea(2, playerX, 0, playerDistance, titleY, 504);//1P服务器二级
  RegisterButtonArea(2, playerX + playerDistance, 0, playerDistance, titleY, 505);//2P服务器二级

  //右侧按钮：上方是三大按钮，下方是各类选项
  for (int iMode = 1; iMode <= 2; iMode++)
    buttonUpperSettings[iMode - 1] = RegisterButtonsArea(iMode, 3, 1,
      settingsX, settingsY - settingsHeight * 7 / 2, settingsWidth, settingsHeight, 200, 1, 0);

  const int SettingsNum[2] = { 11, 9 };//两种模式需要注册的设置按钮个数
  //第i项是否需要调节按钮
  const bool isAdjustRequired[2][11] = { { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1 }, { 0, 1, 0, 1, 0, 0, 0, 0, 1 } };
  for (int iMode = 1; iMode <= 2; iMode++)
  {
    buttonLowerSettings[iMode - 1] = RegisterButtonsArea(iMode, SettingsNum[iMode - 1], 1,
      settingsX, settingsY, settingsWidth, settingsHeight, 203, 1, 0);
    for (int i = 0; i < 11; i++)
      if (isAdjustRequired[iMode - 1][i])
        RegisterHorizontalAdjust(iMode, FindButton(buttonLowerSettings[iMode - 1], i, 0), 303 + i, 403 + i);
  }
  int buttonUndo = RegisterButtonsArea(2, 2, 1, settingsX, settingsY + 9 * settingsHeight + 2, settingsWidth, settingsHeight, 212, 1, 0);
}
//判断释放文件时鼠标位置在哪个区域
int TaskArea(int OriginX, int OriginY)
{
  int x, y, task, account;
  x = (int)(OriginX / ratio);
  y = (int)(OriginY / ratio);
  if (mode == 1)
  {
    if (x >= parameterX && x < parameterX + 5 * parameterWidth && y >= parameterY + parameterHeight && y < parameterY + 3 * parameterHeight)
    {
      task = (x - parameterX) / parameterWidth;
      account = (y - parameterY) / parameterHeight - 1;
      return task * 10 + account;
    }
  }
  else if (mode == 2)
  {
    int area = GetArea(OriginX, OriginY);
    if (area / 10000 == 1 || area / 10000 == 6 || area / 10000 == 7)
    {
      int order = area % 10000 / 100 - 1;//order=第几条高级任务
      if (order == -1)
        return -1;
      if (area % 10 == TRACK_0)
        return order * 10;
      else if (area % 10 == TRACK_1)
        return order * 10 + 1;
    }
  }
  return -1;
}
//显示底部信息
void ViewBottomInfo()
{
  settextcolor(grayWhite);
  //outtextxy(titleX, wndHeight - tipHeight, tracePrint);
#ifdef _DEBUG
  int list = curList;
  char message[200];
  POINT point;
  GetCursorPos(&point);
  ScreenToClient(hWndActuator, &point);
  sprintf_s(message, "区域%d", area);
  outtextxy(1100, wndHeight - tipHeight, message);//显示当前点击区域（调试用）
  sprintf_s(message, "备份%d/%d", plot[list].currentBackup, plot[list].backupNum);
  outtextxy(1200, wndHeight - tipHeight, message);
#endif
}
//显示上一次提示信息
void ViewLastTip()
{
  settextcolor(strstr(lastTip, "识图") == lastTip ? WHITE : RGB(255, 255, 0));
  outtextxy(titleX, wndHeight - 27, lastTip);
  ViewBottomInfo();
}
//显示提示语句
void ViewTip(const char *tip)
{
  if (strcmp(lastTip, tip) == 0)
    return;

  strcpy_s(lastTip, tip);
  BeginBatchDraw();
  solidrectangle(0, wndHeight - 27, wndWidth, wndHeight);
  ViewLastTip();
  EndBatchDraw();
}
//获取提示信息内容
int GetTip(int mode, int area, char(&tip)[200])
{
  if (area == 1)
    return strcpy_s(tip, "普通任务：用于重复执行同一关卡。");
  if (area == 2)
    return strcpy_s(tip, "高级任务：可一键完成签到施肥、悬赏、公会、刷技能等多项任务。");

  if (area / 100 >= 2 && area / 100 <= 4)
  {
    int option = area % 100;
    if (option == 0)
      return strcpy_s(tip, "全部停止：停止所有普通任务和高级任务。");
    if (option == 1)
      return strcpy_s(tip, "后台运行：隐藏软件窗口，再次运行“轨道执行器.exe”可恢复显示。");
    if (option == 2)
      return strcpy_s(tip, "一键报错：一键打包截图和日志，生成报错信息。在交流群提问/报错必须上传报错信息，否则无法解决问题。");
  }

  if (mode == 1)
  {
    if (area / 1000 >= 1 && area / 1000 <= 7)
    {
      int row = area % 100;
      if (row == 1 || row == 2)
        return strcpy_s(tip, "拖动轨道文件到此处自动填写。右键打开轨道。Ctrl+右键打开轨道所在文件夹。");
      if (row == 8 || row == 9)
        return strcpy_s(tip, "将左侧图标拖动到账号标签或游戏画面内即可绑定。");
    }
    if (area / 100 >= 2 && area / 100 <= 4)
    {
      int option = area % 100;
      if (option == 3 || option == 4)
        return strcpy_s(tip, "带卡选择：背包中有多张同种卡时携带最高星还是最靠前的卡。");
      if (option == 5 || option == 6)
        return strcpy_s(tip, "翻牌数量：通关后翻几张牌。");
      if (option == 7 || option == 8)
        return strcpy_s(tip, "背包满时：出现背包已满或未带特定卡的提示时是否继续执行。建议选择继续。");
      if (option == 9 || option == 10)
        return strcpy_s(tip, "经验满时：出现经验已达上限的提示时是否继续执行。建议选择继续。");
      if (option == 11 || option == 12)
        return strcpy_s(tip, "补阵强度：0%为识别空位补阵，100%为暴力遍历补阵。强度增大会导致游戏卡顿，建议选择0%。");
    }
    return strcpy_s(tip, tracePrint);
  }
  else if (mode == 2)
  {
    if (area / 1000 >= 1 && area / 1000 <= 7)
    {
      int row = area % 100;
      if (row == 1 || row == 2)
        return strcpy_s(tip, "将左侧图标拖动到账号标签或游戏画面内即可绑定。");
    }
    if (area / 100 >= 2 && area / 100 <= 4)
    {
      int option = area % 100;
      if (option == 3 || option == 4)
        return strcpy_s(tip, "自动还原：任务停止时还原识别到的公会、情侣和大赛任务。");
      if (option == 5 || option == 6)
        return strcpy_s(tip, "自动刷新：掉线中断时刷新继续。一条任务刷新满设定次数后将被跳过。");
      if (option == 7)
        return strcpy_s(tip, "操作速度：控制建房和刷技能的速度。建议选择快。");
      if (option == 8)
        return strcpy_s(tip, "加速时间：设置全体任务的开局加速时间。");
      if (option == 9)
        return strcpy_s(tip, "一键配轨：为所有勾选的任务配置通用轨道。");
      if (option == 10)
        return strcpy_s(tip, "检查任务：检查轨道是否合格，统计需要的钥匙和卡片。");
    }
    return strcpy_s(tip, tracePrint);
  }
  return strcpy_s(tip, "");
}
//显示提示信息
void RepaintTip()
{
  //根据当前鼠标位置决定提示信息
  POINT point;
  GetCursorPos(&point);
  ScreenToClient(hWndActuator, &point);
  char tip[200];
  GetTip(mode, GetArea(point.x, point.y), tip);
  ViewTip(tip);
}
//显示顶部信息
void ViewTopInfo()
{
  char message[200];
  if (mode == 2)
  {
    int list = curList;

    //显示列表路径
    settextcolor(grayWhite);
    char truncatedPath[maxPath];//截短的列表路径
    TruncateWithEllipsis(truncatedPath, plot[list].advancePath, 480);
    sprintf_s(message, "列表路径：%s", truncatedPath);
    outtextxy(titleX, 5, message);

    //显示锁定框
    PaintButtonGrid(buttonLock);
    outtextxy(lockX + 25, lockY, "锁定");
    if (plot[list].isAdvanceLocked)
      outtextxy(lockX, lockY + 3, "√");

    //显示服务器和二级密码
    settextcolor(RGB(255, 255, 0));
    for (int account = 0; account < 2; account++)
    {
      char serverString[100], passwordString[100];

      if (plot[list].server[account] == 0)
        strcpy_s(serverString, "最近");
      else
        sprintf_s(serverString, "%d服", plot[list].server[account]);

      if (strlen(plot[list].password[account]) == 0)
        strcpy_s(passwordString, "无二级密码");
      else
      {
        strcpy_s(passwordString, plot[list].password[account]);
        int i = 0;
        while (passwordString[i])
          passwordString[i++] = '*';
      }

      sprintf_s(message, "[%dP] %s | %s", account + 1, serverString, passwordString);
      outtextxy(playerDistance * account + playerX, playerY, message);
    }

  }
}
//载入进度条
void LoadProgress()
{
  FILE *f;
  if (fopen_s(&f, "附加程序\\文本\\进度条颜色.txt", "r"))//打开文件
    return;
  for (int i = 0; i < progressBarLength; i++)
    fscanf_s(f, "%d\n", &progressLeftColor[i]);
  fscanf_s(f, "-----\n");
  for (int i = 0; i < progressBarLength; i++)
    fscanf_s(f, "%d\n", &progressRightColor[i]);
  fclose(f);

  if (fopen_s(&f, "附加程序\\文本\\鼠头颜色.txt", "r"))//打开文件
    return;
  for (int i = 0; i < ratColorWidth; i++)
    fscanf_s(f, "%x\n", &ratColor[i]);
  fclose(f);
}
//门票检查公共变量：启动时读取，不再修改
int entrance[maxTypeNum][20];//(type,level)需要的门票编号，无门票为0
//设置非岛屿关卡所需的门票
void InitTicket()
{
  //0悬赏：神殿钥匙
  for (int level = 0; level < levelNum[0]; level++)
    entrance[0][level] = 2;
  //1勇士、2魔塔、3跨服：无需钥匙
  //4副本-0假期：假期票
  entrance[4][0] = 1;
  //5任务：不计算钥匙
}
//根据门票名称查询门票编号。查找失败返回-1
int GetTicketCode(const char *fullTicketName)
{
  char ticketName[50] = {};//钥匙/徽章主名，如"神殿"
  int style = 0;
  strcpy_s(ticketName, fullTicketName);
  char *key = strstr(ticketName, "钥匙");
  char *badge = strstr(ticketName, "徽章");
  if (key)
    key[0] = 0;
  else if (badge)
  {
    badge[0] = 0;
    style = 1;
  }

  //如果名称为"神殿钥匙"，查找"神殿"
  for (int code = 0; code < ticketNum; code++)
    if (strcmp(ticketName, ticket[code].name) == 0 && style == ticket[code].style)
      return code;
  return -1;
}
//读取普通关卡列表（需要先载入门票）
void LoadNormalLevel()
{
  InitTicket();

  char s[1000], prompt[30];
  char *info = nullptr, *nextInfo = nullptr;

  FILE *f;
  const char path[] = "附加程序\\文本\\普通关卡.txt";
  if (fopen_s(&f, path, "r"))//打开文件
    ReportMissingFile(path);

  int type = -1;//岛屿类型（实际类型要+6）
  int level = -1;//岛屿关卡
  while (!feof(f))
  {
    fgets(f, s);//读取一行文字
    if (strlen(s) == 0)
      break;
    if (strstr(s, "##"))//如果找到了"##"，类型+1，记录岛屿名称
    {
      type++;
      level = -1;
      info = strtok_s(s, "\t", &nextInfo);
      strcpy_s(typeName[basicTypeNum + type], info + 2);//岛屿名称
      strcpy_s(islandPrompt[type], "");

      info = strtok_s(NULL, "\t", &nextInfo);//记录岛屿坐标
      islandLocation[type].x = atoi(info);
      info = strtok_s(NULL, "\t", &nextInfo);
      islandLocation[type].y = atoi(info);
    }
    else //否则记录关卡名称
    {
      level++;
      levelNum[basicTypeNum + type]++;
      info = strtok_s(s, "\t", &nextInfo);//第1段：关卡名称
      strcpy_s(islandLevel[type][level], info);
      sprintf_s(prompt, "%d=%s", level, islandLevel[type][level]);
      if (level > 0)
        strcat_s(islandPrompt[type], "，");
      strcat_s(islandPrompt[type], prompt);

      info = strtok_s(NULL, "\t", &nextInfo);//第2段：关卡气泡X坐标
      islandLevelButton[type][level].x = atoi(info);
      info = strtok_s(NULL, "\t", &nextInfo);//第3段：关卡气泡Y坐标
      islandLevelButton[type][level].y = atoi(info);
      info = strtok_s(NULL, "\t", &nextInfo);//第4段：门票
      int ticketCode = GetTicketCode(info); //查询门票编号
      if (ticketCode == -1)
      {
        char message[1000] = {};
        sprintf_s(message, "未找到门票：%s", info);
        PopMessage(nullptr, message);
        exit(0);
      }
      entrance[basicTypeNum + type][level] = ticketCode;//记录本关门票
    }
  }
  fclose(f);
}
//初始化普通任务参数
void InitTaskParam(int task)
{
  memset(&param[task], 0, sizeof(param[task]));

  const POINT noTag = { -1, -1 };
  if (IsAdvance(task))
    sprintf_s(param[task].title, "高级任务%d", GetList(task));
  else
    sprintf_s(param[task].title, "普通任务%d", task);
  strcpy_s(param[task].track[0], "Track.txt");
  param[task].maxGames = 100;
  for (int account = 0; account < 2; account++)
  {
    param[task].isProceed[account] = 1;
    param[task].isCollect[account] = 1;
    param[task].tag[account] = noTag;
  }
}
//初始化高级任务参数
void InitAdvanceParam(int list)
{
  for (int account = 0; account < 2; account++)
    strcpy_s(plot[list].password[account], "");
}
//初始化执行器参数
void InitParameter()
{
  //初始化公用参数
  zoom = 100;//缩放默认为100%
  plantDelay = 50;//放卡延迟默认50ms
  noImageOperation = 1;//无图像时操作默认为“提示”

  customOption = 0;//带卡选择默认为“最高星”
  flipNum = 2;//翻牌数量默认为2
  bagFullOperation = 1;//背包满时操作默认为“继续”
  expFullOperation = 1;//经验满时操作默认为“继续”
  repairLevel = 0;//补阵强度默认0%

  missionRecovery = 1;//自动还原默认为“开”
  maxRefreshTimes = 3;//自动刷新次数默认为3
  operationSpeed = 2;//操作速度默认为“快”

  //初始化普通任务参数
  for (int task = 0; task < maxTaskNum; task++)
    InitTaskParam(task);

  //初始化高级任务参数
  for (int list = 0; list < listNum; list++)
    InitAdvanceParam(list);
}
//读取执行器参数
void LoadParameter()
{
  InitParameter();//初始化执行器参数

  FILE *f;
  if (fopen_s(&f, "用户参数\\执行器参数.txt", "r"))//打开文件
    return;

  //文件结构：公用参数→高级任务→普通任务
  char s[1000];
  fgets(f, s);
  //读取公用参数。直到到达下一个标题行或文件结束
  while (strlen(s) > 0 && strstr(s, "--") != s)
  {
    if (strstr(s, "缩放比例=") == s)
      zoom = atoi(s + 9);
    else if (strstr(s, "放卡延迟=") == s)
      plantDelay = atoi(s + 9);
    else if (strstr(s, "带卡选择=") == s)
      customOption = atoi(s + 9);
    else if (strstr(s, "翻牌数量=") == s)
      flipNum = atoi(s + 9);
    else if (strstr(s, "背包满时=") == s)
      bagFullOperation = atoi(s + 9);
    else if (strstr(s, "经验满时=") == s)
      expFullOperation = atoi(s + 9);
    else if (strstr(s, "补阵强度=") == s)
      repairLevel = atoi(s + 9);
    else if (strstr(s, "自动还原=") == s)
      missionRecovery = atoi(s + 9);
    else if (strstr(s, "自动刷新=") == s)
      maxRefreshTimes = atoi(s + 9);
    else if (strstr(s, "操作速度=") == s)
      operationSpeed = atoi(s + 9);
    else if (strstr(s, "加速时间=") == s)
      globalAccelarationTime = atoi(s + 9);
    fgets(f, s);
  }
  //读取高级任务参数
  for (int list = 0; list < listNum; list++)
  {
    fgets(f, s);//跳过标题行
    //读取高级任务参数。直到到达下一个标题行
    while (strlen(s) > 0 && strstr(s, "--") != s)
    {
      if (strstr(s, "1P服务器=") == s)
        plot[list].server[0] = atoi(s + 9);
      else if (strstr(s, "1P二级密码=") == s)
        strcpy_s(plot[list].password[0], s + 11);
      else if (strstr(s, "2P服务器=") == s)
        plot[list].server[1] = atoi(s + 9);
      else if (strstr(s, "2P二级密码=") == s)
        strcpy_s(plot[list].password[1], s + 11);
      fgets(f, s);
    }
  }
  //读取普通任务参数
  for (int task = 0; task < maxTaskNum; task++)
  {
    fgets(f, s);//跳过标题行
    //读取普通任务参数。直到到达下一个标题行
    while (strlen(s) > 0 && strstr(s, "--") != s)
    {
      if (strstr(s, "任务标题=") == s)
        strcpy_s(param[task].title, s + 9);
      else if (strstr(s, "1P轨道=") == s)
        strcpy_s(param[task].track[0], s + 7);
      else if (strstr(s, "2P轨道=") == s)
        strcpy_s(param[task].track[1], s + 7);
      else if (strstr(s, "房主轨道=") == s)
        strcpy_s(param[task].track[0], s + 9);
      else if (strstr(s, "队友轨道=") == s)
        strcpy_s(param[task].track[1], s + 9);
      else if (strstr(s, "执行局数=") == s)
        param[task].maxGames = atoi(s + 9);
      else if (strstr(s, "每局用时=") == s)
        param[task].gameTime = atoi(s + 9);
      else if (strstr(s, "继续作战=") == s)
        sscanf_s(s + 9, "%d,%d", &param[task].isProceed[0], &param[task].isProceed[1]);
      else if (strstr(s, "自动收集=") == s)
        sscanf_s(s + 9, "%d,%d", &param[task].isCollect[0], &param[task].isCollect[1]);
      else if (strstr(s, "使用卡组=") == s)
        sscanf_s(s + 9, "%d,%d", &param[task].deck[0], &param[task].deck[1]);
      else if (strstr(s, "1P标签=") == s)
        sscanf_s(s + 7, "%d,%d", &param[task].tag[0].x, &param[task].tag[0].y);
      else if (strstr(s, "2P标签=") == s)
        sscanf_s(s + 7, "%d,%d", &param[task].tag[1].x, &param[task].tag[1].y);
      else if (strstr(s, "1P大厅=") == s)
        strcpy_s(param[task].hallName[0], s + 7);
      else if (strstr(s, "2P大厅=") == s)
        strcpy_s(param[task].hallName[1], s + 7);
      else if (strstr(s, "初始大厅=") == s)
        strcpy_s(param[task].mainName, s + 9);
      else
      {
        for (int account = 0; account < 2; account++)
        {
          char title[20];
          sprintf_s(title, "%dP隐藏信息=", account + 1);
          if (strstr(s, title) == s)
          {
            HideInfo &hideInfo = param[task].hideInfo[account];
            sscanf_s(s + 11, "%d,%d,%d,%d,%d,%d\n", &hideInfo.hidden, &hideInfo.maximized,
              &hideInfo.rect.left, &hideInfo.rect.top, &hideInfo.rect.right, &hideInfo.rect.bottom);
          }
        }
      }
      fgets(f, s);
    }
  }
  fclose(f);
}

//用v4.4.2以前的方式读取执行器参数
void LoadParameter_v4_4_2()
{
  InitParameter();//初始化执行器参数

  FILE *f;
  char path[100], s[1000];

  //读取空间3366登录设置
  strcpy_s(path, "用户参数\\空间3366登录设置.txt");
  if (!fopen_s(&f, path, "r"))//打开文件
  {
    fgets(f, s);
    fgets(f, s);
    fgets(f, s);
    plot[0].server[0] = atoi(s + 9);
    fgets(f, s);
    plot[0].server[1] = atoi(s + 9);
    fclose(f);
  }

  //读取执行器参数
  const POINT noTag = { -1, -1 };
  strcpy_s(path, "用户参数\\执行器参数.txt");
  if (!fopen_s(&f, path, "r"))//打开文件
  {
    while (!feof(f))
    {
      fgets(f, s);//读取第1行
      if (strlen(s) == 0)
        break;
      if (strstr(s, "缩放比例=") == s)
        zoom = atoi(s + 9);
      else if (strstr(s, "放卡延迟=") == s)
        plantDelay = atoi(s + 9);
      else if (strstr(s, "带卡选择=") == s)
        customOption = atoi(s + 9);
      else if (strstr(s, "翻牌数量=") == s)
        flipNum = atoi(s + 9);
      else if (strstr(s, "背包满时=") == s)
        bagFullOperation = atoi(s + 9);
      else if (strstr(s, "经验满时=") == s)
        expFullOperation = atoi(s + 9);
      else if (strstr(s, "补阵强度=") == s)
        repairLevel = atoi(s + 9);
      else if (strstr(s, "自动刷新=") == s)
        maxRefreshTimes = atoi(s + 9);
      else if (strstr(s, "操作速度=") == s)
        operationSpeed = atoi(s + 9);
    }

    fclose(f);
  }

  //读取每个任务的参数
  const int oldMaxTaskNum = 6;//旧版任务数量
  for (int task = 0; task < oldMaxTaskNum; task++)
  {
    sprintf_s(path, "用户参数\\任务%d参数.txt", task);
    if (fopen_s(&f, path, "r"))
      continue;//如果文件不存在或无法打开，则跳过此任务的读取
    fgets(f, s);
    if (strstr(s, "房主轨道") == NULL) //如果第1行不是轨道路径，则跳过此任务的读取
    {
      fclose(f);
      continue;
    }
    sprintf_s(param[task].track[0], s + 9);//轨道路径
    fgets(f, s);
    sprintf_s(param[task].track[1], s + 9);//轨道路径
    fgets(f, s);
    param[task].maxGames = atoi(s + 9);//执行局数
    fgets(f, s);
    param[task].gameTime = atoi(s + 9);
    fgets(f, s);
    param[task].isProceed[0] = atoi(s + 13);
    fgets(f, s);
    param[task].isProceed[1] = atoi(s + 13);
    fgets(f, s);
    param[task].isCollect[0] = atoi(s + 13);
    fgets(f, s);
    param[task].isCollect[1] = atoi(s + 13);
    fgets(f, s);
    param[task].deck[0] = atoi(s + 13);
    fgets(f, s);
    param[task].deck[1] = atoi(s + 13);
    fgets(f, s);
    sscanf_s(s + 9, "%d,%d", &param[task].tag[0].x, &param[task].tag[0].y);
    fgets(f, s);
    sscanf_s(s + 9, "%d,%d", &param[task].tag[1].x, &param[task].tag[1].y);
    fclose(f);//读取完毕，关闭文档
  }
}
//配置迁移
void ParameterShifter()
{
  //如果没有任务0参数，无需迁移
  if (!FileExist("用户参数\\任务0参数.txt"))
    return;

  LoadParameter_v4_4_2();//用旧版方式读取参数
  SaveParameter();//用新版方式保存参数

  //删除旧版参数文件
  char path[maxPath];
  for (int task = 0; task < maxTaskNum; task++)
  {
    sprintf_s(path, "用户参数\\任务%d参数.txt", task);
    remove(path);
  }
  remove("用户参数\\空间3366登录设置.txt");

  //如果是更新程序诱发的迁移，迁移完结束程序
  if (FileExist("用户参数\\shifter.txt"))
  {
    remove("用户参数\\shifter.txt");
    exit(0);
  }
}
//检查游戏句柄是否失效
void CheckHallWindow()
{
  bool updateParam = false;
  for (int task = 0; task < maxTaskNum; task++)
    for (int account = 0; account < 2; account++)
    {
      //1. 检查大厅标题是否失效。如果失效，隐藏状态设为“未隐藏”
      HWND hWndHall = Hall(param[task].hallName[account]);
      if (!hWndHall)
      {
        param[task].hideInfo[account].hidden = false;
        updateParam = true;
        isRepaintRequired = 1;//发送重绘指令
      }

      //2. 检查游戏窗口是否失效，更新param[task].isWindow
      bool isWindow = IsWindow(param[task].hWnd[account]) != 0;
      if (isWindow != param[task].isWindow[account])
      {
        param[task].isWindow[account] = isWindow;
        isRepaintRequired = 1;//发送重绘指令
      }
    }
  if (updateParam)
    SaveParameter();
}
//显示高级任务输出信息
void ViewOutput()
{
  int list = curList;
  //输出信息框
  const int outputX = titleX, outputY = titleY, outputWidth = titleWidth + parameterWidth;
  const int textX = outputX + 8, textY = outputY + 10, textHeight = 40;//输出文字位置
  char advanceTip[6][100] = { //高级任务操作提示
    "操作说明",
    "1. 删除：右击任务编号",
    "2. 交换：依次单击两任务编号",
    "3. 复制：按住Ctrl单击任务编号",
    "4. 配置完成后在任意岛屿启动",
    "5. 识别到VIP7以下自动开启收集" };

  setlinecolor(grayWhite);
  PaintGrid(1, 1, outputX, outputY, outputWidth, titleHeight * 7);
  if (plot[list].outputNum == 0)
  {
    settextcolor(grayWhite);
    for (int i = 0; i < 6; i++)
      outtextxy(textX, textY + i * textHeight, advanceTip[i]);
  }
  else
  {
    for (int i = 0; i < 6; i++)
    {
      int order = (plot[list].outputPage - 1) * 6 + i;
      if (order < plot[list].outputNum)
      {
        if (order == plot[list].outputNum - 1)
          settextcolor(taskColor[list]);
        else
          settextcolor(grayWhite);
        outtextxy(textX, textY + i * textHeight, plot[list].output[order]);
      }
    }
    char pageString[10];
    int MaxOutputPage = (plot[list].outputNum - 1) / 6 + 1;
    sprintf_s(pageString, "%d/%d", plot[list].outputPage, MaxOutputPage);
    settextcolor(grayWhite);
    if (plot[list].isAdvanceStarted == 0)
      CenterView("×", outputX + 10, outputY + 13 * titleHeight / 2 + 1);
    CenterView(pageString, outputX + outputWidth / 2, textY + 6 * textHeight + 10);
    PaintHorizontalAdjust(outputX + outputWidth / 2, textY + 6 * textHeight + 10, 50);
  }
}
//显示轨道路径
void ViewTrack(const char *trackPath, int ButtonClassCode, int row, int column, bool isGray = false)
{
  const int TrackWidth = parameterWidth - 40;
  char Track[3][maxPath] = {};
  SetFontSize(defaultTextSize);
  if (textwidth(trackPath) <= TrackWidth)//如果原字体一行能显示得下
    PrintButton(ButtonClassCode, row, column, trackPath, 0, 0, isGray);
  else //如果原字体一行显示不下
  {
    SetFontSize(16);//字体缩小为16px
    Truncate(trackPath, Track[1], Track[0], TrackWidth);//分成两行
    TruncateWithEllipsis(Track[2], Track[0], TrackWidth);//第二行如果过长，截尾并添加省略号
    if ((int)strlen(Track[2]) > zero)//如果一行显示不下
    {
      PrintButton(ButtonClassCode, row, column, Track[1], 0, -9, isGray);
      PrintButton(ButtonClassCode, row, column, Track[2], 0, 9, isGray);
    }
    else //如果一行显示得下
      PrintButton(ButtonClassCode, row, column, Track[1], 0, 0, isGray);
    SetFontSize(defaultTextSize);//字体还原为20px
  }
}
//显示普通任务参数
void ViewParameter()
{
  const int trackWidth = parameterWidth - 40;
  char option[17][13] = { "", "1P轨道", "2P轨道", "执行局数", "每局用时", "继续作战", "自动收集", "使用卡组",
    "1P大厅", "2P大厅", "游戏句柄", "操作", "任务进度", "关卡状态", "本局用时", "平均用时", "累计用时" };
  char collectString[3][13] = { "关", "物品", "全部" };
  char doubleOption[2][3][13] = {};//两个账号的继续作战、收集、卡组
  char gameString[2][13] = {};//两个账号的游戏句柄
  char content[17][maxPath] = {};
  char startString[7] = {}, hideString[2][7] = {};//启动和隐藏按钮的显示
  int size;//轨道路径字体大小

  settextcolor(grayWhite);
  setlinecolor(grayWhite);
  for (int i = 1; i <= 11; i++)//显示选项名称
    CenterView(option[i], titleX + titleWidth / 2, titleY + (2 * i + 1) * titleHeight / 2);
  for (int i = 12; i <= 16; i++)//显示选项名称
    CenterView(option[i], titleX + titleWidth / 2, titleY + (2 * i + 1) * titleHeight / 2 + 2);

  for (int task = 0; IsNormal(task); task++)
  {
    settextcolor(taskColor[task]);

    //填写每一栏要显示的内容
    sprintf_s(content[0], param[task].title);

    for (int account = 0; account < 2; account++)
      ViewTrack(param[task].track[account], buttonParameter[0], account + 1, task);

    sprintf_s(content[3], "%d", param[task].maxGames);

    if (param[task].gameTime == zero)
      sprintf_s(content[4], "自动检测");
    else
      sprintf_s(content[4], "%ds", param[task].gameTime);

    //继续作战
    sprintf_s(content[5], "|");
    for (int i = 0; i <= 1; i++)
      if (param[task].isProceed[i] == zero)
        sprintf_s(doubleOption[i][0], "否");
      else
        sprintf_s(doubleOption[i][0], "是");

    //自动收集
    sprintf_s(content[6], "|");
    sprintf_s(doubleOption[0][1], collectString[param[task].isCollect[0]]);
    sprintf_s(doubleOption[1][1], collectString[param[task].isCollect[1]]);

    //使用卡组
    sprintf_s(content[7], "|");
    for (int i = 0; i <= 1; i++)
      if (param[task].deck[i] == zero)
        sprintf_s(doubleOption[i][2], "当前");
      else
        sprintf_s(doubleOption[i][2], "卡组%d", param[task].deck[i]);

    //大厅标题
    for (int account = 0; account < 2; account++)
    {
      char hallName[maxPath] = {};
      strcpy_s(hallName, param[task].hallName[account]);
      char *bar = strstr(hallName, "|");
      if (bar)
        *bar = 0;
      Truncate(content[8 + account], hallName, 170);
    }

    //游戏句柄
    sprintf_s(content[10], "|");
    for (int account = 0; account < 2; account++)
      if (param[task].tag[account].x == -1) //无标签
        sprintf_s(gameString[account], "");
      else if (param[task].tag[account].x == 0 && param[task].tag[account].y == 0) //已绑定游戏窗口
      {
        param[task].isWindow[account] = IsWindow(param[task].hWnd[account]) != 0;
        if (param[task].isWindow[account])
          sprintf_s(gameString[account], "%d", (int)param[task].hWnd[account]);
        else
          sprintf_s(gameString[account], "--");
      }
      else //待绑定游戏窗口
      {
        param[task].isWindow[account] = IsWindow(param[task].hWnd[account]) != 0;
        if (param[task].isWindow[account])
          sprintf_s(gameString[account], "%d", (int)param[task].hWnd[account]);
        else
          sprintf_s(gameString[account], "%d,%d", param[task].tag[account].x, param[task].tag[account].y);
      }

    //启动隐藏
    sprintf_s(content[11], "");
    if (isTaskStarted[task] == zero)
      sprintf_s(startString, "启动");
    else
      sprintf_s(startString, "停止");
    for (int account = 0; account < 2; account++)
      if (param[task].hideInfo[account].hidden)
        sprintf_s(hideString[account], "显示%d", account + 1);
      else
        sprintf_s(hideString[account], "隐藏%d", account + 1);

    //填写状态信息
    if (strcmp(state[task].tip, "未开始") == zero)
      for (int i = 12; i <= 16; i++)
        strcpy_s(content[i], "");
    else //已经启动则进行记录
    {
      GetTimeStringMS(content[14], state[task].currentTime);
      GetTimeStringMS(content[15], state[task].averageTime / 1000);
      GetTimeStringHMS(content[16], state[task].totalTime);
      if (strcmp(state[task].tip, "运行中") == zero || strcmp(state[task].tip, "已停止") == zero)
      {
        sprintf_s(content[12], "%d/%d（%s）", state[task].games, state[task].maxGames, state[task].tip);//任务进度
        switch (state[task].wave)//关卡状态
        {
        case waveOutOfLevel:
          sprintf_s(content[13], "未进入关卡");
          break;
        case waveLoot:
          sprintf_s(content[13], "战利品");
          break;
        case waveGrade:
          sprintf_s(content[13], "结算");
          break;
        case waveFlip:
          sprintf_s(content[13], "翻牌");
          break;
        case waveNoImage:
          sprintf_s(content[13], "无图像");
          break;
        case waveTower:
          sprintf_s(content[13], "魔塔选层");
          break;
        case wavePet:
          sprintf_s(content[13], "宠塔选层");
          break;
        case wavePeak:
          sprintf_s(content[13], "巅峰对决");
          break;
        default:
          sprintf_s(content[13], "第%d.%d波",
            state[task].wave, state[task].smallWave);
          break;
        }
      }
      else if (strcmp(state[task].tip, "已完成") == zero)
      {
        sprintf_s(content[12], "%d/%d（已完成）", state[task].maxGames, state[task].maxGames);//任务进度
        sprintf_s(content[13], "未进入关卡");
      }
      else //中断的各种情形
      {
        sprintf_s(content[12], "%d/%d（已中断）", state[task].games, state[task].maxGames);//任务进度
        sprintf_s(content[13], "%s", state[task].tip);
      }
    }

    /*显示对应内容*/
    size = GetFontSize();

    settextcolor(taskColor[task]);
    setlinecolor(taskColor[task]);
    /*隐藏和显示按钮*/
    for (int i = -1; i <= 1; i++)
      CenterRectangle(parameterX + (2 * task + 1) * parameterWidth / 2 + 70 * i, parameterY + 23 * parameterHeight / 2, 30, 15);
    CenterView(startString, parameterX + (2 * task + 1) * parameterWidth / 2 - 70, parameterY + (2 * 11 + 1) * parameterHeight / 2);
    CenterView(hideString[0], parameterX + (2 * task + 1) * parameterWidth / 2, parameterY + (2 * 11 + 1) * parameterHeight / 2);
    CenterView(hideString[1], parameterX + (2 * task + 1) * parameterWidth / 2 + 70, parameterY + (2 * 11 + 1) * parameterHeight / 2);

    SetFontSize(18);
    if (monitor[task].isExist)
      settextcolor(taskColor[task]);
    else
      settextcolor(RGB(128, 128, 128));
    CenterView("info", parameterX + (task + 1) * parameterWidth - 19, parameterY + parameterHeight / 2);
    SetFontSize(size);

    for (int i = 0; i <= 16; i++)
    {
      if (i <= 11)
        settextcolor(taskColor[task]);
      else
        settextcolor(stateColor[task]);

      if (i == zero || (i >= 3 && i <= 10))
        CenterView(content[i], parameterX + (2 * task + 1) * parameterWidth / 2, parameterY + (2 * i + 1) * parameterHeight / 2);
      else if (i >= 12)
        CenterView(content[i], parameterX + (2 * task + 1) * parameterWidth / 2, parameterY + (2 * i + 1) * parameterHeight / 2 + 2);

      if (i >= 5 && i <= 7)
      {
        CenterView(doubleOption[0][i - 5], parameterX + (2 * task + 1) * parameterWidth / 2 - 45, parameterY + (2 * i + 1) * parameterHeight / 2);
        CenterView(doubleOption[1][i - 5], parameterX + (2 * task + 1) * parameterWidth / 2 + 45, parameterY + (2 * i + 1) * parameterHeight / 2);
      }
      if (i == 10)
      {
        CenterView(gameString[0], parameterX + (2 * task + 1) * parameterWidth / 2 - 53, parameterY + (2 * i + 1) * parameterHeight / 2);
        CenterView(gameString[1], parameterX + (2 * task + 1) * parameterWidth / 2 + 53, parameterY + (2 * i + 1) * parameterHeight / 2);
      }

      if (i >= 5 && i <= 7)
      {
        SetFontSize(16);
        CenterView("▲", parameterX + (task + 1) * parameterWidth - 10, parameterY + (2 * i + 1) * parameterHeight / 2 - 7);
        CenterView("▼", parameterX + (task + 1) * parameterWidth - 10, parameterY + (2 * i + 1) * parameterHeight / 2 + 10);
        CenterView("▲", parameterX + task * parameterWidth + 10, parameterY + (2 * i + 1) * parameterHeight / 2 - 7);
        CenterView("▼", parameterX + task * parameterWidth + 10, parameterY + (2 * i + 1) * parameterHeight / 2 + 10);
        SetFontSize(size);
      }
      if (i >= 8 && i <= 9)
      {
        PaintTargetCursor(parameterX + task * parameterWidth + 12, parameterY + (2 * i + 1) * parameterHeight / 2);
        if ((i == 8 && param[task].tag[0].x != -1) || (i == 9 && param[task].tag[1].x != -1))
          CenterView("×", parameterX + (task + 1) * parameterWidth - 10, parameterY + (2 * i + 1) * parameterHeight / 2);
      }
      if ((i == 1 && strlen(param[task].track[0]) > 0) || (i == 2 && strlen(param[task].track[1]) > 0))
      {
        CenterView("×", parameterX + (task + 1) * parameterWidth - 10, parameterY + (2 * i + 1) * parameterHeight / 2);
        CenterView("↑", parameterX + task * parameterWidth + 7, parameterY + (2 * i + 1) * parameterHeight / 2 + 1);
        CenterView("↓", parameterX + task * parameterWidth + 12, parameterY + (2 * i + 1) * parameterHeight / 2 + 1);
      }
      if (i == 12)
        if (isTaskStarted[task] == zero)//任务未运行时显示×，运行时不可删除
          if (strcmp(state[task].tip, "未开始") != zero)
            CenterView("×", parameterX + (task + 1) * parameterWidth - 10, parameterY + (2 * i + 1) * parameterHeight / 2 + 2);
    }
  }
  setlinecolor(grayWhite);
}
//显示高级任务参数
void ViewAdvanceParameter()
{
  int list = curList;
  int task = GetTask(list);
  const int ParameterNum = 10;
  //参数标题
  char title[ParameterNum][13] = { "", "1P大厅", "2P大厅", "游戏句柄", "操作", "任务进度",
    "关卡状态", "本局用时", "平均用时", "累计用时" };
  char content[ParameterNum][30] = {};//参数显示内容
  char startString[7], hideString[2][7];//启动 隐藏
  char gameString[2][13];//两个账号的大厅句柄

  //填写参数信息

  //任务标题
  sprintf_s(content[0], param[task].title);

  //大厅标题
  for (int account = 0; account < 2; account++)
  {
    char hallName[maxPath] = {};
    strcpy_s(hallName, param[task].hallName[account]);
    char *bar = strstr(hallName, "|");
    if (bar)
      *bar = 0;
    Truncate(content[1 + account], hallName, 170);
  }

  //账号标签
  sprintf_s(content[3], "|");
  for (int account = 0; account < 2; account++)
    if (param[task].tag[account].x == -1) //无标签
      sprintf_s(gameString[account], "");
    else if (param[task].tag[account].x == 0 && param[task].tag[account].y == 0) //已绑定游戏窗口
    {
      param[task].isWindow[account] = IsWindow(param[task].hWnd[account]) != 0;
      if (param[task].isWindow[account])
        sprintf_s(gameString[account], "%d", (int)param[task].hWnd[account]);
      else
        sprintf_s(gameString[account], "--");
    }
    else
    {
      param[task].isWindow[account] = IsWindow(param[task].hWnd[account]) != 0;
      if (param[task].isWindow[account])
        sprintf_s(gameString[account], "%d", (int)param[task].hWnd[account]);
      else
        sprintf_s(gameString[account], "%d,%d", param[task].tag[account].x, param[task].tag[account].y);
    }

  sprintf_s(content[4], "");//操作

  /*填写状态信息*/
  if (strcmp(state[task].tip, "未开始") == zero)
    for (int i = 5; i < 10; i++)
      strcpy_s(content[i], "");
  else //已经启动则进行记录
  {
    GetTimeStringMS(content[7], state[task].currentTime);
    GetTimeStringMS(content[8], state[task].averageTime / 1000);
    GetTimeStringHMS(content[9], state[task].totalTime);
    if (strcmp(state[task].tip, "运行中") == zero || strcmp(state[task].tip, "已停止") == zero)
    {
      sprintf_s(content[5], "%d/%d（%s）", state[task].games, state[task].maxGames, state[task].tip);//任务进度
      switch (state[task].wave)//关卡状态
      {
      case waveOutOfLevel:
        sprintf_s(content[6], "未进入关卡");
        break;
      case waveLoot:
        sprintf_s(content[6], "战利品");
        break;
      case waveGrade:
        sprintf_s(content[6], "结算");
        break;
      case waveFlip:
        sprintf_s(content[6], "翻牌");
        break;
      case waveNoImage:
        sprintf_s(content[6], "无图像");
        break;
      case waveTower:
        sprintf_s(content[6], "魔塔选层");
        break;
      case wavePet:
        sprintf_s(content[6], "宠塔选层");
        break;
      case wavePeak:
        sprintf_s(content[6], "巅峰对决");
        break;
      default:
        sprintf_s(content[6], "第%d.%d波", state[task].wave, state[task].smallWave);
        break;
      }
    }
    else if (strcmp(state[task].tip, "已完成") == zero)
    {
      sprintf_s(content[5], "%d/%d（已完成）", state[task].maxGames, state[task].maxGames);//任务进度
      sprintf_s(content[6], "未进入关卡");
    }
    else //中断的各种情形
    {
      sprintf_s(content[5], "%d/%d（已中断）", state[task].games, state[task].maxGames);//任务进度
      sprintf_s(content[6], "%s", state[task].tip);
    }
  }

  if (plot[list].isAdvanceStarted == 0)
    sprintf_s(startString, "启动");
  else
    sprintf_s(startString, "停止");

  for (int account = 0; account < 2; account++)
    if (param[task].hideInfo[account].hidden)
      sprintf_s(hideString[account], "显示%d", account + 1);
    else
      sprintf_s(hideString[account], "隐藏%d", account + 1);

  setlinecolor(grayWhite);
  PaintButtonGroupGrid(buttonParameterTitle[1]);
  PaintButtonGroupGrid(buttonParameter[1]);
  PaintButtonGroupGrid(buttonStateTitle[1]);
  PaintButtonGroupGrid(buttonState[1]);
  settextcolor(grayWhite);

  int size = GetFontSize();
  setlinecolor(taskColor[list]);
  settextcolor(taskColor[list]);
  /*隐藏和显示按钮*/
  for (int i = -1; i <= 1; i = i++)
    CenterRectangle(parameterX + parameterWidth / 2 + 70 * i, parameterY + 23 * parameterHeight / 2, 30, 15);
  CenterView(startString, parameterX + parameterWidth / 2 - 70, parameterY + (2 * 11 + 1) * parameterHeight / 2);
  CenterView(hideString[0], parameterX + parameterWidth / 2, parameterY + (2 * 11 + 1) * parameterHeight / 2);
  CenterView(hideString[1], parameterX + parameterWidth / 2 + 70, parameterY + (2 * 11 + 1) * parameterHeight / 2);

  SetFontSize(18);
  if (monitor[task].isExist)
    settextcolor(taskColor[list]);
  else
    settextcolor(RGB(128, 128, 128));
  CenterView("info", parameterX + parameterWidth - 19, parameterY + 13 * parameterHeight / 2);
  SetFontSize(size);

  //显示参数和状态内容
  for (int i = 0; i < ParameterNum; i++)
  {
    if (i < 5) //参数部分
    {
      settextcolor(grayWhite);
      PrintButton(buttonParameterTitle[1], 0, i, title[i]); //参数标题
      settextcolor(taskColor[list]);
      PrintButton(buttonParameter[1], 0, i, content[i]); //参数内容
    }
    else //状态部分
    {
      settextcolor(grayWhite);
      PrintButton(buttonStateTitle[1], 0, i - 5, title[i]); //状态标题
      settextcolor(stateColor[list]);
      PrintButton(buttonState[1], 0, i - 5, content[i]); //状态内容
    }

    if (i == 0) //高级任务切换按钮
      PaintHorizontalAdjust(FindButton(buttonParameter[1], 0, 0));
    else if (i == 1 || i == 2) //1P/2P标签左侧的图标和右侧的×
    {
      PaintTargetCursor(parameterX + 12, parameterY + (2 * i + 15) * parameterHeight / 2);
      if ((i == 1 && param[task].tag[0].x != -1) || (i == 2 && param[task].tag[1].x != -1))
        CenterView("×", parameterX + parameterWidth - 10,
          parameterY + (2 * i + 15) * parameterHeight / 2);
    }
    else if (i == 3) //大厅句柄
    {
      CenterView(gameString[0], parameterX + parameterWidth / 2 - 53,
        parameterY + (2 * i + 15) * parameterHeight / 2);
      CenterView(gameString[1], parameterX + parameterWidth / 2 + 53,
        parameterY + (2 * i + 15) * parameterHeight / 2);
    }
    else if (i == 5) //任务进度右侧的×
    {
      if (isTaskStarted[task] == zero)//任务未运行时显示×，运行时不可删除
        if (strcmp(state[task].tip, "未开始") != zero)
          CenterView("×", parameterX + parameterWidth - 10,
            parameterY + (2 * i + 15) * parameterHeight / 2 + 2);
    }

  }
  setlinecolor(grayWhite);
}
//显示缩放按钮
void ViewZoom()
{
  settextcolor(grayWhite);
  PaintHorizontalAdjust(zoomX, zoomY, zoomWidth, zoomHeight); //显示缩放比例
  char zoomString[10];
  sprintf_s(zoomString, "%d %%", zoom);
  CenterView(zoomString, zoomX + zoomWidth / 2, zoomY + zoomHeight / 2);
}
//画勾选框
void PaintCheckBox(int buttonClassCode, int row, int column, int offsetX)
{
  int code0 = FindButton(buttonClassCode, row, column);
  PaintCheckBox(buttons[code0].x, buttons[code0].y, buttons[code0].width,
    buttons[code0].height, offsetX);
}
//画勾选框的√
void PaintCheck(int buttonClassCode, int row, int column, int offsetX)
{
  int buttonCode = buttonGroup[buttonClassCode].firstButton
    + row * buttonGroup[buttonClassCode].column + column;
  MyRect button = buttons[buttonCode];
  PaintCheck(button.x, button.y, button.width, button.height, offsetX);
}
//显示右侧设置按钮
void ViewSetting()
{
  const int MaxSettingsNum = 5; //设置按钮个数
  char upperSettings[3][20] = { "全部停止", "后台运行", "一键报错" };
  char settingsString[2][2 * MaxSettingsNum][20] = {
    { "带卡选择", "", "翻牌数量", "", "背包满时", "", "经验满时", "", "补阵强度", "" },
    { "自动还原", "", "自动刷新", "", "操作", "加速", "一键配轨", "检查任务" } };

  setlinecolor(grayWhite);
  settextcolor(grayWhite);
  for (int i = 0; i <= 2; i++) //"全部停止", "后台运行", "一键报错" 
    CenterView(upperSettings[i], settingsX + settingsWidth / 2, upperSettingsY + (2 * i + 1) * settingsHeight / 2);
  PaintGrid(3, 1, settingsX, upperSettingsY, settingsWidth, settingsHeight);//按钮
  ViewZoom();//显示缩放按钮

  int SettingsNum = 0;
  if (mode == 1)
  {
    SettingsNum = 5;

    if (customOption == 0) //带卡选择
      sprintf_s(settingsString[0][1], "最高星");
    else if (customOption == 1)
      sprintf_s(settingsString[0][1], "最靠前");

    sprintf_s(settingsString[0][3], "%d", flipNum);//翻牌数量

    if (bagFullOperation == 1) //背包满时
      sprintf_s(settingsString[0][5], "继续");
    else
      sprintf_s(settingsString[0][5], "停止");

    if (expFullOperation == 1) //经验满时
      sprintf_s(settingsString[0][7], "继续");
    else
      sprintf_s(settingsString[0][7], "停止");

    if (repairLevel == 0) //补阵强度
      sprintf_s(settingsString[0][9], "0 %%");
    else
      sprintf_s(settingsString[0][9], "%d0 %%", repairLevel);

    PaintGrid(SettingsNum * 2, 1, settingsX, settingsY, settingsWidth, settingsHeight);//按钮
    for (int i = 0; i < SettingsNum * 2; i++) //显示文字
      CenterView(settingsString[0][i], settingsX + settingsWidth / 2, settingsY + (2 * i + 1) * settingsHeight / 2);
    for (int i = 1; i < SettingsNum * 2; i++) //显示箭头
      if (i % 2 == 1 || i == 10)
        PaintHorizontalAdjust(settingsX, settingsY + i * settingsHeight, settingsWidth, settingsHeight);

    int size = GetFontSize();
    SetFontSize(17);
    CenterView("以上设置也对", settingsX + settingsWidth / 2, settingsY + 21 * settingsHeight / 2 - 6);
    CenterView("高级任务生效", settingsX + settingsWidth / 2, settingsY + 21 * settingsHeight / 2 + 13);
    SetFontSize(size);
  }
  else if (mode == 2)
  {
    int list = curList;
    SettingsNum = 4;

    sprintf_s(settingsString[1][1], missionRecovery == 0 ? "关" : "开");

    if (maxRefreshTimes == 0) //自动刷新
      sprintf_s(settingsString[1][3], "关");
    else if (maxRefreshTimes == unlimitedRefreshTimes)
      sprintf_s(settingsString[1][3], "无限制");
    else
      sprintf_s(settingsString[1][3], "%d次", maxRefreshTimes);

    if (operationSpeed == 2) //操作速度
      sprintf_s(settingsString[1][4], "操作：快");
    else if (operationSpeed == 1)
      sprintf_s(settingsString[1][4], "操作：中");
    else
      sprintf_s(settingsString[1][4], "操作：慢");

    if (globalAccelarationTime == 0)
      sprintf_s(settingsString[1][5], "加速：关");
    else
      sprintf_s(settingsString[1][5], "加速：%d", globalAccelarationTime);

    PaintGrid(SettingsNum * 2, 1, settingsX, settingsY, settingsWidth, settingsHeight);//按钮
    for (int i = 0; i < SettingsNum * 2; i++) //显示文字
    {
      if (i == 3)
        LongCenterView(settingsString[1][i], settingsX + settingsWidth / 2,
          settingsY + (2 * i + 1) * settingsHeight / 2, settingsWidth - 5);
      else
        CenterView(settingsString[1][i], settingsX + settingsWidth / 2,
          settingsY + (2 * i + 1) * settingsHeight / 2);
    }
    for (int i = 1; i < (SettingsNum - 2) * 2; i = i + 2) //显示箭头
      PaintHorizontalAdjust(settingsX, settingsY + i * settingsHeight, settingsWidth, settingsHeight);

    //翻页按钮
    char pageString[10] = {};
    int totalAdvancePage = min(plot[list].advanceNum / advanceNumPerPage + 1, maxAdvancePage);
    sprintf_s(pageString, "%d/%d", plot[list].advancePage, totalAdvancePage);
    CenterView(pageString, settingsX + settingsWidth / 2, settingsY + 17 * settingsHeight / 2 + 2);
    PaintHorizontalAdjust(settingsX + settingsWidth / 2, settingsY + 17 * settingsHeight / 2 + 2, settingsWidth / 2);

    //撤销按钮
    PaintGrid(2, 1, settingsX, settingsY + 9 * settingsHeight + 2, settingsWidth, settingsHeight);
    CenterView("撤销", settingsX + settingsWidth / 2, settingsY + 19 * settingsHeight / 2 + 2);
    CenterView("恢复", settingsX + settingsWidth / 2, settingsY + 21 * settingsHeight / 2 + 2);
  }
}
//显示选项卡：普通任务和高级任务
void ViewMode()
{
  char *ModeString[2] = { "普通任务", "高级任务" };
  PaintGrid(2, 1, modeX, modeY, modeWidth, modeHeight);
  for (int i = 0; i <= 1; i++)
  {
    if (mode == i + 1)
      settextcolor(RGB(255, 0, 0));
    else
      settextcolor(grayWhite);
    CenterView(ModeString[i], modeX + modeWidth / 2, modeY + (2 * i + 1) * modeHeight / 2);
  }
}
//判断高级任务选中状态：0=无选中，1=部分选中，2=全部选中
int AdvanceSelectionStates(int list)
{
  int oneSelected = 0, allSelected = 1;
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    if (plot[list].advance[order].isSelected == 1)
      oneSelected = 1;
    else
      allSelected = 0;
  }
  if (oneSelected == 0)
    return 0;
  if (allSelected == 1)
    return 2;
  return 1;
}
//列表path是否为识别任务预制列表
int IsMissionList(const char *path)
{
  if (strcmp(path, "预制列表\\公会任务.txt") == 0)
    return GUILD;
  if (strcmp(path, "预制列表\\情侣任务.txt") == 0)
    return LOVER;
  char contestListPath[maxPath] = {};
  const char roleString[3][10] = { "", "单人", "双人" };
  for (int issue = 0; issue < tournamentNum; issue++)
    for (int roleNum = 1; roleNum <= 2; roleNum++)
    {
      sprintf_s(contestListPath, "预制列表\\美食大赛_%s期_%s.txt", contestName[issue], roleString[roleNum]);
      if (strcmp(path, contestListPath) == 0)
        return CONTEST;
    }
  return 0;
}
//高级任务list的当前列表是否为预制列表
int IsMissionList(int list)
{
  return IsMissionList(plot[list].advancePath);
}

//显示高级任务列表
void ViewList()
{
  int list = curList;
  const int tickBoxOffsetX = -17, idOffsetX = 14;
  const int trackWidth = parameterWidth - 40;
  char content[advanceTitleNum][100] = {};
  char title[advanceTitleNum][13] = { "", "类型", "关卡", "局数", "钥匙", "房主", "1P轨道", "卡组", "2P轨道", "卡组" };
  //当前打开的是否为公会任务或情侣任务列表
  bool isMissionList = IsMissionList(list) != 0;

  //给识别任务的ID涂色
  for (int orderInPage = 0; orderInPage < advanceNumPerPage; orderInPage++)
  {
    int order = (plot[list].advancePage - 1) * advanceNumPerPage + orderInPage;
    if (order >= plot[list].advanceNum)
      break;
    if (plot[list].advance[order].missionStyle > 0)
    {
      setfillcolor(missionStyleColor[plot[list].advance[order].missionStyle - 1]);
      MyRect &button = buttons[FindButton(buttonAdvance, orderInPage + 1, 0)];
      solidrectangle(button.x, button.y, button.x + button.width, button.y + button.height);
    }
  }
  setfillcolor(defaultFillColor);

  setlinecolor(grayWhite);
  PaintButtonGroupGrid(buttonAdvance, 1, advanceNumPerPage + 1, 0);//绘制按钮格子

  settextcolor(grayWhite);
  for (int i = 0; i < advanceTitleNum; i++)//显示标题
    PrintButton(buttonAdvance, 0, i, title[i]);

  int Code0 = FindButton(buttonAdvance, 0, 0);
  PaintCheckBox(buttonAdvance, 0, 0, tickBoxOffsetX);//画全选框

  switch (AdvanceSelectionStates(list)) //全选框状态
  {
  case 2://全部选中
    PaintCheck(buttonAdvance, 0, 0, tickBoxOffsetX);//画全选框
    break;
  case 1://部分选中
    setfillcolor(grayWhite);
    fillrectangle(buttons[Code0].x + advanceWidth[0] / 2 + tickBoxOffsetX - 5, buttons[Code0].y + 15,
      buttons[Code0].x + advanceWidth[0] / 2 + tickBoxOffsetX + 5, buttons[Code0].y + 25);
    setfillcolor(defaultFillColor);
  }

  PrintButton(buttonAdvance, 0, 0, "ID", idOffsetX, 0);
  if (plot[list].advanceNum < maxAdvanceNum)
  {
    int newButtonPageOrder = plot[list].advanceNum - (plot[list].advancePage - 1) * advanceNumPerPage;
    if (newButtonPageOrder >= 0 && newButtonPageOrder < advanceNumPerPage)
    {
      if (plot[list].advanceSelected == 0)
        PrintButton(buttonAdvance, newButtonPageOrder + 1, 0, "【新增】");
      else
        PrintButton(buttonAdvance, newButtonPageOrder + 1, 0, "【复制】");
    }
  }
  //显示高级任务文字
  for (int pageOrder = 0; pageOrder < advanceNumPerPage; pageOrder++)
  {
    int order = (plot[list].advancePage - 1) * advanceNumPerPage + pageOrder;
    if (order >= plot[list].advanceNum)
      break;
    if (plot[list].advanceSelected - 100 == order)
      settextcolor(RGB(255, 0, 0));
    else if (plot[list].advance[order].isSelected == 1)
    {
      if (plot[list].advance[order].type < basicTypeNum)
        settextcolor(taskColor[plot[list].advance[order].type / 2]);
      else if (plot[list].advance[order].type < basicTypeNum + 6) //主线：BasicTypeNum ~ BasicTypeNum + 5
        settextcolor(taskColor[3]);
      else if (plot[list].advance[order].type < basicTypeNum + 11) //副本：BasicTypeNum + 6 ~ BasicTypeNum + 10
        settextcolor(taskColor[4]);
      else
        settextcolor(taskColor[5]);
    }
    else
      settextcolor(RGB(144, 144, 144));

    //显示轨道路径
    bool isGray[2] = {};//是否使用灰色显示轨道和卡组
    for (int account = 0; account < 2; account++)
    {
      isGray[account] = plot[list].advance[order].deck[account] == 0;
      bool gray = IsNoDeck(plot[list].advance[order]) || isGray[account];//除定时时刻以外的灰色显示规则
      int contentOrder = TRACK_0 + 2 * account;
      if (IsNoTrackParam(plot[list].advance[order])) //没有轨道参数
      {
        if (IsTimer(plot[list].advance[order]) && account == 0) //定时：显示定时时刻
        {
          char timerString[10] = {};
          GetTimerString(timerString, plot[list].advance[order].timer);
          PrintButton(buttonAdvance, pageOrder + 1, contentOrder, timerString, 0, 0);
        }
        else if (IsSign(plot[list].advance[order]) && account == 0) //签到：显示温馨id
        {
          int harmonyNum = 0;
          for (int i = 0; i < maxHarmonySize; i++)
            if (strlen(plot[list].harmony[i]) > 0)
              harmonyNum++;
          char harmonyString[50] = "点此设置温馨礼包";
          if (harmonyNum > 0)
            sprintf_s(harmonyString, "%d个温馨礼包", harmonyNum);
          PrintButton(buttonAdvance, pageOrder + 1, contentOrder, harmonyString, 0, 0);
        }
        else //其他：显示"-"
          PrintButton(buttonAdvance, pageOrder + 1, contentOrder, "-", 0, 0, gray);
      }
      else //有轨道参数：显示轨道
        ViewTrack(plot[list].advance[order].track[account], buttonAdvance, pageOrder + 1, contentOrder, gray);
    }

    for (int i = 0; i < advanceTitleNum; i++)//清空显示内容
      sprintf_s(content[i], "");

    //ID、类型、关卡
    strcpy_s(content[ID], GetID(list, order));//ID
    sprintf_s(content[TYPE], "%s", typeName[plot[list].advance[order].type]);//类型
    GetLevelName(content[LEVEL], plot[list].advance[order].type, plot[list].advance[order].level);//关卡

    //局数
    if (IsNoGames(plot[list].advance[order]))
      sprintf_s(content[GAMES], "-");
    else
      sprintf_s(content[GAMES], "%d", plot[list].advance[order].maxGames);

    //钥匙
    if (IsNoTrack(plot[list].advance[order]))
      sprintf_s(content[KEY], "-");
    else
      sprintf_s(content[KEY], "%s", plot[list].advance[order].isProceed ? "√" : "");

    //房主
    if (IsNoBattle(plot[list].advance[order]))
      strcpy_s(content[HOST], "-");
    else if (plot[list].advance[order].host == 2 || plot[list].advance[order].dual)
      strcpy_s(content[HOST], "同步");
    else
      sprintf_s(content[HOST], "%dP", plot[list].advance[order].host + 1);

    //显示卡组
    for (int account = 0; account < 2; account++)
    {
      int contentOrder = TRACK_0 + 2 * account;
      sprintf_s(content[contentOrder], "");

      if (IsNoDeck(plot[list].advance[order])) //无需卡组的任务
        strcpy_s(content[contentOrder + 1], "-");
      else
      {
        if (plot[list].advance[order].deck[account] == 0)
          sprintf_s(content[contentOrder + 1], "×");
        else
        {
          if (isMissionList) //公会任务列表显示为+n
            sprintf_s(content[contentOrder + 1], "+%d", plot[list].advance[order].deck[account] - 1);
          else if (IsNoBattle(plot[list].advance[order])) //操作任务
            strcpy_s(content[contentOrder + 1], "√");
          else
            sprintf_s(content[contentOrder + 1], "%d", plot[list].advance[order].deck[account]);
        }
      }
    }

    PaintCheckBox(buttonAdvance, pageOrder + 1, 0, tickBoxOffsetX);

    //显示序号和勾选框
    if (plot[list].advance[order].isSelected == 1)
      PaintCheck(buttonAdvance, pageOrder + 1, 0, tickBoxOffsetX);
    PrintButton(buttonAdvance, pageOrder + 1, 0, content[0], idOffsetX, 0);

    //显示第1-8项文字：1类型 2关卡 3局数 4钥匙 5房主 6轨道1 7卡组1 8轨道2 9卡组2
    for (int i = 1; i < advanceTitleNum; i++)
    {
      int account = (i - TRACK_0) / 2; //i = 6, 7对应account = 0，i = 8, 9对应account = 1
      int dx = 0, dy = 0;
      bool gray = false;
      if (i == TYPE || i == DECK_0 || i == DECK_1) //类型 卡组
        dx = -6;
      if (i == TRACK_0 || i == TRACK_1)//未使用的轨道变灰
        gray = isGray[account];
      if (i == DECK_0 || i == DECK_1) //未使用的卡组变灰
        gray = IsNoDeck(plot[list].advance[order]) || isGray[account];
      PrintButton(buttonAdvance, pageOrder + 1, i, content[i], dx, dy, gray);
    }

    //显示调节、删除轨道、交换轨道符号
    int size = GetFontSize();
    for (int i = 1; i < advanceTitleNum; i++)
    {
      int account = (i - TRACK_0) / 2; //i = 6, 7对应account = 0，i = 8, 9对应account = 1
      //无轨任务不显示轨道删除交换符号
      if (!(IsNoTrackParam(plot[list].advance[order]) && (i == TRACK_0 || i == TRACK_1)))
      {
        if (i == 1)
        {
          SetFontSize(16);
          PrintButton(buttonAdvance, pageOrder + 1, i, "▲", advanceWidth[i] / 2 - 10, -7);
          PrintButton(buttonAdvance, pageOrder + 1, i, "▼", advanceWidth[i] / 2 - 10, 10);
          SetFontSize(size);
        }
        if (i == DECK_0 || i == DECK_1)
        {
          SetFontSize(16);
          bool gray = IsNoDeck(plot[list].advance[order]) || isGray[account];
          PrintButton(buttonAdvance, pageOrder + 1, i, "▲", advanceWidth[i] / 2 - 10, -7, gray);
          PrintButton(buttonAdvance, pageOrder + 1, i, "▼", advanceWidth[i] / 2 - 10, 10, gray);
          SetFontSize(size);
        }
        if ((i == TRACK_0 && strlen(plot[list].advance[order].track[0])) || (i == TRACK_1 && strlen(plot[list].advance[order].track[1])))
        {
          PrintButton(buttonAdvance, pageOrder + 1, i, "×", advanceWidth[i] / 2 - 10, 0, isGray[account]);
          PrintButton(buttonAdvance, pageOrder + 1, i, "↑", -advanceWidth[i] / 2 + 7, 1, isGray[account]);
          PrintButton(buttonAdvance, pageOrder + 1, i, "↓", -advanceWidth[i] / 2 + 12, 1, isGray[account]);
        }
      }
    }
    SetFontSize(16);

    //绘制第2项“关卡”的调节按钮（与任务类型有关）

    //右侧调节按钮
    if (plot[list].advance[order].type == 0 || plot[list].advance[order].type == 1 || plot[list].advance[order].type == 3 ||
      plot[list].advance[order].type == 4 || plot[list].advance[order].type == 5 || IsNoBattle(plot[list].advance[order]))
    {
      PrintButton(buttonAdvance, pageOrder + 1, 2, "▲", advanceWidth[2] / 2 - 10, -7);
      PrintButton(buttonAdvance, pageOrder + 1, 2, "▼", advanceWidth[2] / 2 - 10, 10);
    }
    if (plot[list].advance[order].type == 3)//左侧调节按钮
    {
      PrintButton(buttonAdvance, pageOrder + 1, 2, "▲", -advanceWidth[2] / 2 + 10, -7);
      PrintButton(buttonAdvance, pageOrder + 1, 2, "▼", -advanceWidth[2] / 2 + 10, 10);
    }
    SetFontSize(size);
  }
}
//绘制高级任务界面
void RepaintAdvance()
{
  BeginBatchDraw();
  solidrectangle(0, 0, wndWidth, wndHeight);
  setlinecolor(grayWhite);
  ViewMode();
  ViewOutput();
  ViewAdvanceParameter();
  ViewList();
  ViewSetting();
  ViewTopInfo();//显示顶部信息
  ViewLastTip();//显示最近提示信息
  EndBatchDraw();
  /*备份图像，减少重绘占用*/
  setaspectratio(1, 1);
  getimage(&imageBackup, 0, 0, (int)(wndWidth * ratio), (int)(wndHeight * ratio));
  setaspectratio(ratio, ratio);
}
void RepaintParameter()//绘制参数界面
{
  /*界面：网格，标题，参数，控制按钮*/
  BeginBatchDraw();
  solidrectangle(0, 0, wndWidth, wndHeight);
  setlinecolor(grayWhite);
  ViewMode();
  PaintGrid(12, 1, titleX, titleY, titleWidth, titleHeight);//选项栏
  PaintGrid(12, 5, parameterX, parameterY, parameterWidth, parameterHeight);//参数内容
  PaintGrid(5, 1, titleX, titleY + 12 * titleHeight + 2, titleWidth, titleHeight);//选项栏
  PaintGrid(5, 5, parameterX, parameterY + 12 * parameterHeight + 2, parameterWidth, parameterHeight);//参数内容
  ViewParameter();

  ViewSetting();
  ViewTopInfo();//显示顶部信息
  ViewLastTip();//显示最近提示信息
  EndBatchDraw();
  /*备份图像，减少重绘占用*/
  setaspectratio(1, 1);
  getimage(&imageBackup, 0, 0, (int)(wndWidth * ratio), (int)(wndHeight * ratio));
  setaspectratio(ratio, ratio);
}
void RepaintOld()
{
  BeginBatchDraw();
  solidrectangle(0, 0, wndWidth, wndHeight - tipHeight - 1);
  setaspectratio(1, 1);
  putimage(0, 0, (int)(wndWidth * ratio), (int)((wndHeight - tipHeight - 1) * ratio), &imageBackup, 0, 0);
  setaspectratio(ratio, ratio);
  EndBatchDraw();
}
//适应屏幕分辨率启动画图窗口
HWND InitGraphDPI()
{
  int screenWidth = GetSystemMetrics(SM_CXSCREEN);
  int screenHeight = GetSystemMetrics(SM_CYSCREEN);
  originRatio = min((float)screenWidth / (wndWidth + 20), (float)screenHeight / 864);
  ratio = originRatio * zoom / 100;//改变窗口尺寸和显示比例，不影响点击位置判定
  HWND hWnd = initgraph((int)(wndWidth * ratio), (int)(wndHeight * ratio));
  setaspectratio(ratio, ratio);
  imageBackup.Resize((int)(wndWidth * ratio), (int)(wndHeight * ratio));
  return hWnd;
}
//删除账号account的标签
void DeleteTag(int task, int account)
{
  if (param[task].hideInfo[account].hidden)
  {
    MessageBox(hWndActuator, "请先显示原来的游戏窗口", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }
  param[task].tag[account].x = -1;
  param[task].tag[account].y = -1;
  param[task].hWnd[account] = nullptr;
  memset(param[task].hallName[account], 0, sizeof(param[task].hallName[account]));
  if (IsDual1P(task)) //高级任务连带删除2P任务的句柄
  {
    param[task + 1].tag[account] = param[task].tag[account];
    param[task + 1].hWnd[account] = param[task].hWnd[account];
    strcpy_s(param[task + 1].hallName[account], param[task].hallName[account]);
    param[task + 1].hideInfo[account] = param[task].hideInfo[account];//拷贝隐藏状态
  }
}
//是否为大厅或微端句柄
bool IsHallClass(const char *className)
{
  return strcmp(className, "ApolloRuntimeContentWindow") == 0 || strcmp(className, "DUIWindow") == 0;
}
//是否为适应版本的游戏大厅
bool IsHall(HWND hWndHall)
{
  if (!IsWindow(hWndHall))
    return false;
  char className[256];
  GetClassNameA(hWndHall, className, sizeof(className));
  if (strcmp(className, "DUIWindow") != 0)
    return false;

  //验证1-3级子窗口是否存在
  HWND hWnd1 = FindWindowExA(hWndHall, NULL, "TabContentWnd", NULL); //1级子窗口
  if (!hWnd1)
    return false;
  HWND hWnd2 = FindWindowEx(hWnd1, NULL, "CefBrowserWindow", NULL); //2级子窗口
  if (!hWnd2)
    return false;
  HWND hWnd3 = FindWindowEx(hWnd2, NULL, "Chrome_WidgetWin_0", NULL); //3级子窗口
  if (!hWnd3)
    return false;
  return true;
}
//是否为微端
bool IsMicroEnd(HWND hWndHall)
{
  if (!IsWindow(hWndHall))
    return false;
  char className[256];
  GetClassNameA(hWndHall, className, sizeof(className));
  return strcmp(className, "ApolloRuntimeContentWindow") == 0;
}
//判断大厅是否可用。0=可用，1=大厅版本错误，2=非微端或大厅（不可用）
int IsHallAvaliable(HWND hWndHall)
{
  //大厅不存在，返回2
  if (!IsWindow(hWndHall))
    return 2;

  char className[256];
  GetClassNameA(hWndHall, className, sizeof(className));
  //hWndHall为微端，判定为可用
  if (strcmp(className, "ApolloRuntimeContentWindow") == 0)
    return 0;
  //hWndHall为大厅，需要验证1-3级子窗口是否存在
  if (strcmp(className, "DUIWindow") == 0)
  {
    HWND hWnd1 = FindWindowExA(hWndHall, NULL, "TabContentWnd", NULL); //1级子窗口
    if (!hWnd1)
      return 1;
    HWND hWnd2 = FindWindowEx(hWnd1, NULL, "CefBrowserWindow", NULL); //2级子窗口
    if (!hWnd2)
      return 1;
    HWND hWnd3 = FindWindowEx(hWnd2, NULL, "Chrome_WidgetWin_0", NULL); //3级子窗口
    if (!hWnd3)
      return 1;
    return 0;
  }
  return 2; //其他窗口不可用
}
//抓取账号account的标签
void GrabTag(int task, int account)
{
  if (param[task].hideInfo[account].hidden)
  {
    MessageBox(hWndActuator, "请先显示原来的游戏窗口", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }
  SetSystemCursor(crossCursor, (DWORD)IDC_ARROW);//将箭头和十字光标替换
  while (GetAsyncKeyState(VK_LBUTTON))//等待鼠标左键松开
    Sleep(10);
  SetSystemCursor(crossCursor, (DWORD)IDC_ARROW);//光标恢复正常

  POINT locPointed;//抓取的位置s
  GetCursorPos(&locPointed);
  HWND hWndPointed = WindowFromPoint(locPointed);//记录抓取的窗口句柄
  char className[256];
  GetClassName(hWndPointed, className, 256);//获取窗口类名

  HWND hWndHall = nullptr;
  int HallResult = IsHallAvaliable(hWndPointed);

  //微端或正确版本大厅
  if (HallResult == 0)
  {
    RECT hallRect;
    GetWindowRect(hWndPointed, &hallRect);//获取大厅位置
    if (locPointed.y < 1)
      locPointed.y = 1;
    int x = locPointed.x - hallRect.left;
    int y = locPointed.y - hallRect.top;
    //记录相对坐标，但最少是1
    param[task].tag[account].x = x >= 1 ? x : 1;
    param[task].tag[account].y = y >= 1 ? y : 1;

    int tagNum = GetTagNum(hWndPointed);//获取大厅标签数量
    if (tagNum == 1) //只有1个标签
      param[task].hWnd[account] = GetActiveGameWindow(hWndPointed);//绑定唯一游戏窗口
    else
      param[task].hWnd[account] = nullptr; //游戏窗口无法确定，填写为NULL
    hWndHall = hWndPointed; //大厅窗口就是抓取的窗口
  }
  //错误版本大厅
  else if (HallResult == 1)
  {
    PopMessage(hWndActuator, "游戏大厅版本不适配，\n请使用群文件的游戏大厅安装包。");
    return;
  }
  //游戏窗口
  else if (strcmp(className, "NativeWindowClass") == 0 || strcmp(className, "WebPluginView") == 0)
  {
    hWndHall = GetHallWindow(hWndPointed); //寻找大厅窗口
    if (!hWndHall) //如果从游戏窗口找不到大厅窗口，说明版本不适配
    {
      PopMessage(hWndActuator, "游戏大厅版本不适配，\n请使用群文件的游戏大厅安装包。");
      return;
    }
    param[task].hWnd[account] = hWndPointed; //游戏窗口就是抓取的窗口
    param[task].tag[account].x = 0;
    param[task].tag[account].y = 0;
  }
  //选服窗口
  else if (strcmp(className, "Chrome_RenderWidgetHostHWND") == 0)
  {
    param[task].hWnd[account] = GetGameWindowFromServer(hWndPointed); //寻找游戏窗口
    hWndHall = GetHallWindow(hWndPointed); //寻找大厅窗口
    param[task].tag[account].x = 0;
    param[task].tag[account].y = 0;
  }
  else //都不是
  {
    PopMessage(hWndActuator, "请抓取大厅账号标签、微端标题栏或游戏画面。");
    return;
  }
  GetWindowTextA(hWndHall, param[task].hallName[account], sizeof(param[task].hallName[account]));//记录大厅标题
  GetWindowTextA(GetMainHallWindow(hWndHall), param[task].mainName, sizeof(param[task].mainName));//记录主窗口标题
  if (IsDual1P(task)) //高级任务把抓得的句柄同步到2P任务
  {
    param[task + 1].tag[account] = param[task].tag[account];
    param[task + 1].hWnd[account] = param[task].hWnd[account];
    strcpy_s(param[task + 1].hallName[account], param[task].hallName[account]);
    strcpy_s(param[task + 1].mainName, param[task].mainName);
    param[task + 1].hideInfo[account] = param[task].hideInfo[account];//拷贝隐藏状态
  }
}
//隐藏或显示窗口
void HideOrShow(int task, int account)
{
  if (!param[task].hideInfo[account].hidden) //隐藏1P大厅
  {
    if (strlen(param[task].hallName[account]) == 0)
      PopMessage(hWndActuator, "请先抓取要隐藏的窗口句柄，\n将靶形光标拖动到窗口标题栏即可。");
    else
    {
      int hideResult = HideWindow(Hall(param[task].hallName[account]), param[task].hideInfo[account]);
      if (hideResult == 2)
        PopMessage(task, "权限不足。请先关闭当前窗口，然后\n右键执行器选择“以管理员身份运行”。");
      else if (hideResult == 1)
        PopMessage(task, "未找到窗口。");
      else
      {
        //检查所有任务的窗口，句柄相同的都设为隐藏
        for (int iTask = 0; iTask < maxTaskNum; iTask++)
          for (int iAccount = 0; iAccount < 2; iAccount++)
            if (strcmp(param[iTask].hallName[iAccount], param[task].hallName[account]) == 0)
              param[iTask].hideInfo[iAccount] = param[task].hideInfo[account];
      }
    }
  }
  else //显示1P大厅
  {
    RecoverWindow(Hall(param[task].hallName[account]), param[task].hideInfo[account]);
    InvalidateRect(GetActiveGameWindow(param[task].hWnd[account]), NULL, false);//重画游戏窗口
    for (int iAccount = 0; iAccount < 2; iAccount++)//检查两个账号的所有窗口，句柄相同的都设为显示
      for (int iTask = 0; iTask < maxTaskNum; iTask++)
        if (strcmp(param[iTask].hallName[iAccount], param[task].hallName[account]) == 0)
          param[iTask].hideInfo[iAccount] = param[task].hideInfo[account];
  }
}
//根据轨道路径打开轨道
void OpenTrack(char *extendedPath)
{
  const char *path = GetPureTrack(extendedPath);

  char cmd[1000];
  if (strlen(path) == 0)
    return;
  if (!FileExist(path)) //如果轨道文件不存在，进行提示
  {
    MessageBox(hWndActuator, "轨道文件不存在。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }
  if (strchr(path, ':')) //如果有冒号，说明是绝对路径
    sprintf_s(cmd, "\"%s\\轨道编辑器.exe\" \"%s\"", workDirect, path);
  else //否则是相对路径
    sprintf_s(cmd, "\"%s\\轨道编辑器.exe\" \"%s\\%s\"", workDirect, workDirect, path);
  Execute(cmd);
}
//打开轨道路径所在文件夹
void OpenTrackFolder(const char *extendedPath)
{
  const char *path = GetPureTrack(extendedPath);

  char cmd[1000];
  if (strlen(path) == 0)
    return;
  if (!FileExist(path)) //如果轨道文件不存在，进行提示
  {
    MessageBox(hWndActuator, "轨道文件不存在。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }
  if (strchr(path, ':')) //如果有冒号，说明是绝对路径
    sprintf_s(cmd, "explorer.exe /select,\"%s\"", path);
  else //否则是相对路径
    sprintf_s(cmd, "explorer.exe /select,\"%s\\%s\"", workDirect, path);
  Execute(cmd);
}
//调节自动刷新次数
void AdjustRefreshTimes()
{
  if (area == 306) //刷新次数下调
  {
    if (maxRefreshTimes > 10)
      maxRefreshTimes = 10;
    else if (maxRefreshTimes > 5)
      maxRefreshTimes = 5;
    else if (maxRefreshTimes > 3)
      maxRefreshTimes = 3;
    else
      maxRefreshTimes = 0;
  }
  if (area == 406) //刷新次数上调
  {
    if (maxRefreshTimes < 3)
      maxRefreshTimes = 3;
    else if (maxRefreshTimes < 5)
      maxRefreshTimes = 5;
    else if (maxRefreshTimes < 10)
      maxRefreshTimes = 10;
    else
      maxRefreshTimes = unlimitedRefreshTimes;
  }
}
//调节缩放
void AdjustZoom()
{
  if (Adjust(zoom, 50, 100, 10, 314, 414))//缩放比例
  {
    solidrectangle(0, 0, wndWidth, wndHeight);//清屏
    ratio = originRatio * zoom / 100;//改变窗口尺寸和显示比例，不影响点击位置判定
    setaspectratio(ratio, ratio);//改变显示比例
    imageBackup.Resize((int)(wndWidth * ratio), (int)(wndHeight * ratio));//修改备份图像大小

    RECT oldRect, newRect;
    GetWindowRect(hWndActuator, &oldRect);
    Resize(NULL, (int)(wndWidth * ratio), (int)(wndHeight * ratio));//改变窗口大小
    GetWindowRect(hWndActuator, &newRect);
    int newWidth = newRect.right - newRect.left;
    int newHeight = newRect.bottom - newRect.top;
    MoveWindow(hWndActuator, oldRect.left + oldRect.right - newRect.right, oldRect.top, newWidth, newHeight, true);
  }
}
//批量调节：房主，1P卡组，2P卡组
void BatchAdjust(int list, int item, int originValue, int newValue)
{
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    if (IsNoBattle(plot[list].advance[order])) //批量调节不影响特殊任务
      continue;
    int *value = nullptr;
    if (item == HOST)
      value = &plot[list].advance[order].host;
    else if (item == DECK_0)
      value = &plot[list].advance[order].deck[0];
    else if (item == DECK_1)
      value = &plot[list].advance[order].deck[1];
    if (*value == originValue)
      *value = newValue;
  }
}
//批量交换：房主，1P卡组，2P卡组
void BatchExchange(int list, int item, int oldValue, int newValue)
{
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    if (IsNoBattle(plot[list].advance[order])) //批量交换不影响特殊任务
      continue;
    int *value = nullptr;
    if (item == HOST)
      value = &plot[list].advance[order].host;
    else if (item == DECK_0)
      value = &plot[list].advance[order].deck[0];
    else if (item == DECK_1)
      value = &plot[list].advance[order].deck[1];
    if (*value == oldValue)
      *value = newValue;
    else if (*value == newValue)
      *value = oldValue;
  }
}
// 查找所有游戏大厅和微端，进行截图
int __stdcall EnumHall(HWND hWnd, LPARAM lParam)
{
  char className[256];
  char pngName[256] = {};
  if (GetClassName(hWnd, className, sizeof(className)))
  {
    if (strcmp(className, "DUIWindow") == 0 || strcmp(className, "ApolloRuntimeContentWindow") == 0)
    {
      if (IsWindowVisible(hWnd) && !IsIconic(hWnd))
      {
        if (strcmp(className, "DUIWindow") == 0)
          sprintf_s(pngName, "报错信息\\大厅[%d].png", (int)hWnd);
        else
          sprintf_s(pngName, "报错信息\\微端[%d].png", (int)hWnd);
        WindowToBitmap(hWnd, pngName, 0, 0, INT_MAX, INT_MAX, 20);
      }
    }
  }
  return TRUE;
}
// 查找所有3.9版以上的轨道编辑器窗口，发送点击指令
int __stdcall EnumEditor(HWND hWnd, LPARAM lParam)
{
  char className[256];
  char windowTitle[256];
  int version0 = 0, version1 = 0;
  if (GetClassName(hWnd, className, sizeof(className)))
  {
    if (strcmp(className, "EasyXWnd") == 0)
    {
      GetWindowTextA(hWnd, windowTitle, sizeof(windowTitle));
      if (sscanf_s(windowTitle, "轨道编辑器v%d.%d", &version0, &version1) == 2)
      {
        while (version1 >= 10)
          version1 = version1 / 10;
        if (version0 * 10 + version1 >= 39)//3.9以上版本
          LeftClick(hWnd, 9999, 9999);
      }
    }
  }
  return TRUE;
}
void ShotActuator()
{
  char pngName[2][maxPath] = { "报错信息\\普通任务.png", "报错信息\\高级任务.png" };
  if (mode == 1)
  {
    saveimage(pngName[0], &imageBackup);
    mode = 2;
    RepaintAdvance();
    saveimage(pngName[1], &imageBackup);
    mode = 1;
    RepaintParameter();
  }
  else if (mode == 2)
  {
    saveimage(pngName[1], &imageBackup);
    mode = 1;
    RepaintParameter();
    saveimage(pngName[0], &imageBackup);
    mode = 2;
    RepaintAdvance();
  }
}
void MakeErrorFile()
{
  //删除原来的报错信息.zip，创建或清空报错信息文件夹
  if (FileExist("报错信息.zip"))
    remove("报错信息.zip");
  if (FileExist("报错信息"))
    DeleteFolder("报错信息");
  CreatePath("报错信息");
  //1. 截图编辑器
  EnumWindows(EnumEditor, 0);
  //2. 截图执行器
  ShotActuator();
  //3. 截图游戏大厅
  EnumWindows(EnumHall, 0);
  //4. 保存最新执行记录
  char lastRecordName[maxPath] = {};
  char sourceRecord[maxPath] = {};//原执行记录路径
  char destRecord[maxPath] = {};//报错信息文件夹中的执行记录路径
  if (GetLastFolder("执行记录", lastRecordName)) //如果找到了最新执行记录文件夹
  {
    sprintf_s(sourceRecord, "执行记录\\%s", lastRecordName);
    sprintf_s(destRecord, "报错信息\\%s", lastRecordName);
    CopyDirect(sourceRecord, destRecord);
  }
  //5. 版本号和DPI
  FILE *f;
  if (fopen_s(&f, "报错信息\\记录信息.txt", "w") == 0)
  {
    fprintf(f, "版本号=%s\n", version);
    fprintf(f, "DPI=%d\n", DPI);
    int updateNum = 0, successfulUpdateNum = 0, iconicNum = 0;
    CountUpdateSuccessRate(updateNum, successfulUpdateNum, iconicNum);
    fprintf(f, "截图%d帧，成功%d帧，最小化%d帧\n", updateNum, successfulUpdateNum, iconicNum);
    fclose(f);
  }
  //6. 打包成zip
  char command[1024];  // PowerShell命令
  sprintf_s(command, "powershell Compress-Archive -Path '%s\\报错信息\\*' -DestinationPath '%s\\报错信息.zip'",
    workDirect, workDirect);
  Sleep(100);
  int result = Execute(command, true, true);  // 执行命令
  char tip[1000] = {};
  if (result == 0) //压缩成功
  {
    DeleteFolder("报错信息");
    strcpy_s(tip, "已生成压缩文件[报错信息.zip]，\n请上传至交流群并@管理员或群主。");
  }
  else
    strcpy_s(tip, "已生成[报错信息]文件夹，请压缩后\n上传至交流群并@管理员或群主。");
  MessageBox(hWndActuator, tip, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
}
//响应上方三个按钮的点击：全部停止、后台运行、一键报错
void ResponseUpperSetting()
{
  if (area == 200) //全部停止
  {
    for (int list = 0; list < listNum; list++)//停止高级任务0-4
      if (plot[list].isAdvanceStarted)
        plot[list].isAdvanceStarted = 0;

    for (int task = 0; task < maxTaskNum; task++)//停止所有普通任务
      if (isTaskStarted[task])
        isTaskStarted[task] = 0;
  }
  else if (area == 201) //后台运行
    ShowWindow(hWndActuator, SW_HIDE);
  else if (area == 202) //一键报错
    MakeErrorFile();
}
//是否有监视窗口启动
bool isAnyMonitorExist()
{
  for (int task = 0; task < maxTaskNum; task++)
    if (monitor[task].isExist)
      return true;
  return false;
}
//启动任务监视器
DWORD __stdcall MonitorThread(void *vpTask)
{
  int task = (int)vpTask;
  InitTransparentWindow(monitor[task]);//创建监视窗口
  monitor[task].initSettings(hMonitorFont, hMonitorPen);//初始化绘图设置
  monitor[task].isExist = true;//记录窗口存在

  //不断检测游戏窗口左上角位置，保持一致
  POINT origin = { 0, 0 };//监视窗口左上角位置
  RECT rect = {};
  while (true) //等候消息，仅在调用完本账号的ReadLevelInfo时重绘
  {
    GetWindowRect(work[task].hWnd[GetFirstPerformedAccount(task)], &rect);
    //如果监视窗口已关闭，退出线程
    if (!monitor[task].isExist)
      return 0;
    //如果游戏窗口位置有变，移动监视窗口使两者对齐
    if (rect.left != origin.x || rect.top != origin.y)
    {
      origin.x = rect.left;
      origin.y = rect.top;
      SetWindowPos(monitor[task].hWnd, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_NOSIZE);
    }
    Sleep(1);
  }
  return 0;
}
//打开/关闭监视窗口
void SwitchMonitor(int task)
{
  if (!monitor[task].isExist) //打开监视窗口
    CreateThread(NULL, 0, MonitorThread, (void *)task, 0, NULL);
  else //关闭监视窗口
    SendMessageA(monitor[task].hWnd, WM_DESTROY, 0, 0);
}
//获取某个关卡需要的门票编号
int GetTicketCode(int type, int level)
{
  if (type == 2) //魔塔不统计
    return 0;
  return entrance[type][level];
}
//检查全部任务所需的门票数量
void CountUsedTicket(int list, int(&ticketCount)[2][maxTicketNum])
{
  memset(ticketCount, 0, sizeof(ticketCount));
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    Advance &advance = plot[list].advance[order];
    if (advance.isSelected && advance.isProceed)
    {
      int code = GetTicketCode(advance.type, advance.level);
      //如果需要用钥匙，进行记录
      if (code >= 1)
      {
        for (int account = 0; account < 2; account++)
          if (advance.host == 2 || advance.host == account)
          {
            ticketCount[account][code] += advance.maxGames;
            if (ticket[code].style == 1)
              ticketCount[account][code + 1] += advance.maxGames;
          }
      }
    }
  }
}
//是否有任务正在启动
bool IsAnyTaskRunning()
{
  for (int task = 0; task < maxTaskNum; task++)//检查普通任务线程
    if (isTaskStarted[task])
      return true;
  for (int list = 0; list < listNum; list++)//检查高级任务线程
    if (plot[list].isAdvanceStarted)
      return true;
  return false;
}
//是否有大厅隐藏
bool IsAnyHallHidden()
{
  for (int task = 0; task < maxTaskNum; task++)//检查普通任务线程
    for (int account = 0; account < 2; account++)
      if (param[task].hideInfo[account].hidden)
        return true;
  return false;
}
//统计列表中的任务所需的门票和卡组
void CountList(int list)
{
  int task = GetTask(list);
  if (plot[list].isAdvanceStarted_Confirm)
  {
    PopMessage(task, "任务运行时无法检查。");
    return;
  }

  //1. 检查门票
  CountUsedTicket(list, plot[list].usedTicketCount);//统计门票数量

  //2. 检查防御卡
  plot[list].isChecking = true;//检查标记
  plot[list].isCheckingSuccessful = false;//检查结果预设为失败
  CreateThread(nullptr, 0, StartList, (void *)list, 0, nullptr);//以检查模式启动高级任务线程
  while (plot[list].isChecking) //等待检查结束
    Sleep(10);
  if (!plot[list].isCheckingSuccessful) //检查失败则退出
    return;

  //3. 如果检查成功，绘制检查结果到统计面板
  memset(panel, 0, sizeof(panel));
  int ticketEndY = GetTicketEndY(list);
  usedPanelHeight = GetSlotEndY(list, ticketEndY);
  usedPanelHeightDPI = usedPanelHeight * DPI / 96;
  //3.1 在panel中绘制门票和卡组
  for (int account = 0; account < 2; account++)
  {
    PaintTicket(list, account, false);
    PaintSlot(list, account, ticketEndY, false);
  }
  //3.2 将图片放大并绘制到pPanelDPI
  pScaler->PaintZoomedImage();
  //3.3 在pPanelDPI中标注文字和绘制框线
  SetWorkingImage(pPanelDPI);
  setaspectratio((float)DPI / 96, (float)DPI / 96);
  SetPanelFont();//设置标注字体
  //3.3.1 标注门票和卡槽名称
  for (int account = 0; account < 2; account++)
  {
    ViewTicketName(list, account);
    ViewSlotName(list, account, ticketEndY);
  }
  //3.3.2 标注标题
  CenterView("1P", panelTitleWidth + areaWidth / 2, panelTitleHeight / 2);
  CenterView("2P", panelTitleWidth + 3 * areaWidth / 2, panelTitleHeight / 2);
  CenterView("门票", panelTitleWidth / 2, (panelTitleHeight + ticketEndY) / 2);
  CenterView("卡槽", panelTitleWidth / 2, (ticketEndY + usedPanelHeight) / 2);
  //3.3.3绘制框线
  //2条竖线
  for (int account = 0; account < 2; account++)
    line(panelTitleWidth + account * areaWidth, 0,
      panelTitleWidth + account * areaWidth, usedPanelHeight);
  //2条横线
  line(0, panelTitleHeight, panelWidth, panelTitleHeight);
  line(0, ticketEndY, panelWidth, ticketEndY);

  setaspectratio(1, 1);
  SetWorkingImage(nullptr);

  //4. 弹窗展示
  plot[list].isDeepCheckingSuccessful = false;//检查结果预设为失败
  //如果选择了【检查背包】，则启动深度检查任务
  if (PopCountDialog(list) == IDOK)
  {
    //1. 设置第n+1条任务为【检查】
    memset(&plot[list].advance[plot[list].advanceNum], 0, sizeof(Advance));
    plot[list].advance[plot[list].advanceNum].type = 17;
    plot[list].advance[plot[list].advanceNum].level = 7;

    //2. 以【深度检查】标记启动高级任务
    plot[list].isDeepChecking = true;//检查标记
    char info[1000] = {};
    //更新用户截图成功则允许启动任务
    if (UpdateUserShot(false, info))
    {
      //打开高级任务开关，因为这是一次正常执行任务的启动
      plot[curList].isAdvanceStarted = 1;
      plot[curList].isAdvanceStarted_Confirm = 1;
      CreateThread(nullptr, 0, StartList, (void *)list, 0, nullptr);//以检查模式启动高级任务线程
    }
    else //否则弹窗报错
      PopMessage(task, info);
  }
}
//编辑高级任务
void EditAdvance()
{
  char message[200] = {};
  int xPos, yPos, buttons;//鼠标按下
  RepaintAdvance();//绘制高级参数界面
  while (true)//循环检测鼠标事件
  {
    if (lastClickMessage > zero) //检测到鼠标按下
    {
      buttons = lastClickMessage / 100000000;
      xPos = lastClickMessage % 100000000 / 10000;
      yPos = lastClickMessage % 10000;
      lastClickMessage = 0;
      area = GetArea(xPos, yPos);
      if (buttons == 1)//如果按下的是左键
      {
        if (area == 1)
        {
          mode = 1;
          return;
        }

        //任务运行期间，不允许调整参数（撤销、全选、param修改、列表修改）
        if (plot[curList].isAdvanceStarted_Confirm)
          if (area == 212 || area == 213 || area / 100 == 5 || area == 2508 || area == 3508 ||
            area == 2509 || area == 3509 || area == 3512 || area >= 10000)
            area = 99;
        if (area == 99)
          MessageBox(hWndActuator, "任务运行时不允许修改参数", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);

        if (area == 15) //启动或停止高级任务
        {
          if (plot[curList].isAdvanceStarted == 0) //高级任务未启动：启动高级任务
          {
            int task = GetTask(curList);
            if (!isLoadAndUpdateFinished || plot[curList].isAdvanceStarted_Confirm == 1 || isTaskStarted_Confirm[task] == 1
              || isTaskStarted_Confirm[task + 1] == 1)
              PopMessage(task, "启动过快，请稍等1秒。");
            else
            {
              int missionStyle = IsMissionList(curList);//判断预制列表类型
              if (missionStyle != 0) //如果属于预制列表，不允许直接启动
              {
                sprintf_s(message, "预制列表不能直接执行。\n"
                  "如需执行%s，请在自己的列表中添加\n"
                  "一项类型为【任务】，关卡为【%s】的任务。",
                  fullMissionName[missionStyle - 1], missionName[missionStyle - 1]);
                PopMessage(task, message);
              }
              else
              {
                char info[1000] = {};
                //更新用户截图成功则允许启动任务
                if (UpdateUserShot(false, info))
                {
                  plot[curList].isAdvanceStarted = 1;
                  plot[curList].isAdvanceStarted_Confirm = 1;
                  CreateThread(nullptr, 0, StartList, (void *)curList, 0, nullptr);//启动高级任务线程
                }
                else //否则弹窗报错
                  PopMessage(task, info);
              }
            }
          }
          else //高级任务已启动：停止高级任务
          {
            isTaskStarted[GetTask(curList)] = 0;//任务启动状态设为“未启动”
            isTaskStarted[GetTask(curList) + 1] = 0;//任务启动状态设为“未启动”
            plot[curList].isAdvanceStarted = 0;//刹停高级任务
          }
        }
        if (area == 25 || area == 35) //1P或2P大厅的隐藏和显示
          HideOrShow(GetTask(curList), area / 10 - 2);//隐藏/显示的大厅账号
        if (area == 55) //输出信息向左翻页
        {
          if (plot[curList].outputPage > 1)
            plot[curList].outputPage--;
        }
        if (area == 65) //输出信息向右翻页
        {
          if (plot[curList].outputPage < (plot[curList].outputNum - 1) / 6 + 1)
            plot[curList].outputPage++;
        }
        if (area == 75) //清空输出信息
        {
          if (plot[curList].isAdvanceStarted == 0)
            plot[curList].outputNum = 0;
        }
        if (area == 85) //info按钮
        {
          SwitchMonitor(GetTask(curList));
          SwitchMonitor(GetTask(curList) + 1);
          Sleep(100);
        }
        ResponseUpperSetting();

        AdjustZoom();//缩放
        Adjust(missionRecovery, 0, 1, 1, 304, 404, 1);//自动还原
        AdjustRefreshTimes();//自动刷新
        Adjust(operationSpeed, 0, 2, 1, -1, 207, 1);//操作速度

        if (area == 208) //调整全局加速时间
          InputNum(&globalAccelarationTime, 0, 999,
            "设置全体任务的开局加速时间（0~999），单位毫秒。\n如需单独设置某些任务的加速时间，请见使用手册。",
            "加速时间");

        if (area == 209) //一键配轨
          MatchCommonTrackWithChoice(curList);

        if (area == 210) //检查任务
          CountList(curList);

        if (area == 311)//高级任务列表左翻页
        {
          if (plot[curList].advancePage > 1)
            plot[curList].advancePage--;
          plot[curList].isAdvanceSelectionRetained = 1;
        }
        if (area == 411)//高级任务列表右翻页
        {
          int TotalAdvancePage = min(plot[curList].advanceNum / advanceNumPerPage + 1, maxAdvancePage);
          if (plot[curList].advancePage < TotalAdvancePage)
            plot[curList].advancePage++;
          plot[curList].isAdvanceSelectionRetained = 1;
        }

        if (area == 212)//撤销按钮
          if (IsUndoAllowed(curList))
            Undo(curList);
        if (area == 213)//恢复按钮
          if (IsRepeatAllowed(curList))
            Repeat(curList);

        if (area == 501)//全选按钮
        {
          if (AdvanceSelectionStates(curList) == 2)//如果已经全选了，全部取消选中
          {
            for (int order = 0; order < plot[curList].advanceNum; order++)
              plot[curList].advance[order].isSelected = 0;
          }
          else //否则全部选中
          {
            for (int order = 0; order < plot[curList].advanceNum; order++)
              plot[curList].advance[order].isSelected = 1;
          }
        }

        if (area == 502) //锁定按钮
          plot[curList].isAdvanceLocked = !plot[curList].isAdvanceLocked;

        if (area == 503)//修改列表路径
        {
        }

        if (area == 504)
          EditServerAndPassword(0);//编辑1P服务器和二级
        if (area == 505)
          EditServerAndPassword(1);//编辑2P服务器和二级

        //改为：本体1000+，左点击2000+，右点击3000+，左调节4000+&5000+：右调节6000+&7000+

        if (area == 1507)
          InputAdvanceTitle(curList);

        Adjust(curList, 0, listNum - 1, 1, 2507, 3507);//切换高级任务

        if (area == 2508) //抓取1P标签
          GrabTag(GetTask(curList), 0);
        if (area == 2509) //抓取2P标签
          GrabTag(GetTask(curList), 1);


        if (area / 100 == 35) //参数右侧的×
        {
          switch (area)
          {
          case 3508: //删除1P标签
            DeleteTag(GetTask(curList), 0);
            break;
          case 3509: //删除2P标签
            DeleteTag(GetTask(curList), 1);
            break;
          case 3512: //删除状态信息
            if (isTaskStarted[GetTask(curList)] == zero)
              ResetState(GetTask(curList));
            break;
          }
        }

        if (area / 10000 >= 1 && area / 10000 <= 7)//单击高级任务格子（或两侧的箭头）
        {
          int order = (plot[curList].advancePage - 1) * advanceNumPerPage + area % 10000 / 100 - 1;//任务序号
          int item = area % 100;//项数
          if (order < plot[curList].advanceNum && item == CHECK) //勾选或取消勾选
          {
            plot[curList].advance[order].isSelected = 1 - plot[curList].advance[order].isSelected;
            //如果勾选了识别类任务，则删除所有衍生任务
            if (plot[curList].advance[order].type == 5 && plot[curList].advance[order].isSelected)
              ReturnMission(curList, plot[curList].advance[order].level);
          }
          else if (plot[curList].isAdvanceLocked == false) //未锁定才能修改任务
          {
            if (order < plot[curList].advanceNum)
            {
              switch (item)
              {
              case ID: //编号
                GetAsyncKeyState(VK_CONTROL);
                if (GetAsyncKeyState(VK_CONTROL)) //按住Ctrl点击编号：复制
                {
                  if (plot[curList].advanceNum < maxAdvanceNum)
                  {
                    if (plot[curList].advanceSelected == 0) //无选中轨道：复制本轨道到后面
                    {
                      CopyAdvance(curList, order);
                      InsertAdvance(curList, plot[curList].advanceNum - 1, order + 1);
                    }
                    else //已选中轨道：将选中轨道复制到所点轨道之前
                    {
                      CopyAdvance(curList, plot[curList].advanceSelected - 100);
                      InsertAdvance(curList, plot[curList].advanceNum - 1, order);
                    }
                  }
                }
                else //直接点击编号：选中轨道或交换
                {
                  if (plot[curList].advanceSelected == 0)
                  {
                    plot[curList].advanceSelected = 100 + order;
                    plot[curList].isAdvanceSelectionRetained = 1;
                  }
                  else if (plot[curList].advanceSelected != 100 + order)
                    ExchangeAdvance(curList, plot[curList].advanceSelected - 100, order);
                }
                break;
              case TYPE: //任务类型
                if (area / 10000 == 2) //上调
                {
                  if (plot[curList].advance[order].type < maxTypeNum - 1)
                  {
                    plot[curList].advance[order].type++;
                    plot[curList].advance[order].level = defaultLevel[plot[curList].advance[order].type];
                    plot[curList].advance[order].missionStyle = 0;//取消任务标记
                  }
                }
                else if (area / 10000 == 3) //下调
                {
                  if (plot[curList].advance[order].type > zero)
                  {
                    plot[curList].advance[order].type--;
                    plot[curList].advance[order].level = defaultLevel[plot[curList].advance[order].type];
                    plot[curList].advance[order].missionStyle = 0;//取消任务标记
                  }
                }
                if (IsNoBattle(plot[curList].advance[order]) && plot[curList].advance[order].host == 2)
                  plot[curList].advance[order].host = 0;
                break;
              case LEVEL: //关卡
                if (plot[curList].advance[order].type == 2) //魔塔：输入魔塔关卡
                  InputTower(&plot[curList].advance[order].level);
                else if (plot[curList].advance[order].type == 0 || plot[curList].advance[order].type == 1 || plot[curList].advance[order].type == 4 ||
                  plot[curList].advance[order].type == 5 || IsNoBattle(plot[curList].advance[order]))
                {
                  if (area / 10000 == 2)//右侧上调
                  {
                    if (plot[curList].advance[order].level < levelNum[plot[curList].advance[order].type] - 1)
                      plot[curList].advance[order].level++;
                  }
                  else if (area / 10000 == 3)//右侧下调
                  {
                    if (plot[curList].advance[order].level > zero)
                      plot[curList].advance[order].level--;
                  }
                }
                else if (plot[curList].advance[order].type == 3) //跨服：左右都有调节
                {
                  if (area / 10000 == 2)//右侧上调
                  {
                    if (plot[curList].advance[order].level % 8 < 7)
                      plot[curList].advance[order].level++;
                  }
                  else if (area / 10000 == 3)//右侧下调
                  {
                    if (plot[curList].advance[order].level % 8 > zero)
                      plot[curList].advance[order].level--;
                  }
                  else if (area / 10000 == 4)//左侧上调
                  {
                    if (plot[curList].advance[order].level / 8 < 5)
                      plot[curList].advance[order].level += 8;
                  }
                  else if (area / 10000 == 5)//左侧下调
                  {
                    if (plot[curList].advance[order].level / 8 > zero)
                      plot[curList].advance[order].level -= 8;
                  }
                }
                else
                  InputNum(&plot[curList].advance[order].level, 0, levelNum[plot[curList].advance[order].type] - 1,
                    islandPrompt[plot[curList].advance[order].type - basicTypeNum], "选择关卡");
                break;
              case GAMES: //局数
                if (IsNoGames(plot[curList].advance[order]))
                  MessageBox(hWndActuator, "此任务无需填写局数。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
                else
                  InputNum(&plot[curList].advance[order].maxGames, 1, 9999, "输入执行刷图的局数（1~9999）。", "执行局数");
                break;
              case KEY: //钥匙
                if (IsNoTrack(plot[curList].advance[order]))
                  MessageBox(hWndActuator, "此任务无需填写钥匙。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
                else
                  plot[curList].advance[order].isProceed = 1 - plot[curList].advance[order].isProceed;
                break;
              case HOST:case DECK_0:case DECK_1: //房主，1P、2P卡组
              {
                if (item == HOST && IsNoBattle(plot[curList].advance[order]))
                {
                  MessageBox(hWndActuator, "此任务无需填写房主。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
                  break;
                }
                int account = (item - TRACK_0) / 2;
                int &oldValue = item == HOST ? plot[curList].advance[order].host : plot[curList].advance[order].deck[account];
                //特殊任务：必然是调节卡组。只能0调到1, 1调到0
                if (IsNoBattle(plot[curList].advance[order]))
                {
                  if (!IsNoDeck(plot[curList].advance[order]))
                    if (area / 10000 == 2 || area / 10000 == 3)
                      oldValue = oldValue == 0 ? 1 : 0;
                }
                else //其他任务：卡组0-6范围，房主0-2范围，支持批量调节和交换
                {
                  if (plot[curList].advanceSelected == 0) //无任务选中：调节数值
                  {
                    int newValue = -1;//要调至的数值，-1表示无需调节
                    int maxValue = item == HOST ? 3 : 7;//允许调节的最大值+1
                    if (item == HOST || area / 10000 == 2)//上调
                      newValue = (oldValue + 1) % maxValue;
                    else if (area / 10000 == 3)//下调
                      newValue = (oldValue + maxValue - 1) % maxValue;
                    if (newValue >= 0) //如果需要调节
                    {
                      GetAsyncKeyState(VK_SHIFT);
                      if (GetAsyncKeyState(VK_SHIFT)) //按住VK_SHIFT：批量调节数值
                        BatchAdjust(curList, item, oldValue, newValue);
                      else //否则只调本项任务的卡组
                        oldValue = newValue;
                    }
                  }
                  else //选中了任务：交换数值。不能和特殊任务交换
                  {
                    if (!IsNoBattle(plot[curList].advance[plot[curList].advanceSelected - 100]))
                    {
                      int &target = item == HOST ? plot[curList].advance[plot[curList].advanceSelected - 100].host
                        : plot[curList].advance[plot[curList].advanceSelected - 100].deck[account];
                      GetAsyncKeyState(VK_SHIFT);
                      if (GetAsyncKeyState(VK_SHIFT)) //按住VK_SHIFT：批量交换数值
                        BatchExchange(curList, item, oldValue, target);
                      else //否则交换两任务数值
                        Swap(&oldValue, &target);
                    }
                  }
                }
              }
              break;
              case TRACK_0:case TRACK_1: //房主、队友轨道
              {
                int account = (item - TRACK_0) / 2;
                if (IsNoTrackParam(plot[curList].advance[order])) //无轨道参数任务
                {
                  if (IsTimer(plot[curList].advance[order]) && account == 0)
                    InputTimer(&plot[curList].advance[order].timer);
                  else if (IsSign(plot[curList].advance[order]) && account == 0)
                    EditHarmony();
                  else
                    MessageBox(hWndActuator, "此任务无需填写轨道。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
                }
                else
                {
                  if (area / 10000 == 6)//交换1P和2P的轨道、卡组
                  {
                    if (strlen(plot[curList].advance[order].track[account]) > 0)
                      ExchangePerformer(curList, order);//切换放卡账号
                  }
                  else if (area / 10000 == 7)//删除轨道
                    strcpy_s(plot[curList].advance[order].track[account], "");
                  else
                    InputTrack(plot[curList].advance[order].track[account]);
                }
              }
              break;
              }
            }
            else if (order == plot[curList].advanceNum && (item == ID || item == CHECK)) //新增和复制
            {
              if (plot[curList].advanceSelected == 0)
                AddAdvance(curList, 0, 0, 1, 1, 1);
              else
                CopyAdvance(curList, plot[curList].advanceSelected - 100);
            }
          }
        }

        if (plot[curList].isAdvanceSelectionRetained == 0)
          plot[curList].advanceSelected = 0;
        plot[curList].isAdvanceSelectionRetained = 0;
      }
      else if (buttons == 2)//鼠标右击
      {
        //任务运行期间，不允许调整参数（清空列表、删除）
        if (plot[curList].isAdvanceStarted_Confirm)
          if (area == 500 || (area / 10000 == 1 && (area % 100 == ID || area % 100 == CHECK)))
            area = 99;
        if (area == 99)
          MessageBox(hWndActuator, "任务运行时不允许修改参数", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        if (area == 204) //删除定时
        {
        }
        if (area == 500)
        {
          if (plot[curList].isAdvanceLocked == false) //未锁定才能修改任务
          {
            int answer = MessageBox(hWndActuator, "确定要清空任务列表？", "提示", MB_OKCANCEL | MB_ICONINFORMATION | MB_SYSTEMMODAL);
            if (answer == IDOK)
              plot[curList].advanceNum = 0;
          }
        }
        if (area / 10000 == 1)//右击高级任务格子
        {
          int order = (plot[curList].advancePage - 1) * advanceNumPerPage + area % 10000 / 100 - 1;//任务序号
          int item = area % 100;//项数
          if (order < plot[curList].advanceNum)
          {
            //无轨任务不允许右键打开轨道
            if (!(IsNoTrack(plot[curList].advance[order]) && (item == TRACK_0 || item == TRACK_1)))
              switch (item)
              {
              case ID:case CHECK: //序号或勾选框：删除任务或插入任务
                if (plot[curList].isAdvanceLocked == false) //未锁定才能修改任务
                {
                  if (plot[curList].advanceSelected == 0)
                    DeleteAdvance(curList, order);
                  else if (plot[curList].advanceSelected != 100 + order)
                    InsertAdvance(curList, plot[curList].advanceSelected - 100, order);
                  plot[curList].advanceSelected = 0;
                }
                break;
              case TRACK_0:case TRACK_1: //房主、队友轨道：打开轨道
              {
                int account = (item - TRACK_0) / 2;
                if (area / 10000 == 1)
                {
                  GetAsyncKeyState(VK_CONTROL);
                  if (GetAsyncKeyState(VK_CONTROL)) //按住VK_CONTROL：打开轨道所在文件夹
                    OpenTrackFolder(plot[curList].advance[order].track[account]);
                  else
                    OpenTrack(plot[curList].advance[order].track[account]);
                }
              }
              break;
              }
          }
        }
      }
      SaveList(curList);
      SaveParameter();
      RepaintAdvance();
    }
    else //没有鼠标按下，就重绘提示
      RepaintTip();

    sleepTime++;
    if (sleepTime % 10 == zero)
      Sleep(1);
    if (sleepTime >= 100)
    {
      CheckHallWindow();//更新大厅句柄
      if (isRepaintRequired == 1)
      {
        isRepaintRequired = 0;
        RepaintAdvance();
      }
      else
        RepaintOld();
      sleepTime = 0;
    }
  }
}
//获取任务task的轨道填写情况。填了双轨返回2，一轨返回序号，0轨返回0
int GetFilledTrack(int task)
{
  int TrackNum = 0;
  int account = 0;
  if (strlen(param[task].track[0]) > 0)
  {
    TrackNum++;
    account = 0;
  }
  if (strlen(param[task].track[1]) > 0)
  {
    TrackNum++;
    account = 1;
  }
  if (TrackNum == 2)
    return 2;
  else
    return account;
}
//编辑普通任务参数
void EditParameter()
{
  int xPos, yPos, buttons;//鼠标按下
  int task, account, i;

  RepaintParameter();//绘制参数界面
  while (true)//循环检测鼠标事件
  {
    if (lastClickMessage > zero) //检测到鼠标按下
    {
      buttons = lastClickMessage / 100000000;
      xPos = lastClickMessage % 100000000 / 10000;
      yPos = lastClickMessage % 10000;
      lastClickMessage = 0;
      area = GetArea(xPos, yPos);
      if (buttons == 1)//如果按下的是左键
      {
        if (area == 2)
        {
          mode = 2;
          return;
        }
        if (area / 10 == 1) //启动和停止
        {
          task = area - 10;
          if (isTaskStarted[task] == zero)//任务未开始：启动任务
          {
            if (!isLoadAndUpdateFinished || isTaskStarted_Confirm[task] == 1)
              PopMessage(task, "启动过快，请稍等1秒。");
            else
            {
              char info[1000] = {};
              //更新用户截图成功则允许启动任务
              if (UpdateUserShot(false, info))
              {
                if (IsParamRunnable(task) == 1)
                {
                  isTaskStarted[task] = 1;//记录任务开始
                  isTaskStarted_Confirm[task] = 1;
                  CreateThread(NULL, 0, StartTask, (void *)task, 0, NULL);//启动任务线程
                }
              }
              else //否则弹窗报错
                PopMessage(task, info);
            }
          }
          else //停止任务
            isTaskStarted[task] = 0;
        }
        if (area / 10 == 2 || area / 10 == 3) //1P或2P大厅的隐藏和显示
        {
          task = area % 10;
          account = area / 10 - 2;//隐藏/显示的大厅账号
          HideOrShow(task, account);
        }

        ResponseUpperSetting();//响应上方三个按钮：全部停止、后台运行、一键报错
        AdjustZoom();
        Adjust(customOption, 0, 1, 1, 304, 404, 1);//带卡选择
        Adjust(flipNum, 1, 5, 1, 306, 406);//翻牌数量
        Adjust(bagFullOperation, 0, 1, 1, 308, 408, 1);//背包满时操作
        Adjust(expFullOperation, 0, 1, 1, 310, 410, 1);//经验满时操作
        Adjust(repairLevel, 0, 10, 1, 312, 412);//补阵强度

        //本体1000+，左点击2000+，右点击3000+，左调节4000+&5000+：右调节6000+&7000+
        //点击本体：输入参数
        if (area / 1000 == 1)
        {
          task = area % 1000 / 100;
          i = area % 100;
          switch (i)
          {
          case 0:
            InputTaskTitle(task);
            break;
          case 1:
            InputTrack(param[task].track[0]);
            break;
          case 2:
            InputTrack(param[task].track[1]);
            break;
          case 3:
            InputNum(&param[task].maxGames, 1, 9999, "输入执行刷图的局数（1~9999）。", "执行局数");
            break;
          case 4:
            InputNum(&param[task].gameTime, 0, 960,
              "输入每局刷图的时间（0~960），单位秒。\n输入0自动检测，否则到设定时间后执行下一局。", "每局用时");
            break;
          }
        }
        //交换轨道或抓取标签
        if (area / 1000 == 2)
        {
          task = area % 1000 / 100;
          i = area % 100;
          switch (i)
          {
          case 1:
            if (strlen(param[task].track[0]) > 0)
            {
              char Track[maxPath];
              strcpy_s(Track, param[task].track[0]);
              strcpy_s(param[task].track[0], param[task].track[1]);
              strcpy_s(param[task].track[1], Track);
            }
            break;
          case 2:
            if (strlen(param[task].track[1]) > 0)
            {
              char Track[maxPath];
              strcpy_s(Track, param[task].track[0]);
              strcpy_s(param[task].track[0], param[task].track[1]);
              strcpy_s(param[task].track[1], Track);
            }
            break;
          case 8://抓取房主标签
            GrabTag(task, 0);
            break;
          case 9://抓取队友标签
            GrabTag(task, 1);
            break;
          }
        }
        //删除轨道或状态
        if (area / 1000 == 3)
        {
          task = area % 1000 / 100;
          i = area % 100;
          switch (i)
          {
          case 0://打开/关闭监视窗口
            SwitchMonitor(task);
            Sleep(100);
            break;
          case 1:
            strcpy_s(param[task].track[0], "");
            break;
          case 2:
            strcpy_s(param[task].track[1], "");
            break;
          case 8:
            DeleteTag(task, 0);//删除抓取的标签
            break;
          case 9:
            DeleteTag(task, 1);//删除抓取的标签
            break;
          case 12://若该任务未运行，可以点×删除状态信息
            if (isTaskStarted[task] == zero)
              ResetState(task);
            break;
          }
        }
        //参数上调
        if (area / 1000 == 4 || area / 1000 == 6)
        {
          if (area / 1000 == 4)
            account = 0;
          else
            account = 1;
          task = area % 1000 / 100;
          i = area % 100;
          switch (i)
          {
          case 5://继续作战上调
            param[task].isProceed[account] = 1 - param[task].isProceed[account];
            break;
          case 6://自动收集上调
            if (param[task].isCollect[account] == 2)
              param[task].isCollect[account] = 0;
            else
              param[task].isCollect[account]++;
            break;
          case 7://使用卡组上调
            if (param[task].deck[account] == 6)
              param[task].deck[account] = 0;
            else
              param[task].deck[account]++;
            break;
          }
        }
        //参数下调
        if (area / 1000 == 5 || area / 1000 == 7)
        {
          if (area / 1000 == 5)
            account = 0;
          else
            account = 1;
          task = area % 1000 / 100;
          i = area % 100;
          switch (i)
          {
          case 5:
            param[task].isProceed[account] = 1 - param[task].isProceed[account];
            break;
          case 6:
            if (param[task].isCollect[account] == zero)
              param[task].isCollect[account] = 2;
            else
              param[task].isCollect[account]--;
            break;
          case 7:
            if (param[task].deck[account] == zero)
              param[task].deck[account] = 6;
            else
              param[task].deck[account]--;
            break;
          }
        }
      }
      else if (buttons == 2)
      {
        if (area / 1000 == 1)//右击轨道：用编辑器打开轨道
        {
          task = area % 1000 / 100;
          i = area % 100;
          switch (i)
          {
          case 1:case 2:
            GetAsyncKeyState(VK_CONTROL);
            if (GetAsyncKeyState(VK_CONTROL)) //按住VK_CONTROL：打开轨道所在文件夹
              OpenTrackFolder(param[task].track[i - 1]);
            else
              OpenTrack(param[task].track[i - 1]);
            break;
          }
        }
      }
      SaveParameter();
      RepaintParameter();
    }
    else //没有鼠标按下，就重绘提示
      RepaintTip();
    sleepTime++;
    if (sleepTime % 10 == zero)
      Sleep(1);
    if (sleepTime >= 100)
    {
      CheckHallWindow();//更新大厅句柄
      if (isRepaintRequired == 1)
      {
        isRepaintRequired = 0;
        RepaintParameter();
      }
      else
        RepaintOld();
      sleepTime = 0;
    }
  }
}
LRESULT CALLBACK MyWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CLOSE:
  {
    bool isAnyTaskRunning = IsAnyTaskRunning();
    bool isAnyHallHidden = IsAnyHallHidden();
    //若存在运行中的任务或未显示的窗口，需要确认退出
    if (isAnyTaskRunning || isAnyHallHidden)
    {
      char message[500];
      if (isAnyTaskRunning && isAnyHallHidden)
        strcpy_s(message, "还有运行中的任务和未显示的窗口，\n确定要退出吗？");
      else if (isAnyTaskRunning)
        strcpy_s(message, "还有运行中的任务，\n确定要退出吗？");
      else
        strcpy_s(message, "还有未显示的窗口，\n确定要退出吗？");

      int closeMessage = MessageBoxA(hWndActuator, message, "提示",
        MB_ICONINFORMATION | MB_OKCANCEL | MB_SYSTEMMODAL);
      if (closeMessage == IDOK)
      {
        ClearThisBackup();
        exit(0);
      }
    }
    else //没任务直接退出
    {
      ClearThisBackup();
      exit(0);
    }
  }
  break;
  case WM_DROPFILES:
  {
    POINT pt;
    GetCursorPos(&pt);//获取鼠标位置
    ScreenToClient(hWnd, &pt);//转化为窗口内坐标
    int area = TaskArea(pt.x, pt.y);
    int account = area % 10;
    HDROP hDrop = (HDROP)wParam;
    char fullPath[maxPath];
    char shortPath[maxPath];
    DragQueryFileA(hDrop, 0, fullPath, sizeof(fullPath));
    DragFinish(hDrop);
    GetShortPath(fullPath, shortPath);
    if (!IsTxtFile(fullPath))
    {
      PopMessage(hWndActuator, "只能打开txt格式文件。");
      break;
    }
    int fileType = CheckFileType(fullPath);
    //普通任务模式：只接受轨道文件
    if (mode == 1)
    {
      if (fileType != 1)
      {
        PopMessage(hWndActuator, "只能打开轨道文件。");
        break;
      }
      int task = area / 10;
      if (IsNormal(task) && account >= 0 && account < 2)
      {
        if (account == 0)
          strcpy_s(param[task].track[0], shortPath);
        else
          strcpy_s(param[task].track[1], shortPath);
        int xPos = 10;
        int yPos = 0;
        lastClickMessage = 100000000 + xPos * 10000 + yPos;
      }
    }
    //高级任务模式：接受轨道文件、列表和签到轨道
    else if (mode == 2)
    {
      int list = curList;
      if (fileType == 0) //如果是无效文件
      {
        PopMessage(hWndActuator, "只能打开轨道或列表文件。");
        break;
      }
      if (fileType == 1 || fileType == 3) //如果是轨道文件
      {
        int pageOrder = area / 10;
        int order = (plot[list].advancePage - 1) * advanceNumPerPage + pageOrder;
        if (order >= zero && order < plot[list].advanceNum && account >= 0 && account < 2)
        {
          if (account == 0)
            strcpy_s(plot[list].advance[order].track[0], shortPath);
          else
            strcpy_s(plot[list].advance[order].track[1], shortPath);
        }
      }
      else if (fileType == 2) //如果是列表文件
      {
        if (plot[list].isAdvanceStarted_Confirm) //任务运行时：不允许修改，把文件中的路径改回去
          MessageBox(hWndActuator, "任务运行时不允许修改参数", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        else
        {
          plot[list].advanceSelected = 0;
          strcpy_s(plot[list].advancePath, shortPath);
          LoadList(list);
          plot[list].advancePage = 1;
          plot[list].backupNum = plot[list].currentBackup = 0;
        }
      }
      int xPos = 10;
      int yPos = 0;
      lastClickMessage = 100000000 + xPos * 10000 + yPos; //模拟点击以重绘界面
    }
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
  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
    // 处理其他消息...
  }
  // 调用默认窗口过程
  return 0;
}
//从以列表中获取公会任务名
void LoadGuildNameFromList(int list, char(&guildName)[maxAdvanceNum][100])
{
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    Advance &advance = plot[list].advance[order];
    strcpy_s(guildName[order], strrchr(advance.track[0], '\\') + 1);
    strrchr(guildName[order], '.')[0] = 0;
  }
}
//载入公会任务名称
int LoadGuildName(char(&guildName)[maxAdvanceNum][100])
{
  FILE *f;
  char s[1000] = {};
  fopen_s(&f, "公会任务名称.txt", "r");
  for (int order = 0; order < 80; order++)
  {
    fgets(f, s);
    strcpy_s(guildName[order], strchr(s, '_') + 1);
  }
  return 80;
}
//制作公会任务顺序表
void MakeGuildSequence(int list)
{
  //1. 读取公会任务名称
  char guildName[maxAdvanceNum][100] = {};
  LoadGuildName(guildName);
  //2. 编写公会任务顺序表
  FILE *f;
  fopen_s(&f, "公会任务顺序表.txt", "w");
  for (int order = 0; order < 80; order++)
  {
    Advance(&advance) = plot[list].advance[order];
    char levelName[100] = {};
    GetLevelName(levelName, advance.type, advance.level);
    //编号 类型 关卡 人数 BOSS 备注
    fprintf(f, "%d\t%s\t%s\t%s\t%s\t%s\n", order + 1, typeName[advance.type], levelName,
      "双人", advance.isProceed ? "打BOSS" : "不打BOSS", guildName[order]);
  }
  fclose(f);
}
//制作公会任务顺序表
void MakeLoverSequence(int list)
{
  FILE *f;
  fopen_s(&f, "情侣任务顺序表.txt", "w");
  for (int order = 0; order < 48; order++)
  {
    Advance(&advance) = plot[list].advance[order];
    char levelName[100] = {};
    GetLevelName(levelName, advance.type, advance.level);
    //编号 类型 关卡 人数 BOSS 备注
    fprintf(f, "%d\t%s\t%s\t%s\t%s\n", order + 1, typeName[advance.type], levelName,
      "双人", advance.isProceed ? "打BOSS" : "不打BOSS");
  }
  fclose(f);
}
//公会任务轨道用任务名命名
int RenameWithGuildName(int list, int nameList)
{
  //读取公会任务名称
  char guildName[maxAdvanceNum][100] = {};
  LoadGuildNameFromList(nameList, guildName);

  //保存公会任务名称
  FILE *f;
  if (fopen_s(&f, "附加程序\\公会任务\\公会任务名称1.txt", "w"))
    return 1;
  for (int order = 0; order < guildNum; order++)
    fprintf(f, "%d\t%s\n", order + 1, guildName[order]);
  fclose(f);

  char guildTrack[2][maxPath] = {};//公会任务轨道
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    Advance &advance = plot[list].advance[order];
    if (!advance.isSelected)
      continue;
    sprintf_s(guildTrack[0], "预制轨道\\公会任务\\%s.txt", guildName[order]);
    sprintf_s(guildTrack[1], "预制轨道\\公会任务\\%s+.txt", guildName[order]);
    if (order >= 2 && order < guildNum)
      for (int account = 0; account < 2; account++)
        if (strlen(advance.track[account]) > 0)
        {
          char prefix[maxPath] = {};
          GetTrackPrefix(advance.track[account], prefix);
          const char *pureTrack = GetPureTrack(advance.track[account]);
          if (!FileExist(pureTrack))
            ReportMissingFile(pureTrack);
          Copy(pureTrack, guildTrack[account]);
          sprintf_s(advance.track[account], "%s%s", prefix, guildTrack[account]);
        }
  }
  SaveList(list);
  return 0;
}
//列表中是否存在识别任务
bool IsMissionExisted(int list)
{
  for (int order = 0; order < plot[list].advanceNum; order++)
    if (plot[list].advance[order].missionStyle > 0)
      return true;
  return false;
}
const int maxAppendixNum = 256;
char appendix[maxAppendixNum][maxPath];
//生成依赖文件.txt
void MakeAppendixList()
{
  int appendixNum = GetFileList("附加程序\\*", appendix, maxAppendixNum);
  FILE *f;
  if (fopen_s(&f, "附加程序\\依赖文件.txt", "w"))
  {
    MessageBox(NULL, "无法写入依赖文件。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    exit(0);
  }
  for (int i = 0; i < appendixNum; i++)
    fprintf(f, "附加程序\\%s\n", appendix[i]);
  fclose(f);
}
//检查依赖文件是否完整，不完整则提示并退出程序
void CheckAppendix()
{
  FILE *f;
  int appendixNum = 0;
  if (fopen_s(&f, "附加程序\\依赖文件.txt", "r"))
  {
    MessageBox(NULL, "无法读取依赖文件，\n请检查是否更新完整。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    exit(0);
  }
  fgets(f, appendix[appendixNum]);
  while (strlen(appendix[appendixNum]) > 0)
    fgets(f, appendix[++appendixNum]);
  fclose(f);
  for (int i = 0; i < appendixNum; i++)
    if (!FileExist(appendix[i]))
    {
      char message[maxPath + 100];
      sprintf_s(message, "缺少依赖文件（夹）：\n%s\n请确保更新完整，且不要把软件移出文件夹。", appendix[i]);
      MessageBox(NULL, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
      exit(0);
    }
}
//创建本次自动备份文件夹
void CreateBackupFolder()
{
  time_t currentTime;
  char backupPath[maxPath] = {};
  char runningTick[maxPath] = {};//本次运行时刻
  for (int list = 0; list < listNum; list++)
  {
    while (true)
    {
      time(&currentTime);
      GetTimeStringForFolder(runningTick, currentTime);//获取本次启动时刻
      sprintf_s(plot[list].backupFolder, "%s-%d", runningTick, list);//填写备份文件夹
      sprintf_s(backupPath, "自动备份\\任务列表\\%s", plot[list].backupFolder);
      if (!FileExist(backupPath))//如果文件夹不存在，则退出循环
        break;
      Sleep(100);
    };
    CreatePath(backupPath);//创建本次自动备份文件夹
  }
}
//生成含有全部通用轨道的列表
void MakeGeneralList(int list)
{
  char folder[maxPath] = {};
  char track[2][maxPath] = {};
  char levelName[100] = {};
  plot[list].advanceNum = 0;
  for (int type = 0; type < 16; type++)
  {
    sprintf_s(folder, "预制轨道\\通用轨道\\%d%s", type, typeName[type]);
    if (!FileExist(folder))
      continue;
    for (int level = 0; level < levelNum[type]; level++)
    {
      GetLevelName(levelName, type, level);
      sprintf_s(track[0], "%s\\%d%s.txt", folder, level, levelName);
      sprintf_s(track[1], "%s\\%d%s+.txt", folder, level, levelName);
      if (!FileExist(track[0]))
        continue;
      AddAdvance(list, type, level, 1, 1, 1);
      strcpy_s(plot[list].advance[plot[list].advanceNum - 1].track[0], track[0]);//填写2P轨道
      if (FileExist(track[1]))
        strcpy_s(plot[list].advance[plot[list].advanceNum - 1].track[1], track[1]);//填写2P轨道
    }
  }
}
//生成情侣任务预制列表
void MakeLoverList(int list)
{
  plot[list].advanceNum = 0;
  for (int type = 6; type <= 9; type++)
    for (int level = 0; level < levelNum[type]; level++)
      AddAdvance(list, type, level, 1, 1, 1);
}
//根据进度填写表面任务
void FillSurface(int list, int account, Contest(&contest)[12][9], int(&contestSize)[12])
{
  plot[list].surfaceSize[account] = 0;
  memset(plot[list].surface[account], 0, sizeof(plot[list].surface[account]));
  //按列数从大到小填写未完成的表面任务
  for (int column = 11; column >= 0; column--)
    if (contestSize[column] > 0)
    {
      Contest &surface = plot[list].surface[account][plot[list].surfaceSize[account]];
      surface = contest[column][contestSize[column] - 1];
      plot[list].surfaceSize[account]++;
    }
  //按列数从大到小填写已完成的表面任务
  for (int column = 11; column >= 0; column--)
    if (contestSize[column] == 0)
    {
      Contest &surface = plot[list].surface[account][plot[list].surfaceSize[account]];
      surface = contest[column][0];
      surface.isFinished = true;
      plot[list].surfaceSize[account]++;
    }
}
//使用定位法模拟完成第issue期大赛任务，返回局数
int LocatingSimulation(int list, int issue)
{
  int contestSize[12] = {}; //各列实际进度
  ResetContestSize(issue, contestSize); //重置各列进度
  plot[list].issue = 0;//设置期数为0

  //进入大赛循环
  int games = 0;
  while (true)
  {
    //1. 根据实际进度填写表面任务
    for (int account = 0; account < 2; account++)
      FillSurface(list, account, tournament[issue].contest, contestSize);

    //2. 根据表面任务定位估计进度
    int lastIssue = plot[list].issue;
    if (!SmartLocateSurface(list))
    {
      PopMessage(nullptr, "全局定位失败");
      exit(0);
    }
    if (plot[list].issue != lastIssue)
    {
      char message[200] = {};
      sprintf_s(message, "第%d步重新定位到%s", games, contestName[plot[list].issue]);
      PopMessage(nullptr, message);
    }

    //3. 根据估计进度确定当前步骤
    int step = GetNextStep(list);
    if (step == tournament[plot[list].issue].minStep) //执行完毕则返回
      break;
    games++;

    //4. 结算实际进度
    for (int column = 0; column < 12; column++)
      if (contestSize[column] > 0)
      {
        Contest &surface = tournament[issue].contest[column][contestSize[column] - 1];//实际表面任务
        Contest &solution = tournament[plot[list].issue].solution[step];//估计的最优任务
        //如果最优任务包含了本列表面任务，本列完成进度+1
        if (IsContestMatched(surface, solution))
          contestSize[column]--;
      }
  }

  //检查能否完成任务
  bool isContestFinished = true;
  for (int column = 0; column < 12; column++)
    if (contestSize[column] > 0)
      isContestFinished = false;
  if (!isContestFinished)
    PopMessage(nullptr, "定位法无法完成任务");
  return games;
}
//向大赛要求字符串中添加1项要求
void AddToRequireString(char(&require)[maxPath], int value, const char *format)
{
  if (value > 0)
  {
    char singleRequire[100] = {};
    sprintf_s(singleRequire, format, value);
    strcat_s(require, singleRequire);
  }
}
//提取附加要求描述
void GetRequireString(Contest &contest, char(&require)[maxPath])
{
  int requireNum = 0;
  char singleRequire[100] = {};
  //卡槽
  AddToRequireString(require, contest.slotNum, "\t%d槽");
  //禁卡
  for (int i = 0; i < contestSlotNum; i++)
    if (contest.slotBanned[i])
    {
      sprintf_s(singleRequire, "\t禁%s", contestSlot[i]);
      strcat_s(require, singleRequire);
    }
  //放卡
  for (int i = 0; i < contestSlotNum; i++)
    if (contest.slotPlanted[i])
    {
      sprintf_s(singleRequire, "\t放%s", contestSlot[i]);
      strcat_s(require, singleRequire);
    }
  //静置
  AddToRequireString(require, contest.restTime, "\t静置%d秒");
  //放卡数
  AddToRequireString(require, contest.cardNum, "\t限放%d卡");
  //星级
  AddToRequireString(require, contest.star, "\t%d星");
  //产出
  AddToRequireString(require, contest.product, "\t产出%d火");
  //火苗
  AddToRequireString(require, contest.energy, "\t剩余%d火");

  //存活火炉数
  AddToRequireString(require, contest.aliveStove, "\t存活%d小火");
  //平均星级限制
  AddToRequireString(require, contest.averageStar, "\t平均%d星");
  //通关时间限制
  AddToRequireString(require, contest.gameTime, "\t用时%d秒");
  //放置酒瓶炸弹的次数
  AddToRequireString(require, contest.bottleTimes, "\t放置酒瓶%d次");
  //免费卡放置次数
  AddToRequireString(require, contest.freeTimes, "\t放置免费卡%d次");
  //卡片最大耗能限制
  AddToRequireString(require, contest.cost, "\t耗能%d");

  //评分
  if (contest.grade > 0)
  {
    sprintf_s(singleRequire, "\t%s评级", gradeString[contest.grade]);
    strcat_s(require, singleRequire);
  }
  //杀敌
  for (int i = 0; i < contest.enemySize; i++)
  {
    sprintf_s(singleRequire, "\t%s", contest.enemy[i]);
    strcat_s(require, singleRequire);
  }
}
//使用实战机制模拟第issue期大赛，保留列表，生成带要求的最优解文件
void RealSimulation(int list, int host, int issue)
{
  //准备工作
  plot[list].advanceNum = 0;//清空列表
  AddAdvance(list, 5, CONTEST - 1, 1, 1, 1);//添加一条任务-大赛
  plot[list].advance[0].host = host;//设置房主
  plot[list].playingOrder = 0; //设置当前执行ID
  plot[list].playingAdvance = &plot[list].advance[0];

  //创建大赛日志
  strcpy_s(plot[list].contestLog, "模拟大赛日志.txt");//填写大赛日志路径
  FILE *f;
  fopen_s(&f, plot[list].contestLog, "w");
  fclose(f);

  //（顺序表1/3）创建顺序表文件
  FILE *fsol;
  char solutionPath[maxPath] = {};
  sprintf_s(solutionPath, "%s期顺序表.txt", contestName[issue]);
  fopen_s(&fsol, solutionPath, "w");

  int contestSize[12] = {}; //各列实际进度
  ResetContestSize(issue, contestSize); //重置各列进度
  plot[list].issue = 0;//设置期数为0

  //进入大赛循环
  int games = 0;
  while (true)
  {
    //1. 根据实际进度填写表面任务
    for (int account = 0; account < 2; account++)
      FillSurface(list, account, tournament[issue].contest, contestSize);

    //1.1 应完成和未完成的记录
    //清空未完成的大赛任务
    plot[list].failedSize = 0;
    memset(plot[list].failedContest, 0, sizeof(plot[list].failedContest));
    //记录未完成的大赛任务，填写每个表面任务的未完成次数
    RecordFailedContest(list);
    //清空应完成的大赛任务
    plot[list].requiredSize = 0;
    memset(plot[list].requiredContest, 0, sizeof(plot[list].requiredContest));

    //2. 根据表面任务定位估计进度
    if (SmartLocateSurface(list)) //如果可以定位期数和进度，选择最优解
      SelectBestSurface(list);
    else //定位失败则选择最小编号任务
      SelectMinSurface(list);
    if (!FillBestContestFromSurface(list, true))//填写最优任务的完整信息，记录日志
      break;
    games++;

    Contest &solution = plot[list].bestContest;//估计的最优任务

    //（顺序表2/3）向顺序表中写入一个步骤
    char require[maxPath] = {};
    GetRequireString(solution, require);
    char levelName[100];
    GetLevelName(levelName, solution.type, solution.level);
    fprintf(fsol, "%d\t%s\t%s\t"
      "%s\t%s%s\n", games, typeName[solution.type], levelName,
      solution.roleNum == 1 ? "单人" : "双人", solution.boss == 1 ? "不打BOSS" : "打BOSS", require);

    //3. 根据任务生成轨道
    int lastSubtaskOrder = -1;
    int result = InsertContest(list, lastSubtaskOrder, false, true);//向列表添加任务，记录日志
    if (result != 0)
    {
      char message[1000] = {};
      if (result == -1)
        sprintf_s(message, "%s %s\n第%d步 无法添加", contestName[issue], host == 2 ? "双人" : "单人",
          plot[list].advanceNum - 1);
      else if (result == 1)
        sprintf_s(message, "%s %s\n第%d步 不符合要求", contestName[issue], host == 2 ? "双人" : "单人",
          plot[list].advanceNum - 1);
      PopMessage(nullptr, message);
    }

    //4. 结算实际进度
    for (int column = 0; column < 12; column++)
      if (contestSize[column] > 0)
      {
        Contest &surface = tournament[issue].contest[column][contestSize[column] - 1];//本列表面任务
        //如果最优任务包含了本列表面任务，本列完成进度+1
        if (IsContestMatched(surface, solution))
          contestSize[column]--;
      }
  }

  //（顺序表3/3）关闭顺序表文件
  fclose(fsol);

  //走完后检查任务是否已经全部完成
  bool isContestFinished = true;
  for (int column = 0; column < 12; column++)
    if (contestSize[column] > 0)
      isContestFinished = false;
  if (!isContestFinished)
    PopMessage(nullptr, "最优解无法完成任务");

  //取消大赛标记，删除第一项识别任务
  for (int order = 0; order < plot[list].advanceNum; order++)
    plot[list].advance[order].missionStyle = 0;
  DeleteAdvance(list, 0);//删除任务-大赛
  plot[list].isAdvanceLocked = true;//锁定列表

  //保存列表
  SaveList(list);
  SaveParameter();
  isRepaintRequired = 1;
}
// 监视窗口过程函数
LRESULT CALLBACK MonitorProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CREATE:
    return 0;
  case WM_LBUTTONDOWN:
    return 0;
  case WM_DESTROY:
    for (int task = 0; task < maxTaskNum; task++)
    {
      if (monitor[task].hWnd == hWnd)
      {
        monitor[task].isExist = false;
        monitor[task].isCreated = false;
        monitor[task].hWnd = 0;
        monitor[task].hDC = 0;
        break;
      }
    }
    PostQuitMessage(0);
    return 0;
  default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
}
//初始化监视器设置
void InitMonitorSetting()
{
  InitClass("MonitorWindow", MonitorProc);
  for (int task = 0; task < maxTaskNum; task++)
  {
    strcpy_s(monitor[task].className, "MonitorWindow");
    sprintf_s(monitor[task].windowText, "任务%d监视", task);
    monitor[task].cx = gameWidth * DPI / 96;
    monitor[task].cy = gameHeight * DPI / 96;
    monitor[task].ratio = (float)DPI / 96;
  }
  hMonitorFont = CreateFont(16, 0, 0, 0, 1000, FALSE, FALSE, FALSE, //创建16号等线字体
    ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "等线");
  hMonitorPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
  hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
}
//截取VIP图像
void CatchVIP()
{
  char path0[maxPath], path1[maxPath];
  for (int vip = 0; vip <= 14; vip++)
  {
    sprintf_s(path0, "VIP大图\\%d.png", vip);
    sprintf_s(path1, "VIP\\%d.png", vip);
    BitmapToColor(path0, work[0].map);
    ColorToBitmap(work[0].map, path1, vipX, vipY, vipWidth, vipHeight);
  }
}
//载入GUI不依赖的文本和图片
DWORD __stdcall LoadImageAndText(void *param)
{
  //数字、字典树和文本
  LoadVerify();//载入动态验证数字和问题
  LoadDigit();//载入数字
  LoadLootDigit();//载入物品数字
  if (developerMode)
    BuildTrie();//开发者模式：读取字库并构建文字识别字典树
  else
    LoadTrie();//用户模式：直接读入文字识别字典树
  LoadSequence();//载入公会、情侣任务顺序表

  //图片
  LoadPicture();//载入界面图片
  LoadVip();//载入VIP图片
  LoadStar();//载入星级图片
  LoadBanner();//载入关卡名图片
  LoadBase();//载入物品底色图片
  WriteCycleArea();//填写“加载中”圆环区域
  LoadLoot();//载入物品图片
  LoadLootScroll();//载入战利品标准滚动条
  LoadGuildImage();//载入公会任务名图片
  LoadCard();//载入自定卡片
  LoadCardTemplate();//载入标准卡片模板
  BitmapToColor("附加程序\\图片\\缺少卡槽截图.png", missingCustom);//载入缺卡截图
  //载入四大用户截图，载入失败则报错
  char info[1000] = {};
  if (!UpdateUserShot(true, info))
    ExitMessage(info);
  LoadProgress();//载入进度条颜色
  LoadMaltoseCoverage();//载入麦芽糖覆盖区域
  LoadMusicInnateGrid();//载入音乐节夜初始地图
  LoadDragon();//载入龙图

  isLoadFinished = true;//记录载入资源完成
  return 0;
}
bool isUpdateDownloaded;//update.txt是否下载成功
int currentVer;//当前版本号
int highestVer;//最新版本号
int lowestVer;//最低可用版本号
char highestVerString[100];//最新版本号字符串
char lowestVerString[100];//最低版本号字符串
//从网站中下载update.txt
DWORD __stdcall DownloadUpdate(void *param)
{
  //获取当前版本号
  char currentVerString[100];
  sscanf_s(version, "v%s", currentVerString, sizeof(currentVerString));
  currentVer = GetIntegerVersion(currentVerString);

  //下载最新版本信息update.txt
  char link[300] = "https://doc.mstrack.cn/download/update.txt";
  char path[maxPath] = "附加程序\\update.txt";
  remove(path);
  DeleteUrlCacheEntryA(link);
  URLDownloadToFileA(nullptr, link, path, 0, 0);//下载update.txt

  //从update.txt中读取版本信息
  FILE *f;
  if (!fopen_s(&f, path, "r"))
  {
    char s[1000];
    //读取最新版本号
    fgets(f, s);
    sscanf_s(s, "最新版本号=%s", highestVerString, sizeof(highestVerString));
    highestVer = GetIntegerVersion(highestVerString);

    //读取最低可用版本号
    fgets(f, s);
    sscanf_s(s, "最低可用版本号=%s", lowestVerString, sizeof(lowestVerString));
    lowestVer = GetIntegerVersion(lowestVerString);

    fclose(f);
    remove(path);
    isUpdateDownloaded = true;//下载成功
  }

  isUpdateFinished = true;//记录下载update.txt完成
  return 0;
}
//载入资源和检查更新
DWORD __stdcall LoadAndUpdate(void *param)
{
  //线程内的ExitMessage必须在执行器窗口上弹窗
  pHWndExit = &hWndActuator;

  //创建载入资源线程
  CreateThread(NULL, 0, LoadImageAndText, (void *)0, 0, NULL);
  //创建update.txt下载线程
  CreateThread(NULL, 0, DownloadUpdate, (void *)0, 0, NULL);

  //等待两个线程均完成，且执行器窗口启动
  while (!(isLoadFinished && isUpdateFinished && hWndActuator))
    Sleep(10);
  //运行到这里说明载入资源没有出错

  //如果update.txt下载成功且存在新版本，则进行弹窗提示
  if (isUpdateDownloaded && currentVer < highestVer)
  {
    char message[1000];
    //如果当前版本低于最低可用版本，提示后退出程序
    if (currentVer < lowestVer)
    {
      sprintf_s(message, "轨道最新版本：v%s\n最低可用版本：v%s\n"
        "当前版本已不能使用，请在群文件下载最新版。",
        highestVerString, lowestVerString);
      ExitMessage(message);
    }
    else //如果当前版本不低于最低可用版本，则提示新版本，但不强制更新
    {
      //先检查是否开启“不再提示”
      int noTipVer = 0;//该版本以下不再提示
      FILE *f;
      char s[1000];
      if (!fopen_s(&f, "用户参数\\不再提示.txt", "r"))
      {
        char noTipVerString[100];
        fgets(f, s);
        sscanf_s(s, "不再提示版本号=%s", noTipVerString, sizeof(noTipVerString));
        noTipVer = GetIntegerVersion(noTipVerString);
        fclose(f);
      }

      //如果最新版本高于不再提示版本，进行提示
      if (highestVer > noTipVer)
        PopVersionTip(highestVerString);
    }
  }

  //取消ExitMessage只能在执行器弹窗限制
  pHWndExit = nullptr;
  isLoadAndUpdateFinished = true;
  return 0;
}
//检查执行器多开
void CheckMultipleRunning(int argc, const char *wndTitle, const char *folderName, POINT pointPosted)
{
  //创建本程序的互斥量
  char mutexName[maxPath];
  sprintf_s(mutexName, "ACTUATOR_%s", folderName);//用文件夹名称命名互斥量
  HANDLE hmutex = CreateMutexA(NULL, FALSE, mutexName);//创建互斥量
  //如果互斥量已存在，说明同一目录下的执行器已打开，不允许重复启动
  if (hmutex && GetLastError() == ERROR_ALREADY_EXISTS)
  {
    hWndActuator = FindWindow("EasyXWnd", wndTitle);//查找标题匹配的窗口
    if (hWndActuator) //如果找到了，恢复窗口
    {
      if (argc > 1) //拖动文件到执行器上
      {
        LeftClick(hWndActuator, pointPosted.x, pointPosted.y);//发送点击指令
        RecoverWindow_NoMove(hWndActuator);//恢复隐藏的窗口，若窗口未被隐藏则不改变位置
      }
      else //直接启动执行器
        CenterShow(hWndActuator);//恢复隐藏的窗口
      SetForegroundWindow(hWndActuator);
    }
    //如果没找到，说明已开执行器还没创建窗口，不做任何操作

    exit(0);//无论是否找到窗口，都结束程序，防止多开
  }
}
int main(int argc, char *argv[])
{
  DPI = SetDPIAware();//设置DPI感知并获取DPI
  ParameterShifter();//配置迁移

  //任务检查面板初始化
  panelWidthDPI = panelWidth * DPI / 96;
  panelHeightDPI = panelHeight * DPI / 96;
  pPanelDPI = new IMAGE(panelWidthDPI, panelHeightDPI);
  if (!pPanelDPI)
    ExitMessage("分配pPanelImage时内存不足。");
  hDCPanelDPI = GetImageHDC(pPanelDPI);//获取大面板HDC
  pScaler = new WindowScaler(hDCPanelDPI);//绑定缩放类

  //获取程序所在目录CurrentDirect
  strcpy_s(programDirect, argv[0]);//拷贝程序目录
  char *slash = strrchr(programDirect, '\\');//找到最右边的反斜杠
  slash[0] = 0;

  //至此程序所在目录获取完毕
  POINT pointPosted = { 0, 0 };//发送给已启动的执行器的点击位置
  char openedListPath[maxPath] = {};//打开的列表路径
  if (argc > 1)//用执行器打开文件时，要把工作目录改回来，并填写任务0轨道或载入任务列表
  {
    _chdir(programDirect);//修改工作目录到程序所在目录
    char shortPath[maxPath];//参数1中的文件相对路径
    GetShortPath(argv[1], shortPath);//获取相对路径
    LoadParameter();
    int fileType = CheckFileType(shortPath);
    if (fileType == 0) //无效文件格式
    {
      PopMessage(nullptr, "只能打开轨道或列表文件。");
      exit(0);
    }
    if (fileType == 1 || fileType == 3) //如果文件类型是轨道
      strcpy_s(param[0].track[0], shortPath);//填入任务0轨道
    else if (fileType == 2) //如果文件类型是列表
    {
      mode = 2;
      pointPosted.y = 2;//点击位置为(0,2)
      strcpy_s(openedListPath, shortPath);//记录列表路径
    }
    SaveParameter();
  }
  _getcwd(workDirect, sizeof(workDirect));//获取工作目录

  /*检查依赖文件完整性：附加程序和执行记录*/
  //如果上级目录有开发者文件，进入开发者模式
  developerMode = FileExist("..\\开发者文件.txt");
  if (developerMode)
  {
    MakePictureList();//生成图片目录
    MakeAppendixList();//生成依赖文件目录
  }
  if (!FileExist("执行记录"))
    CreatePath("执行记录");
  if (!FileExist("附加程序"))
    CreatePath("附加程序");
  if (!FileExist("用户参数"))
    CreatePath("用户参数");
  if (!FileExist("附加程序\\依赖文件.txt"))
    OutputRes("依赖文件", "TEXT", "附加程序\\依赖文件.txt");
  CheckAppendix();//检查依赖文件

  /*获取所在文件夹名称：CurrentDirect的最后一级*/
  char folderName[maxPath];//所在文件夹名称
  slash = strrchr(programDirect, '\\');//找到最右边的斜杠
  if (slash == NULL)
    slash = strrchr(programDirect, '/');//没有反斜杠就找正斜杠
  if (slash == NULL)//如果没有斜杠，说明只有一级目录
    strcpy_s(folderName, programDirect);//CurrentDirect就是文件夹名称
  else
    strcpy_s(folderName, slash + 1);//斜杠右边的就是程序名

  InitMonitorSetting();//初始化监视器

  char wndTitle[100];//窗口标题
  sprintf_s(wndTitle, versionString, version, folderName);//设置窗口标题
  CheckMultipleRunning(argc, wndTitle, folderName, pointPosted);

  //能运行到这里，说明启动的是执行器主程序
  ClearBackupAndLog();//清理之前的自动备份
  CreateBackupFolder();//创建备份文件夹

  //CheckVersion();//检查新版本

  //FindBase();//求解一类物品的底色
  //AutoClassification();
  //return 100;

  //fopen_s(&fRunningTime, "tick.txt", "w");
  //tickBegin = GetTickCount();

  //GUI依赖的信息载入
  InitIDString();//初始化外显ID字符串
  LoadTicket();//载入门票
  LoadNormalLevel();//载入普通关卡（要放在载入门票后面）
  LoadLevelMap();//载入关卡全称（要放在普通关卡后面）
  InitContest();//初始化大赛（要放在关卡全称后面）

  hMutexLoot = CreateMutexA(NULL, FALSE, NULL);//创建战利品读写互斥锁
  hMutexProp = CreateMutexA(NULL, FALSE, NULL);//创建道具读写互斥锁
  hMutexUserImage = CreateMutexA(NULL, FALSE, NULL);//创建邀请互斥锁
  hMutexSaveParameter = CreateMutexA(NULL, FALSE, NULL);//创建保存参数互斥锁
  hMutexLoadList = CreateMutexA(NULL, FALSE, NULL);
  hMutexSaveList = CreateMutexA(NULL, FALSE, NULL);
  hMutexTrack = CreateMutexA(NULL, FALSE, NULL);//创建读取轨道互斥锁
  hMutexTray = CreateMutexA(NULL, FALSE, NULL);//创建读取承载轨道互斥锁
  for (int list = 0; list < listNum; list++) //创建刷新记录互斥锁
    plot[list].hMutexRefreshLog = CreateMutexA(NULL, FALSE, NULL);

  CreateThread(NULL, 0, LoadAndUpdate, (void *)0, 0, NULL);//载入资源和检查更新

  RegisterAllButtons();//注册按钮区域值
  crossCursor = LoadCursor(NULL, IDC_CROSS);//加载十字光标资源

  for (int task = 0; task < maxTaskNum; task++)
    ResetState(task);

  //载入普通任务参数（里面没有列表路径，放心食用）
  LoadParameter();
  SaveParameter();

  //填写高级任务0-4的列表路径
  for (int list = 0; list < listNum; list++)
    sprintf_s(plot[list].advancePath, "用户参数\\自制列表\\高级任务%d.txt", list);
  //如果是用执行器打开列表文件，替换高级任务0的列表路径
  if (strlen(openedListPath) > 0)
    strcpy_s(plot[curList].advancePath, openedListPath);

  //for (int issue = 0; issue < 13; issue++)
  //  FindRepeatedContestInBottom(issue);
  //return 114514;

  //载入5个高级任务的列表
  for (int list = 0; list < listNum; list++)
  {
    plot[list].advancePage = 1;
    plot[list].outputPage = 1;
    LoadList(list);

    //if (list == 0) //生成大赛列表
    //  RealSimulation(list, 2, tournamentNum - 1);
    //else if (list == 1)
    //  RealSimulation(list, 0, tournamentNum - 1);

    SaveList(list);//启动时保存任务列表，形成备份
  }

  //fclose(fRunningTime);

  PopUpdateNotice();//弹出更新公告
  hWndActuator = InitGraphDPI();//启动GUI窗口
  SetWindowLongPtr(hWndActuator, GWLP_WNDPROC, (LONG_PTR)MyWindowProc); // 设置自定义窗口过程
  EnableDragDropForHighIntegrity(hWndActuator);
  DragAcceptFiles(hWndActuator, TRUE);//允许窗口接收文件
  CenterShow(hWndActuator);
  SetForegroundWindow(hWndActuator);
  SetWindowText(hWndActuator, wndTitle);

  HINSTANCE hInstance = GetModuleHandle(NULL);
  HICON hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(101), IMAGE_ICON,
    GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
  SendMessage(hWndActuator, WM_SETICON, ICON_BIG, (LPARAM)hIcon);//设置图标
  SendMessage(hWndActuator, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

  UpdateWindow(hWndActuator);

  setfillcolor(defaultFillColor);

  setbkmode(TRANSPARENT);    //透明字体
  LOGFONT font;
  gettextstyle(&font);       //获取当前字体设置
  font.lfHeight = 20;        //设置字体高度
  font.lfWeight = 1000;      //设置字体粗细
  strcpy_s(font.lfFaceName, "等线");   //设置字体为“等线”
  font.lfQuality = ANTIALIASED_QUALITY;//设置输出效果为抗锯齿
  settextstyle(&font);       //设置字体样式

  while (true)
    if (mode == 1)
      EditParameter();
    else if (mode == 2)
      EditAdvance();
  closegraph();
  return 0;
}