#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include "resource.h"
#include "..\\track.h"

const char versionString[] = "轨道执行器%s by -云涛晓雾- 所属文件夹：%s";
HANDLE hMutexLoot; //读写战利品时的互斥锁
HANDLE hMutexProp; //读写道具时的互斥锁
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
const int advanceWidth[advanceTitleNum] = { 66, 62, 96, 50, 44, 44, parameterWidth - 4, 44, parameterWidth - 4, 44 };
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

//2. 游戏内位置坐标
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
const int proceedX = 430, proceedY = 349;//继续作战
const int claimX = 529, claimY = 349;//领取奖励
const POINT flip[5] = { { 577, 267 }, { 724, 267 }, { 871, 267 }, { 577, 467 }, { 724, 467 } };//5张牌的翻牌位置
const POINT finish = { 710, 495 };//翻牌“完成”
const int firstPageX = 45, firstPageY = 576;//魔塔翻到第1页
const int pageDownX = 150, pageDownY = 576;//魔塔向后翻页
const int embarkX = 618, embarkY = 562;//魔塔“开始挑战”按钮
const int towerX = 113, towerY = 540, towerHeight = 31;//魔塔第1层，每层高度
const int deckX = 424, deckY = 121, deckWidth = 83;//卡组1，卡组宽度
const POINT expNoMoreTip = { 430, 332 };//经验满不再提示
const int bagConfirmX = 430, bagConfirmY = 358;//背包满、经验满确认
const int bagCancelX = 528, bagCancelY = 358;//背包满取消
const POINT retreat = { 915, 560 };//后退（退出房间）
const POINT worldMap = { 862, 41 };//世界地图按钮
const POINT blueBox = { 358, 316 };//“正在登陆服务器”蓝框消失判定位置

//矩形区域类型(x,y,width,height)
struct RectType
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
const int specialTypeNum = 1;//特殊类型数量
const int typeNum = basicTypeNum + islandTypeNum + specialTypeNum;//高级任务类型数量
const int maxAdvancePage = 10;//任务列表最大页数
const int advanceNumPerPage = 16;//任务列表每页任务数
const int maxAdvanceNum = advanceNumPerPage * maxAdvancePage;//任务列表容量
const char bounty[4][7] = { "美味", "火山", "浮空", "星际" };//悬赏关卡名
const int challengeNum = 23;//勇士关卡数
const char challenge[challengeNum][13] = { //勇士关卡名
  "洞君", "阿诺", "冰渣", "轰隆隆", "法老", "保罗", "玛丽", "皮特", "使者", "男爵", "朱莉", "中尉",
  "杰克", "金刚", "小明", "Baby", "美队", "钢铁侠", "绿巨人", "蜘蛛侠", "列车", "水母", "鲨鱼" };
const char crossServer[6][7] = { "古堡", "天空", "炼狱", "水火", "巫毒", "冰封" };//跨服关卡名
const char guild[4][7] = { "任务", "月光", "堕落", "死亡" };//公会关卡名
const char special[3][7] = { "签到", "施肥", "清包" };//特殊任务名称
const char serverName[7][20] = { //服务器名称
  "最近登录", "3366_1服", "3366_2服", "3366_3服",
  "3366_4服", "3366_5服", "3366_6服" };
//任务类型名称
char typeName[typeNum][7] = { "悬赏", "勇士", "魔塔", "跨服", "假期", "公会",
"", "", "", "", "", "", "", "", "", "", "", "特殊" };
int levelNum[typeNum] = { //每种类型的关卡数量
  4, challengeNum, 3005, 48, 1, 4,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 };
int defaultLevel[typeNum] = { //每种类型的默认关卡
  0, 17, 1151, 35, 0, 0,
  7, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0 };
const int maxIslandLevelNum = 17;//岛屿类型最大关卡数量
char islandLevel[islandTypeNum][maxIslandLevelNum][13];//各岛屿各关卡名称
char islandPrompt[islandTypeNum][300];//各岛屿输入关卡时的提示语
POINT islandLocation[islandTypeNum];//各岛屿的位置
POINT islandLevelButton[islandTypeNum][maxIslandLevelNum];//各岛屿各关卡的按钮位置

//高级任务类型
struct AdvanceType
{
  //任务参数
  int selected;//是否勾选
  int type;//任务类型：0悬赏 1勇士 2魔塔 3跨服 4假期 5公会 6-n地图
  int level;//关卡
  int maxGames;//执行局数
  int isProceed;//是否继续作战
  int host;//房主账号（0=1P 1=2P 2=同步任务）
  char track[2][maxPath];//每个轨道的路径
  int deck[2];//每个轨道的使用卡组
  int isMission;//是否公会任务
  //完成情况
  int result;//任务完成结果：0正常完成 1未领奖 2未完成
  int gamesFinished;//完成局数
};

const int maxLogLength = 100000;//高级任务日志最大长度
//任务列表类型
struct PlotType
{
  //列表信息
  char title[100];//任务标题
  AdvanceType advance[maxAdvanceNum];//当前高级任务列表
  AdvanceType missionAdvance[maxAdvanceNum];//公会任务预制列表
  int advanceNum;//当前任务数量 
  int missionAdvanceNum;//当前公会任务数量
  int advancePage;//高级任务列表当前页数
  int advanceSelected;//选中的高级任务
  int isAdvanceSelectionRetained;//是否需要保留任务选中状态
  bool isAdvanceLocked;//任务列表是否锁定
  bool isMissionAdvanceLocked;//公会任务列表是否锁定
  char advancePath[maxPath];//列表路径
  char password[2][15];//二级密码，最长14位
  int server[2];//1P和2P的3366服务器选项

  //执行信息
  int playingOrder;//正在执行的任务编号
  AdvanceType *playingAdvance;//正在执行的任务指针
  int isAdvanceWindowBound;//高级任务所需窗口是否已绑定
  int isAdvanceStarted;//高级任务是否正在运行
  int isAdvanceStarted_Confirm;//高级任务是否正在运行（确认退出任务线程后才会设为0）
  int advanceInvolved[2];//1P和2P是否参与高级任务（以Param抓取为准，不受房主交换影响）
  int timesRefreshed[maxAdvanceNum];//每个任务的已刷新次数
  int isRefreshRequired;//开始本轮任务前是否需要刷新
  int playedAdvanceNum;//已执行的高级任务数量
  //int refreshLevel;//刷新等级：0刷新重试 1刷新跳过 2终止（BindWindow之后出错才能刷新解决）
  bool isMissionClaimFailed;//公会任务领奖是否失败
  bool isLevelInsufficient;//是否等级不足
  int randomZone[7];//0-6的随机排列，用于决定执行高级任务时换到哪个区

  //日志信息
  char refreshLog[maxPath]; //刷新记录
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
//int curTask;//当前显示的任务编号
int curList;//当前显示的高级任务
PlotType plot[listNum];//任务列表

//5. 普通任务常数
const int maxSlotNum = 21;//最大卡槽数
const int sorbetLoc[3] = { 71, 79, 19 };//冰沙位置：左下、右下、右上依次尝试
const int maltoseRequiredSimilarity = 20;//棉麦识别要求的相似度

//任务参数类型
struct ParamType
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
  POINT tag[2];//小号1,2标签坐标，初始值-1,-1
  HWND hWnd[2];//如果直接抓的是游戏区域，则记录，否则为零
  bool isWindow[2];//两个游戏窗口是否有效
  HWND hMainHall, hWndHall[2];//主游戏大厅句柄；两个小号所在的大厅句柄
};
//任务状态类型
struct StateType
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
//需要每波重置的卡片信息
//卡槽信息类型
struct SlotType
{
  //变阵保留信息
  char name[10];
  bool isMaltose;//是不是棉花糖/麦芽糖
  //轨道信息
  int row[maxPlantTimes + 1], column[maxPlantTimes + 1];
  int moment[maxPlantTimes + 1];//第i次种植的行列和时刻
  byte isPlantedInMap[8][10];//卡片是否在i行j列种植（用于缩小补阵识图的范围）
  int maxTimes;//第i张卡的最大种植次数
  int repair;//是否补阵
  int search;//是否索敌
  int independent;//是否独立计时
  int limit;//是否极限成阵
  int retain;//是否继承
  int maxMoment;//第i张卡的最大种植时刻
  int level;//卡片层级
  //识图信息
  int itemCode;//放卡条件中的图像编号
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
  int limitTimes;//极限成阵阶段放置次数
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
struct TargetType
{
  int itemCode;//对应物件的编号
  short similarity[8][11][maxFrame];//7行10列格子每次识别的相似度
  short maxSimilarity[8][11];//7行10列格子最近1秒内的最大相似度
  short globalMaxSimilarity;//所有格子中的最大相似度
  int colorNum;//颜色总数
  int num;//目标数量
  int row, column;//最大相似位置，无图像为(0,0)
};
const int maxGamesAllowed = 9999;
const int maxTargetNum = 10;//每个账号允许的最大目标数量

//自定卡片类型
struct CardType
{
  COLORREF image[templateHeight][templateWidth];//物件图像
  char name[10];//卡片名称
  int requiredSimilarity;//要求的相似度（0%）
  int offsetX, offsetY, width, height;//识别范围的偏移和宽高
};
const int maxCardNum = 200;//自定卡片上限
char cardList[maxCardNum][maxPath];//自定卡片目录
CardType card[maxCardNum];//自定卡片
int cardNum;//自定卡片数量

const int customX = 10, customY = 20, customWidth = 25, customHeight = 15;//识别范围
const int bagX = 382, bagWidth = 49, bagHeight = 57;//背包卡位置
const int bagY1 = 179, bagY2 = 407;//确定bagY的范围
//自定卡类型
struct CustomType
{
  COLORREF image[customHeight][customWidth];//卡槽图像
  char name[100];//卡槽名称
  int priority;//优先级
};
const int maxCustomNum = 500;//自定卡槽上限
char customList[maxCustomNum][maxPath];//自定卡槽文件名列表
COLORREF bag[bagHeight][bagWidth];//背包卡

//战利品类型（保存完整截图用于显示，但只有中间区域用于比较）
struct LootType
{
  bool isReplacedRequired;//是否需要用不绑图像替换
  char name[50];//物品名称
  COLORREF image[lootWidth][lootHeight];//物品图像
};
const int maxLootNum = 1000;
char lootList[maxLootNum][maxPath];
LootType loot[maxLootNum];
int lootNum;

const int lootDigitX = 38, lootDigitY = 34, lootDigitWidth = 6, lootDigitHeight = 8;
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
//任务运行参数类型
struct WorkType
{
  //不能被清空的参数（前8字节）
  HDC hMemDC;//map的内存DC
  COLORREF(&map)[596][950];//先Y后X，BGR格式的地图
  //自定图像
  TargetType target[maxTargetNum];//只给10个识图名额
  int targetNum;//目标数量
  //卡槽信息
  SlotType slot[2][maxSlotNum + 2];//卡槽信息(0.27M)
  char originalName[2][maxSlotNum + 2][10];//ban卡前的卡槽名称
  int originalSlotNum[2];//ban卡前的卡槽数
  //执行器参数：执行器设定的参数
  char track[2][maxPath];//轨道
  char pureTrack[2][maxPath];//去除ban卡列表的轨道路径
  char backupTrack[2][maxPath];//备份轨道路径
  bool isBanEnabled[2];//ban卡是否启用
  bool isSlotBanned[2][maxSlotNum + 2];//卡槽是否被ban，1=是，0=否
  int accelerationTime[2];//两个号的加速时间
  bool isAccelerationRequired;//是否需要加速跳结算
  bool isAccelerationOn[2];//加速是否已开启
  int trackRoleNum[2];//轨道里得到的人数
  int maxGames;//执行局数
  int gameTime;//每局用时（0=自动检测）
  int host;//房主（0=1P，1=2P）
  int deck[2];//卡组
  int isProceed[2];//继续作战
  int isCollect[2];//自动收集
  POINT tag[2];//小号1,2标签坐标，初始值-1,-1
  HWND hMainHall, hWndHall[2];//主游戏大厅句柄；两个小号所在的大厅句柄
  //轨道参数：从轨道文件中读取的参数
  int isInvolved[2];//两个账号是否参与
  int isPerformed[2];//两个账号是否放卡
  int towerx[2];//需要挑战的魔塔层数，非魔塔为0
  int quitTime[2];//退出关卡的时间，不退出为0
  bool isQuitted[2];//关卡是否已退出
  bool isQuitUsed;//退出功能是否已使用
  int roleLocNum[2], roleLoc[2][63];//角色数量，人物1,2的行列
  int slotNum[2];//两个号的卡槽数
  bool isSkillMode;//是否刷技能模式
  //进度参数：只在关卡外发生变化的参数
  int games;//当前局数
  //关卡参数：关卡内变化的参数
  int roamType;//漫游关卡编号：0=非漫游或未记载
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
  COLORREF heavyName[heavyNameHeight][heavyNameWidth];//粗角色名
  bool lightName[lightNameHeight][lightNameWidth];//细角色名（双色）
  COLORREF towerPage[2][10][14];
  COLORREF customScroll[customScrollHeight][1];//背包滚动条
  COLORREF lootScroll[lootScrollHeight][1];//战利品滚动条
  COLORREF loot[lootHeight * 5][lootWidth * 10];//5x10战利品截图
  COLORREF totalLoot[lootHeight * 10][lootWidth * 10];//累计战利品截图

  // 自动截图道具用的颜色信息
  COLORREF initialProp[lootHeight][lootWidth];//移动前的道具背包格子截图
  COLORREF movedProp[lootHeight][lootWidth];//移动后的道具背包格子截图
  COLORREF calibration[calibrationHeight][calibrationWidth];//校准区
  int propOffsetY;//道具背包翻页量（正数）

  CardType card[2][maxSlotNum + 1];
  COLORREF wideTemplate[wideTemplateHeight][wideTemplateWidth];
  COLORREF background[2][wideTemplateHeight][wideTemplateWidth];
  int isBackgroundCatched[2];//背景是否成功截取
  int isGridCatched;//初始地图是否成功截取
  int isTemplateCatched[2][maxSlotNum + 1];//每张卡是否已经被截取模板
  byte colorExist[2097152];//记录模板中存在的颜色(2M)
  int grid[5][8][10];//0-4层级各格子占用的卡片编号

  COLORREF recordedColor[templateWidth * templateHeight];
  int recordedColorNum;
  COLORREF erasedColor[templateWidth * templateHeight];

  //其他参数
  int starfishOrder[2];//两个账号的海星编号
  int magic[2], magicCore[2];//两个账号的幻鸡和幻鸡目标卡（没有目标设为0）
  int isMagicPlantedRecently[2];//幻幻鸡是否已种植

  WorkType() :map(*MallocMap(&hMemDC)) {}
};
//角色名图像类型
struct RoleNameType
{
  COLORREF heavy[heavyNameHeight][heavyNameWidth];//粗角色名
  bool light[lightNameHeight][lightNameWidth];//细角色名（双色）
};
const int maxRoleNameNum = 100;//角色名上限
int roleNameNum;//角色名数量
RoleNameType roleName[maxRoleNameNum];//已保存的粗细角色名
char roleNameList[maxRoleNameNum][maxPath];//角色名目录
//载入角色名
void LoadRoleName()
{
  const char folder[] = "用户参数\\角色名";
  if (!FileExist(folder))
    return;

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, roleNameList, maxRoleNameNum);//查找所有png文件

  FILE *f;
  char heavyNamePath[maxPath], lightNamePath[maxPath];
  roleNameNum = 0;
  for (int i = 0; i < filesNum && roleNameNum < maxRoleNameNum; i++)
  {
    sprintf_s(heavyNamePath, "%s\\%s", folder, roleNameList[i]);//填写粗角色名路径
    if (FileExist(heavyNamePath)) //如果可以打开
    {
      BitmapToColor(heavyNamePath, roleName[roleNameNum].heavy);//载入粗角色名图像
      roleNameList[i][strlen(roleNameList[i]) - 4] = 0;//删去".png"4个字符
      sprintf_s(lightNamePath, "%s\\%s.txt", folder, roleNameList[i]);//填写细角色名路径
      if (fopen_s(&f, lightNamePath, "r") == 0) //如果可以打开，载入细角色名双色图
      {
        for (int y = 0; y < lightNameHeight; y++)
          for (int x = 0; x < lightNameWidth; x++)
            roleName[roleNameNum].light[y][x] = fgetc(f) == '1';
        fclose(f);
        roleNameNum++;
      }
    }
  }
}
//任务保留信息类型
struct ReserveType
{
  COLORREF innateGrid[7][9][gridHeight][gridWidth];//地图7行9列的初始颜色
  byte maltoseTimes[7][9];//7行9列放棉麦的次数
  bool isMaltoseClearRequired;//是否需要在3分钟时清除棉麦次数（仅限音乐节）
  CustomType custom[maxCustomNum];//自定卡槽（点击启动按钮时更新）
  int customNum;//自定卡槽数量
  int bagY;//背包卡Y位置
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
MonitorType monitor[maxTaskNum];//任务的监视窗口信息
ParamType param[maxTaskNum];//任务的计划参数
StateType state[maxTaskNum];//任务的状态
WorkType work[maxTaskNum];//任务的执行参数、轨道参数和运行参数
ReserveType reserve[maxTaskNum];//任务开始时不重置的信息，只在需要时填写
int isTaskStarted[maxTaskNum];//任务是否正在运行
int isTaskStarted_Confirm[maxTaskNum];//任务是否正在运行（确认退出任务线程后才会设为0）
int isTaskHided[maxTaskNum][2];//窗口隐藏状态，1=隐藏，0=显示
int isHallModified[maxTaskNum];//是否手动修改过大厅句柄

//7. 执行器设置
const int recognitionFrame = 10;//识图帧率
const int unlimitedRefreshTimes = 50;//无限制选服次数
const int noImageTimeout = 120;//无图像处理所需时间
int mode = 1;//1=普通任务，2=高级任务
int zoom;//执行器缩放比例
int plantDelay;//放卡延迟
int noImageOperation;//无图像时操作（0=不处理，1=提示，2=停止）
int customOption;//带卡选择（0=最高星，1=最靠前）
int flipNum;//翻牌数量（最多5，因为第6张牌与“开始”按钮重合）
int bagFullOperation;//背包满时操作（0=停止，1=继续）
int expFullOperation;//经验满时操作（0=停止，1=继续）
int repairLevel;//补阵强度，数值1-10对应10%-100%
int timer;//定时器
int timerMode;//定时模式：0=关，1=倒计时，2=启动时刻
int operationSpeed;//操作速度，0=慢速，1=中速，2=快速
int maxRefreshTimes;//最大刷新次数
int globalAccelarationTime;//全局加速时间
int contestOption;//大赛模式

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
const COLORREF taskColor[6] = { RGB(255, 96, 128), RGB(224, 224, 0), RGB(96, 232, 96), RGB(0, 200, 240), RGB(255, 64, 255), RGB(255, 128, 0) };
const COLORREF stateColor[6] = { RGB(192, 60, 80), RGB(160, 160, 0), RGB(32, 184, 32), RGB(0, 128, 192), RGB(192, 0, 192), RGB(192, 96, 0) };
const COLORREF grayWhite = RGB(224, 224, 224);//默认文字的灰白色
COLORREF cardTemplate[4][templateHeight][templateWidth];//海星和0-2转煮蛋的标准模板
const COLORREF firstPageColor = 0x3a6a93;//第1层左边的颜色。用于判断魔塔第一页
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
const int waveLoot = 105;//战利品界面
const int ratColorWidth = 26;
COLORREF ratColor[ratColorWidth];
const int progressRatWidth = 31, progressHeadWidth = 11;//老鼠头宽度；进度条右侧固定颜色宽度
const int progressBarX = 696, progressBarY = 582, progressBarLength = 153;//进度条左端点和长度
COLORREF progressLeftColor[progressBarLength];//进度条左色
COLORREF progressRightColor[progressBarLength];//进度条右色
//进度条右侧可变颜色
const COLORREF possibleRightColor[] = { 0x45caec, 0x46c9ec, 0x48c9ec, 0x49c8ec, 0x4ac7ec, 0x4cc6ec, 0x4dc7ed };

//按钮组类型
struct ButtonGroupType
{
  int firstButton;//第一个按钮编号
  int row, column;//按钮的行数和列数
};

//10. 按钮信息
const int MaxButtonGroupNum = 100;
int buttonGroupNum;//按钮的类数
ButtonGroupType buttonGroup[MaxButtonGroupNum];
const int maxButtonNum = 1000;
int buttonNum;//按钮的个数
RectType buttons[maxButtonNum];
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
  int *HeightArray = (int *)malloc(row * sizeof(int));
  for (int i = 0; i < row; i++)
    HeightArray[i] = height;
  int code = RegisterButtonsArea(mode, row, column, x, y, width, HeightArray, title, RowWeight, ColumnWeight);
  free(HeightArray);
  return code;
}
//注册等宽按钮组，返回按钮组编号
int RegisterButtonsArea(int mode, int row, int column, int x, int y, int width, int *height, int title, int RowWeight, int ColumnWeight)
{
  int *WidthArray = (int *)malloc(column * sizeof(int));
  for (int i = 0; i < column; i++)
    WidthArray[i] = width;
  int code = RegisterButtonsArea(mode, row, column, x, y, WidthArray, height, title, RowWeight, ColumnWeight);
  free(WidthArray);
  return code;
}
//注册等高等宽按钮，返回按钮组编号
int RegisterButtonsArea(int mode, int row, int column, int x, int y, int width, int height, int title, int RowWeight, int ColumnWeight)
{
  int *HeightArray = (int *)malloc(row * sizeof(int));
  int *WidthArray = (int *)malloc(column * sizeof(int));
  for (int i = 0; i < row; i++)
    HeightArray[i] = height;
  for (int i = 0; i < column; i++)
    WidthArray[i] = width;
  int code = RegisterButtonsArea(mode, row, column, x, y, WidthArray, HeightArray, title, RowWeight, ColumnWeight);
  free(HeightArray);
  free(WidthArray);
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
  PaintButtonGroupGrid(ButtonClassCode, 1, MAXINT, 0);
}
//寻找按钮类ButtonClassCode的第row行第column列的按钮编号
int FindButton(int ButtonClassCode, int row, int column)
{
  if (ButtonClassCode < zero)//找不到名称为Name的按钮则返回-1
    return -1;
  return buttonGroup[ButtonClassCode].firstButton + row * buttonGroup[ButtonClassCode].column + column;
}
/*自动转化函数*/
int TransformStopSignal;//自动转化结束信号，1=结束程序
DWORD __stdcall AutoTransform(void *Title)//自动转化（无参数）
{
  HWND hWnd, hActiveWnd;
  TransformStopSignal = 0;
  while (TransformStopSignal == zero)//信号为0才执行剪贴板转化，信号为1立即退出线程
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
    RECT desktop, dialog;
    GetWindowRect(GetDesktopWindow(), &desktop);
    GetWindowRect(hDlg, &dialog);
    int xPos = (desktop.left + desktop.right - (dialog.right - dialog.left)) / 2;
    int yPos = (desktop.top + desktop.bottom - (dialog.bottom - dialog.top)) / 2;
    SetWindowPos(hDlg, HWND_TOP, xPos, yPos, 0, 0, SWP_NOSIZE);

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
struct InputBoxParam
{
  char *dest;//输入对象
  const char *prompt, *title, *defaultStr;//提示语，标题，初始文本
  int maxLength;//最大长度
};
//对话框过程函数
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    InputBoxParam *params = (InputBoxParam *)lParam;// 获取传入的参数
    //设置输入框位置
    RECT Desktop, Dialog;
    GetWindowRect(GetDesktopWindow(), &Desktop);
    GetWindowRect(hDlg, &Dialog);
    int xPos = (Desktop.left + Desktop.right - (Dialog.right - Dialog.left)) / 2;
    int yPos = (Desktop.top + Desktop.bottom - (Dialog.bottom - Dialog.top)) / 2;
    SetWindowPos(hDlg, HWND_TOP, xPos, yPos, 0, 0, SWP_NOSIZE);
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
  return DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWndActuator, DialogProc, (LPARAM)&params);
}
//带ban卡的轨道路径是否合法
bool IsBannedTrackLegal(const char *bannedPath)
{
  if (strchr(bannedPath, '*') || strchr(bannedPath, '?') || strchr(bannedPath, '\"') || strchr(bannedPath, '\t'))
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
template<size_t size> int InputTrack(char(&Track)[size])
{
  char TempTrack[maxPath] = {};
  strcpy_s(TempTrack, Track);

  CreateThread(NULL, 0, AutoTransform, (void *)"轨道路径", 0, NULL);//运行自动转化
  int InputButton = NewInputBox(TempTrack, maxPath,
    "输入相对路径（如Track.txt）或绝对路径（如C:\\Track.txt）。\n拖动文件到执行器内可自动填写。",
    "轨道路径", TempTrack);
  while (!IsTrackLegal(TempTrack) && InputButton == IDOK)
    InputButton = NewInputBox(TempTrack, maxPath,
      "路径中不能有【* ? \" < > |】，请重新输入。\n输入相对路径（如Track.txt）或绝对路径（如C:\\Track.txt）。\n拖动文件到执行器内可自动填写。",
      "轨道路径", TempTrack);
  TransformStopSignal = 1;

  if (InputButton == IDOK)
  {
    strcpy_s(Track, TempTrack);
    return 1;
  }
  return 0;
}

//从定时输入获取定时秒数，格式不匹配返回-1
int getTimer(char *timerString, int *timerMode)
{
  if (timerString[0] == '~') //如果首位是~，检查第1位起是否符合时刻格式
  {
    *timerMode = 2;
    timerString++;
  }
  else
    *timerMode = 1;
  return HmsToSecond(timerString);
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
//输入倒计时，直至符合格式要求为止。输入成功则返回1，否则返回0
int InputTimer(int *pTimer, int *pTimerMode)
{
  char prompt[300] = "输入倒计时hh:mm:ss（如01:20:00表示1小时20分钟后启动）\n"
    "或启动时间~hh:mm:ss（如~16:30:00表示16点30分启动）。\n"
    "可以只输hh:mm；右键删除定时；点【启动】才会开始计时。";
  char errorPrompt[300] = "您的输入不规范，请重新输入。\n";
  strcat_s(errorPrompt, prompt);
  int timerMode = 0;
  char timerString[10] = "00:00:00";
  int InputButton = NewInputBox(timerString, 10, prompt, "定时启动", timerString);
  while (InputButton == IDOK && getTimer(timerString, &timerMode) == -1)
    InputButton = NewInputBox(timerString, 10, errorPrompt, "定时启动", timerString);
  if (InputButton == IDOK)//退出循环时，如果点了确定，视为有效输入
  {
    *pTimer = getTimer(timerString, pTimerMode);
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
      TransformStopSignal = 1;
      return true;
    }
    inputButton = NewInputBox(destCopy, size, illegalPrompt, title, destCopy);//否则继续输入
  }
  TransformStopSignal = 1;
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
  if (tower[0] == 'c' || tower[0] == 'C') //密室 C1-C4
    if (level >= 1 && level <= 4)
      return true;
  return false;
}
//输入魔塔关卡（单人1001-1165；双人2001-2100；密室3001-3004）
bool InputTower(int *pLevel)
{
  char tower[5];//魔塔关卡名
  //根据关卡编号pLevel生成关卡名（如A165)
  sprintf_s(tower, "%c%d", *pLevel / 1000 - 1 + 'A', *pLevel % 1000);
  const char *propmt =
    "单塔A1~A165，A0=自动爬最高层；\n"
    "双塔B1~B100，B0=自动爬最高层；\n"
    "C1=炼金室，C2=藏经阁，C3=水晶宫，C4=威望屋。";
  if (InputString(tower, isTowerLegal, propmt, "选择关卡"))
  {
    if (tower[0] == 'a' || tower[0] == 'A') //单人 A1-A165
      *pLevel = 1000;
    if (tower[0] == 'b' || tower[0] == 'B') //双人 B1-B100
      *pLevel = 2000;
    if (tower[0] == 'c' || tower[0] == 'C') //密室 C1-C4
      *pLevel = 3000;
    *pLevel += atoi(tower + 1);
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
DWORD __stdcall PromptNoImage(void *pTask)//提示无图像
{
  int task = (int)pTask;
  work[task].isPromptOn = 1;
  char *Message = "图像获取失败，请切换到放卡账号，且不要最小化。\n可以用执行器隐藏游戏大厅。";
  PopMessage(task, Message);
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
void Accelerate(int task, int account)
{
  HWND hWndHall = GetHallWindow(work[task].hWnd[account]);//寻找大厅窗口
  if (!hWndHall) //无法找到大厅窗口，放弃加速
    return;
  RECT clientRect;
  GetClientRect(hWndHall, &clientRect);
  //经过DPI换算的加速按钮位置
  int accelerateX = clientRect.right - 340 * DPI / 96, accelerateY = 44 * DPI / 96;
  LeftClick(hWndHall, accelerateX, accelerateY);
  work[task].isAccelerationOn[account] = !work[task].isAccelerationOn[account];//记录加速开启/关闭
}
//退出任务
void ExitTask(int task)
{
  if (monitor[task].isExist) //停止任务时清空窗口
    monitor[task].ClearWindow();

  for (int account = 0; account < 2; account++)
  {
    work[task].isQuitted[account] = false;//重置关卡退出状态
    if (work[task].isAccelerationOn[account]) //关闭加速
      Accelerate(task, account);
  }

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
  const char secretName[5][10] = { "", "炼金室", "藏经阁", "水晶宫", "威望屋" };
  if (type == zero)//悬赏
    strcpy_s(levelName, bounty[level]);
  else if (type == 1)//勇士
    strcpy_s(levelName, challenge[level]);
  else if (type == 2)//魔塔
  {
    if (level / 1000 == 3)
      strcpy_s(levelName, secretName[level % 1000]);
    else if (level / 1000 == 2)
      sprintf_s(levelName, "双%d", level % 1000);
    else
      sprintf_s(levelName, "单%d", level % 1000);
  }
  else if (type == 3)//跨服
    sprintf_s(levelName, "%s%d", crossServer[level / 8], level % 8 + 8);
  else if (type == 4)//假期
    strcpy_s(levelName, "");
  else if (type == 5)//公会
    strcpy_s(levelName, guild[level]);
  else if (type == 17)//特殊
    strcpy_s(levelName, special[level]);
  else
    strcpy_s(levelName, islandLevel[type - basicTypeNum][level]);
}
//填写高级任务字符串（如"[2] 美味(神殿)"）
void GetAdvanceString(int list, int order, char(&str)[100])
{
  char levelString[100];
  GetLevelName(levelString, plot[list].advance[order].type, plot[list].advance[order].level);
  sprintf_s(str, "[%d] %s(%s)", order + 1, typeName[plot[list].advance[order].type], levelString);
}
//结束高级任务并提示Message
void StopAdvance(int list, const char *message = nullptr)
{
  int task = GetTask(list);
  plot[list].isAdvanceStarted = 0;//高级任务停止
  plot[list].isAdvanceStarted_Confirm = 0;//确认高级任务停止
  //将高级任务对应的两个普通任务都刹停
  for (int task0 = task; task0 <= task + 1; task0++)
  {
    isTaskStarted[task0] = 0;
    isTaskStarted_Confirm[task0] = 0;
  }
  isRepaintRequired = 1;//发送重绘指令
  if (message)
    PopMessage(task, message);//弹窗提示
  ExitThread(0);//高级任务结束
}
//创建日志文件
void CreateLogFile(int task, FILE **pFile, const char *code);
//写入日志文件头（启动时间，高级任务还包括已完成任务日志）
void WriteLogHead(int task, FILE *f)
{
  if (IsAdvance(task)) //高级任务：写入过去的日志
  {
    int list = GetList(task);
    char advanceString[100];
    GetAdvanceString(list, plot[list].playingOrder, advanceString);
    fprintf(f, "%s", plot[list].logFinished);//填写已执行完毕的任务日志
    //填写当前高级任务信息：类型、关卡、局数
    fprintf(f, "***** %s %d局\n", advanceString, plot[list].playingAdvance->maxGames);
  }
  fprintf(f, "启动时间：%s\n", work[task].startTimeString); //写入启动时间
}
//往文件f中写入平均用时和每局用时
void WriteLevelTime(int task, FILE *f)
{
  char timeString[30];//时间字符串
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
//高级任务advance是不是公会任务
bool IsMission(AdvanceType &advance)
{
  return advance.type == 5 && advance.level == 0;
}
//是否签到施肥
bool IsSpecial(AdvanceType &advance)
{
  return advance.type == 17;
}
//是否无轨任务（公会任务或签到施肥）
bool IsNoTrack(AdvanceType &advance)
{
  return IsMission(advance) || IsSpecial(advance);
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
    sprintf_s(work[task].lootDirectory, "%s\\ID%d_战利品", work[task].logDirectory, plot[list].playingOrder + 1);
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
  CreateLogFile(task, &f, "记录开始时间"); ///创建日志文件
  WriteLogHead(task, f); //写入启动时间
  fclose(f);
  CheckSleep(task, 100); //防止文件关闭失败
  strcat_s(work[task].embarkString, "任务开始\n");//启动信息增加一条：开始循环刷图
}
//记录每局时间（任务完成时也调用本函数）
void RecordLevelTime(int task, bool isTaskFinished)
{
  if (IsDual2P(task))
    return;
  FILE *f;
  CreateLogFile(task, &f, "记录每局时间"); ///创建日志文件
  WriteLogHead(task, f); //写入启动时间
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
  WriteLevelTime(task, f);//写入平均用时和每局用时
  if (isTaskFinished)//刷图任务结束时写入终止时间
  {
    time(&work[task].taskEndTime);
    GetTimeString(work[task].endTimeString, work[task].taskEndTime);
    fprintf(f, "终止时间：%s\n", work[task].endTimeString);
  }
  fclose(f);
  work[task].lastGameTick = work[task].thisGameTick;
}
//记录中断截图
void RecordShot(int task)
{
  //bool isDualTask = IsAdvance(task) && plot[GetList(task)].playingAdvance->host == 2;
  for (int account = 0; account < 2; account++) //中断截图
    if (work[task].isInvolved[account] == 1)
    {
      char shotPath[maxPath];//截图路径
      if (IsAdvance(task))
      {
        int list = GetList(task);
        sprintf_s(shotPath, "%s\\ID%d_第%d次中断_%dP.png", work[task].logDirectory, plot[list].playingOrder + 1,
          plot[list].timesRefreshed[plot[list].playingOrder] + 1, account + 1);
      }
      else
        sprintf_s(shotPath, "%s\\中断截图_%dP.png", work[task].logDirectory, account + 1);
      if (!WindowToBitmap(work[task].hWnd[account], shotPath))//截图记录中断原因，游戏窗口截不到就截大厅
        WindowToBitmap(work[task].hWndHall[account], shotPath);
    }
}
void WriteParameterToLog(int task, FILE *f)
//普通任务中断：记录启动流程和退出时参数
{
  fprintf(f, "退出时参数：\n");
  for (int account = 0; account < 2; account++)
  {
    fprintf(f, "%dP轨道：%s", account + 1, work[task].track[account]);
    if (strlen(work[task].track[account]) > 0)
    {
      if (work[task].trackRoleNum[account] == 1)
        fprintf(f, "（单人）");
      else
        fprintf(f, "（双人）");
    }
    fprintf(f, "\n");
  }
  if (work[task].gameTime == zero)
    fprintf(f, "每局用时：自动检测\n");
  else
    fprintf(f, "每局用时：%d\n", work[task].gameTime);
  for (int account = 0; account <= 1; account++)
  {
    fprintf(f, "[%dP]卡组%d, 标签(%d,%d), 游戏%d, 大厅%d\n", account + 1, work[task].deck[account],
      work[task].tag[account].x, work[task].tag[account].y, (int)work[task].hWnd[account],
      (int)work[task].hWndHall[account]);
  }
  fprintf(f, "初始大厅句柄：%d\n", (int)work[task].hMainHall);
  if (work[task].noImageTick > zero)
    fprintf(f, "（无图像）\n");
  else
    fprintf(f, "（有图像）\n");
}
const int RETRY_TASK = 1, SKIP_TASK = 2, END_TASK = 3;//报错等级
// 遇错中断时，返回状态并记录错误信息，弹窗提示并结束任务。
// stopLevel：0刷新重试 1刷新跳过 2强制中断。log：是否记录日志
void RecordError0(int task, const char *tip, const char *errorString,
  int stopLevel = RETRY_TASK, bool log = true)
{
  RecordShot(task);//中断截图
  ReturnState(task, tip);//返回状态信息
  strcpy_s(state[task].errorString, errorString);//记录出错原因

  if (IsDual2P(task)) //同步任务2P的报错转交给1P
  {
    strcpy_s(work[task - 1].dualErrorTip, tip);
    strcpy_s(work[task - 1].dualErrorString, errorString);
    work[task - 1].dualErrorLevel = stopLevel;
    work[task - 1].dualError = true;
    return;
  }

  if (log)
  {
    FILE *f;
    CreateLogFile(task, &f, "记录错误信息"); ///创建日志文件
    WriteLogHead(task, f); //写入启动时间（高级任务还包括已完成日志）
    WriteLevelTime(task, f);//写入平均用时和每局用时

    fprintf(f, "第%d局：%s\n", work[task].games, errorString);//写入错误原因和终止时间
    time(&work[task].taskEndTime);
    GetTimeString(work[task].endTimeString, work[task].taskEndTime);
    fprintf(f, "终止时间：%s\n", work[task].endTimeString);

    if (IsNormal(task)) //普通任务中断时记录任务参数
      WriteParameterToLog(task, f);
    fclose(f);
  }

  for (int account = 0; account <= 1; account++)//重置退出状态
    work[task].isQuitted[account] = false;

  if (IsAdvance(task)) //高级任务根据报错等级决定刷新等级
  {
    int list = GetList(task);
    if (maxRefreshTimes == 0) //自动刷新关闭：任何stopLevel都是强制终止
      state[task].refreshLevel = END_TASK;
    else //自动刷新开启：分类讨论
    {
      //大赛模式
      if (contestOption)
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
void RecordError(int task, int account, const char *tip, const char *errorString,
  int stopLevel = RETRY_TASK)
{
  char errorStringWithPlayer[1000];//带[nP]前缀的报错原因
  sprintf_s(errorStringWithPlayer, "[%dP]%s", account + 1, errorString);
  RecordError0(task, tip, errorStringWithPlayer, stopLevel);
}
//等待time毫秒，然后检查任务是否被停止。每1000ms检查一次。
void CheckSleep(int task, int time)
{
  while (time >= 0)
  {
    Sleep(time >= 1000 ? 1000 : time);
    if (isTaskStopped(task))
    {
      strcpy_s(state[task].tip, "已停止");//修改状态为“已停止”
      ExitTask(task);
    }
    if (work[task].dualError) //检查同步任务报错
    {
      work[task].dualError = false;
      RecordError0(task, work[task].dualErrorTip, work[task].dualErrorString, work[task].dualErrorLevel);
    }
    time -= 1000;
  }
}
//创建日志文件
void CreateLogFile(int task, FILE **pFile, const char *code)
{
  if (fopen_s(pFile, work[task].logPath, "w"))
  {
    char message[500];
    ReportLastError();
    sprintf_s(message, "打开日志失败。路径：\n%s\n代码位置：%s", work[task].logPath, code);
    RecordError0(task, "打开日志失败", message, END_TASK, false);
  }
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
    RecordError0(task, "日志爆满", "日志爆满", END_TASK);
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
//自定图像类型
struct ItemType
{
  COLORREF image[templateHeight][templateWidth];//物件图像
  char name[10];//物件名称
  int requiredSimilarity;//要求的相似度
  int offsetX, offsetY, width, height;//识别范围的偏移和宽高
};
const int maxItemNum = 200;//自定图像上限
char itemList[maxItemNum][maxPath];//自定图像目录
ItemType item[maxItemNum];//自定图像
int itemNum;//自定图像数量
//载入自定图像
void LoadItem()
{
  const char folder[] = "用户参数\\自定图像";
  if (!FileExist(folder))
    return;

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, itemList, maxItemNum);//查找所有png文件

  char path[maxPath] = {};
  itemNum = 0;
  for (int i = 0; i < filesNum && itemNum < maxItemNum; i++)
  {
    sprintf_s(path, "%s\\%s", folder, itemList[i]);
    if (FileExist(path)) //如果可以打开
    {
      //如果图片名称合格，则进行记录
      if (IsItemPathLegal(itemList[i], item[itemNum].name, &item[itemNum].requiredSimilarity,
        &item[itemNum].offsetX, &item[itemNum].offsetY, &item[itemNum].width, &item[itemNum].height))
      {
        BitmapToColor(path, item[itemNum].image);//读取卡片图像到image
        itemNum++;
      }
    }
  }
}
//载入战利品
void LoadLoot()
{
  const char folder[] = "附加程序\\图片\\物品";
  if (!FileExist(folder))
    return;

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, lootList, maxLootNum);//查找所有png文件

  char path[maxPath] = {};
  lootNum = 0;
  for (int i = 0; i < filesNum && lootNum < maxLootNum; i++)
  {
    sprintf_s(path, "%s\\%s", folder, lootList[i]);
    if (FileExist(path)) //如果可以打开
    {
      BitmapToColor(path, loot[lootNum].image);//读取卡片图像到image
      lootList[i][strlen(lootList[i]) - 4] = 0;//删除".png"
      strcpy_s(loot[lootNum].name, lootList[i]);//记录名称
      loot[lootNum].isReplacedRequired = true;
      lootNum++;
    }
  }
}
const int trashWidth = lootWidth, trashHeight = lootHeight;
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
  const char folder[] = "用户参数\\可删物品";
  if (!FileExist(folder))
    return;

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, trashList, maxTrashNum);//查找所有png文件

  char path[maxPath] = {};
  trashNum = 0;
  for (int i = 0; i < filesNum && trashNum < maxTrashNum; i++)
  {
    sprintf_s(path, "%s\\%s", folder, trashList[i]);
    if (FileExist(path)) //如果可以打开
    {
      BitmapToColor(path, trash[trashNum].image);//读取卡片图像到image
      trashList[i][strlen(trashList[i]) - 4] = 0;//删除".png"
      strcpy_s(trash[trashNum].name, trashList[i]);//记录名称
      trashNum++;
    }
  }
}

const int bannerX = 388, bannerY = 470, bannerWidth = 100, bannerHeight = 12;
const int maxBannerNum = 30;
int bannerNum;
char bannerList[maxBannerNum][maxPath];
struct BannerType
{
  char name[20];
  COLORREF image[bannerHeight][bannerWidth];
};
BannerType banner[maxBannerNum];
//截图关卡名
void SaveBanner(int task)
{
  char path[maxPath] = {};
  sprintf_s(path, "附加程序\\图片\\关卡名\\0.png");
  ColorToBitmap(work[task].map, path, bannerX, bannerY, bannerWidth, bannerHeight);
  RecordError0(task, "关卡名截图成功", "关卡名截图成功", END_TASK);
}
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
    if (FileExist(path)) //如果可以打开
    {
      BitmapToColor(path, banner[bannerNum].image);//读取卡片图像到image
      bannerList[i][strlen(bannerList[i]) - 4] = 0;//删除".png"
      strcpy_s(banner[bannerNum].name, bannerList[i]);//记录名称
      bannerNum++;
    }
  }
}
//获取关卡编号
int GetBanner(int task)
{
  for (int code = 0; code < bannerNum; code++)
  {
    if (IsBitmapEqual(work[task].map, banner[code].image, bannerWidth, bannerHeight,
      bannerX, bannerY))
      return code;
  }
  return -1;
}
//获取漫游类型
int GetRoamType(int task)
{
  int code = GetBanner(task);
  if (code == -1)
    return 0;
  return atoi(banner[code].name);
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
    if (FileExist(path)) //如果可以打开
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
//返回指向item[itemCode]的目标编号
int FindTarget(int task, int itemCode)
{
  for (int i = 0; i < work[task].targetNum; i++)
    if (work[task].target[i].itemCode == itemCode)
      return i;
  return -1;
}
//将卡槽order的图像加入目标列表（成功返回0，图像不存在返回-1，目标数量已满返回-2）
int AddTarget(int task, int account, int order)
{
  int itemCode = FindItem(work[task].slot[account][order].image);
  if (itemCode == -1)
    return -1;
  int targetCode = FindTarget(task, itemCode);
  if (targetCode == -1) //目标列表不存在该物件，增加一项目标
  {
    if (work[task].targetNum >= maxTargetNum)
      return -2;
    TargetType &target = work[task].target[work[task].targetNum];
    target.itemCode = itemCode;//记录物品编号
    work[task].targetNum++;//目标数量+1
  }
  work[task].slot[account][order].itemCode = itemCode;
  return 0;
}

const int starX = 9, starY = 8, starWidth = 6, starHeight = 8;
COLORREF starImage[17][starHeight][starWidth];//17个星级的颜色
//读取星级颜色
void LoadStar()
{
  char path[maxPath];
  for (int star = 1; star <= 16; star++)
  {
    sprintf_s(path, "附加程序\\图片\\星级\\%d.png", star);
    BitmapToColor(path, starImage[star]);
  }
}
//Work[task].map (x0,y0)位置是几星级
int GetStarXY(int task, int x0, int y0)
{
  for (int star = 1; star <= 16; star++)
    if (IsBitmapEqual(work[task].map, starImage[star], starWidth, starHeight, x0, y0))
      return star;
  return 0;
}
//读取选卡界面row行column列卡片的星级
int GetStar(int task, int row, int column)
{
  int startX = bagX + starX + column * bagWidth;
  int startY = reserve[task].bagY + starY + row * bagHeight;
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
      int startX = bagX + starX + column * bagWidth;
      int startY = reserve[task].bagY + starY + row * bagHeight;
      ColorToBitmap(work[task].map, path, startX, startY, starWidth, starHeight);
    }
  RecordError(task, 0, "保存星级完成", "保存星级完成", END_TASK);
}
//载入自定卡槽
void LoadCustom(int task)
{
  const char folder[] = "用户参数\\自定卡槽";
  if (!FileExist(folder))
    return;

  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.png", folder);
  int filesNum = GetFileList(searchPath, customList, maxCustomNum);//查找所有png文件

  char path[maxPath] = {};
  int &customNum = reserve[task].customNum;
  CustomType(&custom)[maxCustomNum] = reserve[task].custom;
  //先读取两个内置咖啡粉（0=钢丝球，1=咖啡粉）
  for (int code = 0; code < 2; code++)
  {
    sprintf_s(path, "附加程序\\图片\\内置卡槽\\咖啡粉_%d.png", code);
    BitmapToColor(path, bag);//读取卡片图像到bag数组
    custom[code].priority = code;
    strcpy_s(custom[code].name, "咖啡粉");
    for (int x = 0; x < customWidth; x++) //记录图像
      for (int y = 0; y < customHeight; y++)
        custom[code].image[y][x] = bag[customY + y][customX + x];
  }
  customNum = 2;

  for (int i = 0; i < filesNum && customNum < maxCustomNum; i++)
  {
    sprintf_s(path, "用户参数\\自定卡槽\\%s", customList[i]);
    if (strstr(customList[i], "咖啡粉")) //跳过自定咖啡粉的读取
      continue;
    if (FileExist(path)) //如果可以打开
    {
      BitmapToColor(path, bag);//读取卡片图像到bag数组
      char *underline = strrchr(customList[i], '_');//下划线位置
      if (underline == nullptr) //如果没有下划线，优先级为-1
      {
        custom[customNum].priority = -1;//-1优先级
        customList[i][strlen(customList[i]) - 4] = 0;
      }
      else
      {
        custom[customNum].priority = atoi(underline + 1);//记录优先级
        underline[0] = 0;
      }
      strcpy_s(custom[customNum].name, customList[i]);//记录名称
      for (int x = 0; x < customWidth; x++) //记录图像
        for (int y = 0; y < customHeight; y++)
          custom[customNum].image[y][x] = bag[customY + y][customX + x];
      customNum++;
    }
  }
}
//(x0,y0)位置的卡和自定卡code是否相等
bool IsCustomEqualXY(int task, int code, int x0, int y0)
{
  //有一个像素不匹配，返回-1
  for (int x = 0; x < customWidth; x++)
    for (int y = 0; y < customHeight; y++)
      if (reserve[task].custom[code].image[y][x] != work[task].map[y0 + y][x0 + x])
        return false;
  return true;
}
//row行column列的卡和自定卡code是否相等，相等则返回星级，否则返回-1
//如果指定了targetStar，则只查找指定星级；如果targetStar==-1，则无要求
int IsCustomEqual(int task, int code, int row, int column, int targetStar)
{
  //卡片不绑定，返回-1
  int lockX = bagX + 13 + column * bagWidth;
  int lockY = reserve[task].bagY + 51 + row * bagHeight;
  if (work[task].map[lockY][lockX] != 0xff2b9c)
    return -1;

  //卡片不匹配，返回-1
  int startX = bagX + customX + column * bagWidth;
  int startY = reserve[task].bagY + customY + row * bagHeight;
  if (!IsCustomEqualXY(task, code, startX, startY))
    return -1;

  int star = GetStar(task, row, column);//读取星级
  //如果指定了星级，而星级不匹配，也返回-1
  if (targetStar >= 0 && targetStar != star)
    return -1;
  return star;
}
//在本页44张卡中查找自定卡code的位置，返回星级。找不到返回-1
//targetStar：指定星级；targetStar==-1查找任意星级；targetStar==-2查找最高星级
int FindCustom(int task, int code, int targetStar, int *pRow = nullptr, int *pColumn = nullptr)
{
  int maxStar = -1;//本页最高星级
  for (int row = 0; row < 4; row++)
    for (int column = 0; column < 11; column++)
    {
      //查找row行column卡片是否匹配，是则记录星级
      int star = IsCustomEqual(task, code, row, column, max(targetStar, -1));
      if (star > maxStar)//如果匹配且超过已记录的最高星级
      {
        if (pRow) //记录行列
          *pRow = row;
        if (pColumn)
          *pColumn = column;
        maxStar = star;
        if (targetStar >= -1) //如果不要求最高星级，则任意找到一个就返回
          return star;
      }
    }
  //查找完成返回maxStar，如果maxStar==-1则说明查找失败
  return maxStar;
}
//公会任务相关函数
const int missionX = 136, missionWidth = 140, missionHeight = 18;//公会任务名称位置
const int missionStateX = 357, missionStateWidth = 10, missionStateHeight = 10;//“进行中”字样位置
int missionNum = 0;//任务数量
char missionInProgress[missionStateWidth][missionStateHeight];//进行中字样01谱（1=白色）
char mission[maxAdvanceNum][missionWidth][missionHeight];//公会任务名01谱（1=missionColor）
char curMission[missionWidth][missionHeight];//当前读取的任务01谱
const COLORREF missionColor = 0x001b32;//公会任务名边框颜色
const COLORREF missionBarColor = 0x0c497d;//公会任务名左侧竖条颜色
bool missionExist[maxAdvanceNum];//公会任务是否存在
COLORREF missionBitmap[missionHeight][missionWidth];
//载入公会任务名图像到mission数组中
void LoadMission()
{
  char path[maxPath];
  for (missionNum = 0; missionNum < maxAdvanceNum; missionNum++)//载入公会任务
  {
    sprintf_s(path, "附加程序\\公会任务\\%d.png", missionNum + 1);
    if (!FileExist(path))
      break;
    BitmapToColor(path, missionBitmap);
    for (int x = 0; x < missionWidth; x++)
      for (int y = 0; y < missionHeight; y++)
        mission[missionNum][x][y] = (missionBitmap[y][x] == missionColor);
  }
  //读取“进行中”字样
  FILE *f;
  fopen_s(&f, "附加程序\\公会任务\\进行中.txt", "r");
  for (int x = 0; x < missionStateWidth; x++)
    for (int y = 0; y < missionStateHeight; y++)
      missionInProgress[x][y] = fgetc(f) - '0';
  fclose(f);
}
//将Work[task].Map图像中missionY高度的任务记入curMission
void LoadCurMission(int task, int missionY)
{
  for (int x = 0; x < missionWidth; x++)
    for (int y = 0; y < missionHeight; y++)
      curMission[x][y] = work[task].map[missionY + y][missionX + x] == missionColor;
}
//curMission与mission[order]是否一致
bool missionEqual(int order)
{
  for (int x = 0; x < missionWidth; x++)
    for (int y = 0; y < missionHeight; y++)
      if (curMission[x][y] != mission[order][x][y]) //有一个颜色不匹配就返回false
        return false;
  return true;
}
//成长值是否已满
bool isGrowthFull(char *growth)
{
  char *slash = strchr(growth, '/');//找斜杠位置
  if (!slash) //没有斜杠，识别失败，视为成长值已满
    return true;
  int currentGrowth = atoi(growth);
  int maxGrowth = atoi(slash + 1);
  return currentGrowth >= maxGrowth;
}
int ListX = 138, ListY = 126, ListWidth = 23, ListHeight = 229;//房间列表
//6类数字名称
const int LIST = 0, INDOOR = 1, CROSS_LIST = 2, CROSS_INDOOR = 3, FIRE = 4, TREE = 5;
//数字颜色：0=列表；1=房间内；2=跨服列表；3=跨服房间内
const COLORREF digitColor[6] = { 0xf8ffc6, 0xffffff, 0xffffff, 0xdceeff, 0x000000, 0x1ae641 };

//数字类型：保存房间号和火苗0-9数字图像，用于读数
template <size_t width, size_t height>
class DigitType
{
  bool digit[11][width][height];//0-9十个数字和斜杠的图像：颜色等于digitColor为true，否则为false
  int method;//get函数使用的方法：0=列表；1=房间内；2=跨服列表；3=跨服房间内
  const char *name;
  int digitNum;
  int digitX, digitY, digitDX;//起始XY坐标和数字间距
  int lengthDX[6];//数字间距，长度增加1位带来的左偏移
  int roomDX, roomDY;//房间间距（只有跨服列表需要设定）
  //判断(x0,y0)起始的图像是否为数字n
  bool Equal(int task, int x0, int y0, int n, COLORREF color2 = -1, COLORREF color3 = -1)
  {
    for (int x = 0; x < width; x++)
      for (int y = 0; y < height; y++)
      {
        bool isMatched = work[task].map[y0 + y][x0 + x] == digitColor[method]
          || work[task].map[y0 + y][x0 + x] == color2
          || work[task].map[y0 + y][x0 + x] == color3;
        if (isMatched != this->digit[n][x][y])
          return false;
      }
    return true;
  }
  //读取从(x0,y0)开始的数字
  int GetDigitXY(int task, int x0, int y0, COLORREF color2 = -1, COLORREF color3 = -1)
  {
    for (int n = 0; n < digitNum; n++)
      if (Equal(task, x0, y0, n, color2, color3))
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
    case LIST:
      name = "列表房间号";
      digitX = 150, digitY = 0, digitDX = 6;
      for (int i = 1; i <= 4; i++)
        lengthDX[i] = i * 3;
      break;
    case INDOOR:
      name = "房间内房间号";
      digitX = 164, digitY = 28, digitDX = 7;
      for (int i = 1; i <= 4; i++)
        lengthDX[i] = i * 3;
      break;
    case CROSS_LIST:
      name = "列表房间号";//跨服
      digitX = 409, digitY = 116, digitDX = 6;
      roomDX = 263, roomDY = 123;
      break;
    case CROSS_INDOOR:
      name = "跨服房间内房间号";
      digitX = 64, digitY = 26, digitDX = 8;
      break;
    case FIRE:
      name = "火苗";
      digitX = 172, digitY = 75, digitDX = 7;
      lengthDX[2] = 4, lengthDX[3] = 7, lengthDX[4] = 11, lengthDX[5] = 14;
      break;
    case TREE:
      name = "列表房间号";//公会树成长值
      digitNum = 11;//除了0-9还要识别斜杠
      digitX = 192, digitY = 103, digitDX = 6;
      break;
    }
  }
  //载入文件
  void Load()
  {
    FILE *f;
    char path[maxPath];
    for (int n = 0; n < digitNum; n++)
    {
      sprintf_s(path, "附加程序\\数字\\%s\\%d.txt", name, n);
      fopen_s(&f, path, "rb");
      for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
          digit[n][x][y] = fgetc(f) == '1';
      fclose(f);
    }
  }
  //读取位置不定的成长值，返回字符串长度
  int GetMovingGrowth(int task, char(&str)[10], int movingGrowthX,
    int movingGrowthY, int movingGrowthWidth, COLORREF color2 = -1, COLORREF color3 = -1)
  {
    memset(str, 0, sizeof(str));

    int slashX = 0;//斜杠位置
    //检测斜杠位置
    for (int x = movingGrowthX; x < movingGrowthX + movingGrowthWidth; x++)
      if (Equal(task, x, movingGrowthY, 10, color2, color3))
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
      int digit = GetDigitXY(task, x, movingGrowthY, color2, color3);
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
    return GetMovingGrowth(task, str, growthX, growthY, growthWidth, 0x00ff00, 0xff0000);
  }
  //读取可摘果实
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
  //成长值是否已满
  bool isFull(int task)
  {
    char growth[10] = {};
    GetGrowth(task, growth);
    char fruit[10] = {};
    GetFruit(task, fruit);
    //如果今日成长值已满，或者可摘果实不是0/0（即公会树成熟），则视为成长值已满
    return isGrowthFull(growth) || strcmp(fruit, "0/0") != 0;
  }
  //总成长值是否已满
  bool isTotalFull(int task)
  {
    char growth[10] = {};
    GetTotalGrowth(task, growth);
    return isGrowthFull(growth);
  }
  //读取数字
  int Get(int task, int row = 0, int column = 0, int height = 0, int account = 0)
  {
    int length = 5;//跨服房间号为5位
    if (method == 0 || method == 1 || method == 4)//普通房间号和火苗需要获得长度
    {
      int maxLength = 4;
      if (method == 4)//读取火苗需要根据卡槽数确定digitX
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
  //【跨服列表】读取列表房间号（返回房间数量，每个房间的位置和房间号）
  int GetCrossListDigit(int task, int *x, int *y, int *room)
  {
    int roomNum = 0;//房间数量
    int row, column;
    int RoomDigit;
    for (row = 0; row <= 2; row++)
      for (column = 0; column <= 1; column++)
      {
        RoomDigit = Get(task, row, column);
        if (RoomDigit == -1)
          return roomNum;
        x[roomNum] = digitX + column * roomDX;
        y[roomNum] = digitY + row * roomDY;
        room[roomNum] = RoomDigit;
        roomNum++;
      }
    return roomNum;
  }
  //读取列表房间号（返回房间数量，每个房间的高度和房间号）
  int GetListDigit(int task, int *height, int *room)
  {
    int RoomNum = 0;
    int x, y;
    int row = 0;
    for (y = ListY; y < ListY + ListHeight; y++)
    {
      for (x = ListX; x < ListX + ListWidth; x++)
        if (work[task].map[y][x] == digitColor[method]) //记录连续白色的行数
        {
          row++;
          break;
        }
      if (x == ListX + ListWidth)//一旦断连，行数清零
        row = 0;
      if (row == 8)//连续8行白色视为有效房间号
      {
        height[RoomNum] = y;
        room[RoomNum] = Get(task, 0, 0, y);
        RoomNum++;
        row = 0;
      }
    }
    return RoomNum;
  }
};
DigitType<5, 8> listDigit(0);//列表房间号
DigitType<6, 8> indoorDigit(1);//房间内房间号
DigitType<5, 8> crossListDigit(2);//跨服列表房间号
DigitType<7, 10> crossIndoorDigit(3);//跨服房间内房间号
DigitType<6, 10> energyDigit(4);//火苗数值
DigitType<5, 8> growthDigit(5);//公会树成长值
//载入房间号数字图像
void LoadDigit()
{
  listDigit.Load();
  indoorDigit.Load();
  crossListDigit.Load();
  crossIndoorDigit.Load();
  energyDigit.Load();
  growthDigit.Load();
}
struct BitmapType
{
  COLORREF color[20][20];//颜色信息
  char name[30];//位图名称
  int width, height;//宽度和高度
  int x, y;//起始位置，只适用于定点识别。模糊找图设为-1
};
const int maxBitmapNum = 500;
char bitmapList[maxBitmapNum][maxPath];
int bitmapNum;//用于识图的10x10位图张数
BitmapType bitmap[maxBitmapNum];
//根据图片文件生成图片目录
void MakeBitmapList()
{
  int fileNum = GetFileList("附加程序\\图片\\界面\\*", bitmapList, maxBitmapNum);
  FILE *f;
  fopen_s(&f, "附加程序\\图片目录.txt", "w");
  for (int i = 0; i < fileNum; i++)
    fprintf(f, "%s\n", bitmapList[i]);
  fclose(f);
}
//读取位图列表，返回读取的数量
int LoadBitmapList(char *dir, char(*bitmapList)[maxPath])
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
void LoadSingleBitmap(char *fileName)
{
  char path[maxPath] = {};
  char name[100];

  if (fileName[0] == '(')//括号开头，获取位图名称name和起始坐标
    sscanf_s(fileName, "(%d,%d)%s", &bitmap[bitmapNum].x, &bitmap[bitmapNum].y, name, sizeof(name));
  else //否则(x,y)=(-1,-1)，即找图模式
  {
    strcpy_s(name, fileName);
    bitmap[bitmapNum].x = -1;
    bitmap[bitmapNum].y = -1;
  }
  name[strlen(name) - 4] = 0;//名称删去最后4个字符".png"
  strcpy_s(bitmap[bitmapNum].name, name);//记录位图名称
  sprintf_s(path, "附加程序\\图片\\界面\\%s", fileName);//填写位图完整路径
  if (!FileExist(path))
  {
    char message[100];
    sprintf_s(message, "缺少依赖文件：\n%s\n请检查是否更新完整。", path);
    MessageBox(NULL, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    exit(0);
  }
  //保存图片的颜色信息和尺寸
  BitmapToColor(path, bitmap[bitmapNum].color, &bitmap[bitmapNum].width, &bitmap[bitmapNum].height);
  bitmapNum++;//记录位图数量+1
}
//载入所有图片
void LoadPicture()
{
  int num = LoadBitmapList("附加程序\\图片\\界面", bitmapList);//读取所有位图的名称，返回数量
  bitmapNum = 0;
  for (int order = 0; order < num; order++)
    LoadSingleBitmap(bitmapList[order]);
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
int FindPictureInMap(int task, int bitmapCode, int *px = NULL, int *py = NULL,
  int x0 = 0, int y0 = 0, int x1 = 0, int y1 = 0, COLORREF specificColor = 0)
{
  bool areaSearch = true;//是否区域找图
  RECT area;//找图区域
  //先判断找图模式
  if (bitmap[bitmapCode].x == -1 && bitmap[bitmapCode].y == -1)//默认全屏找图
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
      SetRect(area, bitmap[bitmapCode].x + x0, bitmap[bitmapCode].y + y0, 0, 0);
    }
  }

  if (areaSearch)//区域找图
  {
    for (int y = area.top; y <= area.bottom - bitmap[bitmapCode].height; y++)
      for (int x = area.left; x <= area.right - bitmap[bitmapCode].width; x++)
        if (IsBitmapEqual(work[task].map, bitmap[bitmapCode].color, bitmap[bitmapCode].width, bitmap[bitmapCode].height,
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
    if (IsBitmapEqual(work[task].map, bitmap[bitmapCode].color, bitmap[bitmapCode].width,
      bitmap[bitmapCode].height, area.left, area.top, 0, 0, specificColor))
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
  for (int code = 0; code < bitmapNum; code++)
    if (strcmp(pictureName, bitmap[code].name) == 0)
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
    RecordError0(task, "缺少图片", errorString, END_TASK);
  }
  return FindPictureInMap(task, picture, px, py, x0, y0, x1, y1, specificColor);
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
      RecordError0(task, "缺少图片", errorString, END_TASK);
    }
  }

  //截图游戏窗口
  if (!IsWindow(work[task].hWnd[account]))
    RecordError(task, account, "游戏窗口已关闭", "游戏窗口已关闭");
  if (!IsGameWindowVisible(work[task].hWnd[account])) //窗口未显示，无法识图
    return 0;
  if (!UpdateMap(task, account))//截取屏幕图像，截图失败则返回0
    return 0;

  //从截图中查找图片
  for (int i = 0; i < pictureNum; i++)
    if (FindPictureInMap(task, picture[i], px, py, x0, y0, x1, y1))
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
  int x0 = 0, int y0 = 0, int timeout = 5000, bool report = true)
{
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
          RecordError(task, account, "未进入指定界面", message);
        }
        else
        {
          sprintf_s(message, "未能关闭[%s]界面", pictureName[0]);
          RecordError(task, account, "未关闭指定界面", message);
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
  int dx = 0, int dy = 0, int timeout = 5000, bool report = true)
{
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
        RecordError0(task, "图像获取失败", "连续10分钟未获取到图像，任务已终止", END_TASK);
      else if (NoImageTime >= noImageTimeout * 1000)//否则检查是否超过设定时间
      {
        if (noImageOperation == 1 && work[task].isPromptOn == zero)//提示
          CreateThread(NULL, 0, PromptNoImage, (void *)task, 0, NULL);//提示无图像
        else if (noImageOperation == 2)//中断
          RecordError0(task, "图像获取失败", "连续2分钟未获取到图像，任务已终止", END_TASK);
      }
    }
  }
}
//更新account窗口截图，成功返回1
int UpdateMap(int task, int account)
{
  reserve[task].updateNum++;
  if (!IsWindow(work[task].hWnd[account]))
    RecordError(task, account, "游戏窗口已关闭", "游戏窗口已关闭");
  if (!IsGameWindowVisible(work[task].hWnd[account]))
  {
    reserve[task].iconicNum++;
    return 0;
  }
  work[task].noImageTick = 0;//运行到这里就是有图像了
  int result = MultiPrintWindow(work[task].hWnd[account], work[task].hMemDC, 5);//截图5次直到成功，记录结果
  InvalidateRect(work[task].hWnd[account], NULL, false);//重画
  for (int y = 0; y < gameHeight; y++)
    for (int x = 0; x < gameWidth; x++)
      work[task].map[y][x] &= 0x00ffffff;
  if (result == 1)
    reserve[task].successfulUpdateNum++;
  return result;
}
//用指定窗口图像更新Work[task].map
int UpdateMapFromWindow(int task, HWND hWnd)
{
  reserve[task].updateNum++;
  if (!IsGameWindowVisible(hWnd))
  {
    reserve[task].iconicNum++;
    return 0;
  }
  work[task].noImageTick = 0;//运行到这里就是有图像了
  int result = MultiPrintWindow(hWnd, work[task].hMemDC, 5);//截图5次直到成功，记录结果
  InvalidateRect(hWnd, NULL, false);//重画
  for (int y = 0; y < gameHeight; y++)
    for (int x = 0; x < gameWidth; x++)
      work[task].map[y][x] &= 0x00ffffff;
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
  else if (color[4] == proceedColor) //继续作战
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
//鼠标移动到指定位置
BOOL MoveTo(int task, int account, int x, int y)
{
  if (work[task].isInvolved[account] == 0) //如果账号不参与，则不执行点击
    return 0;
  if (work[task].isDpiAwareRequired[account] == 1)//如果需要DPI修正
    return MouseMoveDPI(work[task].hWnd[account], x, y);
  else
    return MouseMove(work[task].hWnd[account], x, y);
}
BOOL MoveTo(int task, int account, POINT point)
{
  return MoveTo(task, account, point.x, point.y);
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
//单击指定位置
BOOL Click(int task, int account, int x, int y)
{
  //如果账号不参与或已退出关卡，则不执行点击
  if (!work[task].isInvolved[account] || work[task].isQuitted[account])
    return 0;
  if (!IsWindow(work[task].hWnd[account]))//如果窗口不见了
    RecordError(task, account, "游戏窗口已关闭", "游戏窗口已关闭");
  if (work[task].isDpiAwareRequired[account] == 1)//如果需要DPI修正
    return LeftClickDPI(work[task].hWnd[account], x, y);
  else
    return LeftClick(work[task].hWnd[account], x, y);
}
BOOL Click(int task, int account, POINT location)
{
  return Click(task, account, location.x, location.y);
}
//单击地图某个格子
void Lay(int task, int account, int row, int column)
{
  Click(task, account, gridX + (column - 1) * gridWidth + gridWidth / 2,
    gridY + (row - 1) * gridHeight + gridHeight / 2);
}
//放置人物
void LayRole(int task, int account)
{
  for (int times = 1; times <= work[task].roleLocNum[account]; times++)
  {
    Lay(task, account, work[task].roleLoc[account][times] / 10, work[task].roleLoc[account][times] % 10);
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
//翻到第page页
void TurnToPage(int task, int account, int page)
{
  int counter = 0;
  while (GetColor(task, account, 106, 540) != firstPageColor)
  {
    counter++;
    if (counter > 10)
      RecordError(task, account, "魔塔翻页失败", "魔塔无法翻到第一页");
    Click(task, account, firstPageX, firstPageY);
    CheckSleep(task, 200);
  }
  RegionalMapShot(work[task].hWnd[account], work[task].towerPage[1], 95, 571);//记录第1页
  for (int i = 2; i <= page; i++)
  {
    memcpy(work[task].towerPage[0], work[task].towerPage[1], sizeof(work[task].towerPage[0]));//记录新页数到LastPage
    Click(task, account, pageDownX, pageDownY);//点击翻页按钮
    counter = 0;
    while (IsBitmapEqual(work[task].towerPage[1], work[task].towerPage[0], 14, 10))//等到翻页成功
    {
      counter++;
      if (counter > 5)
        RecordError(task, account, "魔塔翻页失败", "魔塔无法向后翻页");
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
      RecordError(task, account, "无法获取图像", "无法获取图像");
    CheckSleep(task, 50);//如果截图失败或图像全黑，就等50 ms再截
  }
}
//点击滚动条，直到滚动生效，但最多等1000ms
template <size_t height>
bool Scroll(int task, int account, POINT location, int scrollX, int scrollY, COLORREF(&scroll)[height][1])
{
  //记录原Map的滚动条
  for (int y = 0; y < height; y++)
    scroll[y][0] = work[task].map[scrollY + y][scrollX];
  Click(task, account, location);//点击滚动条
  //每隔100ms截图，直到滚动条变化
  int counter = 0;
  do
  {
    counter++;
    if (counter > 5) //截图5次检测不到变化，视为滚动失败，不再检测
      return false;
    CheckSleep(task, 100);
    ForcedUpdateMap(task, account);
  } while (IsBitmapEqual(work[task].map, scroll, 1, height, scrollX, scrollY));
  return true;
}
//点击防御卡背包滚动条，直到滚动生效，但最多等1000ms
void ScrollCustom(int task, int account, POINT location)
{
  Scroll(task, account, location, customScrollX, customScrollY, work[task].customScroll);
}
//点击战利品滚动条，直到滚动生效，但最多等1000ms
void ScrollLoot(int task, int account, POINT location)
{
  Scroll(task, account, location, lootScrollX, lootScrollY, work[task].lootScroll);
}
//某个卡槽的所有候选卡类型
struct CandidateType
{
  int num;//候选自定卡个数
  int priority[11];//优先级（从高到低排列）
  int code[11];//自定卡编号
};
//获取卡槽order的所有候选自定卡的编号和优先级（优先级从大到小排列）
void GetCustomCandidate(int task, int account, int order, CandidateType(&candidate))
{
  candidate.num = 0;
  //按编号从大到小遍历自定卡
  for (int code = reserve[task].customNum - 1; code >= 0; code--)
    //如果某张自定卡的名称正好是order号卡槽，则记录
    if (strcmp(reserve[task].custom[code].name, work[task].originalName[account][order]) == 0)
    {
      candidate.code[candidate.num] = code;
      candidate.priority[candidate.num] = reserve[task].custom[code].priority;
      candidate.num++;
    }
}
//判断战利品个数
int GetLootQuantity(int task, int x0, int y0)
{
  for (int num = 2; num <= 9; num++)
    if (IsBitmapCovering(work[task].loot, lootDigit[num], lootDigitWidth, lootDigitHeight, x0, y0, 0, 0, 0xffffff))
      return num;
  return 1;
}

//读取道具数量个位数字（无数字返回-1）
int GetPropQuantity(int task, int x0, int y0)
{
  for (int num = 0; num <= 9; num++)
    if (IsBitmapCovering(work[task].map, lootDigit[num], lootDigitWidth, lootDigitHeight, x0, y0, 0, 0, 0xffffff))
      return num;
  return -1;
}

const int propX = 468, propY = 89, propWidth = 49, propHeight = 49;//道具背包
const int storeX = 31, storeY = 93, storeWidth = 49, storeHeight = 49;//储藏室
const int jewelX = 559, jewelY = 90, jewelWidth = 49, jewelHeight = 49;//宝石分解

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
      RecordError(task, account, "物品入库失败", "物品无法放入仓库");
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
      RecordError(task, account, "物品拿起失败", "物品无法从仓库拿起");
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
      RecordError(task, account, "物品出库失败", "物品无法放回背包");
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
    if (GetPropQuantity(task, digitX, digitY) != -1)
      break;
    counter++;
    if (counter > 30)
      RecordError(task, account, "物品出库失败", "物品无法放回背包");
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
  int thisPropY = propY + row * propHeight - work[task].propOffsetY;//道具格子左上角Y（已计算偏移）
  int digitX = thisPropX + lootDigitX, digitY = thisPropY + lootDigitY;//道具数量个位数位置

  ForcedUpdateMap(task, account);
  CopyMap(work[task].initialProp, work[task].map, thisPropX, thisPropY);//保存移动前的道具图像，用于检验物品入库出库
  int numInProp = GetPropQuantity(task, digitX, digitY);//获取道具数量

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
  int numInStore = GetPropQuantity(task, storeDigitX, storeDigitY);//获取储物区道具数量

  Click(task, account, store);//点击储物仓第1格
  WaitPickStore(task, account, thisPropX, thisPropY);//等待储藏室物品被拿起

  if (column <= 1) //如果列数为0或1，利用保存的图像校准翻页量
  {
    ForcedUpdateMap(task, account);
    for (int y = propY + 6 * propHeight; y >= propY; y--) //从底部往上寻找
      if (IsBitmapEqual(work[task].map, work[task].calibration, calibrationWidth, calibrationHeight,
        thisPropX + calibrationX, y))
      {
        work[task].propOffsetY += thisPropY + calibrationY - y;//更新翻页量
        thisPropY = propY + row * propHeight - work[task].propOffsetY;//重新计算thisPropY
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
  for (int i = 22; i < MAXINT; i++)
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
    if (_access(path, 0) == 0) //如果可以打开
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
    if (FileExist(path)) //如果可以打开
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
          int quantityWidth = digitsNum * (lootDigitWidth + 1) + 1;
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
            int digitX = quantityRight - (i + 1) * (lootDigitWidth + 1) + 1;
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
  int y0 = propY + row * propHeight;
  return IsItemLoading(work[task].map, x0, y0);
}
//道具背包是否已加载好
bool isPropLoaded(int task)
{
  for (int row = 0; row < 7; row++)
    for (int column = 0; column < 9; column++)
      if (isPropLoading(task, row, column))
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
  /*
  FILE *f;
  fopen_s(&f, "1P战利品空格.txt", "w");
  for (int row = 0; row < 5; row++)
  {
    for (int column = 0; column < 10; column++)
      fprintf(f, "%4d", work[task].blankSimilarity[0][row][column]);
    fprintf(f, "\n");
  }
  fclose(f);
  */

  work[task].isLootSaved[account] = true;
  return 1;
}
//自动带卡首轮查找（输入空卡槽信息，输出每张卡存在的最佳候选卡的编号和星级）
void ScanCustom(int task, int account, bool(&isSlotRequired)[maxSlotNum + 1],
  int(&bestCode)[maxSlotNum + 1], int(&bestStar)[maxSlotNum + 1])
{
  const COLORREF customColor[2] = { 0x001f41, 0x1a4263 };//防御卡识别色

  const POINT top = { customScrollX, customScrollY + 2 };
  const POINT pageDown = { customScrollX, customScrollY + customScrollHeight + 8 };
  int row = -1, column = -1;

  CandidateType candidate[maxSlotNum + 1] = {};//21张卡的候选卡信息
  int bestPriority[maxSlotNum + 1] = {};//21张卡已找到的最高优先级
  bool isCandidateUnique = true;//所有卡槽是否只有1个优先级的候选卡
  for (int order = 1; order <= work[task].originalSlotNum[account]; order++)
    if (isSlotRequired[order])
    {
      GetCustomCandidate(task, account, order, candidate[order]);//获取该卡槽的所有候选卡编号和优先级
      if (candidate[order].num == 0) //没有截图的卡不携带
        isSlotRequired[order] = false;
      if (candidate[order].num > 1)
        isCandidateUnique = false;
      bestPriority[order] = -2;//最高优先级预设为-2（不存在）
      bestCode[order] = -1;//最高优先级编号预设为-1（不存在）
      bestStar[order] = -1;//最高星级编号预设为-1（不存在）
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
  for (int page = 0; page <= 21; page++)
  {
    if (page == 0)
    {
      ForcedUpdateMap(task, account);//直接截图
      for (int y = 0; y < customScrollHeight; y++)//记录原滚动条
        work[task].customScroll[y][0] = work[task].map[customScrollY + y][customScrollX];
    }
    else if (page == 1) //第1次
    {
      if (work[task].customScroll[0][0] == scrollBlankColor) //如果滚动条不在顶部
        ScrollCustom(task, account, top);//翻到顶部并截图
    }
    else//第2-21次
      ScrollCustom(task, account, pageDown);//向下翻页并截图

    //确定防御卡起始识别高度bagY
    int x = bagX + customX;
    for (int y = bagY1; y < bagY2; y++)
      if (work[task].map[y][x] == customColor[0] && work[task].map[y + 1][x] == customColor[1]
        && work[task].map[y + 2][x] == customColor[1])
      {
        reserve[task].bagY = y;
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
          int star = FindCustom(task, candidate[order].code[i], -2);
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
//携带自定卡片，返回是否携带成功。若targetStar==-1，则携带任意星级，否则只携带指定星级
bool TakeCustom(int task, int account, int customCode, int targetStar)
{
  const COLORREF scrollBlankColor = 0x054971;//滚动条空白处颜色
  const COLORREF customColor[2] = { 0x001f41, 0x1a4263 };//防御卡识别色
  if (customCode < 0 || customCode >= maxCustomNum)
    return false;

  const POINT top = { 930, 198 };
  const POINT pageDown = { 930, 400 };
  int row = -1, column = -1;

  for (int page = 0; page <= 21; page++) //进行20次翻页识别
  {
    if (page == 0)
    {
      ForcedUpdateMap(task, account);//直接截图
      for (int y = 0; y < customScrollHeight; y++)//记录原滚动条
        work[task].customScroll[y][0] = work[task].map[customScrollY + y][customScrollX];
    }
    else if (page == 1) //第1次
    {
      if (work[task].customScroll[0][0] == scrollBlankColor) //如果滚动条不在顶部
        ScrollCustom(task, account, top);//翻到顶部并截图
    }
    else//第2-21次
      ScrollCustom(task, account, pageDown);//向下翻页并截图

    //确定防御卡起始识别高度bagY
    int x = bagX + customX;
    for (int y = bagY1; y < bagY2; y++)
      if (work[task].map[y][x] == customColor[0] && work[task].map[y + 1][x] == customColor[1]
        && work[task].map[y + 2][x] == customColor[1])
      {
        reserve[task].bagY = y;
        break;
      }

    //在本页找卡，如果找到
    if (FindCustom(task, customCode, targetStar, &row, &column) >= 0)
    {
      //携带卡片
      Click(task, account, bagX + column * bagWidth + customX, reserve[task].bagY + row * bagHeight + customY);
      CheckSleep(task, 100);
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
  TargetType &target = work[task].target[targetCode];
  RecordTemplate(task, item[target.itemCode].image);
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
      memcpy(&work[task].card[account][order], &card[code], sizeof(CardType));
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
  int cardHeight = MAXINT;
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
  /*
  const char* templateName[5] = { "常规", "海星", "煮蛋", "投手", "主神" };
  ColorToBitmap(Work[task].newWideTemplate, "宽模板.png");
  ColorToBitmap(Work[task].newTemplate[account][order], "模板.png");
  char message[100] = {};
  sprintf_s(message, "这是%d类卡：%s", slotTemplateType, templateName[slotTemplateType]);
  RecordError(task, account, "相似度测试", message, END_TASK);
  */

#ifdef _DEBUG
  char path[100];
  sprintf_s(path, "自动备份\\识图数据\\%d.png", order);
  ColorToBitmap(work[task].card[account][order].image, path);//保存模板文件
#endif
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
void PlantToGrid(int task, int account, int order, int row, int column)
{
  if (row >= 1 && row <= 7 && column >= 1 && column <= 9)
  {
    if (order == zero) //铲子
      PostMessage(work[task].hWnd[account], WM_KEYDOWN, 97, 0);//按下键盘1
    else //卡片
    {
      bool isSlotPicked = TryCatchTemplate(task, account, order);
      if (!isSlotPicked)
        PickSlot(task, account, order);//拾起卡槽
    }
    Lay(task, account, row, column);
    Lay(task, account, 3, -1);//释放卡片，防止影响下次种植
  }
}
//执行第times次放卡
void PlantTimes(int task, int account, int order, int times, int row = 0, int column = 0)
{
  //如果row为0，按阵型放置，否则放在指定行数。column同理
  if (row == 0)
    row = work[task].slot[account][order].row[times];
  if (column == 0)
    column = work[task].slot[account][order].column[times];
  PlantToGrid(task, account, order, row, column);
  //补放2次，铲子除外
  if (order != 0)
    for (int i = 0; i < 2; i++)
    {
      CheckSleep(task, 1);
      PlantToGrid(task, account, order, row, column);
    }
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
  CardType &card = work[task].card[account][order];
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
  SlotType &slot = work[task].slot[account][order];

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
  SlotType &slot = work[task].slot[account][order];
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
//获取目标targetCode每个格子的相似度（同时计算每一格的最大相似度）
void GetSimilarityTarget(int task, int targetCode)
{
  TargetType &target = work[task].target[targetCode];

  RecordTemplateTarget(task, targetCode);//将模板颜色记入ColorExist

  for (int i = recognitionFrame - 1; i >= 1; i--) //将最近recognitionRate-1次相似度右移一格
    for (int row = 1; row <= 7; row++)
      for (int column = 1; column <= 10; column++)
        target.similarity[row][column][i] = target.similarity[row][column][i - 1];

  for (int row = 1; row <= 7; row++) //获取每格与模板的相似度
    for (int column = 1; column <= 10; column++)
      target.similarity[row][column][0] = GetSimilarityItem(task, target.itemCode, row, column);

  //求取近recognitionRate次测量的相似度的最大值，统计目标个数
  target.globalMaxSimilarity = 0;
  target.num = 0;
  int combo = 0;//一行内连续存在物件的格子数
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
      if (target.maxSimilarity[row][column] >= item[target.itemCode].requiredSimilarity)
      {
        combo++; //连续存在格数+1
        if (combo % 3 == 1) //连续格数达到1,4,7,10时增加1个物件
          target.num++;
      }
      else
        combo = 0; //连续存在格数清零
      //记录全局最大相似度及所在行列
      if (target.maxSimilarity[row][column] > target.globalMaxSimilarity)
      {
        target.globalMaxSimilarity = target.maxSimilarity[row][column];
        target.row = row;
        target.column = column;
      }
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
const int bossBarX = 441, bossBarY = 566, bossBarLength = 430;//boss血量判定区位置
const COLORREF bossBarColor[2] = { 0x96e0f2, 0xa4e3f9 };//单/双BOSS血条边框颜色
//计算与bossNum个BOSS血条边框的匹配度
int bossBarFitness(int task, int bossNum)
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
  if (bossBarFitness(task, 1) >= 50)
    return 1;
  if (bossBarFitness(task, 2) >= 50)
    return 2;
  return 0;
}
//从截图中更新波次
void GetWave(int task)
{
  bool isWaveChanged = false;//波次是否改变
  //识别到“战利品”，判断为waveLoot波次，否则按两点颜色判断
  int wave = GetMainWave(task);
  //波次不为-1则更新波次
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
    const POINT bossPoint[2] = { { 366, 572 }, { 366, 550 } };//判断两个boss是否有血的位置
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
  /*
  //画框
  for (int row = 0; row <= 7; row++)
    Monitor[task].line(JuiceX, JuiceY + row * GridHeight, JuiceX + 10 * GridWidth, JuiceY + row * GridHeight);
  for (int column = 0; column <= 9; column++)
    Monitor[task].line(JuiceX + column * GridWidth, JuiceY, JuiceX + column * GridWidth, JuiceY + 7 * GridHeight);

    //画框
  for (int row = 0; row <= 7; row++)
  {
    Monitor[task].line(JuiceX, JuiceY + row * GridHeight + juiceCut,
      JuiceX + 9 * GridWidth, JuiceY + row * GridHeight + juiceCut);
    Monitor[task].line(JuiceX, JuiceY + row * GridHeight - juiceCut,
      JuiceX + 9 * GridWidth, JuiceY + row * GridHeight - juiceCut);
  }
  for (int column = 0; column <= 9; column++)
  {
    Monitor[task].line(JuiceX + column * GridWidth + juiceCut, JuiceY,
      JuiceX + column * GridWidth + juiceCut, JuiceY + 7 * GridHeight);
    Monitor[task].line(JuiceX + column * GridWidth - juiceCut, JuiceY,
      JuiceX + column * GridWidth - juiceCut, JuiceY + 7 * GridHeight);
  }
  */

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
  /*
  //显示某物件的最大相似度
  Monitor[task].outtextxy(item[Work[task].target[0].itemCode].name, 10, 10);
  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 10; column++)
    {
      short(&maxSimilarity)[8][10] = Work[task].Slot[account][1].maxSimilarity;
      //short(&maxSimilarity)[8][11] = Work[task].target[0].maxSimilarity;
      if (maxSimilarity[row][column] >= maltoseRequiredSimilarity)
        Monitor[task].settextcolor(RGB(255, 0, 0));
      else
        Monitor[task].settextcolor(RGB(255, 255, 255));
      Monitor[task].outtextxy(maxSimilarity[row][column], ObjectX + (column - 1) * GridWidth,
        ObjectY + (row - 1) * GridHeight + +levelHeight[4]);
    }
    */
    //识别到的图像
  monitor[task].settextcolor(RGB(255, 0, 0));
  monitor[task].SetBackgroundColor(WHITE);
  int itemNum[8][10] = {};//各格已有物件数量
  for (int order = 0; order < work[task].targetNum; order++)
  {
    TargetType &target = work[task].target[order];
    if (target.num > 0)
    {
      int row = target.row;
      int column = target.column;
      if (itemNum[row][column] < 4)
      {
        monitor[task].outtextxy(item[target.itemCode].name, objectX + (column - 1) * gridWidth,
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
}
//从各自账号截图中读取信息（CD和火苗）
void ReadExclusiveInfo(int task, int account)
{
  GetCold(task, account);//读取冷却
  work[task].energy[account] = energyDigit.Get(task, 0, 0, 0, account);//读取火苗
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
//wave是否为关卡结束后的波次
bool IsEndWave(int wave)
{
  return wave == waveOutOfLevel || wave == waveGrade || wave == waveFlip
    || wave == waveTower || wave == waveLoot;
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
void SmartMovePlant(int task, int account, int order, int hunterRow = 0, int hunterColumn = 0)
{
  SlotType &slot = work[task].slot[account][order];
  int level = slot.level;//卡片层级
  if (level == 6) //幻鸡按被复制卡层级来
    level = work[task].slot[account][work[task].magicCore[account]].level;
  if (order == zero) //铲子
  {
    for (int n = 1; n <= slot.maxTimes; n++)//依次单点每个要放置的格子
    {
      PostMessage(work[task].hWnd[account], WM_KEYDOWN, 97, 0);//按下键盘1
      Lay(task, account, slot.row[n], slot.column[n]);
      Sleep(1);
    }
    Lay(task, account, 3, -1);//释放卡片，防止影响下次种植
  }
  else if (level == 5)//5层级卡：不截取模板，直接暴力补阵
  {
    PickSlot(task, account, order);//拾起卡槽
    for (int n = 1; n <= slot.maxTimes; n++)//本次补卡还是遍历
    {
      Lay(task, account, slot.row[n], slot.column[n]);
      Sleep(1);
    }
    Lay(task, account, 3, -1);//释放卡片，防止影响下次种植
  }
  else //0-4层级：智能补阵
  {
    //需要截取模板则截取
    bool isSlotPicked = TryCatchTemplate(task, account, order);//卡槽是否已经拾起

    //如果该卡未截取模板，则无占用，每格相似度均为0，但同层级其他卡可能有占用

    if (level == 0)//0层级：临时填写本卡占用（1-4层级的占用信息在ReadLevelInfo时就填好了）
    {
      memset(work[task].grid[0], 0, sizeof(work[task].grid[0]));//重置0层级占用
      GetOccupied(task, account, order, level);
    }

    //无阵型但有索敌行列，放置1次
    if (slot.maxTimes == 0 && hunterRow > 0 && hunterColumn > 0)
    {
      TryPickSlot(task, account, order, isSlotPicked);//若卡槽未拾起，则拾起卡槽
      Lay(task, account, hunterRow, hunterColumn);
    }

    //有阵型则依次放置，如果有索敌，将第1次放置位置改为索敌位置
    for (int times = 1; times <= slot.maxTimes; times++)//对每个要放卡的位置，判断是否已经占用
    {
      int row = slot.row[times];
      int column = slot.column[times];
      if (times == 1) //第一次改成索敌位置
      {
        if (hunterRow > 0)
          row = hunterRow;
        if (hunterColumn > 0)
          column = hunterColumn;
      }
      //未占用或刷分模式下棉麦最后一格需要补卡
      //if (Work[task].grid[level][row][column] == 0 || (slot.isMaltose && times == slot.maxTimes))
      if (work[task].grid[level][row][column] == 0)
      {
        if (slot.isMaltose) //如果是棉麦，进行一次普通放置后退出
        {
          PlantTimes(task, account, order, 0, row, column);
          reserve[task].maltoseTimes[row - 1][column - 1]++;//棉麦放置次数+1
          return;
        }
        TryPickSlot(task, account, order, isSlotPicked);//若卡槽未拾起，则拾起卡槽
        Lay(task, account, row, column);
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
//种植order号卡片。(row,column)为指定行列，默认按阵型来
bool Plant(int task, int account, int order, int row = 0, int column = 0)
{
  SlotType &slot = work[task].slot[account][order];
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

  //没开补，次数已满，则不再放置
  if (!slot.repair && slot.times > slot.maxTimes)
    return false;

  if (order == maxSlotNum + 1)//使用宝石
    UseAllSkill(task, account);
  else //使用铲子或放置卡片
  {
    //如果卡槽数不为0，order超过卡槽数，则不放置
    if (work[task].realSlotNum[account] > 0 && order > work[task].realSlotNum[account])
      return false;
    if (slot.repair)//需要补阵
      SmartMovePlant(task, account, order, row, column);
    else //不需要补阵
      PlantTimes(task, account, order, slot.times, row, column);
  }
  return true;
}
//关闭文档f，写入错误信息到info，返回出错的行数line
int WriteInfo(char(&info)[1000], FILE *f, const char *path, int line, char *content, const char *tip)
{
  if (f)
    fclose(f);
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
const char *ExtractTrack(int task, int account, const char *extendedPath)
{
  work[task].accelerationTime[account] = 0;
  const char *bannedPath = extendedPath;//仅含ban卡的轨道文件路径
  const char *bar = strchr(extendedPath, '|');//加速指令分隔符
  if (bar) //如果存在加速指令分隔符，使用该轨道的加速时间
  {
    work[task].accelerationTime[account] = atoi(extendedPath);
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

  memset(work[task].isSlotBanned[account], 0, sizeof(work[task].isSlotBanned[account]));
  work[task].isBanEnabled[account] = bannedPath[0] == '<';
  if (work[task].isBanEnabled[account])//如果存在ban卡列表，记录ban卡列表和真正的轨道路径
  {
    path = strchr(bannedPath, '>') + 1;//获取去掉ban卡列表后的轨道路径
    char banList[maxPath];//ban卡列表，如"2,3"
    strcpy_s(banList, bannedPath + 1);
    strchr(banList, '>')[0] = 0;//获取banList
    char *nextInfo = nullptr, *info = strtok_s(banList, ",", &nextInfo);
    while (info)
    {
      int originalOrder = atoi(info);//获取被ban的卡序号
      //记录每个被ban的卡（只允许ban1~21号卡）
      if (originalOrder >= 1 && originalOrder <= maxSlotNum)
        work[task].isSlotBanned[account][originalOrder] = true;
      info = strtok_s(nullptr, ",", &nextInfo);
    }
  }
  return path;
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
  SlotType &slot = work[task].slot[account][orderSelected];
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
    for (int account = 0; account <= 1; account++)
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
  SlotType &slot = work[task].slot[account][order];
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
    int targetCode = FindTarget(task, slot.itemCode);
    TargetType &target = work[task].target[targetCode];
    if (target.num < slot.imageNum)
      return false;
  }
  return true;
}
//获取索敌位置。如果无图像条件或无图像，返回0,0
void GetHunterLocation(int task, int account, int order, int *pRow, int *pColumn)
{
  *pRow = *pColumn = 0;//默认不索敌
  if (order == maxSlotNum + 1) //宝石不索敌
    return;
  SlotType &slot = work[task].slot[account][order];
  //图像数量条件（含索敌）
  if (slot.image[0] && slot.imageNum > 0)
  {
    int targetCode = FindTarget(task, slot.itemCode);
    TargetType &target = work[task].target[targetCode];
    if (target.num < slot.imageNum)
      return;
    //如果识图成功，进行索敌放置
    if (slot.hunterRow[0] == 'a') //行索敌
    {
      int row = target.row;
      if (slot.hunterRow[1] == '+')
        row = target.row + (slot.hunterRow[2] - '0');
      else if (slot.hunterRow[1] == '-')
        row = target.row - (slot.hunterRow[2] - '0');
      if (row >= 1 && row <= 7)//如果计算结果在1-7内，记录索敌行
        *pRow = row;
    }
    else if (slot.hunterRow[0] >= '1' && slot.hunterRow[0] <= '7') //固定行
      *pRow = slot.hunterRow[0] - '0';

    if (slot.hunterColumn[0] == 'b') //列索敌
    {
      int column = target.column;
      if (slot.hunterColumn[1] == '+')
        column = target.column + (slot.hunterColumn[2] - '0');
      else if (slot.hunterColumn[1] == '-')
        column = target.column - (slot.hunterColumn[2] - '0');
      if (column >= 1 && column <= 9)//如果计算结果在1-9内，记录索敌列
        *pColumn = column;
    }
    else if (slot.hunterColumn[0] >= '1' && slot.hunterColumn[0] <= '9') //固定列
      *pColumn = slot.hunterColumn[0] - '0';
  }
}
//检查卡片是否可放置，能则填写时钟
void GetPlantableTick(int task, int account, int order, DWORD checkTick)
{
  SlotType &slot = work[task].slot[account][order];

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
    if (slot.limitTimes >= slot.endNum)
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
//主动检查order号卡是否可放置，可以则放置
bool CheckLimitPlant(int task, int account, int order, DWORD checkTick)
{
  GetPlantableTick(task, account, order, checkTick);//检查order号卡是否可放置，获取plantableTick
  SlotType &slot = work[task].slot[account][order];
  if (slot.plantableTick && checkTick >= slot.plantableTick) //如果已经到达“可放置时刻”
  {
    slot.plantableTick = 0;//无论能否放置都删除“可放置时刻”
    DWORD plantGap = checkTick - slot.lastTick;//距离上次种植经过的时间
    //最小放卡间隔（3秒，刚刚放了幻鸡则是5秒）
    DWORD minPlantGap = 3000;//最小放置间隔
    if (order == 0 && !slot.repair)
      minPlantGap = 0; //铲子没开补最小间隔为0（开补则依然是3000）
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
          int row = 0, column = 0;
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
          GetHunterLocation(task, account, order, &row, &column);//获得索敌位置
          Plant(task, account, order, row, column);//种植卡片
          slot.lastTick = GetTickCount();//记录最近一次放卡时刻
          slot.limitTimes++;//极限放卡次数+1

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
          if (lackeyNum > 0) //放完等100 ms，下次检查才能判断是否放置成功
            CheckSleep(task, 100);
          return true;
        }
    }
  }
  return false;
}
//int isLimitSuccessful = false;//检查是否极限成阵
//检查极限卡，主动放置至多1张符合要求的卡
bool CheckLimitPlant(int task, int account)
{
  DWORD checkTick = GetTickCount();//检查时刻

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
    SlotType &slot = work[task].slot[account][order];
    if (order > work[task].slotNum[account] && order <= maxSlotNum) //跳过无效卡槽
      continue;
    if (!slot.repair && slot.times >= slot.maxTimes) //跳过次数放满了的卡
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
int LoadSingleCard(SlotType &slot, FILE *f, char(&s)[size], char(&seg)[size],
  const char *path, int &line, char(&info)[1000], int totalWave, int totalWaveTick)
{
  strcpy_s(seg, s);//将s复制到seg，用于分割
  if (seg[0] == '\t')//没有卡片名称
    return WriteInfo(info, f, path, line, s, "卡片名称不能为空");

  char *nextName = nullptr;
  char *name = strtok_s(seg, "\t", &nextName);//子串1：卡片名称
  if (strlen(name) > 6)
    return WriteInfo(info, f, path, line, s, "卡片名称不能超过3个汉字");
  strcpy_s(slot.name, name);
  slot.isMaltose = strcmp(name, "棉花糖") == 0 || strcmp(name, "麦芽糖") == 0;

  name = strtok_s(NULL, "\t", &nextName);//子串2：冷却及锁定状态（7s锁补极）
  if (name == NULL || strstr(name, "|"))
    return WriteInfo(info, f, path, line, s, "第二项应为冷却时间和轨道状态");

  if (totalWave > 0 && strstr(name, "继")) //从第0.1波开始，开启继承的卡片无需修改任何信息
    return 0;

  //若未开启继承，重置卡槽信息（保留卡片名）并读取新波次轨道，将计时器重置为本波开始时刻
  char slotName[10] = {};//（临时）卡片名称
  strcpy_s(slotName, slot.name);
  bool isMaltose = slot.isMaltose;//（临时）是否棉麦
  memset(&slot, 0, sizeof(slot));
  strcpy_s(slot.name, slotName);
  slot.isMaltose = isMaltose;

  slot.startTick = totalWaveTick;
  slot.oldPlantTick = 0;

  char *nextCondition = nullptr;
  char *condition = NewStrTok(name, ",", &nextCondition);//第一项：7[3]补极
  slot.search = strstr(condition, "索") ? 1 : 0;//索敌模式
  slot.independent = strstr(condition, "独") ? 1 : 0;//独立计时
  slot.repair = strcmp(slot.name, "宝石") == 0 || strstr(condition, "补") ? 1 : 0;//补阵模式（宝石强制开）
  slot.limit = strstr(condition, "极") ? 1 : 0;//极限成阵模式
  slot.retain = strstr(condition, "继") ? 1 : 0;//是否开启继承
  int cd;//卡牌冷却（临时变量，读取后不使用）
  slot.level = 0;//卡片层级默认为0（表示不占格子）
  sscanf_s(condition, "%d[%d]", &cd, &slot.level);//读取卡片层级

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
        return WriteInfo(info, f, path, line, s, "【延迟】范围为0~960");
      break;
    case 1://达成
      slot.triggerMode = atoi(condition) ? 1 : 0;
      break;
    case 2://火苗
      slot.energy = atoi(condition);
      if (slot.energy < 0 || slot.energy > 15000)
        return WriteInfo(info, f, path, line, s, "【火苗】范围为0~15000");
      break;
    case 3://波次：只能是m.n格式
    {
      if (!IsWaveLegal(condition)) //波次输入不合格
        return WriteInfo(info, f, path, line, s, waveErrorString);
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
          return WriteInfo(info, f, path, line, s, "【卡片】长度不能超过3个汉字");
        strcpy_s(slot.card, condition);
      }
      break;
    case 5://卡片数量
      slot.cardNum = atoi(condition);
      if (slot.cardNum < 0 || slot.cardNum > 63)
        return WriteInfo(info, f, path, line, s, "【卡片数量】范围为0~63");
      break;
    case 6://图像
      if (strcmp(condition, "NULL") == 0)
        strcpy_s(slot.image, "");
      else
      {
        if (strlen(condition) > 8)
          return WriteInfo(info, f, path, line, s, "【图像】长度不能超过4个汉字");
        strcpy_s(slot.image, condition);
      }
      break;
    case 7://图像数量
      slot.imageNum = atoi(condition);
      if (slot.imageNum < 0 || slot.imageNum > 63)
        return WriteInfo(info, f, path, line, s, "【图像数量】范围为0~63");
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
          return WriteInfo(info, f, path, line, s, tip);
        if (length == 1) //a或一位数
        {
          if (!((condition[0] >= '1' && condition[0] <= '7') || condition[0] == 'a'))
            return WriteInfo(info, f, path, line, s, tip);
        }
        else if (length == 3) //a+n
        {
          if (!(condition[0] == 'a' && (condition[1] == '+' || condition[1] == '-') && condition[2] >= '1' && condition[2] <= '6'))
            return WriteInfo(info, f, path, line, s, tip);
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
          return WriteInfo(info, f, path, line, s, tip);
        if (length == 1) //b或一位数
        {
          if (!((condition[0] >= '1' && condition[0] <= '9') || condition[0] == 'b'))
            return WriteInfo(info, f, path, line, s, tip);
        }
        else if (length == 3) //b+n
        {
          if (!(condition[0] == 'b' && (condition[1] == '+' || condition[1] == '-') && condition[2] >= '1' && condition[2] <= '9'))
            return WriteInfo(info, f, path, line, s, tip);
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
          return WriteInfo(info, f, path, line, s, "【跟随卡片】长度不能超过3个汉字");
        strcpy_s(slot.follow, condition);
      }
      break;
    case 11://终止数量
      slot.endNum = atoi(condition);
      if (slot.endNum < 0 || slot.endNum > 63)
        return WriteInfo(info, f, path, line, s, "【终止数量】范围为0~63");
      break;
    case 12://终止时间
      slot.endTime = atoi(condition);
      if (slot.endTime < 0 || slot.endTime > 960)
        return WriteInfo(info, f, path, line, s, "【终止时间】范围为0~960");
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
      return WriteInfo(info, f, path, line, s, "放卡格式应为ttt(.t)|xy，以Tab键分隔");
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
      return WriteInfo(info, f, path, line, s, "放卡时间范围为1~960");

    if (slot.moment[times] > slot.maxMoment)//记录最大轨道时间
      slot.maxMoment = slot.moment[times];

    int row = location / 10;
    int column = location % 10;

    if (location != zero)//如果位置不是00
      if (row < 1 || row > 7 || column == zero)
        return WriteInfo(info, f, path, line, s, "行数范围为1~7，列数范围为1~9");

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
//获得人物个数
int GetRoleNum(int task)
{
  return (work[task].roleLocNum[0] > 0) + (work[task].roleLocNum[1] > 0);
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
      return WriteInfo(info, f, path, line, s, "波次范围为0~23（小波范围为0~11）");
    else if (result == 1)
    {
      existedWaveNum++;
      if (existedWaveNum > 12)
        return WriteInfo(info, f, path, line, s, "最多只允许设置12个波次");
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
int LoadDefalutTrackFrom(const char *path, SlotType slot[], int &slotNum, char(&s)[maxLineSize],
  char(&seg)[maxLineSize], char(&info)[1000], int skippedSlotNum)
{
  char tip[100];
  FILE *file;
  if (fopen_s(&file, path, "r") == 0)//如果能打开承载文件，载入承载方案
  {
    int line = 0;
    do
    {
      fgets(file, s);
      line++;
      if (strlen(s) == 0)
        return WriteInfo(info, file, path, line, s, "找不到卡槽数");
    } while (!strstr(s, "卡槽数"));
    sscanf_s(s, "卡槽数=%d", &slotNum);
    slotNum += 2 - skippedSlotNum;//如果不跳过宝石和铲子，要增加相应的卡槽数
    for (int i = 0; i < skippedSlotNum; i++) //选择性跳过宝石和铲子
    {
      fgets(file, s);
      line++;
    }
    for (int order = 0; order < slotNum; order++)
    {
      fgets(file, s);//读取一行卡片轨道，如小火炉 7s极补锁 002|12
      line++;

      //如果卡槽还没读完，就遇到了空行或--第x波--，说明卡槽数填多了
      if (strlen(s) == 0 || IsWaveTitle(s))
      {
        sprintf_s(tip, "卡槽数=%d，但只读取到%d个卡槽", slotNum, order);
        return WriteInfo(info, file, path, line, s, tip);
      }
      //读取一张承载卡的轨道信息
      if (LoadSingleCard(slot[order], file, s, seg, path, line, info, 0, 0) > 0)
        return line;//读取错误则返回出错的行数（错误信息已经写入info）
      if (slot[order].row[1] > 0 && slot[order].moment[1] == 0) //有阵型无轨道
      {
        if (strcmp(slot[order].name, "铲子") == 0)//铲子：开补并添加第1秒轨道（开局全铲）
        {
          slot[order].repair = 1;
          slot[order].moment[1] = 10;
        }
        else //其他卡视为极限成阵
          slot[order].limit = 1;
      }
    }
    fclose(file);
  }
  return 0;
}
//（仅第0波）载入默认轨道（常用卡和承载卡）。载入成功或无承载轨道返回0，否则返回出错行数
int LoadDefaultTrack(int task, SlotType slot[], int &slotNum, char(&s)[maxLineSize],
  char(&seg)[maxLineSize], char(&info)[1000])
{
  int commonSlotNum = 0;//常用卡数
  char commonPath[maxPath] = {};//常用卡默认轨道
  sprintf_s(commonPath, "预制轨道\\默认轨道\\常用卡片.txt");
  int commonLine = LoadDefalutTrackFrom(commonPath, slot, commonSlotNum, s, seg, info, 2);
  if (commonLine > 0)
    return commonLine;

  int traySlotNum = 0;//承载卡数
  if (IsAdvance(task)) //高级任务载入承载轨道
  {
    int list = GetList(task);
    AdvanceType &currentAdvance = *plot[list].playingAdvance;
    int type = currentAdvance.type;
    int level = currentAdvance.level;
    char levelName[100];
    GetLevelName(levelName, type, level);
    char trayPath[maxPath] = {};//承载卡默认轨道
    if (IsRoamLevel(type, level)) //漫游关卡
      sprintf_s(trayPath, "预制轨道\\默认轨道\\承载\\漫游\\%s.txt", roamName[work[task].roamType]);
    else if (type == 2) //魔塔
    {
      level %= 1000;//提取层数
      if (level % 5 != 0) //161-164转化为165
        level = level / 5 * 5 + 5;
      sprintf_s(trayPath, "预制轨道\\默认轨道\\承载\\%d%s\\%d.txt", type, typeName[type], level);
    }
    else
      sprintf_s(trayPath, "预制轨道\\默认轨道\\承载\\%d%s\\%d%s.txt", type, typeName[type], level, levelName);

    int trayLine = LoadDefalutTrackFrom(trayPath, slot + commonSlotNum, traySlotNum, s, seg, info, 1);
    if (trayLine > 0)
      return trayLine;
  }
  slotNum = commonSlotNum + traySlotNum;
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
bool IsSameSlot(SlotType &slot1, SlotType &slot2)
{
  if (slot1.isMaltose && slot2.isMaltose) //都是棉麦，视为同一张卡
    return true;
  return strcmp(slot1.name, slot2.name) == 0; //否则名称必须严格相同
}
//从文件当前位置读取波次信息，读取slotnum张卡片。读取成功返回0，否则返回出错的行数
int LoadCurrentWave(int task, int account, FILE *f, char(&s)[maxLineSize], char(&seg)[maxLineSize],
  const char *path, int &line, char(&info)[1000], int totalWave)
{
  char tip[100];
  //读取默认承载方案（仅限高级任务第0波，已指定PlayingOrder）
  SlotType defaultSlot[20] = {};
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
  int order = 0;//实际卡槽编号
  for (int lineOrder = -1; lineOrder <= work[task].originalSlotNum[account]; lineOrder++)
  {
    fgets(f, s);//读取一行卡片轨道，如小火炉 7s极补锁 002|12
    line++;
    int originalOrder = lineOrder == -1 ? maxSlotNum + 1 : lineOrder;//原卡槽编号

    //如果卡槽还没读完，就遇到了空行或--第x波--，说明卡槽数填多了
    if (strlen(s) == 0 || IsWaveTitle(s))
    {
      sprintf_s(tip, "卡槽数=%d，但只读取到%d个卡槽", work[task].originalSlotNum[account], lineOrder - 1);
      return WriteInfo(info, f, path, line, s, tip);
    }

    //记录ban卡前的卡槽名称
    char *tab = strchr(s, '\t'); //获取第一个tab符号位置
    if (tab)
    {
      int nameLength = tab - s;//名称的长度
      strncpy_s(work[task].originalName[account][originalOrder], s, nameLength);
      work[task].originalName[account][originalOrder][nameLength] = 0;
    }

    //如果这个卡槽被ban，则跳过
    if (lineOrder > 0 && work[task].isSlotBanned[account][originalOrder])
      continue;

    //记录没有被ban的卡槽的编号
    if (lineOrder == -1)
    {
      if (strstr(s, "宝石")) //第-1行如果是宝石，实际编号为MaxSlotNum + 1
        order = maxSlotNum + 1;
      else //否则实际编号为0，并且视为第0行
        order = lineOrder = 0;
    }
    else if (lineOrder == 0) //铲子实际编号0
      order = 0;
    else //遇到没有跳过的卡，实际卡槽编号+1
      order++;

    SlotType &slot = work[task].slot[account][order];

    if (LoadSingleCard(slot, f, s, seg, path, line, info, totalWave, work[task].totalWaveTick[totalWave]) > 0)
      return line;//返回出错的行数（错误信息已经写入info）

    if (slot.repair && slot.level <= 4) //0-4层级补阵卡需要识别相似度
      slot.isSimilarityRequired = true;
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
          sprintf_s(tip, "未找到【%s.png】截图", slot.image);
          return WriteInfo(info, f, path, line, s, tip);
        }
        if (AddTarget(task, account, order) == -2)
        {
          sprintf_s(tip, "最多只能识别%d张图片", maxTargetNum);
          return WriteInfo(info, f, path, line, s, tip);
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
      //如果开启默认轨道（第0波继承），拷贝默认卡轨道（包括独立计时、极限成阵、放卡条件）
      if (slot.retain)
      {
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
      slot.repair = 1;//开启补阵
      slot.maxTimes = 3;//种植次数为3（用于补阵）
      for (int i = 1; i <= 3; i++)//设置补阵位置
      {
        slot.row[i] = sorbetLoc[i - 1] / 10;
        slot.column[i] = sorbetLoc[i - 1] % 10;
      }
    }
  }

  //读完当前波次的轨道再读一行
  fgets(f, s);
  line++;

  //检查绑定的是哪张卡
  for (int lineOrder = -1; lineOrder <= work[task].slotNum[account]; lineOrder++)
  {
    int order = lineOrder == -1 ? maxSlotNum + 1 : lineOrder;
    SlotType &slot = work[task].slot[account][order];
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
          sprintf_s(tip, "未找到【%s】的条件卡【%s】", slot.name, slot.card);
          return WriteInfo(info, f, path, line, s, tip);
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
          sprintf_s(tip, "未找到【%s】的跟随卡【%s】", slot.name, slot.follow);
          return WriteInfo(info, f, path, line, s, tip);
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
    return WriteInfo(info, f, path, line, s, tip);
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
  if (LoadCurrentWave(task, account, f, s, seg, path, line, info, totalWave) != 0)
    return line;

  fclose(f);
  return 0;
}
//（开局或变阵时）读取轨道第totalWave波信息。读取成功返回0，否则返回出错的行数
int ReloadWave(int task, int account, int totalWave)
{
  char info[1000] = {};
  return ReloadWave(task, account, work[task].backupTrack[account], info, totalWave);
}
//检查整个轨道文件。检查成功返回0，记录人数；无轨道返回-1，打不开返回-2，否则返回出错行数
int LoadTrack(int task, int account, const char *extendedPath, char(&info)[1000],
  int *pRoleNum, bool checkAllWave)
{
  //填写ban卡数组，获得轨道路径
  const char *path = ExtractTrack(task, account, extendedPath);
  strcpy_s(work[task].pureTrack[account], path);

  //检查轨道文件能否打开，能则读取存在的波次
  int line = GetWaveExist(task, account, path, info);
  if (line != 0)
    return line;

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

  int trackNum = (strlen(param[task].track[0]) > 0) + (strlen(param[task].track[1]) > 0);//轨道个数
  work[task].trackRoleNum[account] = 1;

  //读取人物1位置
  char roleString[100];
  if (sscanf_s(s, "人物1位置=%s", roleString, sizeof(roleString)) != 1)
    return WriteInfo(info, f, path, line, s, "缺少人物1位置");
  else
  {
    work[task].roleLocNum[account] = 0;//人物1个数预设为0
    nextName = nullptr;
    name = strtok_s(roleString, ",", &nextName);
    while (name)
    {
      int loc = atoi(name);
      if (loc / 10 < 1 || loc / 10 > 7 || loc % 10 == zero)//人物1位置超出范围
        return WriteInfo(info, f, path, line, s, "行数范围为1~7，列数范围为1~9");
      work[task].roleLocNum[account]++;//人物1个数+1
      work[task].roleLoc[account][work[task].roleLocNum[account]] = loc;//记录人物1位置
      name = strtok_s(nullptr, ",", &nextName);
    }
  }
  fgets(f, s);
  line++;

  //读取人物2位置
  if (sscanf_s(s, "人物2位置=%s", roleString, sizeof(roleString)) == 1)
  {
    if (trackNum == 1) //只有单轨模式需要从轨道中读取人物2位置
    {
      work[task].roleLocNum[1 - account] = 0;//人物2个数预设为0
      nextName = nullptr;
      name = strtok_s(roleString, ",", &nextName);
      while (name)
      {
        int loc = atoi(name);
        if (loc / 10 < 1 || loc / 10 > 7 || loc % 10 == zero)//人物1位置超出范围
          return WriteInfo(info, f, path, line, s, "行数范围为1~7，列数范围为1~9");
        work[task].roleLocNum[1 - account]++;//人物2个数+1
        work[task].roleLoc[1 - account][work[task].roleLocNum[1 - account]] = loc;//记录人物2位置
        name = strtok_s(nullptr, ",", &nextName);
      }
      work[task].trackRoleNum[account] = 2;
    }
    fgets(f, s);
    line++;
  }
  //读取魔塔层数
  if (sscanf_s(s, "魔塔层数=%d", &work[task].towerx[account]) == 1)
  {
    if (work[task].towerx[account] < zero || work[task].towerx[account] > 165)
      return WriteInfo(info, f, path, line, s, "魔塔层数范围为0~165");

    if (IsAdvance(task))
      work[task].towerx[account] = 0;

    fgets(f, s);
    line++;
  }
  //读取退出时间
  if (sscanf_s(s, "退出时间=%d", &work[task].quitTime[account]) == 1)
  {
    if (work[task].quitTime[account] < zero || work[task].quitTime[account] > 960)
      return WriteInfo(info, f, path, line, s, "退出时间范围为0~960");
    fgets(f, s);
    line++;
  }

  //读取刷技能模式
  if (strstr(s, "刷技能模式=") == s)
  {
    if (atoi(s + 11))
      work[task].isSkillMode = true;
    fgets(f, s);
    line++;
  }
  else //如果没有“刷技能模式”一行，则按照退出时间判定
  {
    if (work[task].towerx[work[task].host] && work[task].quitTime[work[task].host] == 1)
      work[task].isSkillMode = true; //单塔+退出时间为1=刷技能模式
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
    return WriteInfo(info, f, path, line, s, "缺少卡槽数");
  else
  {
    if (slotNum < 0 || slotNum > 21)
      return WriteInfo(info, f, path, line, s, "卡槽数范围为0~21");
    work[task].originalSlotNum[account] = slotNum;//记录原卡槽数
  }

  //计算剩余卡槽数
  work[task].slotNum[account] = work[task].originalSlotNum[account];
  for (int originalOrder = 1; originalOrder <= work[task].originalSlotNum[account]; originalOrder++)
    if (work[task].isSlotBanned[account][originalOrder])
      work[task].slotNum[account]--;

  //超出卡槽数的卡全ban了
  if (work[task].isBanEnabled[account])
    for (int originalOrder = work[task].originalSlotNum[account] + 1; originalOrder <= maxSlotNum; originalOrder++)
      work[task].isSlotBanned[account][originalOrder] = true;

  //检查每个波次
  if (checkAllWave)
  {
    for (int totalWave = 0; totalWave < maxTotalWave; totalWave++)
      if (work[task].totalWaveExist[account][totalWave])
      {
        //读取本波（会跳过下波标题），如果出错则结束读取，返回行数（info已在LoadCurrentWave中填好）
        if (LoadCurrentWave(task, account, f, s, seg, path, line, info, totalWave) != 0)
          return line;
        if (strlen(s) > 0 && !IsWaveTitle(s))
        {
          sprintf_s(tip, "卡槽数=%d，但读完%d个卡槽仍未结束", slotNum, slotNum);
          return WriteInfo(info, f, path, line, s, tip);
        }
      }
  }
  fclose(f);
  if (pRoleNum) //记录参与人数
    *pRoleNum = trackNum == 2 ? 2 : GetRoleNum(task);
  return 0;
}
//将Param[task].track[account]读入Work数组。读取成功返回人数，失败返回0并记录错误信息
int LoadTrack(int task, int account, char(&info)[1000], bool checkAllWave)
{
  int roleNum = 0;
  if (LoadTrack(task, account, param[task].track[account], info, &roleNum, checkAllWave) != 0)
    return 0;
  return roleNum;
}
//载入轨道信息到Work数组中。读取成功返回人数，失败返回0并记录错误信息
void LoadTrack(int task, int account, bool checkAllWave)
{
  char info[1000];
  LoadTrack(task, account, info, checkAllWave);
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
      for (int account = 0; account <= 1; account++)
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
    work[task].hMainHall = param[task - 1].hMainHall;//主大厅句柄
    //沿用param[task - 1]的收集和句柄信息
    for (int account = 0; account < 2; account++)
    {
      work[task].hWndHall[account] = param[task - 1].hWndHall[account];//大厅句柄
      work[task].isCollect[account] = param[task - 1].isCollect[account];//收集
    }
    work[task].tag[0] = noTag;//1P不参与，所以没有tag和hWnd
    work[task].hWnd[0] = NULL;
    work[task].tag[1] = param[task - 1].tag[1];//2P的tag和hWnd使用param[task - 1]的2P
    work[task].hWnd[1] = param[task - 1].hWnd[1];
  }
  else //常规任务
  {
    work[task].hMainHall = param[task].hMainHall;//主大厅句柄
    for (int account = 0; account < 2; account++)
    {
      work[task].hWndHall[account] = param[task].hWndHall[account];//大厅句柄
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
void RecoverWindow_Insitu(HWND hWnd)//原位恢复隐藏的窗口
{
  RECT rect;
  int ScrWidth, ScrHeight, WndWidth, WndHeight;
  if (!IsWindow(hWnd)) return;//没有窗口则结束程序
  SetWindowLong(hWnd, GWL_EXSTYLE, 0);//恢复任务栏图标
  ShowWindow(hWnd, SW_RESTORE);//窗口常规显示
  if ((unsigned int)GetWindowLong(hWnd, GWL_STYLE) / 65536 % 2 == 1)//如果窗口允许最大化
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);//最大化
  else
  {
    GetWindowRect(hWnd, &rect);
    WndWidth = rect.right - rect.left, rect;
    WndHeight = rect.bottom - rect.top;
    ScrWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    ScrHeight = GetSystemMetrics(SM_CYFULLSCREEN);
    MoveWindow(hWnd, originalWndPos.x, originalWndPos.y, WndWidth, WndHeight, 1);//窗口回到屏幕原位
  }
}
void HideWindow_Insitu(HWND hWnd)//隐藏窗口并记录位置
{
  RECT rect;
  if (!IsWindow(hWnd)) return;//没有窗口则结束程序
  ShowWindow(hWnd, SW_RESTORE);//窗口去最大化
  GetWindowRect(hWnd, &rect);
  originalWndPos.x = rect.left;
  originalWndPos.y = rect.top;
  MoveWindow(hWnd, 0, 3000, rect.right - rect.left, rect.bottom - rect.top, 1);//窗口沉底到3000纵坐标
  SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);//隐藏任务栏图标
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
void RecoverWindow(HWND hWnd)//恢复隐藏的窗口
{
  RECT rect;
  int ScrWidth, ScrHeight, WndWidth, WndHeight;
  if (!IsWindow(hWnd)) return;//没有窗口则结束程序
  SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_TOOLWINDOW);//恢复任务栏图标
  ShowWindow(hWnd, SW_RESTORE);//窗口常规显示
  if ((unsigned int)GetWindowLong(hWnd, GWL_STYLE) / 65536 % 2 == 1)//如果窗口允许最大化
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);//最大化
  else
  {
    GetWindowRect(hWnd, &rect);
    WndWidth = rect.right - rect.left, rect;
    WndHeight = rect.bottom - rect.top;
    ScrWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    ScrHeight = GetSystemMetrics(SM_CYFULLSCREEN);
    MoveWindow(hWnd, (ScrWidth - WndWidth) / 2, (ScrHeight - WndHeight * 29 / 30) / 2, WndWidth, WndHeight, 1);//窗口回到屏幕并居中
  }
}
int HideWindow(HWND hWnd)//隐藏窗口
{
  RECT rect;
  BOOL MoveResult;
  if (!IsWindow(hWnd))
    return 1;//没有窗口
  ShowWindow(hWnd, SW_RESTORE);//窗口去最大化
  GetWindowRect(hWnd, &rect);
  MoveResult = MoveWindow(hWnd, 0, 3000, rect.right - rect.left, rect.bottom - rect.top, 1);//窗口沉底到3000纵坐标
  //MoveResult = MoveWindow(hWnd, 0, 3000, 5000, 5000, 1);//窗口沉底到3000纵坐标
  if (MoveResult == NULL)//如果移动失败，可能是窗口不存在或“权限不足”
    return 2;
  SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);//隐藏任务栏图标
  return 0;
}
//hWndGame是不是游戏窗口
int IsGameWindow(HWND hWndGame)
{
  if (!IsWindow(hWndGame)) //不是窗口，返回0
    return 0;
  char ClassName[256];
  GetClassName(hWndGame, ClassName, 256);//获取游戏窗口类名
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
  case 4://纯白色检验，可能是QQ空间登录界面或4399还没加载出来
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
  int RefreshX = 228 * DPI / 96, RefreshY = 44 * DPI / 96;
  LeftClick(param[task].hWndHall[account], RefreshX, RefreshY); //重新点击刷新按钮
  sprintf_s(message, "%dP点击刷新按钮(%d,%d)", account + 1, RefreshX, RefreshY);
  addToRefreshLog(task, message);
}
//从大厅窗口hWndHall中寻找平台platform的最近服务器，找到了则返回位置
int FindLatestServer0(int task, int account, int platform, int *px, int *py)
{
  int list = GetList(task);
  int hallWidth = 0, hallHeight = 0;//大厅窗口尺寸
  if (!HallShot(work[task].hWndHall[account], work[task].hallShot, &hallWidth, &hallHeight, 20))//截取大厅图像
    return 0;

  //截图成功，则构造二级指针
  for (int i = 0; i < hallHeight; i++)
    work[task].pHallShot[i] = work[task].hallShot + i * hallWidth;

  int width = 220, height = 135;//需要识别的色块大小（4399）
  if (platform == 2)//QQ空间
  {
    width = 210;
    height = 70;
  }
  else if (platform == 3)//QQ大厅
  {
    width = 220;
    height = 50;
  }
  else if (platform == 4)//选头像界面（纯白色检验）
  {
    width = 220;
    height = 50;
  }
  else if (platform == 5)//断网界面
  {
    width = 220;
    height = 50;
  }

  int x, y;
  int step = 2;//识图步长
  int result = 0; //识图结果
  for (x = hallWidth - width; x >= 0; x -= step)//屏幕找图
  {
    for (y = 0; y <= hallHeight - height; y += step)
      if (IsBitmapFit(isServerColor, work[task].pHallShot, platform, width, height, x, y, step) == 1)
      {
        if (px != NULL)
          *px = x;
        if (py != NULL)
          *py = y;
        result = 1;
        break;
      }
    if (y <= hallHeight - height)
      break;
  }
  if (!(platform >= 4 && result == 1)) //如果没有识别到QQ登录界面或断网界面，直接返回结果
    return result;
  if (platform == 5) //如果识别到了断网界面，点击重试
  {
    int retryX = (work[task].serverRect.right - work[task].serverRect.left) / 2;
    int retryY = 350 * DPI / 96;
    LeftClick(work[task].hWndServer, retryX, retryY);
    char message[200];
    sprintf_s(message, "%dP检测到断网，已点击重试(%d,%d)", account + 1, retryX, retryY);
    addToRefreshLog(task, message);
    CheckSleep(task, 2000);//等待2秒
    work[task].hWndServer = GetActiveServerWindow(work[task].hWndHall[account]);
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
  sprintf_s(initialHeadPath, "用户参数\\QQ头像截图\\默认头像.png", account + 1);//填写初始头像截图路径
  if (!BitmapToColor(headPath, headShot)) //载入截图
    RecordError(task, account, "缺少头像截图", "缺少QQ头像截图，无法登录", END_TASK);
  if (!BitmapToColor(initialHeadPath, initialHead)) //载入初始截图
    RecordError(task, account, "缺少头像截图", "缺少默认头像截图，无法登录", END_TASK);
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
  sprintf_s(shotPath, "%s\\ID%d_第%d次中断_%dP登录.png", work[task].logDirectory, plot[list].playingOrder + 1,
    plot[list].timesRefreshed[plot[list].playingOrder], account + 1);
  WindowToBitmap(work[task].hWndHall[account], shotPath);
  LeftClick(work[task].hWndServer, headXInServer, headYInServer);
  CheckSleep(task, 2000);//等待2秒
  return 0;//返回WaitServerInWindow函数，继续判断选服界面
}
//从大厅窗口hWndHall中寻找平台platform的最近服务器，找到了则返回位置
int FindLatestServer(int task, int account, int platform, int *px, int *py)
{
  int list = GetList(task);
  int result = FindLatestServer0(task, account, platform, px, py);
  if (work[task].hallShot)
  {
    free(work[task].hallShot);
    work[task].hallShot = nullptr;
  }
  return result;
}
//在大厅窗口hWndHall中等待最近服务器出现，返回所属平台
int WaitServerInWindow(int task, int account, int *px = NULL, int *py = NULL)
{
  int list = GetList(task);
  int times = 0;
  while (true)
  {
    times++;
    if (times >= 10)//10秒还未进入预期界面，则报错退出
      RecordError(task, account, "未找到服务器", "未找到最近服务器");
    for (int platform = 1; platform <= 3; platform++)//依次查找4399、QQ空间、QQ大厅色块，找到直接返回
      if (FindLatestServer(task, account, platform, px, py))
        return platform;
    int counter = 0;
    int result = FindLatestServer(task, account, 4, px, py);//查找QQ选头像界面
    while (result < 0)//QQ空间登录界面点击头像，如果头像数目不足则继续识别
    {
      counter++;
      if (counter >= 5)//刷新5次还不能解决，则强制中断
      {
        char Message[100];
        if (result == -1)
          sprintf_s(Message, "%dP登录界面头像数量不足", account + 1);
        else if (result == -2)
          sprintf_s(Message, "%dP登录界面头像未加载", account + 1);
        else if (result == -3)
          sprintf_s(Message, "%dP登录界面没有与截图匹配的头像", account + 1);
        RecordError(task, account, "无法选择头像", Message, END_TASK);
      }
      result = FindLatestServer(task, account, 4, px, py);//继续识别
    }
    FindLatestServer(task, account, 5, px, py);//识别断网界面，识别到则点击重试
    if (GetActiveGameWindow(work[task].hWndHall[account])) //自动出现了游戏窗口，视为微端，后续无需操作
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
        RecordError(task, account, "颜色存在超时", message);
      }
      else
        RecordError(task, account, tip, tip);
    }
    color = GetColor(task, account, x, y);
    CheckSleep(task, 100);
  } while (color == targetColor || color == 0);//如果颜色还没消失，则继续等
}
//高级任务刷新所有游戏窗口
void Refresh(int task)
{
  int list = GetList(task);
  if (plot[list].timesRefreshed[plot[list].playingOrder] == 0)
    sprintf_s(plot[list].refreshLog, "%s\\ID%d_开始前_刷新记录.txt", work[task].logDirectory, plot[list].playingOrder + 1);
  else
    sprintf_s(plot[list].refreshLog, "%s\\ID%d_第%d次中断_刷新记录.txt", work[task].logDirectory, plot[list].playingOrder + 1, plot[list].timesRefreshed[plot[list].playingOrder]);

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
  isHallModified[task] = 1;//禁用CheckHallWindow

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
      hWndOrigin[account] = GetActiveGameWindow(param[task].hWndHall[account]);
      int RefreshX = 228 * DPI / 96, RefreshY = 44 * DPI / 96;
      LeftClick(param[task].hWndHall[account], RefreshX, RefreshY); //点击刷新按钮
      sprintf_s(message, "%dP点击刷新按钮(%d,%d)", account + 1, RefreshX, RefreshY);
      addToRefreshLog(task, message);
    }

  for (int account = 0; account < 2; account++) //双号依次等待原来的游戏窗口关闭
    if (isRefreshRequired[account])
    {
      int counter = 0;
      while (hWndOrigin[account] && hWndOrigin[account] == GetActiveGameWindow(param[task].hWndHall[account]))
      {
        counter++;
        if (counter > 100)
          RecordError(task, account, "刷新失败", "刷新按钮点击无效");
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
      work[task].hWndServer = GetActiveServerWindow(param[task].hWndHall[account]);//找到选服窗口
      sprintf_s(message, "%dP已找到选服窗口(%d)", account + 1, (int)work[task].hWndServer);
      addToRefreshLog(task, message);

      int ServerWidth, ServerHeight;
      GetWindowRect(param[task].hWndHall[account], &work[task].hallRect);//获取大厅窗口位置
      GetWindowRect(work[task].hWndServer, &work[task].serverRect);//获取选服窗口位置
      GetWindowSize(work[task].hWndServer, &ServerWidth, &ServerHeight);
      sprintf_s(message, "%dP已获取选服窗口尺寸(%d*%d)", account + 1, ServerWidth, ServerHeight);
      addToRefreshLog(task, message);

      int x = 0, y = 0;
      platform[account] = WaitServerInWindow(task, account, &x, &y);//等待找到最近服务器，并识别平台类型
      const char *const platformName[4] = { "4399微端", "4399网页", "QQ空间", "QQ大厅" };//平台名称
      sprintf_s(message, "%dP已识别到选服面板，平台为%d(%s)，识别位置(%d,%d)", account + 1, platform[account], platformName[platform[account]], x, y);
      addToRefreshLog(task, message);

      //根据平台和大厅判定坐标确定最近服务器在选服窗口中的坐标
      int LatestServerX = 0, LatestServerY = 0;//最近服务器在选服窗口中的坐标
      if (platform[account] == 1) //4399网页：根据识别位置判断
      {
        LatestServerX = (x - 285) * DPI / 96 + work[task].hallRect.left - work[task].serverRect.left;
        LatestServerY = (y + 45) * DPI / 96 + work[task].hallRect.top - work[task].serverRect.top;
      }
      else if (platform[account] == 2)//QQ空间
      {
        if (plot[list].server[account] < 0 || plot[list].server[account] > 6)
          plot[list].server[account] = 0;
        if (plot[list].server[account] == 0) //空间服最近服务器
        {
          LatestServerX = (work[task].serverRect.right - work[task].serverRect.left) / 2 - 115 * DPI / 96;//中央位置左偏115
          LatestServerY = 267 * DPI / 96;//固定坐标267
        }
        else //3366固定服务器
        {
          const int Server3366X = 176, Server3366Y = 571, Server3366Width = 184, Server3366Height = 36;
          int centerX = (work[task].serverRect.right - work[task].serverRect.left) / 2;//中心X位置
          LatestServerX = centerX + (Server3366X - (plot[list].server[account] - 1) % 3 * Server3366Width) * DPI / 96;//中央位置左偏115
          LatestServerY = (Server3366Y - (plot[list].server[account] - 1) / 3 * Server3366Height) * DPI / 96;//固定坐标267
        }
      }
      else if (platform[account] == 3) //QQ大厅
      {
        LatestServerX = (work[task].serverRect.right - work[task].serverRect.left) / 2 + 30 * DPI / 96;//中央位置右偏30
        LatestServerY = 580 * DPI / 96;//固定坐标580
      }
      sprintf_s(message, "%dP最近服务器在选服窗口中的位置为(%d,%d)", account + 1, LatestServerX, LatestServerY);
      addToRefreshLog(task, message);
      CheckSleep(task, 500);

      param[task].hWnd[account] = NULL;//重新获取游戏窗口
      int counter = 0;
      while (param[task].hWnd[account] == NULL) //每1000ms获取1次游戏窗口
      {
        counter++;
        if (counter > 10)
          RecordError(task, account, "无法进入服务器", "无法进入服务器");
        if (platform[account] >= 1 && platform[account] <= 3) //4399网页、QQ空间、QQ大厅需要点击最近服务器
        {
          char ServerShotPath[maxPath];//选服截图
          sprintf_s(ServerShotPath, "%s\\ID%d_第%d次中断_%dP第%d次选服.png", work[task].logDirectory, plot[list].playingOrder + 1,
            plot[list].timesRefreshed[plot[list].playingOrder], account + 1, counter);
          WindowToBitmap(param[task].hWndHall[account], ServerShotPath);

          LeftClick(work[task].hWndServer, LatestServerX, LatestServerY);//点击最近服务器
          sprintf_s(message, "%dP点击最近服务器(%d,%d)", account + 1, LatestServerX, LatestServerY);
          addToRefreshLog(task, message);
        }
        CheckSleep(task, 1000);
        param[task].hWnd[account] = GetActiveGameWindow(param[task].hWndHall[account]);
      }
      sprintf_s(message, "%dP已进入服务器，游戏窗口(%d)", account + 1, (int)param[task].hWnd[account]);
      addToRefreshLog(task, message);
    }
  isHallModified[task] = 0;//恢复CheckHallWindow

  //游戏窗口已出现，双号依次等待进入游戏
  for (int account = 0; account < 2; account++)
    if (isRefreshRequired[account])
    {
      work[task].hWnd[account] = param[task].hWnd[account];
      const char *jumpButton[] = { "跳转按钮", "跳转按钮_高亮", "假期特惠" };
      WaitPictures(APPEAR, task, account, jumpButton, NULL, NULL, 0, 0, 60000);//等待出现跳转按钮，最多等60秒
      if (platform[account] >= 0 && platform[account] <= 2) //非QQ大厅还要等健康提示
        TryWaitPicture(APPEAR, task, account, "健康提示", 10000);//等待出现健康提示
      sprintf_s(message, "%dP刷新成功", account + 1);
      addToRefreshLog(task, message);
    }
}
//根据大厅句柄hWndHall和坐标Account绑定游戏窗口hWndGame。成功绑定返回1，原本就有窗口返回2
int BindWindow(int task, int account, HWND &hWndGame, HWND hWndHall, POINT Tag)
{
  if (!IsGameWindow(hWndGame))//如果hWndGame不存在或已失效，则通过大厅句柄寻找游戏窗口句柄
  {
    if (!IsWindow(hWndHall))//大厅窗口不存在，直接退出。
      RecordError(task, account, "未找到游戏窗口",
        "未找到游戏窗口。本程序只能用于\n360游戏大厅（极速模式）或微端。", END_TASK);
    char ClassName[256];
    GetClassName(hWndHall, ClassName, 256);//获取窗口类名

    //不是游戏大厅或微端，直接退出
    if (strcmp(ClassName, "ApolloRuntimeContentWindow") != zero && strcmp(ClassName, "DUIWindow") != zero)
      RecordError(task, account, "未找到游戏窗口",
        "未找到游戏窗口。本程序只能用于\n360游戏大厅（极速模式）或微端。", END_TASK);

    //运行到这里说明是游戏大厅或微端
    if (!PostMessage(hWndHall, WM_MOUSEMOVE, 0, MAKELPARAM(0, 0))) //检验权限
      RecordError(task, account, "权限不足", "权限不足，请右键执行器\n选择“以管理员身份运行”。", END_TASK);

    if (GetWindowLong(hWndHall, GWL_STYLE) & WS_MAXIMIZE)//如果窗口最大化，点击高度-8
      LeftClick(hWndHall, Tag.x, Tag.y - 8 * DPI / 96);//点击账号标签
    else
      LeftClick(hWndHall, Tag.x, Tag.y);//点击账号标签
    CheckSleep(task, 500);
    hWndGame = GetActiveGameWindow(hWndHall);//获取小号窗口

    if (hWndGame == NULL) //如果无法通过大厅句柄获取游戏窗口句柄，则刷新或中断
    {
      if (IsAdvance(task)) //高级任务：刷新两个账号
        RecordError(task, account, "未找到游戏窗口", "未找到游戏窗口");
      else
        RecordError(task, account, "未找到游戏窗口", "未找到游戏窗口。本程序只能用于\n360游戏大厅（极速模式）或微端。");
    }
    return 1;
  }

  //如果hWndGame已存在，向hWndGame检验权限
  if (!PostMessage(hWndGame, WM_MOUSEMOVE, 0, MAKELPARAM(0, 0))) //检验权限
    RecordError(task, account, "权限不足", "权限不足，请右键执行器\n选择“以管理员身份运行”。", END_TASK);
  return 2;
}
//验证账号是否处在可以跳转至竞技岛的界面
int isJumpButtonExist(int task, int account)
{
  int list = GetList(task);
  const char *jumpButton[] = { "跳转按钮", "跳转按钮_高亮", "假期特惠" };
  int counter = 0;
  work[task].hWnd[account] = param[task].hWnd[account];
  if (plot[list].advanceInvolved[account])
    while (FindPictures(task, account, jumpButton) == 0)
    {
      counter++;
      if (counter > 10) //1秒内找不到跳转按钮，返回0
        break;
      CheckSleep(task, 100);
    }
  work[task].hWnd[account] = 0;
  if (counter > 10) //1秒内找不到跳转按钮，返回0
    return 0;
  return 1;
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
        BindWindow(task, account, param[task].hWnd[account], param[task].hWndHall[account], param[task].tag[account]);

    for (int account = 0; account < 2; account++)//验证所有参与账号是否显示
      if (plot[list].advanceInvolved[account] == 1)
      {
        if (!IsGameWindowVisible(param[task].hWnd[account]))
          RecordError(task, account, "账号未显示",
            "所有参与刷图的账号都要显示。\n双开请点击大厅右上角“多窗口玩游戏”。", END_TASK);
        //账号已显示，如果不在可跳转界面，刷新两个账号
        if (!isJumpButtonExist(task, account))
          RecordError(task, account, "未找到跳转按钮", "未找到跳转按钮");
      }
    plot[list].isAdvanceWindowBound = 1;
  }
}
//每个任务启动时通过Work.hWndHall绑定Work.hWnd，并根据游戏窗口大小判断点击时是否需要DPI修正
void BindWindow(int task, int account)
{
  int result = BindWindow(task, account, work[task].hWnd[account], work[task].hWndHall[account], work[task].tag[account]);
  char Message[100];
  if (result == 1)
    sprintf_s(Message, "已找到%dP窗口\n", account);
  else
    sprintf_s(Message, "已继承%dP窗口\n", account);
  if (IsNormal(task)) //普通任务顺带更新Param句柄
    param[task].hWnd[account] = work[task].hWnd[account];
  strcat_s(work[task].embarkString, Message);
  RECT GameRect;
  GetClientRect(work[task].hWnd[account], &GameRect);//记录游戏窗口大小
  if (GameRect.right > 950)
    work[task].isDpiAwareRequired[account] = 1;//如果游戏窗口宽度大于950，则需要DPI修正
  else
    work[task].isDpiAwareRequired[account] = 0;//否则不需要修正
}
//清空Work[task]，保留前8字节（截图的内存和地址）
void ResetWork(int task)
{
  memset((char *)&work[task] + 8, 0, sizeof(work[task]) - 8);//清空Work[task]
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
        LoadTrack(task, account, true); //完整读取轨道（第1局执行前）
        sprintf_s(work[task].backupTrack[account], "自动备份\\Actuator%d%d.txt", task, account);
        Copy(work[task].pureTrack[account], work[task].backupTrack[account]);
      }
    for (int account = 0; account < 2; account++) //不参与的账号人物位置个数清零
      if (work[task].isInvolved[account] == 0)
        work[task].roleLocNum[account] = 0;
  }
  strcat_s(work[task].embarkString, "已读取轨道\n");

  /*绑定窗口句柄*/
  HWND hWndExist = NULL;
  for (int account = 0; account <= 1; account++)
    if (work[task].isInvolved[account] == 1)
    {
      BindWindow(task, account);
      if (hWndExist && hWndExist == work[task].hWnd[account])
        RecordError0(task, "绑定冲突", "1P和2P绑定了同一个账号", END_TASK);
      hWndExist = work[task].hWnd[account];
    }

  /*开局切换账号*/
  for (int account = 0; account < 2; account++)
    if (work[task].isPerformed[account] == 1)//如果小号1放卡，若小号1不可见，则点击切换到小号1
    {
      if (!IsGameWindowVisible(work[task].hWnd[account]))
      {
        if (GetWindowLong(work[task].hWndHall[account], GWL_STYLE) & WS_MAXIMIZE)//如果窗口最大化，点击高度-8
          LeftClick(work[task].hWndHall[account], work[task].tag[account].x, work[task].tag[account].y - 8 * DPI / 96);
        else
          LeftClick(work[task].hWndHall[account], work[task].tag[account].x, work[task].tag[account].y);
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
  ResetState(task);//重置任务task的状态
  ResetWork(task);//重置Work[task]
  work[task].isInvolved[0] = Involved[0];//记录账号是否参与和放卡
  work[task].isInvolved[1] = Involved[1];
  work[task].isPerformed[0] = Performed[0];
  work[task].isPerformed[1] = Performed[1];
  GetWorkFromParam(task);//获取任务工作参数
  isHallModified[task] = 0;//允许自动获取大厅句柄（因为已清空工作参数，所以暂时不会自动获取）
  return 1;
}
//是否为魔塔任务（含普通任务）
bool IsTower(int task)
{
  if (IsAdvance(task))
  {
    int list = GetList(task);
    return plot[list].playingAdvance->type == 2;
  }
  else
    return work[task].towerx[0] > 0;
}
//是否为单塔或密室（含普通任务的单塔）
bool IsSingleTower(int task)
{
  if (IsAdvance(task))
  {
    int list = GetList(task);
    return plot[list].playingAdvance->type == 2 &&
      (plot[list].playingAdvance->level / 1000 == 1 || plot[list].playingAdvance->level / 1000 == 3);
  }
  else
    return work[task].towerx[0] > 0;
}
//检查param[task]能否启动，能则传递参数到work[task]，不能则报错
int CheckTask(int task, bool isDualTask = false)
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
  int roleNumChecked[2] = {};//两个轨道检查获得的人数
  int trackNum = 0;//有效轨道数量
  int isInvolved[2] = {};//两个账号是否参与
  int isPerformed[2] = {};//两个账号是否放卡

  //如果是公会任务或签到施肥，不检查轨道，直接通过
  if (IsAdvance(task))
  {
    int list = GetList(task);
    if (IsMission(*plot[list].playingAdvance) || IsSpecial(*plot[list].playingAdvance))
    {
      for (int account = 0; account < 2; account++)
        isInvolved[account] = isPerformed[account] = plot[list].playingAdvance->deck[account] ? 1 : 0;
      return TaskReady(task, isInvolved, isPerformed);//准备启动任务
    }
  }

  for (int account = 0; account < 2; account++)
    if (strlen(param[task].track[account]) > 0)//若轨道存在
    {
      roleNumChecked[account] = LoadTrack(task, account, info[account], true);//载入并检查轨道
      isInvolved[account] = isPerformed[account] = 1;//账号参与+放卡
      trackNum++;
    }
    else //轨道不存在
      roleNumChecked[account] = -1;

  /*下列报错只在普通任务出现，因为高级任务开始前会一次性检查好*/
  if (trackNum == 0)//两个轨道都没填
    PopMessage(task, "请至少填写一个轨道");
  else if (roleNumChecked[0] == 0)//如果轨道1无法正常载入，弹窗提示
    PopMessage(task, info[0]);
  else if (roleNumChecked[1] == 0)//如果轨道2无法正常载入，弹窗提示
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
    int roleNum = GetRoleNum(task); //人物数量
    if (IsAdvance(task) && (param[task].deck[0] == 0 || param[task].deck[1] == 0))
      roleNum = 1;//高级任务Deck = 0的账号不参与
    if (roleNum == 2 && (param[task].tag[0].x == -1 || param[task].tag[1].x == -1))//双人轨道只抓了一个标签时
    {
      if (IsNormal(task)) //普通任务，由用户选择要不要执行
      {
        int Options = PopMessageOK(task, "您使用的是双人轨道，\n但只抓取了一个账号标签，\n是否以单人模式启动？");
        if (Options != IDOK)
          return 0;
        roleNum = 1;
        for (int account = 0; account < 2; account++)
          if (param[task].tag[account].x == -1)
            work[task].roleLocNum[account] = 0;
      }
    }
    //单人刷图，只有放卡账号参与，前面已经记录了；双人刷图，则记录一下两个号都参与
    if (roleNum == 2)
      isInvolved[0] = isInvolved[1] = 1;
    //普通任务魔塔轨道2P参与（高级任务work[task].tower[account]强制为0，不会触发此报错）
    if (work[task].towerx[0] + work[task].towerx[1] > 0 && isInvolved[1])
    {
      PopMessage(task, "魔塔轨道仅限房主单人使用。");
      return 0;
    }
    //刷技能模式
    if (work[task].isSkillMode)
    {
      if (!IsSingleTower(task)) //如果不是单人魔塔
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
    WaitPicture(APPEAR, task, account, "竞技");//等待进入竞技岛界面
  }
  WaitColorDisappear(task, account, blueBox.x, blueBox.y, boxColor);//等待服务器蓝框消失
  CheckSleep(task, 200);
}
//点击活动图标name，进入活动
void ClickActivity(int task, int account, char *name)
{
  int SwitchActivityX = 787, SwitchActivityY = 35;//切换活动
  int x, y;
  if (TryWaitPicture(APPEAR, task, account, name, 500, &x, &y) == 0)//如果没找到图标
  {
    Click(task, account, SwitchActivityX, SwitchActivityY);//点一下箭头
    WaitPicture(APPEAR, task, account, name, &x, &y);//等待图标出现
  }
  Click(task, account, x, y);//点击悬赏图标
}
//发现邀请时接受(1)或拒绝(0)邀请并等0.1秒，未发现邀请时不点击
int DealInvitation(int task, int account, int deal)
{
  POINT DealButton[2] = { { 609, 403 }, { 449, 403 }, };//拒绝和接受按钮位置
  if (FindPicture(task, account, "被邀请界面"))
  {
    Click(task, account, DealButton[deal]);
    CheckSleep(task, 100);
    return 1;
  }
  return 0;
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
  if (level == -1) //如果是打完领奖，最多等5秒，以免漏领
    TryWaitPictures(APPEAR, task, account, claimButton, 5000);//等待奖励出现
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
        RecordError(task, account, "悬赏领奖失败", "悬赏领奖失败", SKIP_TASK);
      //如果要打的关没有“领取”字样则忽略，继续建房开打（打完领不了会记录）
    }
  }
  if (level == -1)
    Click(task, account, closeBounty);//点击关闭悬赏
}
//进入公会任务列表
void EnterMission(int task, int account)
{
  const POINT Jump = { 870, 560 };//跳转按钮
  const POINT GuildMission = { 870, 257 };//跳转到公会任务
  const POINT Uncheck = { 608, 401 };//拒绝邀请+取消选中公会任务
  if (FindPicture(task, account, "跳转列表") == 0)
  {
    Click(task, account, Jump);//点击跳转按钮
    WaitPicture(APPEAR, task, account, "跳转列表");//等待打开跳转列表
  }
  Click(task, account, GuildMission);//跳转到公会任务
  WaitPicture(APPEAR, task, account, "公会任务");//等待进入公会任务界面
  WaitPicture(DISAPPEAR, task, account, "完成次数");//等待“完成次数”字样消失
  CheckSleep(task, 1000);//等待系统自动选中第一个任务
  Click(task, account, Uncheck);//取消选中任务
  CheckSleep(task, 100);
  Click(task, account, Uncheck);
  CheckSleep(task, 500);//等待任务取消选中，开始识图
}
POINT CloseMission = { 853, 56 };//关闭公会任务界面
//领取公会任务奖励
void ClaimMissionAward(int task, int account, bool isOpenRequired = true)
{
  int list = GetList(task);
  POINT Claim = { 642, 533 };//领取按钮

  //如果已经领奖失败过，就不反复领了
  if (plot[list].isMissionClaimFailed)
    return;

  if (isOpenRequired)
    EnterMission(task, account);//进入公会任务面板

  int x, y;
  int counter = 0;
  DealInvitation(task, account, 0);//先处理邀请再检验
  while (FindPicture(task, account, "公会任务完成", &x, &y))
  {
    counter++;
    if (counter > 15)
    {
      plot[list].isMissionClaimFailed = true;
      break;
    }
    Click(task, account, x + 5, y + 5);//点击已完成(+5是为了防止点边角无效）
    CheckSleep(task, 500);
    Click(task, account, Claim);//点击领取按钮
    if (!WaitPicture(APPEAR, task, account, "公会任务已领取", nullptr, nullptr, x, y, 3000, false))
    {
      plot[list].isMissionClaimFailed = true;
      break;
    }
    CheckSleep(task, 500);
    DealInvitation(task, account, 0);
  }
  if (isOpenRequired)
    Click(task, account, CloseMission);//关闭公会任务
}
//领取美食大赛奖励
void ClaimContestAward(int task, int account)
{
  POINT CloseContest = { 887, 51 };
  POINT ScrollTop = { 537, 360 };
  int ScrollHeight = 35;

  ClickActivity(task, account, "大赛图标");//点击大赛图标
  WaitPicture(APPEAR, task, account, "大赛界面");//等待打开大赛界面
  CheckSleep(task, 1000);//等待奖励加载出来
  int counter = 0;
  while (FindPicture(task, account, "大赛背景")) //如果10秒还没加载出来，很可能是网卡了
  {
    counter++;
    if (counter > 10) //10秒还加载不出来，中断任务
      RecordError(task, account, "大赛无法加载", "大赛面板加载失败");
    CheckSleep(task, 1000);
  }

  int x, y;
  for (int i = 0; i <= 6; i++) //分7段领取
  {
    Click(task, account, ScrollTop.x, ScrollTop.y + i * ScrollHeight);//点击第i段大赛任务
    CheckSleep(task, 500);//等待0.5秒
    counter = 0;
    DealInvitation(task, account, 0);//先处理邀请再检验
    while (FindPicture(task, account, "大赛领取按钮", &x, &y))
    {
      counter++;
      if (counter > 15)
        RecordError(task, account, "大赛领奖失败", "无法领取大赛奖励");
      Click(task, account, x + 5, y + 5);//点击领取按钮(+5是为了防止点边角无效）
      CheckSleep(task, 3500);
      DealInvitation(task, account, 0);
    }
  }
  Click(task, account, CloseContest);//点击关闭大赛
}
//关闭健康提示和充值面板
void CloseHealthAndPayment(int task, int account)
{
  const POINT closeHealthPrompt = { 588, 212 };//关闭健康提示
  const POINT closePayment = { 899, 51 };//关闭充值面板
  int counter = 0;
  while (FindPicture(task, account, "健康提示")) //关闭健康提示
  {
    counter++;
    if (counter > 10)
      RecordError(task, account, "无法关闭提示", "无法关闭健康提示");
    Click(task, account, closeHealthPrompt);
    CheckSleep(task, 500);
  }
  counter = 0;
  while (FindPicture(task, account, "假期特惠")) //关闭假期特惠
  {
    counter++;
    if (counter > 10)
      RecordError(task, account, "无法关闭特惠", "无法关闭假期特惠");
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
//跳转到海洋一二区，星际一二区，营地一二区，遗迹一区，竞技一区
void RandomJump(int task, int account)
{
  int list = GetList(task);
  const POINT closeWorldMap = { 828, 61 };
  const POINT changeZone = { 819, 84 };//换线按钮
  char *const zoneName[8] = { "营地一区", "营地二区", "星际一区", "星际二区", "海洋一区", "海洋二区", "遗迹一区", "竞技一区" };
  const POINT zoneLocation[8] = { { 780, 84 }, { 780, 110 }, { 780, 84 }, { 780, 110 }, { 780, 84 }, { 780, 110 }, { 780, 84 }, { 780, 84 } };
  const int islandOrder[7] = { 6, 6, 5, 5, 4, 4, 2 };
  int zoneOrder = 7;//目标小区编号
  const char *targetIslandName = "竞技";//目标岛屿名称
  POINT targetIslandLocation = { 201, 346 };//目标岛屿位置
  if (!plot[list].isLevelInsufficient) //如果等级足够，执行随机跳转
  {
    zoneOrder = plot[list].randomZone[plot[list].playedAdvanceNum % 7];//跳转到的小区编号
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
    if (!TryWaitPicture(DISAPPEAR, task, account, "世界地图", 3000))
    {
      if (plot[list].isLevelInsufficient) //如果竞技岛都跳不了
        RecordError(task, account, "等级不足", "等级不足8级，无法使用高级任务", END_TASK);
      //如果是普通岛屿跳不了
      plot[list].isLevelInsufficient = true;//记录等级不足，下次跳竞技岛
      RecordError(task, account, "等级不足", "等级不足，尝试跳转至竞技岛");
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
      RecordError(task, account, "换区失败", message);
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
  POINT CloseChallenge = { 845, 57 };//关闭勇士界面
  POINT CloseTower = { 927, 34 };//关闭魔塔界面
  POINT CloseCross = { 916, 34 };//关闭跨服界面

  CheckSleep(task, 500);
  //无轨关卡直接退
  if (IsNoTrack(*plot[list].playingAdvance))
    return;
  for (int account = 0; account <= 1; account++)
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
          if (plot[list].playingAdvance->type != 2)
            Click(task, account, retreat);//除魔塔外，翻牌完成后要退出房间
          CheckSleep(task, 1000);//等待1秒

          if (plot[list].playingAdvance->type == 0) //对于悬赏任务，退出后要领取奖励
            ClaimBountyReward(task, account);//领取悬赏奖励
          //对于勇士任务，房主后要关闭勇士界面
          else if (plot[list].playingAdvance->type == 1 && account == work[task].host)
            Click(task, account, CloseChallenge);
          else if (plot[list].playingAdvance->type == 2)//对于魔塔任务，退出后要关闭魔塔界面
            Click(task, account, CloseTower);
        }
      }
      else //如果已退出房间
      {
        //悬赏：都退出了肯定没奖励，啥也不用干；
        //勇士：房主关闭界面即可
        if (plot[list].playingAdvance->type == 1 && account == work[task].host)
          Click(task, account, CloseChallenge);
        else if (plot[list].playingAdvance->type == 2)//魔塔：关闭魔塔界面
          Click(task, account, CloseTower);
        else if (plot[list].playingAdvance->type == 3)//跨服：关闭跨服界面后随机跳转
        {
          Click(task, account, CloseCross);
          const char *jumpButton[] = { "跳转按钮", "跳转按钮_高亮" };
          WaitPictures(APPEAR, task, account, jumpButton);
          CheckSleep(task, 500);
          RandomJump(task, account);
        }
        //假期、公会、普通图：啥也不用干
      }
      if (contestOption == 1)
      {
        CheckSleep(task, 1000);
        ClaimContestAward(task, account);
      }
      if (plot[list].playingAdvance->isMission)
      {
        CheckSleep(task, 1000);
        ClaimMissionAward(task, account);
      }
    }
  CheckSleep(task, 1000);//退出后等待1秒，任务线程结束
}
void FinishTask(int task)
{
  RecordLevelTime(task, true);//记录每局用时
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
int WaitIndoor(int task, int account, bool noError = false, int waitTime = 5000)
{
  int list = GetList(task);
  int digit = -1;
  int counter = 0;
  while (digit == -1)//等待房主进入房间
  {
    counter++;
    if (counter >= waitTime / 100)
    {
      if (noError == false)
      {
        if (account == work[task].host)
          RecordError(task, account, "创建房间失败", "创建房间失败");
        else
          RecordError(task, account, "进入房间失败", "进入房间失败");
      }
      return -1;
    }
    CheckSleep(task, 100);

    const char *indoor[] = { "选卡界面", "选卡界面2" };
    if (FindPictures(task, account, indoor))//如果检测到进入房间
    {
      digit = indoorDigit.Get(task);//读取一次房间号
      if (digit == -1) //进了房间没有房间号，视为0
        digit = 0;
    }
    else //截图失败或未进入房间，房间号为-1
      digit = -1;
  }
  CheckSleep(task, 100);
  return digit;
}
//进入指定房间号的密码房（含输入密码）
int EnterRoom(int task, int account, int roomDigit)
{
  int RoomScrollX = 723, RoomScrollY = 150, RoomScrollHeight = 93;//房间列表滚动条
  int ConfirmPasswordX = 475, ConfirmPasswordY = 335;//进入房间确认密码
  int num, height[5], room[5];
  for (int h = 0; h < 9; h++)//滚动条上中下位置点3遍，每点一遍进行识图查找房间号
  {
    Click(task, account, RoomScrollX, RoomScrollY + RoomScrollHeight * (h % 3));//点击滚动条
    CheckSleep(task, 500);
    if (!UpdateMap(task, account))//截图失败视为进入房间失败
      return 0;
    num = listDigit.GetListDigit(task, height, room);//获取房间列表本页的房间数、各个房间的高度和编号
    for (int n = 0; n < num; n++)
      if (room[n] == roomDigit)
      {
        DealInvitation(task, account, 0);//拒绝邀请
        Click(task, account, ListX, height[n]);//点击房间号
        /*输入密码*/
        CheckSleep(task, 500);
        for (int i = 1; i <= 6; i++)//连续6下退格
          Press(task, account, 8);
        for (int i = 1; i <= 3; i++)//输入222
          Press(task, account, 98);
        CheckSleep(task, 500);
        Click(task, account, ConfirmPasswordX, ConfirmPasswordY);//确认密码，进入房间
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
        RecordError(task, account, "跨服创房失败", "跨服创建房间失败");
      else
        RecordError(task, account, "跨服进房失败", "跨服进入房间失败");
    }
    CheckSleep(task, 100);
    if (UpdateMap(task, account))
      roomDigit = crossIndoorDigit.Get(task);
    else
      roomDigit = -1;
  }
  return roomDigit;
}
//在房间列表创建密码房，返回房间号
int CreatePasswordRoom(int task, int account)
{
  const POINT tickPassword = { 490, 452 };//勾选密码
  const POINT  inputPassword = { 686, 454 };//输入密码
  const POINT  create = { 585, 495 };//创建房间

  CheckSleep(task, 500);//等待500 ms
  Click(task, account, tickPassword);//勾选密码
  WaitPicture(APPEAR, task, account, "密码已勾选");
  Click(task, account, inputPassword);//点击密码输入框
  CheckSleep(task, 500);//必要的停顿
  for (int i = 1; i <= 6; i++)//连续6下退格
    Press(task, account, 8);
  for (int i = 1; i <= 3; i++)//输入222
    Press(task, account, 98);
  CheckSleep(task, 500);//必要的停顿
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
        RecordError(task, account, "跨服换区失败", "跨服换区失败");
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
      RecordError(task, account, "无法点开列表", "无法点开等级筛选列表");
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
    int roomNum = crossListDigit.GetCrossListDigit(task, x, y, room);//获取房间列表本页的房间数、各个房间的高度和编号
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
  int RoomDigit = CreateCrossRoom(task, work[task].host, level);//房主创建跨服房间

  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    EnterCrossInterface(task, 1 - work[task].host);//队友进入跨服2区
    CheckSleep(task, 500);//必要的停顿
    FilterCrossLevel(task, 1 - work[task].host, level);//筛选跨服关卡
    CheckSleep(task, 500);//必要的停顿
    if (EnterCrossRoom(task, 1 - work[task].host, RoomDigit) == 0)//按房间号进入房主的关卡（找不到则翻页）
      RecordError(task, 1 - work[task].host, "未找到房间号", "未找到房主创建的房间号");
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
//记录细角色名
void RecordLightName(int task)
{
  const COLORREF lightNameColor = 0xa45815;//好友列表角色名颜色
  for (int y = 0; y < lightNameHeight; y++)
    for (int x = 0; x < lightNameWidth; x++)
      work[task].lightName[y][x] = work[task].map[lightNameY + y][lightNameX + x] == lightNameColor;
}
//记录粗角色名
void RecordHeavyName(int task)
{
  for (int y = 0; y < heavyNameHeight; y++)
    for (int x = 0; x < heavyNameWidth; x++)
      work[task].heavyName[y][x] = work[task].map[heavyNameY + y][heavyNameX + x];
}
//细角色名和邀请列表第line行的名字是否匹配
bool IsLightNameEqual(int task, int line)
{
  int inviteeX = 460, inviteeY = 150 + line * 30;//邀请列表角色名位置
  const COLORREF inviteeNameColor = 0x993300;//邀请列表角色名颜色
  for (int y = 0; y < lightNameHeight; y++)
    for (int x = 0; x < lightNameWidth; x++)
      if (work[task].lightName[y][x] != (work[task].map[inviteeY + y][inviteeX + x] == inviteeNameColor))
        return false;
  return true;
}
//等待出现邀请界面并接受，成功接受邀请返回1，没有被邀请返回0
int WaitInvitationAndAccept(int task, int account)
{
  POINT accept = { 449, 403 };
  if (TryWaitPicture(APPEAR, task, account, "被邀请界面", 3000) == 1)//如果3秒内出现了被邀请界面
  {
    Click(task, account, accept);//接受邀请
    return 1;
  }
  return 0;
}
//账号1-account邀请account加入room房间（room==0则不验证房间号）
//邀请成功返回1，否则返回0
int Invite(int task, int account, int room = 0)
{
  const int pageNum = 6;
  //翻页按钮位置（大厅3页，好友3页）
  const POINT pageButton[pageNum] = {
    { 458, 130 }, { 562, 459 }, { 562, 459 },
    { 527, 130 }, { 562, 459 }, { 562, 459 } };
  POINT openInvitation = { 410, 546 };
  POINT sendInvitation = { 600, 156 };
  POINT quitInvitation = { 587, 491 };

  int inviteeOrder = -1;//被邀请者序号
  int invitationSuccess = 0;//是否邀请成功

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
      if (IsLightNameEqual(task, line))
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
          invitationSuccess = 1; //正确说明邀请成功
        else
          invitationSuccess = 0; //否则说明邀请失败
      }
      else
        invitationSuccess = 0; //虽然成功点击了邀请按钮，但队友没有收到邀请
      break;
    }
  }
  if (invitationSuccess == 0)
    RecordError(task, account, "邀请失败", "邀请失败");
  Click(task, 1 - account, quitInvitation);//房主退出邀请界面
  return invitationSuccess;
}
//将细角色名保存到文件
bool SaveLightName(int task, const char *path)
{
  FILE *f;
  if (fopen_s(&f, path, "w"))
    return false;
  for (int y = 0; y < lightNameHeight; y++)
    for (int x = 0; x < lightNameWidth; x++)
      fputc(work[task].lightName[y][x] ? '1' : '0', f);
  fclose(f);
  return true;
}
//将粗细角色名保存到文件
void SaveRoleName(int task)
{
  char heavyNamePath[maxPath], lightNamePath[maxPath];
  for (int code = 0; code < 99; code++)
  {
    sprintf_s(heavyNamePath, "用户参数\\角色名\\%d.png", code);
    if (!FileExist(heavyNamePath)) //如果该粗角色名不存在，进行新增
    {
      ColorToBitmap(work[task].heavyName, heavyNamePath);
      sprintf_s(lightNamePath, "用户参数\\角色名\\%d.txt", code);
      SaveLightName(task, lightNamePath);
      LoadRoleName();//重新载入角色名
      break;
    }
  }
}
//截取被邀请者(account)角色名
int CatchInviteeName(int task, int account)
{
  POINT Friend = { 40, 106 };
  POINT CloseFriend = { 757, 102 };
  bool isHeavyNameCatched = false;

  //更新被邀请账号截图，保存粗角色名
  if (UpdateMap(task, account))
  {
    RecordHeavyName(task);
    isHeavyNameCatched = true;
    //如果粗角色名已有记录，直接读取细角色名
    for (int code = 0; code < roleNameNum; code++)
      if (IsBitmapEqual(work[task].heavyName, roleName[code].heavy, heavyNameWidth, heavyNameHeight, 0, 0, 0, 0, 0xffffff))
      {
        memcpy(work[task].lightName, roleName[code].light, sizeof(work[task].lightName));
        return 1;
      }
    CheckSleep(task, 100);
  }

  for (int i = 0; i < 3; i++)
    Click(task, account, 152, 44);//三击角色名
  CheckSleep(task, 500);//必要的停顿
  PressCtrl(task, account, 'C');//复制
  Click(task, 1 - account, Friend);//房主打开好友界面
  WaitPicture(APPEAR, task, 1 - account, "好友界面");
  CheckSleep(task, 1000);
  SearchInviteeName(task, account);//搜索队友名字
  RecordLightName(task);//记录队友细角色名
  Click(task, 1 - account, CloseFriend);//关闭好友界面
  //至此细角色名已保存成功，如果粗角色名也保存成功，则进行保存
  if (isHeavyNameCatched)
    SaveRoleName(task);
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
  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    if (!CatchInviteeName(task, 1 - work[task].host))//截图保存队友名字
      RecordError(task, 1 - work[task].host, "截图昵称失败", "无法截图队友昵称");
    CheckSleep(task, 500);//必要的停顿
  }
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
  if (TryWaitPicture(APPEAR, task, account, "勇士界面", 3000) == 0) //如果没等到就再打开一次
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
      RecordError(task, account, "BOSS切换失败", "勇士BOSS向后切换失败");
  }
  while (CurrentLevel > level)//当前关卡大于目标关卡，向前切换
  {
    Click(task, account, LastBossX, LastBossY);
    if (WaitPicture(APPEAR, task, account, challenge[CurrentLevel - 1], &x, &y))
      CurrentLevel--;
    else
      RecordError(task, account, "BOSS切换失败", "勇士BOSS向前切换失败");
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
  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    if (!CatchInviteeName(task, 1 - work[task].host))//截图保存队友名字
      RecordError(task, 1 - work[task].host, "截图昵称失败", "无法截图队友昵称");
    CheckSleep(task, 500);//必要的停顿
  }
  int room = CreateChallengeRoom(task, work[task].host, level);//房主创建假期房间
  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    SelectiveSleep(task, 500);
    Invite(task, 1 - work[task].host, room);//邀请队友加入房间，邀请失败会报错
  }
}
//进入魔塔界面(tag：1单人 2双人 3密室)
void EnterTower(int task, int account, int rolesNum)
{
  int TowerX = 319, TowerY = 456;
  const POINT tagLocation[3] = { { 43, 67 }, { 114, 67 }, { 189, 67 } };//单人 双人 密室

  if (rolesNum == 2) //双塔先截图队友昵称，用于邀请
  {
    if (!CatchInviteeName(task, 1 - work[task].host))//截图保存队友名字
      RecordError(task, 1 - work[task].host, "截图昵称失败", "无法截图队友昵称");
    CheckSleep(task, 500);//必要的停顿
  }

  Click(task, account, worldMap);//打开世界地图
  WaitPicture(APPEAR, task, account, "世界地图");//等待打开世界地图
  Click(task, account, TowerX, TowerY);//进入魔塔蛋糕
  WaitPicture(APPEAR, task, account, "魔塔界面");//等待进入魔塔选层界面
  CheckSleep(task, 500);
  WaitColorDisappear(task, account, 273, 557, 0x4f6e87, "魔塔加载超时");//等待层数正常加载
  CheckSleep(task, 500);
  Click(task, account, tagLocation[rolesNum - 1]);//选择标签（单人 双人 密室）
  CheckSleep(task, 500);
}
bool isMissionInProgress(int task, int missionY)
{
  for (int x = 0; x < missionStateWidth; x++)
    for (int y = 0; y < missionStateHeight; y++)
    {
      char isWhite = work[task].map[missionY + y][missionStateX + x] == 0xffffff;
      if (isWhite != missionInProgress[x][y]) //有一个颜色不匹配就返回false
        return false;
    }
  return true;
}
//添加高级任务
void AddAdvance(int list, int type, int level, int maxGames, int deck0, int deck1)
{
  plot[list].advance[plot[list].advanceNum].selected = 1;
  plot[list].advance[plot[list].advanceNum].type = type;
  plot[list].advance[plot[list].advanceNum].level = level;
  plot[list].advance[plot[list].advanceNum].maxGames = maxGames;
  plot[list].advance[plot[list].advanceNum].isProceed = 1;
  strcpy_s(plot[list].advance[plot[list].advanceNum].track[0], "Track.txt");
  strcpy_s(plot[list].advance[plot[list].advanceNum].track[1], "");
  plot[list].advance[plot[list].advanceNum].deck[0] = deck0;
  plot[list].advance[plot[list].advanceNum].deck[1] = deck1;
  plot[list].advanceNum++;
}
//一键配轨：为所有任务填写通用轨道
void MatchCommonTrack(int list)
{
  char levelName[100];//关卡名
  char track[2][maxPath] = {};//房主和队友的轨道路径
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    AdvanceType &advance = plot[list].advance[order];
    if (advance.selected == 0) //跳过未勾选的任务
      continue;
    for (int account = 0; account < 2; account++)
    {
      strcpy_s(advance.track[account], "");//轨道先留空
      if (advance.deck[account] > 0)
        advance.deck[account] = 1;//原本有卡组的，卡组设为1
    }
    if (IsNoTrack(advance)) //跳过无轨关卡
      continue;
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
}
//带弹窗选择的一键配轨
void MatchCommonTrackWithChoice(int list)
{
  const char *message = "将为勾选的任务填写通用轨道和卡组1，\n是否继续？";
  if (PopMessageOK(GetTask(list), message) == IDOK)
    MatchCommonTrack(list);
}
//创建默认高级任务
void CreateInitialAdvance(int list)
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
  plot[list].advance[6].host = 2;//魔塔房主改为“同步”
  MatchCommonTrack(list);
  for (int order = 2; order <= 4; order++)
    strcpy_s(plot[list].advance[order].track[0], "Track.txt");
}
const int ADVANCE = 0, MISSION = 1;//高级任务列表和公会任务列表
//从path读取任务列表。target：0=Advance，1=MissionAdvance
int LoadAdvanceFrom(int list, char *path, int target)
{
  AdvanceType *adv = target == ADVANCE ? plot[list].advance : plot[list].missionAdvance;
  int &num = target == ADVANCE ? plot[list].advanceNum : plot[list].missionAdvanceNum;
  bool &isLocked = target == ADVANCE ? plot[list].isAdvanceLocked : plot[list].isMissionAdvanceLocked;

  char s[1000];//列表一行信息
  FILE *f;

  if (fopen_s(&f, path, "r")) //读取失败
  {
    if (target == ADVANCE) //如果是读取当前列表失败，则创建初始列表
      CreateInitialAdvance(list);
    return 0;
  }
  memset(adv, 0, maxAdvanceNum * sizeof(AdvanceType));
  num = 0;
  isLocked = false;

  int version = 0;//0=没有title的版本；1=3.9以前的版本；2=3.9及以后的版本
  while (!feof(f))
  {
    fgets(f, s);//读取一行
    if (strstr(s, "列表版本号")) //新版
    {
      int mainVersion = 0, secondVersion = 0;
      sscanf_s(s, "列表版本号=v%d.%d", &mainVersion, &secondVersion);
      if (secondVersion >= 10)
        secondVersion = secondVersion / 10;
      if (mainVersion * 10 + secondVersion >= 39)
        version = 2;
      else
        version = 1;
      fgets(f, s);
      if (strstr(s, "锁定="))
      {
        isLocked = atoi(s + 5) == 1;
        fgets(f, s);
      }
    }
    if (strlen(s) > 0)//非空才读取信息
    {
      int itemsScanned = sscanf_s(s, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d", &adv[num].selected,
        &adv[num].type, &adv[num].level, &adv[num].maxGames,
        &adv[num].deck[0], &adv[num].deck[1], &adv[num].host,
        &adv[num].isMission, &adv[num].isProceed);
      if (version == 0 && adv[num].type >= 5) //旧版5以上的类型要加1
        adv[num].type++;
      if (adv[num].type == 2 && adv[num].level < 1000)//如果是魔塔类型
        adv[num].level += 1000;
      if (version <= 1 && adv[num].type == 5) //中间版本没有公会任务，关卡+1
        adv[num].level++;
      if (itemsScanned < 7)
        adv[num].host = 0;//房主
      if (itemsScanned < 8)
        adv[num].isMission = 0;//是否公会任务
      if (itemsScanned < 9)
        adv[num].isProceed = 1;//是否继续作战
      for (int account = 0; account <= 1; account++)
        fgets(f, adv[num].track[account]);//读取一行
      num++;
    }
  }
  fclose(f);
  return 1;
}
//从列表文件读取任务列表
int LoadAdvance(int list)
{
  return LoadAdvanceFrom(list, plot[list].advancePath, ADVANCE);
}
//保存高级任务列表
void SaveAdvanceTo(int list, char *path, int target)
{
  AdvanceType *adv = target == ADVANCE ? plot[list].advance : plot[list].missionAdvance;
  int &num = target == ADVANCE ? plot[list].advanceNum : plot[list].missionAdvanceNum;
  bool &isLocked = target == ADVANCE ? plot[list].isAdvanceLocked : plot[list].isMissionAdvanceLocked;

  FILE *f;
  if (fopen_s(&f, path, "w"))
    return;
  fprintf(f, "列表版本号=%s\n", version);
  fprintf(f, "锁定=%d\n", (int)isLocked);
  for (int order = 0; order < num; order++)
    fprintf(f, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n%s\n%s\n", adv[order].selected, adv[order].type,
      adv[order].level, adv[order].maxGames, adv[order].deck[0], adv[order].deck[1],
      adv[order].host, adv[order].isMission, adv[order].isProceed,
      adv[order].track[0], adv[order].track[1]);
  fclose(f);
}
//任务列表是否改变
int IsAdvanceChanged(int list)
{
  char path[50];
  sprintf_s(path, "自动备份\\任务列表\\%s\\%d.txt", plot[list].backupFolder, plot[list].currentBackup);
  return (int)!IsFileEqual(plot[list].advancePath, path);//如果当前任务列表与最新备份不同，则视为改变
}
//保存高级任务并备份
void SaveAdvance(int list)
{
  SaveAdvanceTo(list, plot[list].advancePath, ADVANCE);
  if (IsAdvanceChanged(list))//如果列表有变，进行备份
  {
    char path[50];
    plot[list].currentBackup++;
    plot[list].backupNum = plot[list].currentBackup;
    sprintf_s(path, "自动备份\\任务列表\\%s\\%d.txt", plot[list].backupFolder, plot[list].currentBackup);
    SaveAdvanceTo(list, path, 0);
  }
}
//识别公会任务，将“进行中”的战斗任务编号在missionExist中记为1；发现新公会任务返回true
bool ReadMission(int task, int account, bool(&missionExist)[maxAdvanceNum])
{
  bool isNewMissionPrompted = false;
  int list = GetList(task);
  int counter = 0;
  //截取公会任务面板
  while (!UpdateMap(task, account))
  {
    counter++;
    if (counter > 10)
      RecordError(task, account, "任务截图失败", "公会任务截图失败");
    CheckSleep(task, 100);
  }
  //识别任务存入数组
  int streak = 0; //连续颜色数量
  for (int y = 162; y <= 536; y++) //在高度范围内查找公会任务并保存文件
  {
    if (work[task].map[y][missionX - 3] == missionBarColor)
    {
      streak++;
      if (streak == missionHeight + 1) //找到一条公会任务，进行记录
      {
        int missionY = y - missionHeight;
        LoadCurMission(task, missionY);
        //与已有的每个任务比较
        for (int order = 0; order < missionNum; order++)
        {
          if (missionEqual(order)) //与任务order图像一致则记录编号
          {
            if (isMissionInProgress(task, missionY)) //如果状态为“进行中”
            {
              if (order == 30 && work[task].map[missionY + 2][missionX + 3] != 0x00ffff)
                missionExist[31] = true;//薄荷海滩夜晚
              else
              {
                missionExist[order] = true;//记录任务编号存在
                if (order == 78) //78号任务“鼠族难题”对应两个任务
                  missionExist[79] = true;
              }
            }
            break;
          }
          if (order == missionNum - 1)//如果匹配失败，截图并记录
          {
            char newMissionPath[maxPath] = {};
            char newTrackPath[maxPath] = {};
            sprintf_s(newMissionPath, "附加程序\\公会任务\\%d.png", missionNum + 1);
            sprintf_s(newTrackPath, "预制轨道\\公会任务\\%d_未命名.txt", missionNum + 1);
            ColorToBitmap(work[task].map, newMissionPath, missionX, missionY, missionWidth, missionHeight);

            LoadMission();//更新公会任务截图
            LoadAdvanceFrom(list, "预制列表\\公会任务.txt", MISSION);
            plot[list].missionAdvance[plot[list].missionAdvanceNum].selected = 0;
            plot[list].missionAdvance[plot[list].missionAdvanceNum].type = 6;
            plot[list].missionAdvance[plot[list].missionAdvanceNum].level = 2;
            plot[list].missionAdvance[plot[list].missionAdvanceNum].maxGames = 1;
            plot[list].missionAdvance[plot[list].missionAdvanceNum].host = 0;
            strcpy_s(plot[list].missionAdvance[plot[list].missionAdvanceNum].track[0], newTrackPath);
            strcpy_s(plot[list].missionAdvance[plot[list].missionAdvanceNum].track[1], "");
            plot[list].missionAdvance[plot[list].missionAdvanceNum].deck[0] = 1;
            plot[list].missionAdvance[plot[list].missionAdvanceNum].deck[1] = 1;
            plot[list].missionAdvance[plot[list].missionAdvanceNum].isMission = 0;
            plot[list].missionAdvanceNum++;
            SaveAdvanceTo(list, "预制列表\\公会任务.txt", MISSION);
          }
        }
      }
    }
    else
      streak = 0;
  }
  return isNewMissionPrompted;
}
//【全程识图】进入公会副本房间列表，并切换到竞技一区
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
  WaitPicture(APPEAR, task, account, "房间列表");//等待打开房间列表
  RandomJump(task, account);
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
  WaitPicture(APPEAR, task, account, "房间列表");//等待打开房间列表
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
    if (!WaitPicture(DISAPPEAR, task, account, "签到按钮", nullptr, nullptr, x, y, 3000, false))
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
  int isSignRequired = TryWaitPicture(APPEAR, task, account, "VIP签到按钮", 3000);//先等VIP签到按钮亮
  int isWageRequired = TryWaitPicture(APPEAR, task, account, "工资领取按钮", 1000);//再等待工资领取按钮亮
  //点击VIP签到
  if (isSignRequired)
  {
    Click(task, account, sign);
    if (!TryWaitPicture(APPEAR, task, account, "VIP签到按钮_灰", 3000))
      plot[list].playingAdvance->result = 2;
    CheckSleep(task, 500);
  }
  //点击工资领取
  if (isWageRequired)
  {
    Click(task, account, wage);
    if (!TryWaitPicture(APPEAR, task, account, "工资领取按钮_灰", 3000))
      plot[list].playingAdvance->result = 2;
    CheckSleep(task, 500);
  }
  Click(task, account, closePanel);//点击关闭签到面板
  CheckSleep(task, 100);
  return 1;
}
//签到（每日签到和VIP签到）
void Sign(int task, int account)
{
  DailySign(task, account);
  VipSign(task, account);
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
  int x0, int y0, COLORREF specificColor = 0, int timeout = 5000)
{
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
      if (WaitAreaChange(task, account, guildList, guildListX, guildListY, guildListColor, 3000))
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
      RecordError(task, account, "公会列表打不开", "打开公会列表失败");
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
    RecordError(task, account, "前往公会树失败", "前往公会树失败");
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
      if (growthDigit.isFull(task) || growthDigit.isTotalFull(task))
        if (!ChooseAnotherTree(task, account, avaliable, treeCode))
          return 0;

    //如果成长值满，就换一棵树，直到成长值未满；如果翻页翻到头了，则返回0
    while (growthDigit.isFull(task))
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
        RecordError(task, account, "浇水施肥失败", "浇水施肥失败");
      CheckSleep(task, 100);
      //如果成长值获取成功，成长值不为空字符串，视为浇水成功
      if (UpdateMap(task, account) && growthDigit.GetGrowth(task, growthString))
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
  const POINT guild = { 778, 557 };
  const POINT guildActivity = { 747, 412 };
  const POINT guildGarden = { 839, 331 };
  const POINT closeGarden = { 853, 56 };
  const POINT closeGuild = { 904, 57 };

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
      RecordError(task, account, "无法进入花园", "无法进入公会花园");
  }

  //等待公会花园加载完成
  counter = 0;
  char growth[10];
  while (true)
  {
    counter++;
    if (counter > 10)
      RecordError(task, account, "花园加载失败", "公会花园加载失败");
    CheckSleep(task, 500);
    if (UpdateMap(task, account))
    {
      growthDigit.GetGrowth(task, growth);
      //第一条件：成长值不为"0/0"且标题为“我的公会花园”
      if (strcmp(growth, "0/0") != 0 && FindPictureInMap(task, "我的公会花园"))
      {
        //达成第一条件后，继续等待第二条件
        int counter1 = 0;
        while (true)
        {
          counter1++;
          if (counter1 > 10)
            RecordError(task, account, "公会树加载失败", "公会树加载失败");
          CheckSleep(task, 500);
          if (UpdateMap(task, account))
          {
            growthDigit.GetTotalGrowth(task, growth);
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

  int isMissionFinished[2] = {};//两个任务是否正常完成
  bool isPageFinished = false;//页数是否已经用尽
  if (missionExist[0]) //浇水任务
  {
    isMissionFinished[0] = Water(task, account, water, confirm, 1, avaliable, treeCode);
    if (isMissionFinished[0] == 0)
      isPageFinished = true;
  }
  if (missionExist[1] && !isPageFinished) //施肥任务
    isMissionFinished[1] = Water(task, account, manure, confirm, 3, avaliable, treeCode);

  Click(task, account, closeGarden);
  CheckSleep(task, 100);
  Click(task, account, closeGuild);
  CheckSleep(task, 100);
  EnterMission(task, account);//进入公会任务面板
  ClaimMissionAward(task, account, false);//领奖
  memset(missionExist, 0, sizeof(missionExist));//所有公会任务预设为不存在
  ReadMission(task, account, missionExist);//识别公会任务存入数组
  //如果施肥任务还未完成，宣告任务失败
  if (missionExist[1])
    plot[list].playingAdvance->result = 2;
  Click(task, account, CloseMission);//关闭公会任务
  return 1;
}
//判断(x,y)位置的道具是否为可删物品
bool IsTrash(int task, int x, int y)
{
  for (int code = 0; code < trashNum; code++)
    if (IsBitmapEqual(work[task].map, trash[code].image, lootCoreWidth, lootCoreHeight,
      x + lootCoreX, y + lootCoreY, lootCoreX, lootCoreY))
      return true;
  return false;
}
//寻找第一个可删物品位置（物品区左上角(x0,y0)，列数columnsNum）
bool FindTrash(int task, int account, int x0, int y0, int columnsNum,
  int *pRow, int *pColumn)
{
  ForcedUpdateMap(task, account);//强制截图
  //起始搜索位置：宝石从头开始搜，道具从上次删除的位置开始搜
  int start = columnsNum == 7 ? 0 : *pRow * columnsNum + *pColumn + 1;
  for (int order = start; order < 63; order++)
  {
    int row = order / columnsNum;
    int column = order % columnsNum;
    int x = x0 + column * lootWidth;
    int y = y0 + row * lootHeight;
    if (IsTrash(task, x, y))
    {
      *pRow = row;
      *pColumn = column;
      return true;
    }
  }
  return false;
}
//寻找第一个可删道具
bool FindTrashProp(int task, int account, int *pRow, int *pColumn)
{
  return FindTrash(task, account, propX, propY, 9, pRow, pColumn);
}
//寻找第一个可删宝石
bool FindTrashJewel(int task, int account, int *pRow, int *pColumn)
{
  return FindTrash(task, account, jewelX, jewelY, 7, pRow, pColumn);
}
//解除二级密码（通过礼券购买物品）
void ReleasePassword(int task, int account)
{
  int list = GetList(task);

  const POINT set = { 210, 84 };//打开游戏设置
  const POINT closeSet = { 609, 140 };//关闭游戏设置
  //该任务是刷新后执行的，所以现在正在美味镇

  //1. 查看是否存在二级密码：打开设置，检查是否有“修改、删除密码”选项
  Click(task, account, set);//打开游戏设置
  WaitPicture(APPEAR, task, account, "游戏设置");
  int isSecondExist = TryWaitPicture(APPEAR, task, account, "有二级密码", 2000);//是否存在二级密码
  Click(task, account, closeSet);//关闭游戏设置
  CheckSleep(task, 500);

  //2. 如果有二级密码，去许愿池解除二级
  if (isSecondExist) //如果有二级密码，尝试删除第一格物品触发二级验证
  {
    const POINT mall = { 550, 555 };//商城
    const POINT propMall = { 574, 67 };//商城_道具
    const POINT giftWares = { 892, 515 };//礼券商品
    const POINT purchase = { 172, 290 };//购买
    const POINT confirmPurchase = { 426, 346 };//确认购买
    const POINT confirmPassword = { 438, 387 };//确认二级密码
    const POINT closeMall = { 919, 27 };//关闭商城

    Click(task, account, mall);//打开商城
    WaitPicture(APPEAR, task, account, "商城");
    CheckSleep(task, 500);
    Click(task, account, propMall);//点击道具
    WaitPicture(APPEAR, task, account, "商城_道具");
    CheckSleep(task, 500);
    Click(task, account, giftWares);//点击礼券商品
    WaitPicture(APPEAR, task, account, "礼券商品");
    CheckSleep(task, 500);
    Click(task, account, purchase);//点击购买
    WaitPicture(APPEAR, task, account, "系统提示_购买");
    CheckSleep(task, 500);
    Click(task, account, confirmPurchase);//确认购买
    WaitPicture(APPEAR, task, account, "二级密码确认");
    CheckSleep(task, 500);
    StringToWindow(plot[list].password[account], work[task].hWnd[account]);//输入二级密码
    CheckSleep(task, 1000);
    Click(task, account, confirmPassword);//确认二级密码
    WaitPicture(DISAPPEAR, task, account, "二级密码确认");//等待二级密码确认消失
    CheckSleep(task, 500);
    Click(task, account, closeMall);//关闭商城
    CheckSleep(task, 500);
  }
}
//删除无用道具
void DeleteProp(int task, int account)
{
  const POINT bag = { 592, 559 };//背包
  const POINT propBag = { 781, 68 };//道具背包
  const POINT arrange = { 902, 474 };//整理
  const POINT delete0 = { 845, 474 };//删除物品
  const POINT confirmDelete = { 425, 350 };//确认删除物品
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

  //2. 等待道具加载完成，30秒加载不好则刷新
  int counter = 0;
  while (!(UpdateMap(task, account) && isPropLoaded(task)))
  {
    counter++;
    if (counter > 30)
      RecordError(task, account, "道具加载失败", "道具加载失败");
    CheckSleep(task, 1000);
  }

  //3. 删除物品
  Click(task, account, delete0);//拿起删除按钮
  CheckSleep(task, 500);
  int row = -1, column = 8;//可删物品位置（预设为(0,0)前一个位置）
  //查找第一个可删物品，直到没有可删物品为止
  while (FindTrashProp(task, account, &row, &column))
  {
    Click(task, account, propX + (2 * column + 1) * propWidth / 2,
      propY + (2 * row + 1) * propHeight / 2);//点击可删物品
    WaitPicture(APPEAR, task, account, "删除物品提示");//等待删除提示出现
    Click(task, account, confirmDelete);//确认删除
    WaitPicture(DISAPPEAR, task, account, "删除物品提示");//等待删除提示消失
    CheckSleep(task, 500);//等待500ms
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
  int row = -1, column = 6;//可分解宝石位置（预设为(0,0)前一个位置）
  //查找第一个可分解宝石，直到没有可分解宝石为止
  while (FindTrashJewel(task, account, &row, &column))
  {
    Click(task, account, jewelX + (2 * column + 1) * jewelWidth / 2,
      jewelY + (2 * row + 1) * jewelHeight / 2);//点击可删宝石
    WaitPicture(DISAPPEAR, task, account, "宝石分解槽");//等待宝石放入分解槽
    Click(task, account, decompose);//点击分解
    WaitPicture(APPEAR, task, account, "宝石分解槽");//等待宝石消失
    CheckSleep(task, 500);//等待500ms
  }
  Click(task, account, closeWorkshop);//关闭背包
  CheckSleep(task, 100);
}
//执行特殊任务：签到施肥
int EnterSpecial(int task, int level)
{
  if (level == 0)//签到
  {
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
        Sign(task, account);
    FinishTask(task);
  }
  else if (level == 1)//浇水施肥
  {
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
      {
        EnterMission(task, account);//进入公会任务面板
        memset(missionExist, 0, sizeof(missionExist));//所有公会任务预设为不存在
        ReadMission(task, account, missionExist);//识别公会任务存入数组
        ClaimMissionAward(task, account, false);//领取奖励
        Click(task, account, CloseMission);//关闭公会任务界面
        CheckSleep(task, 500);

        //missionExist[0] = missionExist[1] = 1;

        if (missionExist[0] || missionExist[1])
          WaterAndManure(task, account);//进行浇水施肥
      }
    FinishTask(task);
  }
  else if (level == 2)//清理背包
  {
    LoadTrash();//更新可删物品
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
      {
        ReleasePassword(task, account);//去许愿池解除二级密码
        DeleteProp(task, account);//删除道具
        DecomposeJewel(task, account);//分解宝石
      }
    FinishTask(task);
  }
  return 1;
}
//进入公会任务识别或公会副本，进入成功返回1
int EnterGuild(int task, int level)
{
  if (level == 0) //如果是公会任务识别
  {
    bool isNewMissionFound[2] = {};
    memset(missionExist, 0, sizeof(missionExist));//所有公会任务预设为不存在
    for (int account = 0; account < 2; account++)
      if (param[task].deck[account])
      {
        EnterMission(task, account);//进入公会任务面板
        isNewMissionFound[account] = ReadMission(task, account, missionExist);//识别公会任务存入数组
        ClaimMissionAward(task, account, false);//领取奖励
        Click(task, account, CloseMission);
      }
    FinishTask(task);//直接宣告任务完成（完成后根据数组替换任务）
  }

  EnterGuildRoomList(task, work[task].host, level);//房主进入房间列表
  SelectiveSleep(task, 500);
  int IndoorDigit = CreatePasswordRoom(task, work[task].host);//房主创建密码房并获取房间号
  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    EnterGuildRoomList(task, 1 - work[task].host, level);//队友进入房间列表
    SelectiveSleep(task, 1000);
    if (EnterRoom(task, 1 - work[task].host, IndoorDigit) == 0)//队友进入密码房
      RecordError(task, 1 - work[task].host, "未找到房间号", "未找到房主创建的房间号");
  }
  return 1;
}
//双号进入悬赏房间，进入成功返回1
int EnterBounty(int task, int level)
{
  EnterBoutyRoomList(task, work[task].host, level);//房主进入房间列表
  SelectiveSleep(task, 500);
  int IndoorDigit = CreatePasswordRoom(task, work[task].host);//房主创建密码房并获取房间号
  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    EnterBoutyRoomList(task, 1 - work[task].host, level);//队友进入房间列表
    SelectiveSleep(task, 1000);
    if (EnterRoom(task, 1 - work[task].host, IndoorDigit) == 0)//队友进入密码房
      RecordError(task, 1 - work[task].host, "未找到房间号", "未找到房主创建的房间号");
  }
  return 1;
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
  WaitPicture(APPEAR, task, account, typeName[min(islandOrder, 6) + basicTypeNum]);//等待进入岛屿
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
      RecordError(task, account, "未进入指定界面", errorString);
    }
    if (counter % 10 == 1)
      Click(task, account, SwitchActivity);//点一下箭头
    CheckSleep(task, 100);
  }
}
//进入普通关卡房间列表，并切换到竞技一区
void EnterNormalRoomList(int task, int account, int IslandOrder, int Level)
{
  EnterIsland(task, account, IslandOrder);//跳转至指定岛屿
  SelectiveSleep(task, 500);
  DealInvitation(task, account, 0);
  Click(task, account, islandLevelButton[IslandOrder][Level]);
  WaitPicture(APPEAR, task, account, "房间列表");//等待打开房间列表
  RandomJump(task, account);
}
//进入普通关卡
void EnterNormal(int task, int IslandOrder, int Level)
{
  EnterNormalRoomList(task, work[task].host, IslandOrder, Level);//房主进入房间列表
  SelectiveSleep(task, 500);
  int IndoorDigit = CreatePasswordRoom(task, work[task].host);//房主创建密码房并获取房间号

  if (work[task].isInvolved[1 - work[task].host] == 1)//如果有队友
  {
    EnterNormalRoomList(task, 1 - work[task].host, IslandOrder, Level);//队友进入房间列表
    SelectiveSleep(task, 1000);
    if (EnterRoom(task, 1 - work[task].host, IndoorDigit) == 0)//队友进入密码房
      RecordError(task, 1 - work[task].host, "未找到房间号", "未找到房主创建的房间号");
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
  for (int account = 0; account <= 1; account++)
    if (work[task].isInvolved[account] == 1 && work[task].isQuitted[account] == 1)
    {
      work[task].isQuitted[account] = false;//账号恢复未退出状态（这样才能进行Click点击）
      isAnyAccountQuitted = true;
      GetGameState(task, account, NULL, &wave[account], NULL);
      counter = 0;
      while (wave[account] != waveOutOfLevel && wave[account] != waveGrade
        && wave[account] != waveFlip && wave[account] != waveTower)
      {
        counter++;
        if (counter > 50) //点退出按钮后5秒不能结束关卡，报错“退出关卡失败”
          RecordError(task, account, "退出关卡失败", "退出关卡失败");
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
      RecordError0(task, "翻牌超时", "翻牌30秒未结束");
    for (int account = 0; account <= 1; account++)
      if (work[task].isInvolved[account] == 1) //读取参与账号的波数，判断是否需要翻牌，是否已返回选卡界面
      {
        if (GetGameState(task, account, NULL, &wave[account], NULL) == 1) //如果识图成功
        {
          if (wave[account] != waveOutOfLevel && wave[account] != waveTower)//没有回到选卡/魔塔界面
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
    for (int account = 0; account <= 1; account++)
      if (isFilpRequired[account])
        ClickFree(task, account, flipNum);//点“免费”
    CheckSleep(task, 500);
    for (int account = 0; account <= 1; account++)
      if (isFilpRequired[account])
        Click(task, account, finish);//点“完成”
    CheckSleep(task, 500);
    work[task].currentTime++;
  }

  const char *indoor[] = { "选卡界面", "选卡界面2" };
  //如果回到的是waveOutOfLevel（选卡界面、岛屿主界面、勇士界面、跨服界面），且执行过手动退出，则所有在房间内的账号都要退出
  for (int account = 0; account <= 1; account++)
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
      //普通任务只有魔塔（含刷技能）能触发退出
      if (!work[task].isSkillMode) //非刷技能模式等100ms，以免太快了
        CheckSleep(task, 100);
      SelectiveSleep(task, 400);
      if (work[task].towerx[work[task].host]) //魔塔（普通任务）：啥也不做
        return;

      //到这里就只可能是高级任务了
      int list = GetList(task);
      if (plot[list].playingAdvance->type == 2) //魔塔（高级任务）：啥也不做
        return;
      if (plot[list].playingAdvance->type == 0) //悬赏：直接重走进房程序
        EnterBounty(task, plot[list].playingAdvance->level);
      else if (plot[list].playingAdvance->type == 1) //勇士：重走进房程序（勇士界面已打开不影响）
        EnterChallenge(task, plot[list].playingAdvance->level);
      //魔塔：啥也不需要做
      else if (plot[list].playingAdvance->type == 3) //跨服：重走进房程序（省去了进跨服界面这一步）
        EnterCross(task, plot[list].playingAdvance->level);
      else if (plot[list].playingAdvance->type == 4) //假期：直接重走进房程序
        EnterVacation(task);
      else if (plot[list].playingAdvance->type == 5)//公会副本：直接重走进房程序
        EnterGuild(task, plot[list].playingAdvance->level);
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
  POINT Quit = { 923, 580 };
  POINT ConfirmQuit = { 473, 383 };
  const char *quitButton[] = { "退出按钮", "退出按钮亮" };
  if (!work[task].isSkillMode && FindPictures(task, account, quitButton) == 0)
    return 0;//未发现退出按钮，本次退出失败
  Click(task, account, Quit);
  Click(task, account, ConfirmQuit);
  work[task].isQuitted[account] = true;//记录关卡已退出
  return 1;
}
//进入关卡
void EnterLevel(int task)
{
  int list = GetList(task);
  for (int account = 0; account < 2; account++)
    if (work[task].isInvolved[account])
      CloseHealthAndPayment(task, account);

  //打勇士/假期/双塔前先把两个号换到一个区
  if (plot[list].playingAdvance->type == 1 || plot[list].playingAdvance->type == 4
    || (plot[list].playingAdvance->type == 2 && plot[list].playingAdvance->level / 1000 == 2))
    for (int account = 0; account < 2; account++)
      if (work[task].isInvolved[account])
        RandomJump(task, account);

  CheckSleep(task, 100);
  if (plot[list].playingAdvance->type == 0)//悬赏
    EnterBounty(task, plot[list].playingAdvance->level);
  else if (plot[list].playingAdvance->type == 1)//勇士
    EnterChallenge(task, plot[list].playingAdvance->level);
  else if (plot[list].playingAdvance->type == 2)//魔塔
    EnterTower(task, work[task].host, plot[list].playingAdvance->level / 1000);//房主进入魔塔界面
  else if (plot[list].playingAdvance->type == 3)//跨服
    EnterCross(task, plot[list].playingAdvance->level);
  else if (plot[list].playingAdvance->type == 4)//假期
    EnterVacation(task);
  else if (plot[list].playingAdvance->type == 5)//公会副本
    EnterGuild(task, plot[list].playingAdvance->level);
  else if (plot[list].playingAdvance->type == 17)//签到施肥清包
    EnterSpecial(task, plot[list].playingAdvance->level);
  else
    EnterNormal(task, plot[list].playingAdvance->type - basicTypeNum, plot[list].playingAdvance->level);
  CheckSleep(task, 100);
}
//进入魔塔层数
void EnterTowerFloor(int task, int account, int rolesNum, int floor)
{
  int targetPage = (floor + 14) / 15;//目标层所在页数
  int towerThisPage = (floor - 1) % 15 + 1;//本层高度
  const POINT secret[5] = { { 60, 135 }, { 60, 230 }, { 60, 325 }, { 60, 420 }, { 60, 515 } };

  //第一局时检查是否处于魔塔选层界面，并翻到指定页数
  if (work[task].games == reserve[task].gamesFinished + 1)
  {
    int wave = 0;
    int counter = 0;//识图次数
    GetGameState(task, account, NULL, &wave, NULL);//第一次检查波数，第18波是魔塔界面
    while (wave != waveTower)
    {
      counter++;
      if (counter >= 3) //检查三次还没到就报错
        RecordError(task, account, "未进入选层界面", "请进入魔塔选层界面启动");
      CheckSleep(task, 50);
      GetGameState(task, account, NULL, &wave, NULL);
    }
    if ((rolesNum == 1 || rolesNum == 2) && floor > 0) //单塔或双塔
      TurnToPage(task, account, targetPage);//翻到目标页
  }

  //选择层数
  if ((rolesNum == 1 || rolesNum == 2) && floor > 0) //单塔或双塔
    Click(task, account, towerX, towerY - (towerThisPage - 1) * towerHeight);
  else if (rolesNum == 3)//密室
    Click(task, account, secret[floor]);
  if (!work[task].isSkillMode)
    CheckSleep(task, 100);
  SelectiveSleep(task, 100);

  //开始挑战
  Click(task, account, embarkX, embarkY);
  if (!work[task].isSkillMode)
    CheckSleep(task, 100);
  SelectiveSleep(task, 100);

  //等待进入房间
  int counter = 0;
  int wave = 0;
  GetGameState(task, account, NULL, &wave, NULL);
  while (wave != waveOutOfLevel)
  {
    counter++;
    if (counter >= 20)//如果超过10秒没进入房间，报错退出
      RecordError(task, account, "进入魔塔失败", "进入魔塔失败");
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
    int tower = work[task].towerx[work[task].host];
    level = tower == 0 ? 0 : 1000 + tower;
  }
  int rolesNum = level / 1000;//人数
  int floor = level % 1000;//层数

  if (rolesNum == 2) //双塔模式：队友先退房
  {
    const char *indoor[] = { "选卡界面", "选卡界面2" };
    if (FindPictures(task, 1 - work[task].host, indoor))
      Click(task, 1 - work[task].host, retreat);
  }

  EnterTowerFloor(task, work[task].host, rolesNum, floor);//房主进入层数

  if (rolesNum == 2) //双塔模式：邀请队友
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
        RecordError0(task, "房主可能已掉线", "房主未进入关卡，可能已掉线");
      else if (Entry == 2)//Entry==2说明双人已经进入了关卡
        break;
      else if (Entry == -2)//Entry==-2说明队友已准备
      {
        CheckSleep(task, 2000);//等2秒以免房主有延迟
        break;
      }
      else if (iColor >= MaxWaitTime * 10)//超时没准备上
        RecordError0(task, "队友无法准备", "队友无法准备");//弹窗报错
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
                  RecordError(task, 1 - work[task].host, "经验已达上限", "经验已达上限", END_TASK);
                else //如果设置经验满时继续
                {
                  Click(task, 1 - work[task].host, expNoMoreTip);//勾选不再提示
                  Click(task, 1 - work[task].host, bagConfirmX, bagConfirmY);//确认
                }
              }
              else if (Box == 2) //背包满弹窗
              {
                if (bagFullOperation == 0) //如果设置背包满时停止
                  RecordError(task, 1 - work[task].host, "背包满或未带卡",
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
      RecordError0(task, "进入关卡失败", "进入关卡失败");//弹窗报错

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
              RecordError(task, work[task].host, "经验已达上限", "经验已达上限", END_TASK);
            else //如果设置经验满时继续
            {
              Click(task, work[task].host, expNoMoreTip);//勾选不再提示
              Click(task, work[task].host, bagConfirmX, bagConfirmY);//确认
            }
          }
          else if (Box == 2) //背包满弹窗
          {
            if (bagFullOperation == 0) //如果设置背包满时停止
              RecordError(task, work[task].host, "背包满或未带卡",
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
  for (int account = 0; account <= 1; account++)
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
void ReadyAndStart(int task, int &Entry)
{
  int MaxWaitTime;
  if (work[task].games == reserve[task].gamesFinished + 1)//如果是第一局，只给10秒，方便查错
    MaxWaitTime = 10;
  else //后续都给30秒，以应对翻牌不同步
    MaxWaitTime = 30;

  /*准备程序*/
  if (work[task].isInvolved[1 - work[task].host] == 1) //如果队友参与，则执行准备程序
    Ready(task, MaxWaitTime, Entry);

  /*开始程序*/
  if (work[task].isInvolved[work[task].host] == 1) //如果房主参与，则执行开始程序
    Start(task, MaxWaitTime, Entry);

  /*等待加载完成进入关卡*/
  int counter = 0;
  while (Entry <= 0)
  {
    counter++;
    if (counter > 600) //最多允许加载60秒
      RecordError0(task, "加载超时", "加载超时");
    CheckSleep(task, 100);//每0.1秒识图
    GetGameStateFromAnyAccount(task, &Entry, NULL, NULL);//读取关卡进入状态
  }
  if (work[task].games == reserve[task].gamesFinished + 1)
    strcat_s(work[task].embarkString, "已进入关卡\n");

  /*检查人物数量*/
  if (GetRoleNum(task) == 2 && Entry == 1)//双人刷图只进了一个人，说明另一个号掉线了
    RecordError0(task, "队友可能已掉线", "队友未进入关卡，可能已掉线");
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
        RecordError(task, account, "卡组无法翻页", "卡组无法翻页");
      Click(task, account, button);
      CheckSleep(task, 100);
    }
    CheckSleep(task, 100);//截完图等100 ms，否则下次截图可能会黑
  }
}
//检查所有空卡槽。需要分出咖啡粉前后的卡槽。
void CheckEmptySlot(int task, int account, bool &isCoffeeExist, bool(&isBeforeCoffee)[maxSlotNum + 1],
  bool &isAnySlotBeforeCoffee, bool(&isAfterCoffee)[maxSlotNum + 1], bool &isAnySlotAfterCoffee)
{
  const COLORREF emptySlotColor = 0x1a4263;
  const POINT DeckSlot = { 395, 91 };
  const int DeckSlotWidth = 48;
  bool isDay = false;//是否白天关卡
  isCoffeeExist = false;//咖啡是否需要携带
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
      if (isDay && strcmp(work[task].originalName[account][order], "咖啡粉") == 0)
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
void TakeCustomGroup(int task, int account, bool(&isSlotRequired)[maxSlotNum + 1])
{
  char message[100] = {};
  int bestCode[maxSlotNum + 1] = {};//每个卡槽匹配到的最佳候选卡的编号
  int bestStar[maxSlotNum + 1] = {};//每个卡槽匹配到的最佳候选卡的星级
  //第一轮扫描：检查每个空卡槽所能匹配的最高优先级，记录其编号
  ScanCustom(task, account, isSlotRequired, bestCode, bestStar);

  if (customOption == 1) //如果带卡选择为“位置”，则最优星级设为-1（表示不选择星级）
    for (int order = 0; order <= maxSlotNum; order++)
      bestStar[order] = -1;

  //如果某张空卡槽最优编号为-1，宣告order号卡不存在
  for (int order = 1; order <= work[task].originalSlotNum[account]; order++)
    if (isSlotRequired[order] && bestCode[order] == -1)
    {
      if (strcmp(work[task].originalName[account][order], "咖啡粉") == 0)
        strcpy_s(message, "未找到[木塞子]（替咖啡粉）");
      else
        sprintf_s(message, "未找到[%s]卡片", work[task].originalName[account][order]);
      RecordError(task, account, "找卡失败1", message);
    }

  //依次查找并携带每个空卡槽的最高优先级卡片
  for (int order = 1; order <= work[task].originalSlotNum[account]; order++)
    if (isSlotRequired[order] && !TakeCustom(task, account, bestCode[order], bestStar[order]))
    {
      sprintf_s(message, "未找到[%s]卡片", work[task].originalName[account][order]);
      RecordError(task, account, "找卡失败2", message);
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
  for (int code = 0; code < reserve[task].customNum; code++)
    if (strcmp(reserve[task].custom[code].name, "木塞子") == 0)
    {
      TakeCustom(task, account, code, -1);
      return;
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
    if (IsTower(task))
      isDeckRequired = true;
    //某账号设置了退出时间需要带卡
    for (int account = 0; account < 2; account++)
      if (work[task].isPerformed[account] && work[task].quitTime[account] > 0)
        isDeckRequired = true;
  }
  if (!isDeckRequired)
    return;

  const POINT DeckSlot = { 411, 73 };
  const int DeckSlotWidth = 48;
  for (int account = 0; account <= 1; account++)//选择卡组
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

      //如果设置了自定卡，查找并选择自定卡（条件：账号参与放卡且可见）
      if (IsGameWindowVisible(work[task].hWnd[account]))
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
            TakeCustomGroup(task, account, isBeforeCoffee);
          //如果有咖啡粉，携带咖啡粉
          if (isCoffeeExist)
            TakeCustom(task, account, 1, -1);
          //如果有咖啡粉后的卡槽，检查并携带
          if (isAnySlotAfterCoffee)
            TakeCustomGroup(task, account, isAfterCoffee);
        }
        else //不放卡账号：如果卡组全空，带木塞子
          TakePlugIfEmpty(task, account);
      }

      int isBanEnabled = 0;//这个账号是否需要ban卡
      for (int order = 1; order <= maxSlotNum; order++)
      {
        if (isBanEnabled = work[task].isSlotBanned[account][order])
          break;
      }
      if (isBanEnabled == 1)
      {
        for (int order = 1; order <= maxSlotNum; order++) //根据ban卡列表取消卡组
        {
          if (order == 1) //翻到1-11槽
            goToSlotEnd(task, account, 0, true);
          else if (order == 12)//翻到12-21槽
            goToSlotEnd(task, account, 1, true);
          if (work[task].isSlotBanned[account][order]) //卡片被ban
          {
            if (order <= 11)
              Click(task, account, DeckSlot.x + (order - 1) * DeckSlotWidth, DeckSlot.y);//点掉卡槽
            else
              Click(task, account, DeckSlot.x + (order - 11) * DeckSlotWidth, DeckSlot.y);//点掉卡槽
            CheckSleep(task, 100);
          }
        }
      }
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
  FILE *f;
  fopen_s(&f, "附加程序\\文本\\龙图.txt", "rb");
  for (int i = 0; i < 1 << 21; i++)
    dragonTable[i] = fgetc(f);
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
//【任务线程】开始任务task
DWORD __stdcall StartTask(void *VPtask)
{
  int task = (int)VPtask;//获取任务序号
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
    if (plot[list].playingAdvance->type == 17 && plot[list].playingAdvance->level == 2) //清包任务强制刷新
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
    if (plot[list].playingAdvance->type == 17 && plot[list].playingAdvance->level == 2) //清包任务的下一轮也强制刷新
      plot[list].isRefreshRequired = 1;
    AdvanceBindWindow(task);//首次执行时绑定param.hWnd。绑定失败将刷新游戏并获取param.hWnd。保证在可跳转界面。
  }
  LoadTrackAndBindWindow(task);//读取轨道、绑定Work.hWnd和DPI修正（此后才能用Click进行点击）、切换到放卡账号

  //启动前检验一下放卡账号有没有显示
  if (!isPerformedAccountVisible(task))
  {
    PopMessage(task, "所有放卡账号都需要显示。\n显示后任务将继续进行。");
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
  for (work[task].games = reserve[task].gamesFinished + 1; work[task].games < MAXINT; work[task].games++)
  {
    if (IsAdvance(task)) //更新已完成局数
      plot[GetList(task)].playingAdvance->gamesFinished = work[task].games - 1;
    ResetData(task);//重置读第0波轨道，重置种植次数、索敌卡位置
    ResetWave(task, waveOutOfLevel);//每局开始前，波数为“未进入关卡”
    if (work[task].games > work[task].maxGames)//刷完指定局数，结束任务
      FinishTask(task);
    RecordLevelTime(task, false);//记录每局用时
    ReturnState(task, "运行中");//进度games/MaxGames，第waveOutOfLevel波第0秒，平均用时刷新，累计时间刷新

    if (IsTower(task)) //魔塔选层
      EnterTowerLevel(task);

    int wave = -1;
    bool isEnterRequired = true;//是否需要进入关卡
    for (int account = 0; account < 2; account++)
      if (work[task].isInvolved[account])
        if (GetGameState(task, account, &entry, &wave, &box))//如果成功读取状态
        {
          const char *indoor[] = { "选卡界面", "选卡界面2" };
          if (entry > 0 || box == 1)//已进入关卡（包括继续作战弹窗时）：跳过选卡、准备、开始阶段
            isEnterRequired = false; //不需要走带卡、准备开始程序
          //处于选卡界面
          else if (wave == waveOutOfLevel && TryWaitPictures(APPEAR, task, account, indoor, 1000))
          {
            isEnterRequired = true; //需要走带卡、准备开始程序
            work[task].roamType = GetRoamType(task);//记录漫游类型
          }
          else //其他情况报错
            RecordError(task, account, "未进入房间", "未进入房间");
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
      ChooseDeck(task);//选择卡组
      ReadyAndStart(task, entry);//准备和开始（第2局开始，另一个号可能还没翻牌完）
      work[task].isGridCatched = 0;
    }
    else //中途进入关卡，沿用初始地图和强制棉麦格子
      work[task].isGridCatched = 1;

    work[task].totalWaveTick[0] = GetTickCount();//记录进入关卡时刻（第0波时刻）
    for (int account = 0; account <= 1; account++)//两个账号的计时起点均设为进入关卡时刻
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
        SlotType &slot = work[task].slot[host][order];
        if (slot.level == 4)//如果是升级层，进行记录，稍后放置
          isUpgradeExist = true;
        else if (slot.row[1] > 0) //如果有安排位置
          PlantToGrid(task, host, order, slot.row[1], slot.column[1]);//立即放置
      }

      if (isUpgradeExist) //如果有升级卡
      {
        CheckSleep(task, 200);//等0.3秒
        SelectiveSleep(task, 200);
        for (int order = 1; order <= skillSlotNum; order++)
        {
          SlotType &slot = work[task].slot[host][order];
          if (slot.level == 4 && slot.row[1] > 0)
            PlantToGrid(task, host, order, slot.row[1], slot.column[1]);//立即放置
        }
      }
      QuitLevel(task, host);//放完退出
      ReturnStart(task);//回到开始界面
      continue;//本局结束
    }

    int firstPerformedAccount = GetFirstPerformedAccount(task);//第1个放卡账号
    //普通模式等1秒进关放人物
    for (int account = 0; account <= 1; account++)//人物挪到1行1列
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
    for (int account = 0; account <= 1; account++)//放置参与的账号的人物
      if (work[task].isInvolved[account] == 1)
        LayRole(task, account);

    //如果是从房间进入关卡，对第1个放卡账号进行加速，加速时间为两号设置的加速时间的最大值
    if (isEnterRequired)
    {
      int accelerationTime = 0;
      for (int account = 0; account < 2; account++)
        if (work[task].isPerformed[account])
          accelerationTime = max(accelerationTime, work[task].accelerationTime[account]);
      if (accelerationTime)
      {
        Accelerate(task, firstPerformedAccount);//开启加速
        CheckSleep(task, accelerationTime);
        Accelerate(task, firstPerformedAccount);//关闭加速
      }
    }

    CheckSleep(task, 1000 - (GetTickCount() - work[task].totalWaveTick[0]));//等足1秒
    work[task].currentTime = 1;
    ResetWave(task, 0);

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
                RecordError(task, account, "房主已掉线", "房主已掉线");
              else
                RecordError(task, account, "队友已掉线", "队友已掉线");
            }
          }
        if (isProceedPopped) //如果出现继续作战框，点击继续作战或领取奖励
          for (int account = 0; account <= 1; account++)
          {
            if (work[task].isProceed[account] == 1) //继续作战
              Click(task, account, proceedX, proceedY);
            else if (work[task].currentTime >= 92)//领取奖励（需要时间达到1分30秒）
              Click(task, account, claimX, claimY);
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
              for (int account = 0; account < 2; account++)
                if (work[task].isInvolved[account])
                  Accelerate(task, account);

            //等待回到翻牌界面或关卡外
            int counter = 0;
            const int secondCount = 5;//每秒计数
            while (!(work[task].wave == waveFlip || work[task].wave == waveOutOfLevel || work[task].wave == waveTower))
            {
              ReturnState(task, "运行中");//进度games/MaxGames，第Wave波第CurrentTime秒，平均用时刷新，累计时间刷新
              counter++;
              if (counter > 20 * secondCount)//20秒未完成结算算超时
                RecordError0(task, "结算超时", "结算超时");
              CheckSleep(task, 1000 / secondCount);
              if (counter % secondCount == 0)
                work[task].currentTime++;
              ReadLevelInfo(task);
            }

            if (isAccelerationRequired) //关闭加速
              for (int account = 0; account < 2; account++)
                if (work[task].isInvolved[account])
                  Accelerate(task, account);
          }

          //选卡或魔塔选层界面：进入ReturnStart函数
          if (work[task].wave == waveOutOfLevel || work[task].wave == waveTower)
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
            RecordError0(task, "两号结束不同步", "两号结束时间相差超过30秒");
        }
      }
      else//如果不是识图模式
      {
        //每一秒都要点击继续作战或领取奖励
        for (int account = 0; account <= 1; account++)
        {
          if (work[task].isProceed[account] == 1)
            Click(task, account, proceedX, proceedY);
          else
            Click(task, account, claimX, claimY);
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
          else if (work[task].isCollect[account] == 1 && work[task].currentTime % 4 == zero)
            ColumnsCollected[account] = 9; //收集物品：每4秒执行一次，收集1-9列
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
          RecordError0(task, "跨服超时", message);
        }
      }
      else
      {
        if (work[task].currentTime >= 1800) //计时超过30分钟
        {
          sprintf_s(message, "关卡30分钟未结束（第%d.%d波）", work[task].wave, work[task].smallWave);
          RecordError0(task, "关卡超时", message);
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
  FILE *f;
  fopen_s(&f, "用户参数\\执行器参数.txt", "w");//打开文件

  //写入公共参数
  fprintf(f, "缩放比例=%d\n", zoom);
  fprintf(f, "放卡延迟=%d\n", plantDelay);
  fprintf(f, "带卡选择=%d\n", customOption);
  fprintf(f, "翻牌数量=%d\n", flipNum);
  fprintf(f, "背包满时=%d\n", bagFullOperation);
  fprintf(f, "经验满时=%d\n", expFullOperation);
  fprintf(f, "补阵强度=%d\n", repairLevel);
  char timerString[10] = "关";
  if (timerMode == 1)
    sprintf_s(timerString, "%d", timer);
  else if (timerMode == 2)
    sprintf_s(timerString, "~%d", timer);
  fprintf(f, "定时启动=%s\n", timerString);
  fprintf(f, "自动刷新=%d\n", maxRefreshTimes);
  fprintf(f, "操作速度=%d\n", operationSpeed);

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
    fprintf(f, "房主轨道=%s\n", param[task].track[0]);
    fprintf(f, "队友轨道=%s\n", param[task].track[1]);
    fprintf(f, "执行局数=%d\n", param[task].maxGames);
    fprintf(f, "每局用时=%d\n", param[task].gameTime);
    fprintf(f, "继续作战=%d,%d\n", param[task].isProceed[0], param[task].isProceed[1]);
    fprintf(f, "自动收集=%d,%d\n", param[task].isCollect[0], param[task].isCollect[1]);
    fprintf(f, "使用卡组=%d,%d\n", param[task].deck[0], param[task].deck[1]);
    fprintf(f, "房主标签=%d,%d\n", param[task].tag[0].x, param[task].tag[0].y);
    fprintf(f, "队友标签=%d,%d\n", param[task].tag[1].x, param[task].tag[1].y);
    fprintf(f, "大厅句柄=%d,%d\n", (int)param[task].hWndHall[0], (int)param[task].hWndHall[1]);
    fprintf(f, "初始大厅=%d\n", (int)param[task].hMainHall);
  }
  fclose(f);
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
  LoadAdvanceFrom(list, path, 0);
  SaveAdvanceTo(list, plot[list].advancePath, 0);
  int TotalAdvancePage = plot[list].advanceNum / advanceNumPerPage + 1;
  if (plot[list].advancePage > TotalAdvancePage)
    plot[list].advancePage = TotalAdvancePage;
}
//保存至某一个备份
void SaveBackup(int list, int order)
{
  char path[50];
  sprintf_s(path, "自动备份\\任务列表\\%s\\%d.txt", plot[list].backupFolder, order);
  SaveAdvanceTo(list, path, ADVANCE);
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
void GetParamFromAdvance(int task, AdvanceType &advance)
{
  if (advance.host < 2) //单线程任务
  {
    //同步任务2P无需使用，把maxGames设为0阻止任务启动即可
    if (task % 2 == 0)
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
    if (task % 2 == 1) //同步任务1P：1P填写advance的1P轨道和卡组，2P留空
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
  param[task].maxGames = IsNoTrack(advance) ? 1 : advance.maxGames;
  param[task].gameTime = 0;
}
//检查某条任务advance是否合格，不合格返回0写入checkInfo。
//1P参与返回1，2P参与返回2，都参与返回3（返回值用于确定高级任务参与人数）
int CheckQuest(int list, AdvanceType &advance, char(&checkInfo)[1000], bool *pIsSkillMode = nullptr)
{
  int task = GetTask(list);
  if (pIsSkillMode)
    *pIsSkillMode = false;
  ResetWork(task);//重置work[task]
  ResetWork(task + 1);//重置work[task + 1]（用于同步任务）

  char info[2][1000] = {};//检查轨道文件时输出的错误信息
  int loadResult[2] = {};//轨道检查结果
  int trackNum = 0;//有效轨道数量
  int isInvolved[2] = {};//两个账号是否参与
  int isPerformed[2] = {};//两个账号是否放卡

  //两类任务共同机制：检查轨道，出错则提示
  for (int account = 0; account < 2; account++)
    //若轨道存在且卡组不为0
    if (strlen(advance.track[account]) > 0 && advance.deck[account] > 0)
    {
      //载入并检查轨道
      LoadTrack(task, account, advance.track[account], info[account], &loadResult[account], true);
      isInvolved[account] = isPerformed[account] = 1;//该账号参与且放卡
      trackNum++;
    }
    else //轨道不存在，检查结果记为-1
      loadResult[account] = -1;

  if (trackNum == 0)//两个轨道都没填
    strcpy_s(checkInfo, "请至少填写一个轨道");
  else if (loadResult[0] == 0)//如果轨道1无法正常载入，弹窗提示
    strcpy_s(checkInfo, info[0]);
  else if (loadResult[1] == 0)//如果轨道2无法正常载入，弹窗提示
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
        strcpy_s(checkInfo, "同步任务不能用于双人魔塔。");
      else if (IsMission(advance)) //公会任务
        strcpy_s(checkInfo, "同步任务不能用于公会任务。");
      else if (work[task].isSkillMode && !IsSingleTower(task)) //刷技能轨道不在单塔执行
        strcpy_s(checkInfo, "刷技能轨道只能用于单塔或密室。");
      else
        return 3;//检验通过，为双人参与
    }
    else //非同步任务
    {
      //如果某个账号卡组为0，删除该账号人物（即便另一个账号轨道为双人）
      for (int account = 0; account < 2; account++)
        if (advance.deck[account] == 0)
          work[task].roleLocNum[account] = 0;

      //若为双人刷图，则记录两个号都参与。至此参与账号填写完毕。
      if (GetRoleNum(task) == 2)
        isInvolved[0] = isInvolved[1] = 1;

      //房主不参与而队友参与是不允许的
      if (isInvolved[advance.host] == 0 && isInvolved[1 - advance.host] == 1)
      {
        if (advance.deck[advance.host] == 0) //如果是把房主卡组设为了×
          strcpy_s(checkInfo, "房主必须参与任务");
        else //如果没有设×，而是轨道为单人
          strcpy_s(checkInfo, "队友使用了单人轨道，房主未参与。");
      }
      else if (advance.type == 2 && advance.level / 1000 == 1 && isInvolved[1 - advance.host])
        strcpy_s(checkInfo, "单塔必须单人参与。");
      else if (advance.type == 2 && advance.level / 1000 == 2 && !isInvolved[1 - advance.host])
        strcpy_s(checkInfo, "双塔必须双人参与。");
      else if (advance.type == 2 && advance.level / 1000 == 3 && isInvolved[1 - advance.host])
        strcpy_s(checkInfo, "密室必须单人参与。");
      else if (work[task].isSkillMode && !IsSingleTower(task)) //刷技能轨道不在单塔执行
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
//检查当前列表或公会任务列表，合格返回-1（有刷技能模式返回-2），不合格返回出错的order并填写错误信息
int CheckList0(int list, char(&checkInfo)[1000], int(&involved)[2], int &missionOrder, int target = 0)
{
  bool isSkillExist = false;
  AdvanceType *advance = target == ADVANCE ? plot[list].advance : plot[list].missionAdvance;
  int &num = target == ADVANCE ? plot[list].advanceNum : plot[list].missionAdvanceNum;

  missionOrder = -1;
  involved[0] = involved[1] = 0;
  int checkResult[maxAdvanceNum] = {};
  for (int order = 0; order < num; order++)
    if (advance[order].selected == 1)//遍历所有勾选的高级任务
    {
      if (IsMission(advance[order])) //如果是公会任务识别
      {
        if (target != 0) //遇到公会列表中含有公会任务
        {
          strcpy_s(checkInfo, "公会任务列表中不允许嵌套公会任务。");
          return order;
        }
        if (missionOrder >= 0) //如果公会任务已经出线过
        {
          strcpy_s(checkInfo, "公会任务只允许出现一次。");
          return order;
        }
        if (advance[order].host == 2) //如果房主设置为“同步”
        {
          strcpy_s(checkInfo, "同步任务不能用于公会任务。");
          return order;
        }
        if (advance[order].deck[advance[order].host] == 0) //如果把房主卡组设为了×
        {
          strcpy_s(checkInfo, "房主必须参与任务。");
          return order;
        }
        for (int account = 0; account < 2; account++) //填写卡组的号参与
          if (advance[order].deck[account])
            involved[account] = 1;

        missionOrder = order;
        continue;
      }
      else if (IsSpecial(advance[order]))//签到施肥
      {
        for (int account = 0; account < 2; account++) //填写卡组的号参与
          if (advance[order].deck[account])
            involved[account] = 1;
        continue;
      }
      plot[list].playingOrder = order;//更新plot[list].playingOrder，检查承载方案时需要使用
      plot[list].playingAdvance = &advance[order];//更新PlayingAdvance

      bool isSkillMode = false;
      checkResult[order] = CheckQuest(list, advance[order], checkInfo, &isSkillMode);//检查任务
      if (checkResult[order] == 0)//不合格则返回出错的order
        return order;
      if (checkResult[order] == 1 || checkResult[order] == 3)
        involved[0] = 1;
      if (checkResult[order] == 2 || checkResult[order] == 3)
        involved[1] = 1;
      if (isSkillMode)
        isSkillExist = true;
    }
  if (isSkillExist)
    return -2;
  return -1;
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

  char levelString[100] = {};//关卡名
  char message[100] = {};//输出信息
  int refreshLevel = 0;//刷新等级
  char errorString[100] = {};//刷新原因

  while (true)//循环执行任务，如果意外中断则继续循环，正常中断/完成则跳出循环
  {
    //检查param，若能启动则传递参数到work，不能则报错退出（实际上不会触发）
    for (int iTask = task; iTask <= task + 1; iTask++)
      if (CheckTask(iTask, plot[list].advance[order].host == 2) == 0)
        StopAdvance(list, "任务检查不通过。");

    //两个任务均已刷满则结束
    if (work[task].games > work[task].maxGames && work[task + 1].games > work[task + 1].maxGames)
      return 1;
    //否则视为任务正在进行

    //新增输出信息：进行中
    if (IsMission(plot[list].advance[order]))
      strcpy_s(message, "公会任务识别 进行中...");
    else
    {
      GetLevelName(levelString, plot[list].advance[order].type, plot[list].advance[order].level);
      sprintf_s(message, "[%d] %s(%s) 进行中...", order + 1, typeName[plot[list].advance[order].type],
        levelString);
    }
    AddOutput(list, message);

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
    if (IsMission(plot[list].advance[order]))
      strcpy_s(plot[list].output[plot[list].outputNum - 1], "公会任务识别 未完成");
    else
      sprintf_s(plot[list].output[plot[list].outputNum - 1], "[%d] %s(%s) 仅完成 %d/%d", order + 1,
        typeName[plot[list].advance[order].type], levelString, work[task].games - 1, work[task].maxGames);
    //新增输出信息：刷新原因
    Truncate(message, errorString, titleWidth + parameterWidth - 12);
    AddOutput(list, message);
    GetLogFinished(list);//记录已完成的任务日志

    //跳过模式（2）或刷新次数已满：刷新后做下一条任务
    if (refreshLevel == SKIP_TASK || plot[list].timesRefreshed[order] >= maxRefreshTimes)
      break;

    //否则继续完成本条任务剩余局数，记录已刷新次数+1
    for (int iTask = task; iTask <= task + 1; iTask++)
      reserve[iTask].gamesFinished = work[iTask].games - 1;//已完成的局数
    plot[list].timesRefreshed[order]++;//已刷新次数+1
  }

  //结束本条任务的几种情况：正常完成/手动停止/END/SKIP
  if (strcmp(state[task].tip, "已完成") == 0) //1. 正常完成（局数打满且不触发中断）
  {
    if (plot[list].advance[order].result == 2) //未完成：仅限签到施肥
      sprintf_s(plot[list].output[plot[list].outputNum - 1], "[%d] %s(%s) 仅完成 0/1",
        order + 1, typeName[plot[list].advance[order].type], levelString);
    else if (plot[list].advance[order].result == 1) //未领奖：仅限悬赏
      sprintf_s(plot[list].output[plot[list].outputNum - 1], "[%d] %s(%s) 未领奖 %d/%d",
        order + 1, typeName[plot[list].advance[order].type], levelString,
        work[task].maxGames, work[task].maxGames);
    else //已完成
    {
      if (IsMission(plot[list].advance[order]))
        strcpy_s(plot[list].output[plot[list].outputNum - 1], "公会任务识别 已完成");
      else
        sprintf_s(plot[list].output[plot[list].outputNum - 1], "[%d] %s(%s) 已完成 %d/%d",
          order + 1, typeName[plot[list].advance[order].type], levelString,
          work[task].maxGames, work[task].maxGames);//修改输出信息：已完成
    }
  }
  else if (plot[list].isAdvanceStarted == 0) //2. 手动停止
  {
    //修改输出信息：已停止
    if (IsMission(plot[list].advance[order]))
      strcpy_s(plot[list].output[plot[list].outputNum - 1], "公会任务识别 已停止");
    else
      sprintf_s(plot[list].output[plot[list].outputNum - 1], "[%d] %s(%s) 已停止 %d/%d",
        order + 1, typeName[plot[list].advance[order].type], levelString,
        min(work[task].games, work[task].maxGames), work[task].maxGames);
  }
  else if (refreshLevel == END_TASK) //3. END_TASK
  {
    //修改输出信息：已中断
    if (IsMission(plot[list].advance[order]))
      strcpy_s(plot[list].output[plot[list].outputNum - 1], "公会任务识别 已中断");
    else
      sprintf_s(plot[list].output[plot[list].outputNum - 1], "[%d] %s(%s) 已中断 %d/%d",
        order + 1, typeName[plot[list].advance[order].type], levelString,
        min(work[task].games, work[task].maxGames), work[task].maxGames);
    plot[list].isAdvanceStarted = 0; //主动刹停高级任务
  }
  //4. SKIP_TASK（上面已经改过输出信息了）

  GetLogFinished(list);//记录已完成的任务日志
  isRepaintRequired = 1;//发送重绘指令
  return 1;//如果没有刹停高级任务，则继续执行下一条任务
}
//检查当前列表或公会任务列表，合格返回-1，有刷技能模式返回-2，不合格报错停止
int CheckList(int list, char(&checkInfo)[1000], int(&involved)[2], int &missionOrder,
  int target = ADVANCE)
{
  AdvanceType *adv = target == ADVANCE ? plot[list].advance : plot[list].missionAdvance;

  char message[1000];
  //检查列表全部任务，返回出错编号
  int errorOrder = CheckList0(list, checkInfo, involved, missionOrder, target);
  if (errorOrder >= 0)//如果检查出错
  {
    char levelString[100];
    GetLevelName(levelString, adv[errorOrder].type, adv[errorOrder].level);
    if (target == 0)
      sprintf_s(message, "[%d] %s(%s)\n%s", errorOrder + 1, typeName[adv[errorOrder].type],
        levelString, checkInfo);//填写提示信息
    else
      sprintf_s(message, "公会任务列表 [%d] %s(%s)\n%s", errorOrder + 1, typeName[adv[errorOrder].type],
        levelString, checkInfo);//填写提示信息
    StopAdvance(list, message);
  }
  return errorOrder;
}
//高级任务结束，报告未完成的任务
void FinishAdvance(int list)
{
  char failureTip[1000] = "高级任务结束，以下任务未完成：";
  const char resultString[3][100] = { "已完成", "未领奖", "未完成" };
  char advanceString[100] = {};
  char levelName[100] = {};
  bool isAnyAdvanceFailed = false;//是否有未完成的任务
  //逐条检查高级任务
  for (int order = 0; order < plot[list].advanceNum; order++)
    if (plot[list].advance[order].selected)
    {
      GetLevelName(levelName, plot[list].advance[order].type, plot[list].advance[order].level);
      //非轨道任务，根据result提示（目前只有result==2 仅完成0/1 这一种情况）
      if (IsNoTrack(plot[list].advance[order]))
      {
        if (plot[list].advance[order].result == 2)
        {
          sprintf_s(advanceString, "\n[%d] %s(%s) 仅完成0/1", order + 1,
            typeName[plot[list].advance[order].type], levelName);
          strcat_s(failureTip, advanceString);
          isAnyAdvanceFailed = true;
        }
      }
      //轨道任务，如果局数没打满，提示仅完成；否则只可能result==1，提示未领奖
      else
      {
        if (plot[list].advance[order].gamesFinished < plot[list].advance[order].maxGames)
        {
          sprintf_s(advanceString, "\n[%d] %s(%s) 仅完成%d/%d", order + 1,
            typeName[plot[list].advance[order].type], levelName, plot[list].advance[order].gamesFinished,
            plot[list].advance[order].maxGames);
          strcat_s(failureTip, advanceString);
          isAnyAdvanceFailed = true;
        }
        else if (plot[list].advance[order].result == 1)
        {
          sprintf_s(advanceString, "\n[%d] %s(%s) 未领奖", order + 1,
            typeName[plot[list].advance[order].type], levelName);
          strcat_s(failureTip, advanceString);
          isAnyAdvanceFailed = true;
        }
      }
    }
  if (plot[list].isMissionClaimFailed)
  {
    strcat_s(failureTip, "\n公会任务 未领奖");
    isAnyAdvanceFailed = true;
  }
  if (isAnyAdvanceFailed)
    StopAdvance(list, failureTip);
  else
    StopAdvance(list, "高级任务已全部完成。");
}
//【任务线程】开始高级任务序列
DWORD __stdcall StartList(void *vpList)
{
  int list = (int)vpList;
  int task = GetTask(list);
  plot[list].isMissionClaimFailed = false;//领奖失败清零
  plot[list].isLevelInsufficient = false;//等级不足预设为false
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    plot[list].advance[order].result = 0;
    plot[list].advance[order].gamesFinished = 0;
  }
  memset(plot[list].timesRefreshed, 0, sizeof(plot[list].timesRefreshed));//已刷新次数清零
  plot[list].outputNum = 0;//清空输出信息
  plot[list].isRefreshRequired = 0;
  plot[list].isAdvanceWindowBound = 0;//Param[5].hWnd是否被绑定
  memset(plot[list].logFinished, 0, sizeof(plot[list].logFinished));//已完成任务日志清空
  plot[list].playedAdvanceNum = 0;//已执行的任务数量设为0
  ShuffleRandomZone(list);//生成随机换区顺序

  int missionOrder = -1;//公会任务编号
  char CheckInfo[1000];
  char message[1000];

  //检查高级任务列表
  bool isSkillExist = CheckList(list, CheckInfo, plot[list].advanceInvolved,
    missionOrder, ADVANCE) == -2;
  int missionHost = 0;//公会任务房主
  int missionDeck[2] = {};//公会任务卡组
  if (missionOrder >= 0) //如果存在公会任务
  {
    if (plot[list].advanceNum > maxAdvanceNum - 10)
    {
      sprintf_s(message, "识别公会任务前，列表中不能超过%d条任务。", maxAdvanceNum - 10);
      StopAdvance(list, message);
    }
    missionHost = plot[list].advance[missionOrder].host;
    missionDeck[0] = plot[list].advance[missionOrder].deck[0];//记录公会任务卡组
    missionDeck[1] = plot[list].advance[missionOrder].deck[1];
    int newAdvanceInvolved[2] = { 0, 0 };
    int newMissionOrder = -1;
    //载入公会任务列表，如果载入失败则中断任务
    if (LoadAdvanceFrom(list, "预制列表\\公会任务.txt", 1) == 0)
      StopAdvance(list, "载入公会任务列表失败。");
    //检查公会任务列表
    CheckList(list, CheckInfo, newAdvanceInvolved, newMissionOrder, MISSION);
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
    && param[task].hWndHall[0] == param[task].hWndHall[1])
    StopAdvance(list, "请分窗口显示两个账号。");

  //检验通过，填写param[task + 1]的标签
  for (int account = 0; account < 2; account++)
  {
    param[task + 1].tag[account] = param[task].tag[account];
    param[task + 1].hWnd[account] = param[task].hWnd[account];
    param[task + 1].hWndHall[account] = param[task].hWndHall[account];
  }
  param[task + 1].hMainHall = param[task].hMainHall;

  if (strstr(plot[list].advancePath, "美食大赛"))
  {
    contestOption = 1;
    bagFullOperation = 1;
  }
  else
    contestOption = 0;

  if (isSkillExist)
  {
    char tip[300] = "刷技能要求:\n"
      "1. 所有卡片至少1星；\n"
      "2. 操作速度建议调至“快”。\n"
      "点击确认继续执行。";
    int answer = PopMessageOK(task, tip);
    if (answer != IDOK)
      StopAdvance(list);
  }

  if (contestOption == 1)
  {
    int answer = MessageBoxA(hWndActuator,
      "大赛模式遇到任务未完成将立即终止。\n请确保钥匙、徽章及背包空间充足。\n点击确认继续执行。",
      "提示", MB_OKCANCEL | MB_ICONINFORMATION | MB_SYSTEMMODAL);
    if (answer != IDOK) //如果未确认执行大赛
      StopAdvance(list);
  }

  //定时程序
  if (timerMode == 1 || timerMode == 2) //如果开启了定时，则进行等待，并刷新倒计时
  {
    int timerSecond = 0;//需要等待的秒数
    if (timerMode == 1) //倒计时模式：等待时间就是timer
      timerSecond = timer;
    else if (timerMode == 2) //到时模式：等待时间=timer-当前时间
    {
      time_t stdTime = time(NULL);//获取当前系统时间（世界时）
      tm localTime;
      localtime_s(&localTime, &stdTime);//转化为本地时间
      int nowSecond = localTime.tm_hour * 3600 + localTime.tm_min * 60 + localTime.tm_sec;//当前时间秒数
      if (timer < nowSecond)  //如果定时时刻小于当前时刻，定时时刻+24小时
        timerSecond = timer + 86400 - nowSecond;
      else
        timerSecond = timer - nowSecond;
    }
    int countdown = timerSecond;//显示的倒计时
    char timerString[9];
    if (countdown > 0)//若定时时间大于0，新增定时消息
    {
      SecondToHms(timerString, countdown);
      sprintf_s(message, "定时中，剩余时间%s", timerString);
      AddOutput(list, message);
    }
    else
      AddOutput(list, "定时完成");
    DWORD TimerEndTick = GetTickCount() + timerSecond * 1000;//计算倒计时结束时刻
    while (timerSecond > 0) //当需要等待的时间大于0时
    {
      Sleep(10); //一个循环等待10 ms
      if (plot[list].isAdvanceStarted == 0)//如果高级任务被刹停，结束高级任务，定时取消
      {
        strcpy_s(plot[list].output[plot[list].outputNum - 1], "定时取消");//更新定时消息：定时取消
        plot[list].isAdvanceStarted_Confirm = 0;
        isRepaintRequired = 1;
        return 0;
      }
      countdown = (TimerEndTick - GetTickCount() + 999) / 1000;//计算剩余秒数，向上取整
      if (countdown != timerSecond) //如果秒数有变化，更新timer
      {
        timerSecond = countdown;
        if (countdown <= 0)
          strcpy_s(plot[list].output[plot[list].outputNum - 1], "定时完成");//更新定时消息：定时完成
        else
        {
          SecondToHms(timerString, countdown);
          sprintf_s(plot[list].output[plot[list].outputNum - 1], "定时中，剩余时间%s", timerString);//更新定时消息
        }
        isRepaintRequired = 1;
      }
    }
    //定时结束，新增强制刷新
    plot[list].isRefreshRequired = 1;
    strcpy_s(state[task].errorString, "定时启动");//刷新原因是定时启动
  }

  char taskStartTime[100];//任务启动时间字符串，用于给日志文件夹命名
  GetTimeStringForFolder(taskStartTime, time(NULL));//记录本次启动的时间
  sprintf_s(plot[list].advanceFolderPath, "执行记录\\%s_高级任务%d", taskStartTime, list);//填写日志文件夹路径
  CreatePath(plot[list].advanceFolderPath);//创建日志文件夹

  for (int order = -1; order < plot[list].advanceNum; order++)//依次执行各项任务（-1代表公会任务检查任务）
  {
    if (order == -1) //公会任务识别
    {
      if (missionOrder == -1)
        continue;
      StartQuest(list, missionOrder);//执行公会任务识别
    }
    else
    {
      if (plot[list].advance[order].selected == 1)
      {
        plot[list].playedAdvanceNum++;//执行的高级任务数量+1
        StartQuest(list, order);//执行单项任务
      }
    }
    if (plot[list].isAdvanceStarted == 0)//检测到刹车指令，立即结束线程
      StopAdvance(list);
    if (order == -1) //公会任务线程完成后，进行列表替换
    {
      int todayMission[14] = {};//今日任务编号
      int todayMissionNum = 0;//今日任务数量
      for (int i = 2; i < maxAdvanceNum; i++)//0-1是浇水施肥，不进行轨道调用
        if (missionExist[i] == 1)
          todayMission[todayMissionNum++] = i;
      //将公会任务预制列表读入MissionAdvance数组
      if (todayMissionNum >= 1) //如果读取到公会任务
      {
        //missionOrder后面的任务后移todayMissionNum - 1格子
        for (int i = plot[list].advanceNum - 1; i > missionOrder; i--)
          plot[list].advance[i + todayMissionNum - 1] = plot[list].advance[i];
        //[missionOrder ~ missionOrder + todayMissionNum - 1]用公会任务列表替换
        for (int i = 0; i < todayMissionNum; i++)
        {
          plot[list].advance[missionOrder + i] = plot[list].missionAdvance[todayMission[i]];
          plot[list].advance[missionOrder + i].isMission = 1;
          plot[list].advance[missionOrder + i].host = missionHost;
          //如果房主为2P，且1P不参与，需要交换1P和2P的卡组和轨道
          if (missionHost == 1 && missionDeck[0] == 0)
          {
            Swap(&plot[list].advance[missionOrder + i].deck[0], &plot[list].advance[missionOrder + i].deck[1]);
            char temp[maxPath];
            strcpy_s(temp, plot[list].advance[missionOrder + i].track[0]);
            strcpy_s(plot[list].advance[missionOrder + i].track[0], plot[list].advance[missionOrder + i].track[1]);
            strcpy_s(plot[list].advance[missionOrder + i].track[1], temp);
          }
          if (missionDeck[0] > 0)
            plot[list].advance[missionOrder + i].deck[0] = 1 + (plot[list].advance[missionOrder + i].deck[0] + missionDeck[0] - 2) % 6;//替换卡组
          else
            plot[list].advance[missionOrder + i].deck[0] = 0;
          if (missionDeck[1] > 0)
            plot[list].advance[missionOrder + i].deck[1] = 1 + (plot[list].advance[missionOrder + i].deck[1] + missionDeck[1] - 2) % 6;//替换卡组
          else
            plot[list].advance[missionOrder + i].deck[1] = 0;
        }
        plot[list].advanceNum += todayMissionNum - 1;
      }
      else //若没有读取到今日任务，或公会任务列表读取失败，直接删除【公会-任务】
      {
        for (int i = missionOrder + 1; i < plot[list].advanceNum; i++)
          plot[list].advance[i - 1] = plot[list].advance[i];
        plot[list].advanceNum--;
      }
      missionOrder = -1;//列表中已经没有【公会-任务】了
      SaveAdvance(list);//由于列表已改动，需要保存一次并生成备份
      SaveParameter();
      isRepaintRequired = 1;//发送重绘指令
    }
  }
  FinishAdvance(list);//高级任务完成
  StopAdvance(list, "高级任务已全部完成。");
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
    int list = curList;
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
    //任务运行期间，不允许调整参数（包括撤销、全选），但允许切换任务（2507/3507)
    if (plot[list].isAdvanceStarted)
      if (area != 2507 && area != 3507 && (area == 212 || area == 213 || area / 100 == 5 || area >= 1000))
        return 99;
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
  for (int Mode = 1; Mode <= 2; Mode++)
    buttonUpperSettings[Mode - 1] = RegisterButtonsArea(Mode, 3, 1,
      settingsX, settingsY - settingsHeight * 7 / 2, settingsWidth, settingsHeight, 200, 1, 0);

  const int SettingsNum[2] = { 11, 9 };//两种模式需要注册的设置按钮个数
  //第i项是否需要调节按钮
  const bool isAdjustRequired[2][11] = { { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1 }, { 0, 0, 0, 1, 0, 1, 0, 0, 1 } };
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
        return strcpy_s(tip, "定时启动：到达指定时间后启动。");
      if (option == 5 || option == 6)
        return strcpy_s(tip, "自动刷新：掉线中断时刷新继续。一条任务刷新满设定次数后将被跳过。");
      if (option == 7 || option == 8)
        return strcpy_s(tip, "操作速度：控制建房和刷技能的速度。建议选择快。");
      if (option == 9)
        return strcpy_s(tip, "一键配轨：为所有勾选的任务配置通用轨道。");
      if (option == 10)
        return strcpy_s(tip, "加速时间：设置全体任务的开局加速时间。");
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
    TruncateWithEllipsis(truncatedPath, plot[list].advancePath, parameterWidth * 5 - 90);
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
        strcpy_s(passwordString, plot[list].password[account]);

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
//读取普通关卡列表
void LoadNormalLevel()
{
  char s[1000], Message[30];
  char *info = nullptr, *nextInfo = nullptr;

  FILE *f;
  if (fopen_s(&f, "附加程序\\文本\\普通关卡.txt", "r"))//打开文件
    return;

  int Type = -1, Level = -1;
  while (!feof(f))
  {
    fgets(f, s);//读取一行文字
    if (strlen(s) == 0)
      break;
    if (strstr(s, "##"))//如果找到了中括号，模式+1，记录岛屿名称
    {
      Type++;
      Level = -1;
      info = strtok_s(s, "\t", &nextInfo);
      strcpy_s(typeName[basicTypeNum + Type], info + 2);//岛屿名称
      strcpy_s(islandPrompt[Type], "");

      info = strtok_s(NULL, "\t", &nextInfo);//记录岛屿坐标
      islandLocation[Type].x = atoi(info);
      info = strtok_s(NULL, "\t", &nextInfo);
      islandLocation[Type].y = atoi(info);
    }
    else //否则记录关卡名称
    {
      Level++;
      levelNum[basicTypeNum + Type]++;
      info = strtok_s(s, "\t", &nextInfo);
      strcpy_s(islandLevel[Type][Level], info);//关卡名称
      sprintf_s(Message, "%d=%s", Level, islandLevel[Type][Level]);
      if (Level > 0)
        strcat_s(islandPrompt[Type], "，");
      strcat_s(islandPrompt[Type], Message);

      info = strtok_s(NULL, "\t", &nextInfo);//记录关卡按钮坐标
      islandLevelButton[Type][Level].x = atoi(info);
      info = strtok_s(NULL, "\t", &nextInfo);
      islandLevelButton[Type][Level].y = atoi(info);
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

  timerMode = 0;//定时启动默认关闭
  timer = 0;
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
    else if (strstr(s, "定时启动=") == s)
    {
      if (s[9] == "关"[0]) //读取定时模式和定时秒数
      {
        timerMode = 0;
        timer = 0;
      }
      else if (s[9] == '~')
      {
        timerMode = 2;
        timer = atoi(s + 10);
      }
      else
      {
        timerMode = 1;
        timer = atoi(s + 9);
      }
    }
    else if (strstr(s, "自动刷新=") == s)
      maxRefreshTimes = atoi(s + 9);
    else if (strstr(s, "操作速度=") == s)
      operationSpeed = atoi(s + 9);
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
      else if (strstr(s, "房主标签=") == s)
        sscanf_s(s + 9, "%d,%d", &param[task].tag[0].x, &param[task].tag[0].y);
      else if (strstr(s, "队友标签=") == s)
        sscanf_s(s + 9, "%d,%d", &param[task].tag[1].x, &param[task].tag[1].y);
      else if (strstr(s, "大厅句柄=") == s)
        sscanf_s(s + 9, "%d,%d", (int *)&param[task].hWndHall[0], (int *)&param[task].hWndHall[1]);
      else if (strstr(s, "初始大厅=") == s)
        param[task].hMainHall = (HWND)atoi(s + 9);
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
      else if (strstr(s, "定时启动=") == s)
      {
        if (s[9] == "关"[0]) //读取定时模式和定时秒数
        {
          timerMode = 0;
          timer = 0;
        }
        else if (s[9] == '~')
        {
          timerMode = 2;
          timer = atoi(s + 10);
        }
        else
        {
          timerMode = 1;
          timer = atoi(s + 9);
        }
      }
      else if (strstr(s, "自动刷新=") == s)
        maxRefreshTimes = atoi(s + 9);
      else if (strstr(s, "操作速度=") == s)
        operationSpeed = atoi(s + 9);
      else if (strstr(s, "美食大赛=") == s)
        contestOption = atoi(s + 9);
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
    fgets(f, s);
    param[task].hWndHall[0] = (HWND)atoi(s + 13);
    fgets(f, s);
    param[task].hWndHall[1] = (HWND)atoi(s + 13);
    fgets(f, s);
    param[task].hMainHall = (HWND)atoi(s + 13);
    fgets(f, s);
    param[task].hWnd[0] = (HWND)atoi(s + 13);
    fgets(f, s);
    param[task].hWnd[1] = (HWND)atoi(s + 13);
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
//检查两个账号的大厅窗口有无改变，有则实时修改（用Param.hWnd查找Param.hWndHall）
void CheckHallWindow()
{
  HWND hHall[2] = {};
  for (int task = 0; task < maxTaskNum; task++)
  {
    //检查游戏窗口是否失效
    for (int account = 0; account <= 1; account++)
    {
      bool isWindow = IsWindow(param[task].hWnd[account]) != 0;
      if (isWindow != param[task].isWindow[account])
      {
        param[task].isWindow[account] = isWindow;
        isRepaintRequired = 1;//发送重绘指令
      }
    }
    if (isHallModified[task] == 1)//若手动修改过大厅句柄，或没有房主游戏窗口，不再自动获取大厅句柄
      continue;
    //检查两个账号的大厅窗口，如果游戏窗口未前端显示或游戏窗口不存在，将得到NULL
    for (int account = 0; account <= 1; account++)
    {
      HWND hWndGame = param[task].hWnd[account];//采用Param是因为：单刷时也要检查另一个号的大厅变化
      if (hWndGame == NULL || !IsWindow(hWndGame)) //如果游戏窗口不存在（可能是刷新期间）或已关闭，不更新大厅句柄
        continue;
      hHall[account] = GetHallWindow(hWndGame);//逐级向上查找大厅窗口
      if (hHall[account] == NULL) //如果是NULL，可能是藏在主大厅未显示，将大厅窗口设为主大厅
        hHall[account] = param[task].hMainHall;
      else if (param[task].tag[account].x >= 0)//如果执行器中抓取了这个号的坐标，则更新Param参数
      {
        if (param[task].hWndHall[account] != hHall[account])
        {
          param[task].hWndHall[account] = hHall[account];
          SaveParameter();//保存到文件
          isRepaintRequired = 1;//发送重绘指令
        }
      }
    }
  }
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
  char option[17][13] = { "", "房主轨道", "队友轨道", "执行局数", "每局用时", "继续作战", "自动收集", "使用卡组",
    "房主标签", "队友标签", "大厅句柄", "操作", "任务进度", "关卡状态", "本局用时", "平均用时", "累计用时" };
  char collectString[3][13] = { "关", "物品", "全部" };
  char doubleOption[2][3][13];//两个账号的继续作战、收集、卡组
  char hallString[2][13];//两个账号的大厅句柄
  char content[17][maxPath] = {};
  char startString[7], hideString[2][7];//启动和隐藏按钮的显示
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
    /*填写每一栏要显示的内容*/
    sprintf_s(content[0], param[task].title);

    for (int account = 0; account < 2; account++)
      ViewTrack(param[task].track[account], buttonParameter[0], account + 1, task);

    sprintf_s(content[3], "%d", param[task].maxGames);

    if (param[task].gameTime == zero)
      sprintf_s(content[4], "自动检测");
    else
      sprintf_s(content[4], "%ds", param[task].gameTime);

    /*继续作战*/
    sprintf_s(content[5], "|");
    for (int i = 0; i <= 1; i++)
      if (param[task].isProceed[i] == zero)
        sprintf_s(doubleOption[i][0], "否");
      else
        sprintf_s(doubleOption[i][0], "是");

    /*自动收集*/
    sprintf_s(content[6], "|");
    sprintf_s(doubleOption[0][1], collectString[param[task].isCollect[0]]);
    sprintf_s(doubleOption[1][1], collectString[param[task].isCollect[1]]);

    /*使用卡组*/
    sprintf_s(content[7], "|");
    for (int i = 0; i <= 1; i++)
      if (param[task].deck[i] == zero)
        sprintf_s(doubleOption[i][2], "当前");
      else
        sprintf_s(doubleOption[i][2], "卡组%d", param[task].deck[i]);

    //账号标签
    for (int account = 0; account < 2; account++)
      if (param[task].tag[account].x == -1) //无标签
        sprintf_s(content[8 + account], "");
      else if (param[task].tag[account].x == 0 && param[task].tag[account].y == 0) //已绑定游戏窗口
      {
        param[task].isWindow[account] = IsWindow(param[task].hWnd[account]) != 0;
        if (param[task].isWindow[account])
          sprintf_s(content[8 + account], "%d", (int)param[task].hWnd[account]);
        else
          sprintf_s(content[8 + account], "--");
      }
      else
      {
        param[task].isWindow[account] = IsWindow(param[task].hWnd[account]) != 0;
        if (param[task].isWindow[account])
          sprintf_s(content[8 + account], "%d", (int)param[task].hWnd[account]);
        else
          sprintf_s(content[8 + account], "(%d,%d)", param[task].tag[account].x, param[task].tag[account].y);
      }

    sprintf_s(content[10], "|");
    if (param[task].hWndHall[0] == NULL)
      sprintf_s(hallString[0], "");
    else
      sprintf_s(hallString[0], "%d", (int)param[task].hWndHall[0]);
    if (param[task].hWndHall[1] == NULL)
      sprintf_s(hallString[1], "");
    else
      sprintf_s(hallString[1], "%d", (int)param[task].hWndHall[1]);

    sprintf_s(content[11], "");

    if (isTaskStarted[task] == zero)
      sprintf_s(startString, "启动");
    else
      sprintf_s(startString, "停止");

    for (int i = 0; i <= 1; i++)
      if (isTaskHided[task][i] == zero)
        sprintf_s(hideString[i], "隐藏%d", i + 1);
      else
        sprintf_s(hideString[i], "显示%d", i + 1);

    /*填写状态信息*/
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
        CenterView(hallString[0], parameterX + (2 * task + 1) * parameterWidth / 2 - 53, parameterY + (2 * i + 1) * parameterHeight / 2);
        CenterView(hallString[1], parameterX + (2 * task + 1) * parameterWidth / 2 + 53, parameterY + (2 * i + 1) * parameterHeight / 2);
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
  char title[ParameterNum][13] = { "", "1P标签", "2P标签", "大厅句柄", "操作", "任务进度",
    "关卡状态", "本局用时", "平均用时", "累计用时" };
  char content[ParameterNum][30] = {};//参数显示内容
  char startString[7], hideString[2][7];//启动 隐藏
  char hallString[2][13];//两个账号的大厅句柄

  /*填写参数信息*/

  //任务标题
  sprintf_s(content[0], param[task].title);

  //账号标签
  for (int account = 0; account < 2; account++)
    if (param[task].tag[account].x == -1) //无标签
      sprintf_s(content[1 + account], "");
    else if (param[task].tag[account].x == 0 && param[task].tag[account].y == 0) //已绑定游戏窗口
    {
      param[task].isWindow[account] = IsWindow(param[task].hWnd[account]) != 0;
      if (param[task].isWindow[account])
        sprintf_s(content[1 + account], "%d", (int)param[task].hWnd[account]);
      else
        sprintf_s(content[1 + account], "--");
    }
    else
    {
      param[task].isWindow[account] = IsWindow(param[task].hWnd[account]) != 0;
      if (param[task].isWindow[account])
        sprintf_s(content[1 + account], "%d", (int)param[task].hWnd[account]);
      else
        sprintf_s(content[1 + account], "(%d,%d)", param[task].tag[account].x, param[task].tag[account].y);
    }

  sprintf_s(content[1 + 2], "|"); //大厅句柄
  if (param[task].hWndHall[0] == NULL)
    sprintf_s(hallString[0], "");
  else
    sprintf_s(hallString[0], "%d", (int)param[task].hWndHall[0]);
  if (param[task].hWndHall[1] == NULL)
    sprintf_s(hallString[1], "");
  else
    sprintf_s(hallString[1], "%d", (int)param[task].hWndHall[1]);

  sprintf_s(content[1 + 3], "");//操作

  /*填写状态信息*/
  if (strcmp(state[task].tip, "未开始") == zero)
    for (int i = 4; i <= 8; i++)
      strcpy_s(content[1 + i], "");
  else //已经启动则进行记录
  {
    GetTimeStringMS(content[1 + 6], state[task].currentTime);
    GetTimeStringMS(content[1 + 7], state[task].averageTime / 1000);
    GetTimeStringHMS(content[1 + 8], state[task].totalTime);
    if (strcmp(state[task].tip, "运行中") == zero || strcmp(state[task].tip, "已停止") == zero)
    {
      sprintf_s(content[1 + 4], "%d/%d（%s）", state[task].games, state[task].maxGames, state[task].tip);//任务进度
      switch (state[task].wave)//关卡状态
      {
      case waveOutOfLevel:
        sprintf_s(content[1 + 5], "未进入关卡");
        break;
      case waveLoot:
        sprintf_s(content[1 + 5], "战利品");
        break;
      case waveGrade:
        sprintf_s(content[1 + 5], "结算");
        break;
      case waveFlip:
        sprintf_s(content[1 + 5], "翻牌");
        break;
      case waveNoImage:
        sprintf_s(content[1 + 5], "无图像");
        break;
      case waveTower:
        sprintf_s(content[1 + 5], "魔塔选层");
        break;
      default:
        sprintf_s(content[1 + 5], "第%d.%d波", state[task].wave, state[task].smallWave);
        break;
      }
    }
    else if (strcmp(state[task].tip, "已完成") == zero)
    {
      sprintf_s(content[1 + 4], "%d/%d（已完成）", state[task].maxGames, state[task].maxGames);//任务进度
      sprintf_s(content[1 + 5], "未进入关卡");
    }
    else //中断的各种情形
    {
      sprintf_s(content[1 + 4], "%d/%d（已中断）", state[task].games, state[task].maxGames);//任务进度
      sprintf_s(content[1 + 5], "%s", state[task].tip);
    }
  }

  if (plot[list].isAdvanceStarted == 0)
    sprintf_s(startString, "启动");
  else
    sprintf_s(startString, "停止");

  for (int i = 0; i <= 1; i++)
    if (isTaskHided[task][i] == zero)
      sprintf_s(hideString[i], "隐藏%d", i + 1);
    else
      sprintf_s(hideString[i], "显示%d", i + 1);

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
      CenterView(hallString[0], parameterX + parameterWidth / 2 - 53,
        parameterY + (2 * i + 15) * parameterHeight / 2);
      CenterView(hallString[1], parameterX + parameterWidth / 2 + 53,
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
  RectType button = buttons[buttonCode];
  PaintCheck(button.x, button.y, button.width, button.height, offsetX);
}
//显示右侧设置按钮
void ViewSetting()
{
  const int MaxSettingsNum = 5; //设置按钮个数
  char upperSettings[3][20] = { "全部停止", "后台运行", "一键报错" };
  char SettingsString[2][2 * MaxSettingsNum][20] = {
    { "带卡选择", "", "翻牌数量", "", "背包满时", "", "经验满时", "", "补阵强度", "" },
    { "定时启动", "", "自动刷新", "", "操作速度", "", "一键配轨", "" } };

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
      sprintf_s(SettingsString[0][1], "最高星");
    else if (customOption == 1)
      sprintf_s(SettingsString[0][1], "最靠前");

    sprintf_s(SettingsString[0][3], "%d", flipNum);//翻牌数量

    if (bagFullOperation == 1) //背包满时
      sprintf_s(SettingsString[0][5], "继续");
    else
      sprintf_s(SettingsString[0][5], "停止");

    if (expFullOperation == 1) //经验满时
      sprintf_s(SettingsString[0][7], "继续");
    else
      sprintf_s(SettingsString[0][7], "停止");

    if (repairLevel == 0) //补阵强度
      sprintf_s(SettingsString[0][9], "0 %%");
    else
      sprintf_s(SettingsString[0][9], "%d0 %%", repairLevel);

    PaintGrid(SettingsNum * 2, 1, settingsX, settingsY, settingsWidth, settingsHeight);//按钮
    for (int i = 0; i < SettingsNum * 2; i++) //显示文字
      CenterView(SettingsString[0][i], settingsX + settingsWidth / 2, settingsY + (2 * i + 1) * settingsHeight / 2);
    for (int i = 1; i < SettingsNum * 2; i++) //显示箭头
      if (i % 2 == 1 || i == 10)
        PaintHorizontalAdjust(settingsX, settingsY + i * settingsHeight, settingsWidth, settingsHeight);
  }
  else if (mode == 2)
  {
    int list = curList;
    SettingsNum = 4;

    if (timerMode == 0) //定时启动
      sprintf_s(SettingsString[1][1], "关");
    else
    {
      char timerString[9];
      SecondToHms(timerString, timer);
      if (timerMode == 1)
        sprintf_s(SettingsString[1][1], "%s", timerString);
      else if (timerMode == 2)
        sprintf_s(SettingsString[1][1], "~%s", timerString);
    }

    if (maxRefreshTimes == 0) //自动刷新
      sprintf_s(SettingsString[1][3], "关");
    else if (maxRefreshTimes == unlimitedRefreshTimes)
      sprintf_s(SettingsString[1][3], "无限制", maxRefreshTimes);
    else
      sprintf_s(SettingsString[1][3], "%d次", maxRefreshTimes);

    if (operationSpeed == 2) //操作速度
      sprintf_s(SettingsString[1][5], "快");
    else if (operationSpeed == 1)
      sprintf_s(SettingsString[1][5], "中");
    else
      sprintf_s(SettingsString[1][5], "慢");

    if (globalAccelarationTime == 0)
      sprintf_s(SettingsString[1][7], "加速：关");
    else
      sprintf_s(SettingsString[1][7], "加速：%d", globalAccelarationTime);

    PaintGrid(SettingsNum * 2, 1, settingsX, settingsY, settingsWidth, settingsHeight);//按钮
    for (int i = 0; i < SettingsNum * 2; i++) //显示文字
    {
      if (i == 3)
        LongCenterView(SettingsString[1][i], settingsX + settingsWidth / 2,
          settingsY + (2 * i + 1) * settingsHeight / 2, settingsWidth - 5);
      else
        CenterView(SettingsString[1][i], settingsX + settingsWidth / 2,
          settingsY + (2 * i + 1) * settingsHeight / 2);
    }
    for (int i = 3; i < (SettingsNum - 1) * 2; i = i + 2) //显示箭头
      PaintHorizontalAdjust(settingsX, settingsY + i * settingsHeight, settingsWidth, settingsHeight);

    //翻页按钮
    char PageString[10];
    int TotalAdvancePage = min(plot[list].advanceNum / advanceNumPerPage + 1, maxAdvancePage);
    sprintf_s(PageString, "%d/%d", plot[list].advancePage, TotalAdvancePage);
    CenterView(PageString, settingsX + settingsWidth / 2, settingsY + 17 * settingsHeight / 2 + 2);
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
    if (plot[list].advance[order].selected == 1)
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
//显示高级任务列表
void ViewAdvance()
{
  int list = curList;
  const int tickBoxOffsetX = -16, idOffsetX = 14;
  const int trackWidth = parameterWidth - 40;
  char content[advanceTitleNum][100] = {};
  char title[advanceTitleNum][13] = { "", "类型", "关卡", "局数", "钥匙", "房主", "1P轨道", "卡组", "2P轨道", "卡组" };
  char *isMissionList = strstr(plot[list].advancePath, "公会任务.txt");//当前打开的是否为公会任务列表

  //公会任务涂色
  setfillcolor(RGB(30, 72, 30));
  RectType &rightButton = buttons[FindButton(buttonAdvance, 0, 0)];
  int advanceRight = rightButton.x + rightButton.width;
  for (int pageOrder = 0; pageOrder < advanceNumPerPage; pageOrder++)
  {
    int order = (plot[list].advancePage - 1) * advanceNumPerPage + pageOrder;
    if (order >= plot[list].advanceNum)
      break;
    if (plot[list].advance[order].isMission == 1)
    {
      RectType &button = buttons[FindButton(buttonAdvance, pageOrder + 1, 0)];
      solidrectangle(advanceX, button.y, advanceRight, button.y + button.height);
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
    else if (plot[list].advance[order].selected == 1)
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

    //是否使用灰色显示轨道和卡组（卡组为0时以灰色显示）
    bool isGray[2] = { plot[list].advance[order].deck[0] == 0, plot[list].advance[order].deck[1] == 0 };

    //战斗任务：填写并显示轨道路径
    for (int account = 0; account < 2; account++)
    {
      int contentOrder = TRACK_0 + 2 * account;
      if (!IsNoTrack(plot[list].advance[order]))
        ViewTrack(plot[list].advance[order].track[account], buttonAdvance, pageOrder + 1, contentOrder, isGray[account]);
      else
        PrintButton(buttonAdvance, pageOrder + 1, contentOrder, "-", 0, 0, isGray[account]);
    }

    for (int i = 0; i < advanceTitleNum; i++)//清空显示内容
      sprintf_s(content[i], "");
    sprintf_s(content[ID], "%d", order + 1);//ID
    sprintf_s(content[TYPE], "%s", typeName[plot[list].advance[order].type]);//类型
    GetLevelName(content[LEVEL], plot[list].advance[order].type, plot[list].advance[order].level);//关卡

    //局数和钥匙
    if (IsNoTrack(plot[list].advance[order]))
    {
      sprintf_s(content[GAMES], "-");
      sprintf_s(content[KEY], "-");
    }
    else
    {
      sprintf_s(content[GAMES], "%d", plot[list].advance[order].maxGames);
      sprintf_s(content[KEY], "%s", plot[list].advance[order].isProceed ? "√" : "");//钥匙
    }

    //房主
    if (IsSpecial(plot[list].advance[order]))
      strcpy_s(content[HOST], "-");
    else if (plot[list].advance[order].host == 2)
      strcpy_s(content[HOST], "同步");
    else
      sprintf_s(content[HOST], "%dP", plot[list].advance[order].host + 1);

    //显示卡组
    for (int account = 0; account < 2; account++)
    {
      int contentOrder = TRACK_0 + 2 * account;
      sprintf_s(content[contentOrder], "");

      if (plot[list].advance[order].deck[account] == 0)
        sprintf_s(content[contentOrder + 1], "×");
      else
      {
        if (isMissionList) //公会任务列表显示为+n
          sprintf_s(content[contentOrder + 1], "+%d", plot[list].advance[order].deck[account] - 1);
        else if (IsSpecial(plot[list].advance[order])) //特殊任务只显示√
          strcpy_s(content[contentOrder + 1], "√");
        else
          sprintf_s(content[contentOrder + 1], "%d", plot[list].advance[order].deck[account]);
      }
    }

    PaintCheckBox(buttonAdvance, pageOrder + 1, 0, tickBoxOffsetX);

    //显示序号和勾选框
    if (plot[list].advance[order].selected == 1)
      PaintCheck(buttonAdvance, pageOrder + 1, 0, tickBoxOffsetX);
    PrintButton(buttonAdvance, pageOrder + 1, 0, content[0], idOffsetX, 0);

    //显示第1-8项文字：1类型 2关卡 3局数 4钥匙 5房主 6轨道1 7卡组1 8轨道2 9卡组2
    for (int i = 1; i < advanceTitleNum; i++)
    {
      int account = (i - TRACK_0) / 2; //i = 6, 7对应account = 0，i = 8, 9对应account = 1
      int dx = 0, dy = 0;
      bool gray = false;
      if (i == TYPE || i == DECK_0 || i == DECK_1) //类型 卡组
        dx = -7;
      if (i == TRACK_0 || i == TRACK_1 || i == DECK_0 || i == DECK_1)//未使用的轨道和卡组变灰
        gray = isGray[account];
      PrintButton(buttonAdvance, pageOrder + 1, i, content[i], dx, dy, gray);
    }

    //显示调节、删除轨道、交换轨道符号
    int size = GetFontSize();
    for (int i = 1; i < advanceTitleNum; i++)
    {
      int account = (i - TRACK_0) / 2; //i = 6, 7对应account = 0，i = 8, 9对应account = 1
      if (!(IsNoTrack(plot[list].advance[order]) && (i == TRACK_0 || i == TRACK_1)))//公会任务不显示轨道删除交换符号
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
          PrintButton(buttonAdvance, pageOrder + 1, i, "▲", advanceWidth[i] / 2 - 10, -7, isGray[account]);
          PrintButton(buttonAdvance, pageOrder + 1, i, "▼", advanceWidth[i] / 2 - 10, 10, isGray[account]);
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
    if (plot[list].advance[order].type == 0 || plot[list].advance[order].type == 1 || plot[list].advance[order].type == 3 ||
      plot[list].advance[order].type == 5 || plot[list].advance[order].type == 17)//右侧调节按钮
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
  ViewAdvance();
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
void OpenGraph_Fit()//适应屏幕分辨率启动画图窗口
{
  int nScreenWidth, nScreenHeight;
  nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
  nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
  originRatio = min((float)nScreenWidth / (wndWidth + 20), (float)nScreenHeight / 864);
  ratio = originRatio * zoom / 100;//改变窗口尺寸和显示比例，不影响点击位置判定
  initgraph((int)(wndWidth * ratio), (int)(wndHeight * ratio));
  setaspectratio(ratio, ratio);
  imageBackup.Resize((int)(wndWidth * ratio), (int)(wndHeight * ratio));
}
//删除账号account的标签
void DeleteTag(int task, int account)
{
  if (isTaskHided[task][account] == 1)
  {
    MessageBox(hWndActuator, "请先显示原来的游戏窗口", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }
  param[task].tag[account].x = -1;
  param[task].tag[account].y = -1;
  param[task].hWnd[account] = nullptr;
  param[task].hWndHall[account] = nullptr;
  isTaskHided[task][account] = 0;//隐藏状态设为0
  isHallModified[task] = 1;//手动删除后，不再自动获取大厅句柄
  if (IsDual1P(task)) //高级任务连带删除2P任务的句柄
  {
    param[task + 1].tag[account] = param[task].tag[account];
    param[task + 1].hWnd[account] = param[task].hWnd[account];
    param[task + 1].hWndHall[account] = param[task].hWndHall[account];
    isTaskHided[task + 1][account] = 0;//隐藏状态设为0
    isHallModified[task + 1] = 1;//手动删除后，不再自动获取大厅句柄
  }
}
//抓取账号account的标签
void GrabTag(int task, int account)
{
  HWND hWndPointed;
  if (isTaskHided[task][account] == 1)
  {
    MessageBox(hWndActuator, "请先显示原来的游戏窗口", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }
  SetSystemCursor(crossCursor, (DWORD)IDC_ARROW);//将箭头和十字光标替换
  while (GetAsyncKeyState(VK_LBUTTON))//等待鼠标左键松开
    Sleep(10);
  SetSystemCursor(crossCursor, (DWORD)IDC_ARROW);//光标恢复正常

  POINT locPointed;//抓取的位置
  GetCursorPos(&locPointed);
  hWndPointed = WindowFromPoint(locPointed);//记录抓取的窗口句柄
  char ClassName[256];
  GetClassName(hWndPointed, ClassName, 256);//获取窗口类名
  //微端窗口或大厅窗口
  if (strcmp(ClassName, "ApolloRuntimeContentWindow") == 0 || strcmp(ClassName, "DUIWindow") == 0)
  {
    RECT HallRect;
    GetWindowRect(hWndPointed, &HallRect);//获取大厅位置
    if (locPointed.y < 1)
      locPointed.y = 1;
    int x = locPointed.x - HallRect.left;
    int y = locPointed.y - HallRect.top;
    //记录相对坐标，但最少是1（点击大厅(1,1)位置不会有反应）
    param[task].tag[account].x = x >= 1 ? x : 1;
    param[task].tag[account].y = y >= 1 ? y : 1;

    int tagNum = GetTagNum(hWndPointed);//获取大厅标签数量
    if (tagNum == 1) //只有1个标签
      param[task].hWnd[account] = GetActiveGameWindow(hWndPointed);//绑定唯一游戏窗口
    else
      param[task].hWnd[account] = NULL; //游戏窗口无法确定，填写为NULL
    param[task].hWndHall[account] = hWndPointed; //大厅窗口就是抓取的窗口
  }
  //游戏窗口
  else if (strcmp(ClassName, "NativeWindowClass") == 0 || strcmp(ClassName, "WebPluginView") == 0)
  {
    param[task].tag[account].x = 0;
    param[task].tag[account].y = 0;
    param[task].hWnd[account] = hWndPointed; //游戏窗口就是抓取的窗口
    param[task].hWndHall[account] = GetHallWindow(hWndPointed); //寻找大厅窗口
  }
  //选服窗口
  else if (strcmp(ClassName, "Chrome_RenderWidgetHostHWND") == 0)
  {
    param[task].tag[account].x = 0;
    param[task].tag[account].y = 0;
    param[task].hWnd[account] = GetGameWindowFromServer(hWndPointed); //寻找游戏窗口
    param[task].hWndHall[account] = GetHallWindow(hWndPointed); //寻找大厅窗口
  }
  else //都不是
  {
    MessageBox(hWndActuator, "请抓取大厅账号标签、微端标题栏或游戏画面。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }
  param[task].hMainHall = GetMainHallWindow(param[task].hWndHall[account]);//记录大厅主窗口
  isTaskHided[task][account] = 0;//隐藏状态设为0
  isHallModified[task] = 1;//手动抓取后，不再自动获取大厅句柄
  if (IsDual1P(task)) //高级任务把抓得的句柄同步到2P任务
  {
    param[task + 1].tag[account] = param[task].tag[account];
    param[task + 1].hWnd[account] = param[task].hWnd[account];
    param[task + 1].hWndHall[account] = param[task].hWndHall[account];
    param[task + 1].hMainHall = param[task].hMainHall;
    isTaskHided[task + 1][account] = 0;//隐藏状态设为0
    isHallModified[task + 1] = 1;//手动抓取后，不再自动获取大厅句柄
  }
}
//隐藏或显示窗口
void HideOrShow(int task, int account)
{
  if (isTaskHided[task][account] == zero) //隐藏1P大厅
  {
    if (param[task].hWndHall[account] == NULL)
      MessageBox(hWndActuator, "请先抓取要隐藏的窗口句柄，\n将靶形光标拖动到窗口标题栏即可。", "", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    else
    {
      int hideResult = HideWindow((HWND)param[task].hWndHall[account]);
      if (hideResult == 2)
        PopMessage(task, "权限不足，请以管理员身份运行。");
      else if (hideResult == 1)
        PopMessage(task, "未找到窗口。");
      else if ((HWND)param[task].hWndHall[account] != hWndActuator)//隐藏的不是本窗口才需要修改隐藏状态
      {
        for (int acc = 0; acc <= 1; acc++)//检查两个账号的所有窗口，句柄相同的都设为隐藏
          for (int i = 0; i < maxTaskNum; i++)
            if (param[i].hWndHall[acc] == param[task].hWndHall[account])
              isTaskHided[i][acc] = 1;
      }
    }
  }
  else //显示1P大厅
  {
    RecoverWindow(param[task].hWndHall[account]);
    InvalidateRect(GetActiveGameWindow(param[task].hWnd[account]), NULL, false);//重画游戏窗口
    isTaskHided[task][account] = 0;
    for (int acc = 0; acc <= 1; acc++)//检查两个账号的所有窗口，句柄相同的都设为显示
      for (int i = 0; i < maxTaskNum; i++)
        if (param[i].hWndHall[acc] == param[task].hWndHall[account])
          isTaskHided[i][acc] = 0;
  }
}
void ExchangeAdvance(int list, int order1, int order2)
{
  if (order1 == order2)
    return;
  AdvanceType Temp = plot[list].advance[order1];
  plot[list].advance[order1] = plot[list].advance[order2];
  plot[list].advance[order2] = Temp;
}
//将高级任务order1插入到order2位置，其他任务顺序保持
void InsertAdvance(int list, int order1, int order2)
{
  if (order1 == order2)
    return;
  AdvanceType Temp = plot[list].advance[order1];
  if (order1 > order2) //向前插入
    for (int i = order1; i >= order2 + 1; i--)
      plot[list].advance[i] = plot[list].advance[i - 1];
  else if (order1 < order2) //向后插入
    for (int i = order1; i <= order2 - 1; i++)
      plot[list].advance[i] = plot[list].advance[i + 1];
  plot[list].advance[order2] = Temp;
}
//识别还原：将公会任务还原为识别前的状态（公会-任务）
void MissionReturn(int list)
{
  bool isMissionExist = false;//公会-任务一项是否存在
  int firstMission = -1;
  int missionDeck[2] = { 0, 0 };
  for (int order = plot[list].advanceNum - 1; order >= 0; order--)
  {
    if (IsMission(plot[list].advance[order]))
      isMissionExist = true;
    if (plot[list].advance[order].isMission == 1)
    {
      firstMission = order;
      missionDeck[0] = plot[list].advance[order].deck[0];
      missionDeck[1] = plot[list].advance[order].deck[1];
      DeleteAdvance(list, order);
    }
  }
  if (isMissionExist == false && firstMission >= 0)
  {
    AddAdvance(list, 5, 0, 1, missionDeck[0], missionDeck[1]);
    InsertAdvance(list, plot[list].advanceNum - 1, firstMission);
  }
}
//从完整路径提取纯轨道路径
const char *GetPureTrack(const char *extendedPath)
{
  const char *bannedPath = extendedPath;
  const char *bar = strchr(extendedPath, '|');
  if (bar)
    bannedPath = bar + 1;
  const char *path = bannedPath;
  const char *braket = strchr(bannedPath, '>');
  if (braket)
    path = braket + 1;
  return path;
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
void BatchAdjust(int list, int item, int origin, int target)
{
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    if (IsSpecial(plot[list].advance[order])) //批量调节不影响特殊任务
      continue;
    int *value = nullptr;
    if (item == HOST)
      value = &plot[list].advance[order].host;
    else if (item == DECK_0)
      value = &plot[list].advance[order].deck[0];
    else if (item == DECK_1)
      value = &plot[list].advance[order].deck[1];
    if (*value == origin)
      *value = target;
  }
}
//批量交换：房主，1P卡组，2P卡组
void BatchExchange(int list, int item, int origin, int target)
{
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    if (IsSpecial(plot[list].advance[order])) //批量交换不影响特殊任务
      continue;
    int *value = nullptr;
    if (item == HOST)
      value = &plot[list].advance[order].host;
    else if (item == DECK_0)
      value = &plot[list].advance[order].deck[0];
    else if (item == DECK_1)
      value = &plot[list].advance[order].deck[1];
    if (*value == origin)
      *value = target;
    else if (*value == target)
      *value = origin;
  }
}
// 查找所有游戏大厅和微端，进行截图
int __stdcall EnumHall(HWND hWnd, LPARAM lParam)
{
  char className[256];
  char pngName[256] = {};
  int version0 = 0, version1 = 0;
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
        WindowToBitmap(hWnd, pngName);
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
      GetWindowText(hWnd, windowTitle, sizeof(windowTitle));
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
    HideWindow(hWndActuator);
  else if (area == 202) //一键报错
    MakeErrorFile();
}
//切换放卡账号
void ExchangePerformer(int list, int order)
{
  //交换轨道
  char track[maxPath];
  strcpy_s(track, plot[list].advance[order].track[0]);
  strcpy_s(plot[list].advance[order].track[0], plot[list].advance[order].track[1]);
  strcpy_s(plot[list].advance[order].track[1], track);
  //交换卡组
  int tempDeck = plot[list].advance[order].deck[0];
  plot[list].advance[order].deck[0] = plot[list].advance[order].deck[1];
  plot[list].advance[order].deck[1] = tempDeck;
  //交换房主（如果是同步则不交换）
  if (plot[list].advance[order].host < 2)
    plot[list].advance[order].host = 1 - plot[list].advance[order].host;
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
//编辑高级任务
void EditAdvance()
{
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
        if (area == 15) //启动或停止高级任务
        {
          if (plot[curList].isAdvanceStarted == 0) //高级任务未启动：可以启动
          {
            if (plot[curList].isAdvanceStarted_Confirm == 1 || isTaskStarted_Confirm[GetTask(curList)] == 1
              || isTaskStarted_Confirm[GetTask(curList) + 1] == 1)
              MessageBox(hWndActuator, "请勿频繁启动", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
            else
            {
              LoadCustom(GetTask(curList));//读取自定卡槽
              LoadCustom(GetTask(curList) + 1);
              plot[curList].isAdvanceStarted = 1;
              plot[curList].isAdvanceStarted_Confirm = 1;
              CreateThread(NULL, 0, StartList, (void *)curList, 0, NULL);//启动高级任务线程
            }
          }
          else
          {
            isTaskStarted[GetTask(curList)] = 0;//任务启动状态设为“未启动”
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
          Sleep(100);
        }
        if (area == 99)
          MessageBox(hWndActuator, "任务运行时不允许修改参数", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        ResponseUpperSetting();

        if (area == 204) //设置定时启动
          InputTimer(&timer, &timerMode);

        AdjustZoom();//缩放
        AdjustRefreshTimes();//自动刷新
        Adjust(operationSpeed, 0, 2, 1, 308, 408, 0);//操作速度

        if (area == 209) //一键配轨
          MatchCommonTrackWithChoice(curList);
        if (area == 210) //调整全局加速时间
          InputNum(&globalAccelarationTime, 0, 999,
            "设置全体任务的开局加速时间（0~999），单位毫秒。\n如需单独设置某些任务的加速时间，请见使用手册。",
            "加速时间");

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
              plot[curList].advance[order].selected = 0;
          }
          else //否则全部选中
          {
            for (int order = 0; order < plot[curList].advanceNum; order++)
              plot[curList].advance[order].selected = 1;
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
            plot[curList].advance[order].selected = 1 - plot[curList].advance[order].selected;
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
                  if (plot[curList].advance[order].type < typeNum - 1)
                  {
                    plot[curList].advance[order].type++;
                    plot[curList].advance[order].level = defaultLevel[plot[curList].advance[order].type];
                    plot[curList].advance[order].isMission = 0;//取消公会任务标记
                  }
                }
                else if (area / 10000 == 3) //下调
                {
                  if (plot[curList].advance[order].type > zero)
                  {
                    plot[curList].advance[order].type--;
                    plot[curList].advance[order].level = defaultLevel[plot[curList].advance[order].type];
                    plot[curList].advance[order].isMission = 0;//取消公会任务标记
                  }
                }
                break;
              case LEVEL: //关卡
                if (plot[curList].advance[order].type == 2) //魔塔：输入魔塔关卡
                  InputTower(&plot[curList].advance[order].level);
                else if (plot[curList].advance[order].type == 0 || plot[curList].advance[order].type == 1 || plot[curList].advance[order].type == 4 ||
                  plot[curList].advance[order].type == 5 || plot[curList].advance[order].type == 17)
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
                if (IsNoTrack(plot[curList].advance[order]))
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
                if (item == HOST && IsSpecial(plot[curList].advance[order]))
                {
                  MessageBox(hWndActuator, "此任务无需填写房主。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
                  break;
                }
                int account = (item - TRACK_0) / 2;
                int &oldValue = item == HOST ? plot[curList].advance[order].host : plot[curList].advance[order].deck[account];
                //特殊任务：必然是调节卡组。只能0调到1, 1调到0
                if (IsSpecial(plot[curList].advance[order]))
                {
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
                    if (!IsSpecial(plot[curList].advance[plot[curList].advanceSelected - 100]))
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
                if (IsNoTrack(plot[curList].advance[order]))
                  MessageBox(hWndActuator, "此任务无需填写轨道。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
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
        if (area == 99)
          MessageBox(hWndActuator, "任务运行时不允许修改参数", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        if (area == 204) //删除定时
        {
          timerMode = 0;
          timer = 0;
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
            //不是公会任务才能修改局数、房主、轨道、卡组
            if (!(IsMission(plot[curList].advance[order]) && item >= 3 && item <= 8))
              switch (item)
              {
              case ID:case CHECK: //序号或勾选框：删除任务
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
      SaveAdvance(curList);
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
            if (isTaskStarted_Confirm[task] == 1)
              MessageBox(hWndActuator, "请勿频繁启动", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
            else if (CheckTask(task) == 1)
            {
              LoadCustom(task);//读取自定卡槽
              isTaskStarted[task] = 1;//记录任务开始
              isTaskStarted_Confirm[task] = 1;
              CreateThread(NULL, 0, StartTask, (void *)task, 0, NULL);//启动任务线程
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
    bool isAnyTaskRunning = false;
    for (int task = 0; task < maxTaskNum; task++)//检查普通任务线程
      if (isTaskStarted[task])
        isAnyTaskRunning = true;
    for (int list = 0; list < listNum; list++)//检查高级任务线程
      if (plot[list].isAdvanceStarted)
        isAnyTaskRunning = true;

    if (isAnyTaskRunning) //如果有任务正在启动，确认了才能退出
    {
      int CloseMessage = MessageBox(hWndActuator, "还有任务正在运行，\n确定要退出吗？", "提示",
        MB_ICONINFORMATION | MB_OKCANCEL | MB_SYSTEMMODAL);
      if (CloseMessage == IDOK)
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
      if (IsNormal(task) && account >= 0 && account <= 1)
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
    else if (mode == 2)
    {
      int list = curList;
      if (fileType == 0) //如果是无效文件
      {
        PopMessage(hWndActuator, "只能打开轨道或列表文件。");
        break;
      }
      if (fileType == 1) //如果是轨道文件
      {
        int pageOrder = area / 10;
        int order = (plot[list].advancePage - 1) * advanceNumPerPage + pageOrder;
        if (order >= zero && order < plot[list].advanceNum && account >= 0 && account <= 1)
        {
          if (account == 0)
            strcpy_s(plot[list].advance[order].track[0], shortPath);
          else
            strcpy_s(plot[list].advance[order].track[1], shortPath);
        }
      }
      else if (fileType == 2) //如果是列表文件
      {
        if (plot[list].isAdvanceStarted == 1) //任务运行时：不允许修改，把文件中的路径改回去
          MessageBox(hWndActuator, "任务运行时不允许修改参数", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        else
        {
          plot[list].advanceSelected = 0;
          strcpy_s(plot[list].advancePath, shortPath);
          LoadAdvance(list);
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
//公会任务列表轨道用任务名命名
int RenameWithMission(int list)
{
  //读取公会任务名称
  FILE *f;
  char missionName[maxAdvanceNum][100] = {};
  if (fopen_s(&f, "附加程序\\公会任务\\公会任务名称.txt", "r"))
    return 1;
  for (int order = 0; order < missionNum; order++)
    fscanf_s(f, "%s\n", missionName[order], sizeof(missionName[order]));
  fclose(f);

  char trackPath[2][maxPath] = {};
  for (int order = 0; order < plot[list].advanceNum; order++)
  {
    sprintf_s(trackPath[0], "预制轨道\\公会任务\\%d_%s.txt", order + 1, missionName[order]);
    sprintf_s(trackPath[1], "预制轨道\\公会任务\\%d_%s+.txt", order + 1, missionName[order]);
    if (order >= 2 && order <= 59)
      for (int account = 0; account < 2; account++)
        if (strlen(plot[list].advance[order].track[account]) > 0)
        {
          Copy(plot[list].advance[order].track[account], trackPath[account]);
          strcpy_s(plot[list].advance[order].track[account], trackPath[account]);
        }
  }
  SaveAdvance(list);
  return 0;
}
//列表中是否存在具体公会任务
bool isMissionExisted(int list)
{
  for (int order = 0; order < plot[list].advanceNum; order++)
    if (plot[list].advance[order].isMission == 1)
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
char crossName[6][8][9];//跨服6种环境8个关卡的全称
//读取跨服关卡列表
void LoadCrossLevel()
{
  FILE *f;
  if (fopen_s(&f, "附加程序\\文本\\跨服关卡.txt", "r"))//打开文件
  {
    PopMessage(nullptr, "打不开[跨服关卡.txt]");
    return;
  }
  char s[1000];
  char *info = nullptr, *nextInfo = nullptr;
  for (int i = 0; i < 6; i++)
  {
    fgets(f, s);//读取一行文字
    strcpy_s(crossName[i][0], strtok_s(s, "\t", &nextInfo));
    for (int j = 1; j < 4; j++)
      strcpy_s(crossName[i][j], strtok_s(NULL, "\t", &nextInfo));
  }
  fclose(f);
}
//大赛任务信息
struct ContestType
{
  int roleNum, type, level;//人数，任务类型，关卡
  char track[MAX_PATH];//预制轨道路径
  char requirement[100];//附加要求
  char extracted[100];//已提取的条件
  char remaining[100];//未提取的条件
  char exception[100];//异常（无轨道、格式不对或条件无法处理）
  char gradeInfo[20];//评级要求
  char energyInfo[20];//火苗限制
  char slotInfo[20];//卡槽数限制
  char banInfo[20];//ban卡要求
  char plantInfo[20];//放卡要求
  int requiredSlotNum;
  char ban[20];
  char plant[20];
};
const int maxContestNum = maxAdvanceNum;
ContestType contest[maxContestNum];
int contestNum;
const char *const crossMapName[6] = { "古堡", "天空", "炼狱", "水火", "巫毒", "冰封" };
//从字符串的begin位置开始删除num个字符
void deleteString(char *begin, int num)
{
  for (char *c = begin; c[0] != 0; c++)
    c[0] = c[num];
}
//提取等级条件
bool ExtractRequirement(char(&contestInfo)[20], char *requirement, const char *info)
{
  char commaGrade[10] = {};
  char *infoLoc = strstr(requirement, info);
  if (infoLoc)
  {
    sprintf_s(contestInfo, info);
    deleteString(infoLoc, strlen(info));
    return true;
  }
  return false;
}
//从字符串中抽取条件
void ExtractRequirement(ContestType &contest, char *requirement)
{
  if (strlen(requirement) == 0)
    return;
  //提取等级条件
  ExtractRequirement(contest.gradeInfo, requirement, "SSS");
  ExtractRequirement(contest.gradeInfo, requirement, "SS");
  //提取火苗条件
  char energy[20] = {};
  for (int i = 1; i <= 10; i++)
  {
    sprintf_s(energy, "%dk火", i);
    ExtractRequirement(contest.energyInfo, requirement, energy);
  }
  //提取卡槽数限制条件
  char slotRestrict[20] = {};
  for (int i = 1; i <= 14; i++)
  {
    sprintf_s(slotRestrict, "限%d张卡", i);
    if (ExtractRequirement(contest.slotInfo, requirement, slotRestrict))
      contest.requiredSlotNum = i;
    sprintf_s(slotRestrict, "%d张卡限制", i);
    if (ExtractRequirement(contest.slotInfo, requirement, slotRestrict))
      contest.requiredSlotNum = i;
  }
  //放置xxx，不使用xxx，禁xxx
  const char banString[4][20] = { "放置", "不使用", "禁", "禁用" };
  for (int i = 0; i < 4; i++)
  {
    char *infoLoc = strstr(requirement, banString[i]);
    if (infoLoc)
    {
      char info[100] = {};//放卡ban卡条件
      strcpy_s(info, infoLoc);//先拷贝"不使用"及后面的所有字符
      char *comma = strstr(info, "、");
      if (comma) //如果有顿号，从顿号位置截断
        comma[0] = 0;
      if (i == 0) //要放置的卡
      {
        strcpy_s(contest.plantInfo, info);
        strcpy_s(contest.plant, info + strlen(banString[i]));
      }
      else //禁用的卡
      {
        strcpy_s(contest.banInfo, info);
        strcpy_s(contest.ban, info + strlen(banString[i]));
      }
      deleteString(infoLoc, strlen(info));//从要求中删除放卡ban卡条件
    }
  }
}
//读取大赛信息到Contest数组中
void ReadContestInfo(const char *contestPath)
{
  FILE *f;
  if (fopen_s(&f, contestPath, "r"))
  {
    PopMessage(nullptr, "无法打开输入文件");
    return;
  }
  memset(contest, 0, sizeof(contest));
  contestNum = 0;
  //格式：关卡名+含有人数的字段+要求
  char s[1000];
  for (int code = 0; code < maxContestNum && !feof(f); code++)
  {
    fgets(f, s);
    if (strlen(s) == 0)
      break;

    /*1.匹配人数*/
    char *nextInfo = nullptr;
    char *info = strtok_s(s, "\t", &nextInfo);//关卡名
    if (strstr(info, "单人"))
      contest[code].roleNum = 1;
    else if (strstr(info, "组队"))
      contest[code].roleNum = 2;

    /*2.匹配关卡名*/
    info = strtok_s(nullptr, "\t", &nextInfo);
    contest[code].type = -1;
    //匹配普通关卡
    for (int type = basicTypeNum; type < basicTypeNum + islandTypeNum; type++)
    {
      for (int level = 0; level < levelNum[type]; level++) //遍历关卡
      {
        const char surroudingName[4][4] = { "日", "夜", "陆", "水" };//环境名称
        char levelName[100] = {}, site[100] = {}, surrounding[100] = {};//关卡全称、地点（色拉）、环境（陆）
        GetLevelName(levelName, type, level);//获取关卡全称
        GetLevelName(site, type, level);//地点预设为全称
        char *endOfLevelName = site + strlen(site) - 2;//关卡尾字
        for (int j = 0; j < 4; j++)//根据关卡尾字确定地点名（色拉）和环境名（陆）
          if (strcmp(endOfLevelName, surroudingName[j]) == 0)//如果尾字是环境名
          {
            strcpy_s(surrounding, endOfLevelName);//记录环境名
            site[strlen(site) - 2] = 0;//删除尾字
            break;
          }
        if (strcmp(site, "港口") == 0) //关卡名港口改成玛奇朵，防止匹配不到全称
          strcpy_s(site, "玛奇朵");
        if (strstr(info, site)) //如果地点名（色拉）和环境名（陆）都匹配
          if (strstr(info + strlen(site), surrounding))
          {
            contest[code].type = type; //记录匹配结果
            contest[code].level = level;
            sprintf_s(contest[code].track, "预制轨道\\通用轨道\\%d%s\\%d%s.txt",
              type, typeName[type], level, levelName);
            break;
          }
      }
    }
    //若匹配失败，继续匹配跨服关卡
    if (contest[code].type == -1)
    {
      for (int stage = 0; stage < 6; stage++) //匹配跨服关卡
        for (int level = 0; level < 4; level++)
          if (strstr(info, crossName[stage][level]))
          {
            contest[code].type = 3; //记录匹配结果
            contest[code].level = stage * 8 + level;
            sprintf_s(contest[code].track, "预制轨道\\通用轨道\\3跨服\\%d%s%d.txt",
              contest[code].level, crossMapName[stage], level + 8);
            break;
          }
    }

    /*3.填写附加要求*/
    info = strtok_s(nullptr, "\t", &nextInfo);
    if (info)
    {
      strcpy_s(contest[code].requirement, info);
      strcpy_s(contest[code].remaining, info);
      ExtractRequirement(contest[code], contest[code].remaining);//提取条件
    }

    //4.填写已提取的条件
    int extractedNum = 0;
    for (int j = 0; j < 5; j++)
    {
      char *info = contest[code].gradeInfo + j * sizeof(contest[code].gradeInfo);
      if (strlen(info))
      {
        if (extractedNum > 0)
          strcat_s(contest[code].extracted, ",");
        strcat_s(contest[code].extracted, info);
        extractedNum++;
      }
    }

    //5.删除剩余条件中的顿号
    char *comma = strstr(contest[code].remaining, "、");
    while (comma)
    {
      deleteString(comma, 2);
      comma = strstr(contest[code].remaining, "、");
    }
    //删完还有剩余条件，用小括号括起来
    if (strlen(contest[code].remaining))
    {
      char remaining[100];
      strcpy_s(remaining, contest[code].remaining);
      sprintf_s(contest[code].remaining, "(%s)", remaining);
    }

    //6.检查轨道，填写ban卡列表和异常信息
    const int task = 0;
    char trackInfo[1000];
    bool isBanEnabled = false;//ban卡是否启用
    bool isSlotBanned[maxSlotNum + 1] = {};
    int result = LoadTrack(task, 0, contest[code].track, trackInfo, nullptr, true);
    if (result == -1)
      strcpy_s(contest[code].exception, "无轨道");
    else if (result != 0)
      strcpy_s(contest[code].exception, "轨道有误");
    else //轨道正常时，检查
    {
      if (strlen(contest[code].ban))//如果存在ban卡条件
      {
        for (int order = 1; order <= work[task].slotNum[0]; order++)
        {
          char *name = work[task].slot[0][order].name;
          if (strstr(contest[code].ban, name))//如果轨道中有被ban的卡
          {
            //小火炉和气泡不能ban
            if (strcmp(name, "小火炉") == 0 || strcmp(name, "气泡") == 0)
              strcpy_s(contest[code].exception, "无法禁卡");
            else //否则记录ban卡
            {
              isBanEnabled = true;//记录ban卡已启用
              isSlotBanned[order] = true;
            }
            break;
          }
        }
      }
      //如果存在卡槽数限制，把所有不用的卡都ban了，但不能ban咖啡粉
      if (contest[code].requiredSlotNum)
      {
        isBanEnabled = true;//记录ban卡已启用
        int remainingSlotNum = work[task].slotNum[0];
        for (int order = 1; order <= work[task].slotNum[0]; order++)
        {
          SlotType &slot = work[task].slot[0][order];
          //ban掉所有非咖啡粉的无轨卡
          if (strcmp(slot.name, "咖啡粉") != 0 && slot.moment[1] == 0 && !slot.limit)
            isSlotBanned[order] = true;
          if (isSlotBanned[order])
            remainingSlotNum--;
        }
        //如果剩下的卡槽数仍然大于卡槽数限制
        if (remainingSlotNum > contest[code].requiredSlotNum)
          strcpy_s(contest[code].exception, "卡槽超限");
      }
      //添加ban卡前缀
      if (isBanEnabled)
      {
        char originTrack[maxPath] = {};
        strcpy_s(originTrack, contest[code].track);//拷贝原轨道
        strcpy_s(contest[code].track, "<");//完整路径以"<"开头
        int bannedNum = 0;//已ban数量
        char banString[10] = {};
        for (int order = 1; order <= work[task].slotNum[0]; order++)
        {
          if (isSlotBanned[order])
          {
            if (bannedNum == 0)
              sprintf_s(banString, "%d", order);
            else
              sprintf_s(banString, ",%d", order);
            bannedNum++;
            strcat_s(contest[code].track, banString);
          }
        }
        strcat_s(contest[code].track, ">");
        strcat_s(contest[code].track, originTrack);
      }
    }
    //有异常信息则用中括号括起来
    if (strlen(contest[code].exception))
    {
      char exception[100];
      strcpy_s(exception, contest[code].exception);
      sprintf_s(contest[code].exception, "[%s]", exception);
    }
    contestNum++;
  }
  fclose(f);
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
//由大赛顺序表生成预制列表
void MakeContestList(const char *contestPath, const char *listPath)
{
  LoadCrossLevel();//读取跨服关卡名称
  ReadContestInfo(contestPath);//从大赛顺序表中读取大赛信息

  FILE *fout;
  fopen_s(&fout, listPath, "w");
  fprintf(fout, "列表版本号=%s\n", version);
  for (int i = 0; i < contestNum; i++)
  {
    if (contest[i].type == -1)
    {
      PopMessage(nullptr, "无法打开输入文件");
      return;
    }
    else
    {
      fprintf(fout, "1\t%d\t%d\t1\t1\t%d\t0\n", contest[i].type, contest[i].level, contest[i].roleNum != 1);
      fprintf(fout, "%s\n%s%s%s\n", contest[i].track, contest[i].extracted,
        contest[i].remaining, contest[i].exception);
    }
  }
  fclose(fout);
}
//由单人列表生成双人列表
void Make2PContestList(int list)
{
  for (int order = plot[list].advanceNum - 1; order >= 0; order--)
  {
    if (plot[list].advance[order].deck[1] == 0) //2P卡组为×，则复制一份，房主轨道卡组交换
    {
      CopyAdvance(list, order);
      InsertAdvance(list, plot[list].advanceNum - 1, order + 1);
      ExchangePerformer(list, order + 1);
    }
  }
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
int main(int argc, char *argv[])
{
  DPI = SetDPIAware();//设置DPI感知并获取DPI
  ParameterShifter();//进行配置迁移

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
    if (fileType == 1) //如果文件类型是轨道
      strcpy_s(param[0].track[0], shortPath);//填入任务0轨道
    else //如果文件类型是列表
    {
      mode = 2;
      pointPosted.y = 2;//点击位置为(0,2)
      strcpy_s(openedListPath, shortPath);//记录列表路径
    }
    SaveParameter();
  }
  _getcwd(workDirect, sizeof(workDirect));//获取工作目录

  /*检查依赖文件完整性：附加程序和执行记录*/
  if (FileExist("..\\开发者文件.txt"))//如果上级目录有开发者文件
  {
    MakeBitmapList();//生成图片目录
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

  InitMonitorSetting();

  //创建本程序的互斥量
  char wndTitle[100];//窗口标题
  sprintf_s(wndTitle, versionString, version, folderName);//设置窗口标题
  char mutexName[maxPath];
  sprintf_s(mutexName, "ACTUATOR_%s", folderName);//用文件夹名称命名互斥量

  HANDLE hmutex = CreateMutexA(NULL, FALSE, mutexName);//创建互斥量
  //如果互斥量已存在，说明同一目录下的执行器已打开，不允许重复启动
  if (hmutex && GetLastError() == ERROR_ALREADY_EXISTS)
  {
    hWndActuator = FindWindow("EasyXWnd", wndTitle);//查找窗口标题
    if (hWndActuator) //如果找到了，恢复窗口
    {
      if (argc > 1) //拖动文件到执行器上
      {
        LeftClick(hWndActuator, pointPosted.x, pointPosted.y);//发送点击指令
        RecoverWindow_NoMove(hWndActuator);//恢复隐藏的窗口，若窗口未被隐藏则不改变位置
      }
      else //直接启动执行器
        RecoverWindow(hWndActuator);//恢复隐藏的窗口并居中
      SetForegroundWindow(hWndActuator);
    }
    //如果没找到，说明已开执行器还没加载窗口，不做任何操作
    return 0;//不允许多开
  }
  else //如果执行器没打开
    ClearBackupAndLog();//清理之前的自动备份

  //能运行到这里，说明启动的是执行器主程序而不是唤出程序
  CreateBackupFolder();

  //FindBase();//求解一类物品的底色
  //AutoClassification();
  //return 100;

  LoadVip();//载入VIP图片
  LoadRoleName();//载入已记录的角色名
  LoadStar();//载入星级图像
  LoadItem();//载入自定图像
  LoadBanner();//载入关卡旗帜
  LoadBase();//载入物品底色
  WriteCycleArea();//填写“加载中”圆环区域
  LoadLoot();//载入战利品
  LoadLootDigit();//载入物品数字
  LoadLootScroll();//载入战利品标准滚动条
  LoadCard();//载入自定卡片
  LoadMission();//载入公会任务名图像
  LoadDigit();//载入数字图像
  LoadPicture();//载入用于识别界面的位图
  LoadCardTemplate();//载入标准卡片模板
  LoadNormalLevel();//读取普通关卡列表
  LoadProgress();//载入进度条颜色
  LoadMaltoseCoverage();//载入麦芽糖覆盖区域
  LoadMusicInnateGrid();//载入音乐节夜初始地图
  LoadDragon();//载入龙图
  hMutexLoot = CreateMutexA(NULL, FALSE, NULL);//创建战利品读写互斥锁
  hMutexProp = CreateMutexA(NULL, FALSE, NULL);//创建道具读写互斥锁
  for (int list = 0; list < listNum; list++) //创建刷新记录互斥锁
    plot[list].hMutexRefreshLog = CreateMutexA(NULL, FALSE, NULL);

  //MakeContestList("原始列表_奇境1期.txt", "美食大赛_奇境1期.txt");
  //return 114514;

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
  //载入5个高级任务的列表
  for (int list = 0; list < listNum; list++)
  {
    plot[list].advancePage = 1;
    plot[list].outputPage = 1;
    LoadAdvance(list);
    if (isMissionExisted(list)) //如果列表中存在公会任务
    {
      SaveBackup(list, 2);//保存至2.txt
      MissionReturn(list);//还原公会任务
      SaveBackup(list, 1);//保存至1.txt
      LoadBackup(list, 2);//载入2.txt，CurrentBackup=2
      plot[list].backupNum = 2;//备份数量设为2
    }
    SaveAdvance(list);//启动时保存任务列表，形成备份
  }
  for (int task = 0; task < maxTaskNum; task++)
    isHallModified[task] = 1;
  OpenGraph_Fit();
  hWndActuator = GetHWnd();
  SetWindowLongPtr(hWndActuator, GWLP_WNDPROC, (LONG_PTR)MyWindowProc); // 设置自定义窗口过程
  EnableDragDropForHighIntegrity(hWndActuator);
  DragAcceptFiles(hWndActuator, TRUE);//允许窗口接收文件
  RecoverWindow(hWndActuator);
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