#ifndef _DEBUG
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")
#endif
#include "resource.h"
#include "..\\track.h"
const char versionString[] = "轨道编辑器%s by -云涛晓雾-";

//1. 编辑器位置常数
const int wndWidth = 1340, wndHeight = 775;//窗口宽度和高度
const int tipHeight = 27;//提示高度
const int recordWndWidth = 262, recordWndHeight = 775;//录制轨道时的新窗口尺寸
const int maxCommonNum = 260, maxSlotNum = 21, minTime = 1, maxTime = 960;//最大背包容量、卡槽数；最小、最大轨道时间
const int modeX = 1188, modeY = 30, modeWidth = 112, modeHeight = 45;//选项卡位置
const int zoomX = modeX, zoomY = 0, zoomWidth = modeWidth, zoomHeight = modeY;//缩放按钮
const int roleSlotX = 40, roleSlotY = 30, roleSlotWidth = 70, roleSlotHeight = 89;
const int mapSlotX = roleSlotX + roleSlotWidth, mapSlotY = roleSlotY, mapSlotWidth = roleSlotWidth, mapSlotHeight = roleSlotHeight;//阵型编辑卡槽
const int mapGridX = 279, mapGridY = 140, mapGridWidth = 88, mapGridHeight = 86;//地图格子位置
const int bagX = 40, bagY = mapGridY, bagWidth = 110, bagHeight = 40;//背包卡位置
const int productionX = bagX, productionY = bagY, productionWidth = bagWidth, productionHeight = bagHeight;//生产信息显示
const int bagOptionX = bagX, bagOptionY = mapGridY + 7 * mapGridHeight - 40, bagOptionWidth = bagWidth, bagOptionHeight = 40;//背包选项
const int recordX = 20, recordY = 110, recordHeight = 30;//录制信息
const int recordPageCX = recordWndWidth / 2, recordPageCY = recordY + 20 * recordHeight + 20;//录制翻页
const int recordEndCX = recordWndWidth / 2, recordEndCY = 40;//结束录制
const int timelineX = 175, timelineY = 50, timelineWidth = 16, bigScale = 10;//时间轴；1s宽度；大刻度间距
const int trackX = timelineX - timelineWidth / 2, trackY = 127, trackWidth = timelineWidth, trackHeight = 41;//轨道
const int trackNum = 61;//一页的轨道数量
const int undoX = modeX, undoY = mapGridY + 7 * mapGridHeight - 2 * trackHeight, undoWidth = modeWidth, undoHeight = trackHeight;//撤销按钮
const int waveX = modeX, waveY = mapGridY + trackHeight, waveWidth = modeWidth / 4, waveHeight = 30;
const int buttonX = modeX, buttonY = waveY + 3 * waveHeight, buttonWidth = modeWidth, buttonHeight = trackHeight;//自动布轨按钮
const int trackSlotX = 40, trackSlotY = trackY, trackSlotWidth = trackX - trackSlotX, trackSlotHeight = trackHeight;//轨道编辑卡槽
const int pageX = modeX, pageY = mapGridY + 3 * waveHeight + 6 * trackHeight + 20, pageWidth = modeWidth / 4, pageHeight = 30;//翻页框
const int stackX = modeX, stackY = 331, stackWidth = modeWidth, stackHeight = 31;//堆叠区
const int moveX = trackSlotX, moveY = 30, moveWidth = 26, moveHeight = 42;//轨道编辑方向键
const int playerX = 140, playerHeight = roleSlotY, playerTitleWidth = 40, playerOptionWidth = 60, playerDistance = 480;//[1P]字样宽度；[1P]和[2P]的距离
const int skillX = playerX + 2 * playerDistance - 25, skillY = 5, skillWidth = 20, skillHeight = 20;//刷技能勾选框
const int queueTitleX = trackSlotX, queueTitleWidth = trackSlotWidth;//优先队列标题
const int queueX = trackX + trackWidth, queueY = trackY - 32, queueWidth = 80, queueHeight = 32;//优先队列
//2. 生产信息常数
const int fireworkYield[17] = { 450, 486, 522, 558, 612, 666, 738, 810, 882, 954, 1098, 1242, 1386, 1602, 1818, 2250, 2700 };//花火龙产值
const int rabbitFactor[2][17] = {//不转和一转炎焱兔的增幅倍数*100
  { 110, 115, 120, 125, 130, 135, 140, 145, 150, 160, 170, 180, 200, 220, 240, 260, 280 },
  { 140, 145, 150, 155, 160, 165, 170, 175, 180, 190, 200, 210, 230, 250, 270, 290, 310 } };
int initialEnergy, isDay, InnateFire, InnateCup;//初始火苗，是否为白天，初始小火，初始酒杯
const int weaponYield[6][16] = {//产火宝石单轮产量
  { 25, 25, 25, 50, 50, 50, 75, 75, 75, 100, 125, 130, 135, 145, 155, 165 },//回火
  { 20, 25, 30, 40, 50, 60, 69, 78, 90, 120, 180, 200, 220, 240, 260, 280 },//产能
  { 5, 10, 15, 20, 25, 30, 40, 50, 60, 80, 100, 110, 120, 130, 140, 150 },//生产
  { 6, 12, 18, 24, 30, 36, 48, 60, 72, 96, 120, 130, 140, 150, 160, 180 },//光能
  { 20, 30, 40, 50, 60, 75, 90, 105, 120, 135, 150, 170, 190, 210, 230, 260 },//神佑
  { 5, 10, 15, 20, 25, 30, 40, 50, 60, 80, 100, 105, 110, 115, 125, 140 } };//转化
const int weaponPeriod[6][16] = {//产火宝石生产间隔
  { 40, 40, 35, 40, 35, 30, 35, 30, 20, 20, 15, 15, 15, 15, 15, 15 },//回火
  { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },//产能
  { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },//生产
  { 15, 15, 15, 14, 14, 14, 13, 13, 12, 11, 10, 10, 10, 10, 10, 10 },//光能
  { 15, 15, 15, 14, 14, 14, 13, 13, 12, 11, 10, 10, 10, 10, 10, 10 },//神佑
  { 20, 20, 19, 19, 18, 17, 17, 15, 14, 13, 11, 10, 9, 8, 7, 6 } };//疾风

//3. 颜色信息常数
const COLORREF defaultFillColor = RGB(30, 30, 30);//默认填充色
//正常轨道、紧凑轨道间隙、冷却冲突轨道间隙、极限轨道
const COLORREF normalTrack = RGB(0, 192, 255), closeGap = RGB(0, 96, 128), errorGap = RGB(144, 0, 0),
limitTrack = RGB(64, 128, 0), retainTrack = RGB(128, 128, 0), anotherTrack = RGB(192, 0, 255),
indefiniteTrack = RGB(128, 255, 0);
const COLORREF roleColor = RGB(255, 255, 0), productColor = RGB(255, 255, 0);//人物，生产信息
const COLORREF coldLine[2] = { RGB(255, 255, 0), RGB(170, 170, 0) };//冷却提示线
const COLORREF levelColor[7] = { RGB(255, 255, 0), RGB(255, 128, 0), RGB(0, 255, 0), RGB(255, 255, 255),
RGB(255, 0, 255), RGB(0, 224, 255), RGB(0, 224, 255) };//人物，承载，护罩，普通，升级，冷却，特殊
const COLORREF darkLevelColor[7] = { RGB(128, 128, 0), RGB(128, 64, 0), RGB(0, 128, 0), RGB(128, 128, 128),
RGB(128, 0, 128), RGB(0, 112, 128), RGB(0, 112, 128) };//人物，承载，护罩，普通，升级，冷却，特殊

//4. 编辑器参数
int curAccount = 0;//正在编辑的人物
int accountNum = 1;
int totalWaveExist[2][maxTotalWave];//两个账号的第0-13波是否存在（开局时填好）
int curTotalWave[2];//两个账号正在编辑的波数
char notation[2][1000];//两个账号的注释
char wndTitle[100];//编辑器标题
const int maxStack = 99;//堆叠容量
const int maxViewStack = 9;//堆叠显示容量
const int trackSlotNumPerPage = 14;//每页显示的卡槽数量
IMAGE imageBackup;//编辑器图像备份
int commonPage = 1;//防御卡背包当前页数
int lastClickMessage;//上次鼠标点击信息：单击1xxxxyyyy，右击2xxxxyyyy，无消息0
char lastTip[200];//上一次显示的提示（用于判断要不要更新提示）
HWND hWndEditor;
HCURSOR crossCursor;//十字光标
char tracePrint[1000];//调试输出
int trackSlotSelectionTick;//选中轨道卡槽时刻，用于双击判定
int sleepTime;//等待时间
int mode = 1;//1=阵型编辑；2=轨道编辑；3=录制轨道
int bagMode = -1;//-1=显示防御卡；-2=显示生产信息；0-99=正在编辑的背包防御卡序号
int page = 1;//当前显示轨道页数（1页=1分钟）
int recordPage = 1;//当前显示录制的页数（1页=20条）
//1=阵型编辑;2=轨道编辑;401-410=翻页;501=503=设置三大时间;2000-2600=时间轴,600-601==移动轨道
int state;//0=未选中任何格子，101-114=选中卡槽，300=选中人物，501-503=设置三大时间,1000+=选中地图层级
int isStateRetained;//是否保持状态不清零
int trackSlotSelected;//选中轨道卡槽
bool isTrackSlotSelectionRetained;//轨道卡槽的选中状态是否需要保留
int queueSelected;//选中的队列（712=标题，700-711=第0-11张卡）
bool isQueueSelectionRetained;//队列的选中状态是否需要保留
int firstTrackSlot = -1;//轨道编辑界面显示的第一条轨道
int timeSelected;//选中的时间
char runTimeString[30];//本次启动的时间
int currentBackup, backupNum;//当前备份数，总备份数
float ratio, originRatio;//窗口缩放比例。ratio=1.25意味着分辨率太高，点击位置判定偏大，需要除以该比例。
int zoom;//缩放比例

const char modeString[2][13] = { "阵型编辑", "轨道编辑" };
const char levelName[7][7] = { "无", "承载", "护罩", "普通", "升级", "冷却", "特殊" };
const char styleName[9][10] = { "非生产", "无秒产", "火炉型", "酒杯型", "四转太", "单轮型", "增幅型", "同小火", "同酒杯" };

//放卡条件
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
struct SlotType  //卡牌类型
{
  /*卡牌信息*/
  char name[10];//卡牌名称
  int cd;//卡牌cd
  int level;//卡牌所属层级：铲子0，承载1，护罩2，普通3，升级4，冷却5，特殊6
  int cost;//耗能（275+用1275表示）
  int style;//生产类型
  int star;//星级（决定生产间隔和火兔增幅）
  int sun;//单朵火苗数值
  int sunNum;//火苗数量
  int transfer;//转职
  /*轨道状态*/
  bool prior;//是否绝对优先
  bool lock;//是否锁定轨道。锁定的卡片不参与自动布轨
  bool repair;//是否自动补阵
  bool limit;//是否极限成阵
  bool independent;//是否独立计时
  bool search;
  bool retain;//是否保持上一波
  /*放卡条件*/
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
const SlotType emptyCard;//空卡（定义的时候各项都是0）
const SlotType shovel = { "铲子", 0, 0, 0, 0, 0, 0, 0, 0 };
const SlotType jewel = { "宝石", 120, 0, 0, 0, 0, 0, 0, 0 };
//地图格子类型
struct GridType
{
  int depthSelected[7];//6个层级选中的深度，默认为0
  int depth[7];//6个层级的堆叠数量
  char name[7][maxStack][10];//6个层级(普通，护罩，承载，升级，冷却，特殊)，9个深度的卡牌名称
  int times[7][maxStack];//6个层级，99个深度卡牌次序
};
//录制卡片类型
struct RecordType
{
  int order;//卡片序号
  int time;//种植时间(0.1秒)
  int loc;//种植位置
  int exist;//是否存在
};

//5. 轨道参数
char fullTrackPath[2][maxPath];//轨道文件完整路径
char shortTrackPath[2][maxPath];//轨道文件相对路径
char trackName[2][maxPath];//轨道文件名
char tempTrackForSave[maxPath];//SaveTrack用的临时轨道文件
int tower[2];//魔塔层数
int quitTime[2];//退出时间
bool isSkillMode;//是否刷技能模式
int roleLocNum[2];//两个人物的放置次数，
int roleLoc[2][63];//人物1/2位置（第一个位置存放在roleLoc[account][1]）
int slotNum[2];//卡槽数
int realSlotNum[2];//（录制轨道）游戏中识别到的卡槽数
SlotType slot[2][maxSlotNum + 2];
GridType map[2][8][10];//地图数据：每个格子的种卡信息
int location[2][maxSlotNum + 2][maxPlantTimes + 2];//卡片种植位置
int track[2][maxSlotNum + 2][maxTime + 1];//轨道数据：第i张卡第j秒有放置则为1，否则为0
int moment[2][maxSlotNum + 2][maxPlantTimes + 2];//卡片种植时刻，单位0.1s
int trackBackup[2][maxSlotNum + 2][maxTime + 1];//备份轨道
int momentBackup[2][maxSlotNum + 2][maxPlantTimes + 2];//备份放卡时刻

const int maxQueueSize = 12;//优先队列容量
char queue[2][maxQueueSize][10];//放卡优先队列（记录卡片名）
int queueSize[2];//优先队列大小

//6. 其他参数
int bagNum[2];//背包卡数量
SlotType bag[2][maxCommonNum + 1];
int weapon[2][8];//武器信息：回火，产能，生产，光能，神佑，疾风，转化的星级；榴弹类型
int trackType[maxSlotNum + 2][maxTime + 1];//轨道颜色类型：1=正常轨道，2=紧凑轨道间隙，3=冷却冲突轨道间隙

//布轨条件类型
struct ConstraintType
{
  int order;//编号
  int cost;//前n张卡的总耗能
  int maxTime;//目标放置时间
  int maxControllableTime;//最大可控时间（此时间后放的生产卡无法促进目标达成）
};
const int maxProgrammingTime = 120;//最大规划时间
const int maxConstraintNum = 4;//最大限制条件数量
ConstraintType constraint[maxConstraintNum];//限制条件（按目标时间从小到大排序）
int constraintNum;
struct OptimizationType
{
  int order;//编号
  int cost;//耗能
  int minAchievingTime;//当前达成时间
};
OptimizationType minCard;
//自动布轨面板
struct AutoTableType
{
  char card[maxConstraintNum][10];//卡片名
  int minTime[maxConstraintNum];//最小放卡时间
  int maxTime[maxConstraintNum];//最大放卡时间
  char minCard[10];//优化卡片
};
AutoTableType autoTable;//自动布轨面板


//账号account名称为name的卡在几号卡槽？不在返回-1
int GetOrder(int account, const char *name)
{
  for (int order = 0; order <= slotNum[account]; order++)
    if (strcmp(slot[account][order].name, name) == zero)
      return order;
  return -1;
}
//获取人物数量
int GetRoleNum()
{
  return (roleLocNum[0] > 0) + (roleLocNum[1] > 0);
}
//从Loc中搜索：卡片order一共种植了几次
int GetLocTimes(int account, int order)
{
  for (int times = 1; times <= maxPlantTimes; times++)
    if (location[account][order][times] == zero)
      return times - 1;
  return maxPlantTimes;
}
//从Moment中搜索：卡片order在第t秒及以前种植了几次
int GetMomentTimes(int account, int order, int t = maxTime)
{
  //搜索到超过t秒的时刻或Moment已空
  for (int times = 1; times <= maxPlantTimes; times++)
    if (moment[account][order][times] / 10 > t || moment[account][order][times] == zero)
      return times - 1;
  return maxPlantTimes;
}
//从Moment中搜索：卡片order第times次种植是什么时候，没找到则返回0
int GetTime(int account, int order, int times)
{
  return moment[account][order][times] / 10;
}
//获得卡片order的最大轨道时间，没有轨道返回0
int GetMaxTrack(int account, int order)
{
  for (int times = 1; times < maxPlantTimes; times++)
    if (moment[account][order][times] == zero)
    {
      if (times == 1)
        return 0;
      else
        return moment[account][order][times - 1] / 10;
    }
  return 0;
}
//获得所有卡片的最大轨道时间，没有轨道返回0
int GetMaxTrack(int account)
{
  for (int t = maxTime; t > zero; t--)//从960s往0s找
    for (int order = 0; order <= slotNum[account]; order++)//如果有一张卡有轨道，返回t
      if (track[account][order][t] >= 1)
        return t;
  return 0;//找不到轨道返回0
}
//magicTime时刻的幻幻鸡复制的是几号卡、第几次种植？未指定返回0
void GetMagicTarget(int account, int magicTime, int &target, int &times)
{
  //本函数只根据Track数组判断
  target = times = 0;
  if (magicTime <= zero || magicTime > maxTime)
    return;//用超出MaxTime的时间查找，返回0
  int magic = GetOrder(account, "幻幻鸡");
  if (magic == -1)
    return;//卡槽没有幻幻鸡返回0
  int ice = GetOrder(account, "冰激凌");
  int sorbet = GetOrder(account, "冰沙");

  for (int t = magicTime; t >= minTime; t--)
    if (t == magicTime)//对于与幻幻鸡同轨卡片，只考虑卡槽序号在幻幻鸡之前的
    {
      for (int order = GetOrder(account, "幻幻鸡") - 1; order >= 1; order--)
        if (track[account][order][t] && order != ice && order != sorbet)
        {
          target = order;
          times = GetMomentTimes(account, target, t);
          return;
        }
    }
    else //对于幻幻鸡轨道之前的卡片，则倒序考虑所有卡槽，但是不考虑冰激凌、冰沙、幻幻鸡
    {
      for (int order = slotNum[account]; order >= 1; order--)
        if (track[account][order][t] && order != magic && order != ice && order != sorbet)
        {
          target = order;
          times = GetMomentTimes(account, target, t);
          return;
        }
    }
  //若程序运行结束，没有找到复制目标的轨道，则target和times都保持为0
}
//magicTime时刻的幻幻鸡种植的是几号卡牌？未指定返回0
int GetMagicTarget(int account, int magicTime)
{
  //全局寻找时间最接近MagicTime且早于MagicTime的卡牌
  int target, times;
  GetMagicTarget(account, magicTime, target, times);
  return target;
}
//row行column列time时刻种的冰激凌冷却了几号卡？空冷却返回0
int GetIceTarget(int account, int row, int column, int time)
{
  //算法：返回该格目前最上层的卡片
  int magic = 0;//幻幻鸡目标
  int BestScore = 0, BestOrder = 0, BestAccount = account, score;
  //得分=层级*10000+时间*10+非幻鸡1(幻鸡0)，大者胜出。时间晚于times+1者记为0
  for (int mapAccount = 0; mapAccount < accountNum; mapAccount++)//遍历两个账号的地图
    for (int level = 1; level <= 6; level++)//遍历该行该列每个层级的卡片
      for (int depth = 0; depth < map[mapAccount][row][column].depth[level]; depth++)//遍历每一深度卡片
      {
        int order = GetOrder(mapAccount, map[mapAccount][row][column].name[level][depth]);//这张卡在第几个卡槽？
        int t = GetTime(mapAccount, order, map[mapAccount][row][column].times[level][depth]);//这张卡何时种植？
        if (t == zero || t > time + 1 || level == 5)//晚于冰激凌种植时刻，或层级为5，则不用考虑
          score = 0;
        else if (level == 6) //不属于以上情况，考虑幻幻鸡层级
        {
          magic = GetMagicTarget(mapAccount, t);//获得幻幻鸡复制目标序号
          score = slot[mapAccount][magic].level * 10000 + t * 10;
          if (score > BestScore)
          {
            BestScore = score;
            BestOrder = magic;
            BestAccount = mapAccount;
          }
        }
        else//如果是1-4层级
        {
          score = level * 10000 + t * 10 + 1;
          if (score > BestScore)
          {
            BestScore = score;
            BestOrder = order;
            BestAccount = mapAccount;
          }
        }
      }
  if (BestAccount == account)//如果冷却的卡是本账号的卡，直接返回序号
    return BestOrder;
  else //如果冷却的卡是另一个账号的卡，检查本账号有没有这张卡
  {
    int ThisOrder = GetOrder(account, slot[BestAccount][BestOrder].name);
    if (ThisOrder > 0) //有则返回本账号的卡
      return ThisOrder;
  }
  return 0;
}
//选择地图中选中层级的显示深度
void SelectDepth(int row, int column, int level, int DepthLabel)
{
  if (DepthLabel == zero)
    map[curAccount][row][column].depthSelected[level] = 0;//显示最低深度（0深度）
  else if (DepthLabel == 1)
    map[curAccount][row][column].depthSelected[level] = max(0, map[curAccount][row][column].depth[level] - 1);//显示最高深度，最少为1
  else if (DepthLabel == 2)//显示最低空深度
    map[curAccount][row][column].depthSelected[level] = min(maxStack - 1, map[curAccount][row][column].depth[level]);//显示最低空深度，但最多为8
}
//选择全图指定层级的显示深度
void SelectDepth(int level, int DepthLabel)
{
  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 9; column++)
      SelectDepth(row, column, level, DepthLabel);
}
//选择全图的显示深度（0=最低，1=最高，2=空）
void SelectDepth(int DepthLabel)
{
  for (int level = 0; level <= 6; level++)
    SelectDepth(level, DepthLabel);
}
void DecreaseToEmptyDepth()//降低到最低空深度（删除卡槽时执行）
{
  int row, column, level;
  for (row = 1; row <= 7; row++)
    for (column = 1; column <= 9; column++)
      for (level = 0; level <= 6; level++)
        if (map[curAccount][row][column].depthSelected[level] > map[curAccount][row][column].depth[level])//如果深度高于最低空深度
          map[curAccount][row][column].depthSelected[level] = map[curAccount][row][column].depth[level];//显示最低空深度
}
//生产计算与自动布轨

//炎焱兔类型
struct RabbitType
{
  int row, column;//放置位置（用于检查是否被铲除）
  bool isExist;//是否存在
  int amplify[8][10];//各格增幅倍数
};
const int maxRabbitNum = 100;//最大炎焱兔数量
int rabbitNum[2];//当前炎焱兔数量
RabbitType rabbit[2][maxRabbitNum];//炎焱兔
int amplify[8][10];//每个格子的增幅倍数
//每个格子的增幅倍率预设为1倍
void ResetAmplify(int(&amplify)[8][10])
{
  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 9; column++)
      amplify[row][column] = 100;
}
//添加炎焱兔
void AddRabbit(int account, int order, int rabbitRow, int rabbitColumn)
{
  if (rabbitNum[account] >= maxRabbitNum)
    return;
  SlotType &slot0 = slot[account][order];
  RabbitType &rabbit0 = rabbit[account][rabbitNum[account]++];
  rabbit0.row = rabbitRow;
  rabbit0.column = rabbitColumn;
  rabbit0.isExist = true;
  int radius = slot0.transfer == 2 ? 2 : 1;//炎焱兔作用半径
  int isTransfered = slot0.transfer >= 1 ? 1 : 0;//炎焱兔是否转职

  ResetAmplify(rabbit0.amplify);//每个格子的增幅预设为1倍
  //填写炎焱兔作用范围内的增幅倍率
  for (int row = rabbitRow - radius; row <= rabbitRow + radius; row++)
    for (int column = rabbitColumn - radius; column <= rabbitColumn + radius; column++)
      if (row >= 1 && row <= 7 && column >= 1 && column <= 9)
        rabbit0.amplify[row][column] = rabbitFactor[isTransfered][slot0.star];
}
//使用最近加入的炎焱兔更新增幅表（用于新增炎焱兔时）
void UpdateLatestRabbit(int account)
{
  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 9; column++)
      amplify[row][column] = max(amplify[row][column], rabbit[account][rabbitNum[account] - 1].amplify[row][column]);
}
//从头更新炎焱兔增幅表（用于删除炎焱兔时）
void UpdateRabbit()
{
  ResetAmplify(amplify);//每个格子的增幅预设为1倍
  //每个格子的增幅倍率取炎焱兔倍率的最大值
  for (int account = 0; account < accountNum; account++)
    for (int i = 0; i < rabbitNum[account]; i++)
      if (rabbit[account][i].isExist)
        for (int row = 1; row <= 7; row++)
          for (int column = 1; column <= 9; column++)
            amplify[row][column] = max(amplify[row][column], rabbit[account][i].amplify[row][column]);
}
//生产卡类型
struct ProducerType
{
  int row, column;//放置位置（用于检查是否可被炎焱兔增幅）
  bool isExist;//是否存在
  bool isFirework;//是否花火龙
  unsigned short sunNum[maxTime + 1];//每一秒的吐火朵数
  unsigned short sun[maxTime + 1];//每一秒的单朵火苗数量
};
const int maxProducerNum = 500;//最大生产卡数量
int producerNum[2];//当前生产卡数量
ProducerType producer[2][maxProducerNum];//生产卡
const int fireworkDelay[9] = { 10, 10, 9, 9, 8, 7, 6, 5, 3 };//花火龙生产延迟
//添加一张生产卡（不包含花火龙，因为花火龙自动收集，只能单独计算）
void AddProducer(int account, int order, int row, int column, int plantTime)
{
  if (producerNum[account] >= maxProducerNum)
    return;
  SlotType &slot0 = slot[account][order];
  ProducerType &pro = producer[account][producerNum[account]++];
  pro.row = row;
  pro.column = column;
  pro.isExist = true;
  //填写吐火轴
  memset(pro.sunNum, 0, sizeof(pro.sunNum));
  memset(pro.sun, 0, sizeof(pro.sun));
  //花火龙
  if (slot0.style == 5)
  {
    pro.isFirework = true;
    //花火龙的sumNum是技能等级
    int spitTime = plantTime + fireworkDelay[slot0.sunNum];
    if (spitTime <= maxTime - 1)
      pro.sun[spitTime] = slot0.transfer == 2 ? fireworkYield[slot0.star] * 2 : fireworkYield[slot0.star];
    if (plantTime <= maxTime - 1)
      pro.sun[plantTime] = slot0.transfer == 0 ? 0 : 150;
    return;
  }
  pro.isFirework = false;
  int period = 25 - slot0.star;//生产周期
  int startTime = plantTime + 2;//首轮产火时间
  //非秒产卡或(period - 2)秒前放置的次轮秒产卡：首轮产火时间推迟一个周期
  if (slot0.style == 1 || (slot0.style >= 2 && slot0.style <= 3 && plantTime <= period - 2))
    startTime += period;
  int sun = GetRoleNum() == 2 ? slot0.sun * 3 / 5 : slot0.sun;//双人只有3/5的产量
  int growth = GetRoleNum() == 2 ? 6 : 10;//酒杯灯长大后的火苗增值
  if (slot0.style == 3)//酒杯灯：放置后0-89秒正常生产，90秒后产量+10
  {
    for (int t = startTime; t <= maxTime; t += period)
    {
      pro.sunNum[t] = slot0.sunNum;
      if (t <= plantTime + 89)
        pro.sun[t] = sun;
      else
        pro.sun[t] = sun + growth;
    }
  }
  else //其他卡：不会变
  {
    for (int t = startTime; t <= maxTime; t += period)
    {
      pro.sunNum[t] = slot0.sunNum;
      pro.sun[t] = sun;
    }
  }
}
//添加固有生产卡
void AddInnateProducer(int account, int order, int innateOrder)
{
  if (order > 0 && innateOrder > 0)
  {
    for (int i = 1; i <= maxPlantTimes; i++)
    {
      int loc = location[account][innateOrder][i];
      if (loc == 0)
        break;
      int row = loc / 10;
      int column = loc % 10;
      AddProducer(account, order, row, column, 0);
    }
  }
}
int times[2][maxSlotNum + 1];//0-21槽的已放置次数
int cardSpit[2][maxTime + 1];//每一秒的卡片吐火
int weaponSpit[2][maxTime + 1];//每一秒的武器吐火
int skySpit[maxTime + 1];//每一秒的天空吐火

int spit[2][maxTime + 1];//由nP卡片、武器（1P还包括天空）产生的吐火量
int collect[2][maxTime + 1];//从nP吐火的收火量
int cost[2][maxTime + 1];//消耗量
int energy[2][maxTime + 1];//当前能量（用于显示）
int totalCollect[2][maxTime + 1];//从nP吐火的累计收火（用于显示）
int totalCost[2][maxTime + 1];//累计消耗（用于显示）
int rabbitOrder[2];//炎焱兔编号
//填写单个武器吐火轴
void FillSingleWeaponSpit(int account, int yield, int period, int offset)
{
  for (int t = period + offset; t <= maxTime; t += period)
    weaponSpit[account][t] += yield;
}
//填写武器吐火轴
void FillWeaponSpit(int account)
{
  memset(weaponSpit[account], 0, sizeof(weaponSpit[account]));
  //0回火、1产能、2生产、3光能、4神佑、5疾风、6转化
  for (int i = 0; i <= 5; i++)
  {
    //决定产量的宝石星级。计算风化时，要用转化的星级。
    int yieldStar = i == 5 ? weapon[account][6] : weapon[account][i];
    if (yieldStar == -1)//没有产火宝石则跳过
      continue;
    int yield = weaponYield[i][yieldStar];//产量
    if (i == 5) //榴弹炮根据榴弹类型计算产量
      yield *= weapon[account][7];
    else if (i == 0 && GetRoleNum() == 2)//回火宝石双人模式衰减
      yield = yield * 3 / 5;

    int periodStar = weapon[account][i];//决定周期的宝石星级
    int period = (periodStar == -1) ? 20 : weaponPeriod[i][periodStar];//生产周期（榴弹无疾风为20秒）
    int offset = i == 5 ? 3 : 0;//风化要多等3秒才能砸出火，其余武器都是在周期整数倍吐火
    FillSingleWeaponSpit(account, yield, period, offset);
  }
}
//填写天空吐火轴
void FillSkySpit()
{
  memset(skySpit, 0, sizeof(skySpit));
  if (isDay == zero)
    return;
  int sun = GetRoleNum() == 1 ? 25 : 15;//单次吐火
  for (int t = 7; t <= maxTime; t += 7)//每7秒吐火一次
    skySpit[t] = sun;
}
//计算第t秒收火、消耗和能量值
void FillEnergy(int t)
{
  for (int account = 0; account < accountNum; account++)
  {
    //本秒来自nP的收火：第3n+1秒收集第3n-3~3n-1秒的吐火，其他时刻无收火
    if (t >= 4 && t % 3 == 1)
      collect[account][t] += spit[account][t - 4] + spit[account][t - 3] + spit[account][t - 2];

    //nP累计收火=上一秒nP累计收火+本秒nP收火
    totalCollect[account][t] = totalCollect[account][t - 1] + collect[account][t];

    //记录本秒放卡消耗（无论是否有阵型），不会更新times[account][order]
    for (int order = 1; order <= slotNum[account]; order++)
    {
      SlotType &slot0 = slot[account][order];
      if (track[account][order][t])//如果这一秒有轨道
      {
        if (slot0.cost >= 1000) //带+的卡，每放置一次+50火
          cost[account][t] += slot0.cost - 1000 + 50 * times[account][order];
        else
          cost[account][t] += slot0.cost;
      }
    }
    //nP累计消耗=上一秒nP累计消耗+本秒nP消耗
    totalCost[account][t] = totalCost[account][t - 1] + cost[account][t];
  }
  for (int account = 0; account < accountNum; account++)
  {
    //nP总能量=上一秒nP总能量+本秒全场收火-本秒nP消耗
    energy[account][t] = energy[account][t - 1]
      + collect[0][t] + (accountNum == 1 ? 0 : collect[1][t]) - cost[account][t];
  }
}

//更新第t秒放置的生产卡，计算本秒吐火
void FillSpit(int t)
{
  //第一步：添加所有生产卡（如果是升级卡，记录本格已有生产卡消失），统计本秒吐火
  for (int account = 0; account < accountNum; account++)
  {
    for (int order = 1; order <= slotNum[account]; order++)
    {
      SlotType &slot0 = slot[account][order];
      if (track[account][order][t])//如果这一秒有轨道
      {
        times[account][order]++;//统计次数+1
        int loc = location[account][order][times[account][order]];
        if (loc > 0)
        {
          int row = loc / 10;
          int column = loc % 10;
          //如果放置的是升级卡，本格所有已放卡消失
          if (slot0.level == 4)
            for (int i = 0; i < producerNum[account]; i++)
              if (producer[account][i].row == row && producer[account][i].column == column)
                producer[account][i].isExist = false;
          //如果是生产卡，进行添加
          if (slot0.style >= 1 && slot0.style <= 5)
            AddProducer(account, order, row, column, t);
          else if (slot0.level == 6) //如果是幻鸡
          {
            int targetOrder = GetMagicTarget(account, t);//获得幻鸡的复制目标
            if (targetOrder > 0) //如果存在复制目标
            {
              SlotType &target = slot[account][targetOrder];//幻鸡目标
              //如果放置的是升级卡，本格所有已放卡消失
              if (target.level == 4)
                for (int i = 0; i < producerNum[account]; i++)
                  if (producer[account][i].row == row && producer[account][i].column == column)
                    producer[account][i].isExist = false;
              //如果是常驻生产卡，在4秒后添加生产卡
              if (target.style >= 1 && target.style <= 5)
                AddProducer(account, targetOrder, row, column, t + 4);
            }
          }
        }
      }
    }
    //添加完生产卡后，计算本秒卡片吐火（计算炎焱兔增幅）
    for (int i = 0; i < producerNum[account]; i++)
      if (producer[account][i].isExist)
        if (producer[account][i].isFirework) //花火龙：1秒后收到产火
        {
          if (t + 1 <= maxTime)
            collect[account][t + 1] += producer[account][i].sun[t];
        }
        else //不是花火龙则计算吐火
        {
          int sun = producer[account][i].sun[t] * amplify[producer[account][i].row][producer[account][i].column] / 100;
          cardSpit[account][t] += sun * producer[account][i].sunNum[t];
        }
    //统计这一秒单独属于nP的吐火（天空、卡片、武器）
    spit[account][t] += cardSpit[account][t] + weaponSpit[account][t];
    if (account == 0)
      spit[account][t] += skySpit[t];
  }
  //第二步：如果有炎焱兔放置，更新增幅数据
  for (int account = 0; account < accountNum; account++)
    if (rabbitOrder[account] > 0)
      if (track[account][rabbitOrder[account]][t])
      {
        times[account][rabbitOrder[account]]++;//统计次数+1
        int loc = location[account][rabbitOrder[account]][times[account][rabbitOrder[account]]];
        if (loc > 0)
        {
          int row = loc / 10;
          int column = loc % 10;
          AddRabbit(account, rabbitOrder[account], row, column);
          UpdateLatestRabbit(account);
        }
      }
  //第三步：如果放了铲子，本格所有已放卡消失。注意同一秒可能铲了n张卡
  for (int account = 0; account < accountNum; account++)
    if (track[account][0][t])
    {
      for (int shovelTimes = 0; shovelTimes < track[account][0][t]; shovelTimes++)
      {
        times[account][0]++;
        int loc = location[account][0][times[account][0]];
        if (loc > 0)
        {
          int row = loc / 10;
          int column = loc % 10;
          for (int i = 0; i < producerNum[account]; i++)
            if (producer[account][i].row == row && producer[account][i].column == column)
              producer[account][i].isExist = false;
          bool isRabbitRemoved = false;//是否有炎焱兔被铲除
          for (int i = 0; i < rabbitNum[account]; i++)
            if (rabbit[account][i].isExist && rabbit[account][i].row == row && rabbit[account][i].column == column)
            {
              rabbit[account][i].isExist = false;
              isRabbitRemoved = true;
            }
          if (isRabbitRemoved)//如果有炎焱兔被铲除，更新增幅表
            UpdateRabbit();
        }
      }
    }
}
//能量计算准备工作：重置数组，获取炎焱兔编号，添加初始卡，填写天空武器轴
void ReadyCountEnergy()
{
  for (int account = 0; account < accountNum; account++)
  {
    rabbitNum[account] = 0;
    producerNum[account] = 0;
    memset(times[account], 0, sizeof(times[account]));
    memset(cardSpit[account], 0, sizeof(cardSpit[account]));
    memset(spit[account], 0, sizeof(spit[account]));
    memset(collect[account], 0, sizeof(collect[account]));
    memset(cost[account], 0, sizeof(cost[account]));
    memset(energy[account], 0, sizeof(energy[account]));
    memset(totalCollect[account], 0, sizeof(totalCollect[account]));
    memset(totalCost[account], 0, sizeof(totalCost[account]));

    rabbitOrder[account] = GetOrder(account, "炎焱兔");
    energy[account][0] = initialEnergy;
    FillWeaponSpit(account);//填写武器吐火轴
  }
  FillSkySpit();//填写天空吐火轴
  ResetAmplify(amplify);//每个格子的增幅预设为1倍
  totalCollect[0][0] = initialEnergy;//初始能量计入1P生产
  //开局加入1P初始卡
  int innateStove = GetOrder(0, "初始火");
  int innateCup = GetOrder(0, "初始灯");
  int stove = GetOrder(0, "小火炉");
  int cup = GetOrder(0, "酒杯灯");
  AddInnateProducer(0, stove, innateStove);
  AddInnateProducer(0, cup, innateCup);
}
//根据普通轨道计算每秒的生产、消耗及能量
void CountEnergyNormal()
{
  ReadyCountEnergy();
  //从第1秒开始扫描。计算每一秒的吐火（卡片、武器、天空）、收火和消耗
  for (int t = minTime; t <= maxTime; t++)
  {
    FillEnergy(t);
    FillSpit(t);
  }
}
//是否冷却完毕
bool IsCold(int account, int core, int thisTime)
{
  //放置次数为0，当然是冷却好的
  if (times[account][core] == 0)
    return true;
  //上次放卡时间
  int lastTime = moment[account][core][times[account][core]] / 10;
  //时间间隔不小于cd，直接判定冷却完毕
  if (thisTime - lastTime >= slot[account][core].cd)
    return true;
  if (thisTime - lastTime < 3) //间隔小于3秒，冰沙冰激凌也无法冷却，直接判断未冷却
    return false;

  int iceTrack[maxTime + 2] = {};//冷却轨道数据：第i秒有冷却起始线，则为1，否则为0
  //注：t时刻的冷却线，要求t时刻及以前种植的卡，t+3时刻及以后才被冷却

  //记录冰激凌冷却线
  int ice = GetOrder(account, "冰激凌");
  if (ice > zero && ice != core)
  {
    for (int times = 1; times < maxPlantTimes; times++)
    {
      if (location[account][ice][times] == zero || moment[account][ice][times] == zero) //位置和轨道有一个为0，就结束查找
        break;
      int t = moment[account][ice][times] / 10;
      int iceTarget = GetIceTarget(account, location[account][ice][times] / 10, location[account][ice][times] % 10, t);//获得冷却的目标
      if (iceTarget == core) //如果有冷却目标，记录冷却轨道
        iceTrack[t] = 1;
    }
  }
  //记录冰沙冷却线
  int sorbet = GetOrder(account, "冰沙");
  if (sorbet > zero && sorbet != core)
  {
    for (int times = 1; times < maxPlantTimes; times++)
    {
      if (moment[account][sorbet][times] == zero) //轨道为0就结束查找
        break;
      int t = moment[account][sorbet][times] / 10;
      iceTrack[t + 1] = 1;//非冰沙的所有卡记录冷却轨道
    }
  }

  for (int t = lastTime; t <= thisTime - 3; t++)
    if (iceTrack[t] == 1)//如果t时间有冷却轨道
      return true;

  return false;
}
//为卡槽order添加轨道t，若t已有轨道则按冷却向右延长一个轨道（仅限空间足够）
void AddTrack(int account, int order, int currentTime);
//将极限轨道按自动布轨规则展开为普通轨道
void ExpandLimit()
{
  //第一步：保留轨道，计算全局生产，填写已放次数
  CountEnergyNormal();

  //第二步：记录无需布轨的卡片编号和每张卡所需布轨的次数
  int innateStove[2] = {};//初始火
  int innateCup[2] = {};//初始灯
  int magic[2] = {};//幻幻鸡
  int ice[2] = {};//冰激凌
  int sorbet[2] = {};//冰沙
  int totalTimes[2][maxSlotNum + 1] = {};//每张卡所需布轨次数（阵型中的数量）
  for (int account = 0; account < accountNum; account++)
  {
    innateStove[account] = GetOrder(account, "初始火");
    innateCup[account] = GetOrder(account, "初始灯");
    magic[account] = GetOrder(account, "幻幻鸡");
    ice[account] = GetOrder(account, "冰激凌");
    sorbet[account] = GetOrder(account, "冰沙");
    for (int order = 1; order <= slotNum[account]; order++)//获得每张卡的种植次数（需布轨次数）
    {
      if (order == magic[account] || order == ice[account] || order == sorbet[account]
        || order == innateStove[account] || order == innateCup[account])
        totalTimes[account][order] = 0;
      else
        totalTimes[account][order] = GetLocTimes(account, order);
    }
  }

  //第三步：从第1秒开始检查检查每张卡是否可放置，放置时更新总能量
  for (int t = minTime; t <= maxTime; t++)
    for (int account = 0; account < accountNum; account++)
      for (int order = 1; order <= slotNum[account]; order++)
      {
        SlotType &slot0 = slot[account][order];
        //非极限卡、继承卡、布轨已完成的卡不布轨
        if (!slot0.limit || slot0.retain || times[account][order] >= totalTimes[account][order])
          continue;
        if (IsCold(account, order, t)) //如果冷却完毕
        {
          //本次放卡消耗
          int currentCost = slot0.cost < 1000 ? slot0.cost : slot0.cost - 1000 + 50 * times[account][order];
          //如果这一秒能量够用，就放一张（不考虑后面的缺火）
          if (energy[account][t] >= currentCost)
          {
            AddTrack(account, order, t);//添加轨道
            if (slot0.style >= 1 && slot0.style <= 6)//如果放下的是生产卡或炎焱兔，需要重新计算产火
              CountEnergyNormal();
            else //否则更新times[account][order]和能量值即可
            {
              times[account][order]++;
              for (int iT = t; iT <= maxTime; iT++)
                energy[account][iT] -= currentCost;
            }
            //如果这张卡是冰幻核心卡……
          }
        }
      }
}
//考虑极限轨道计算每秒的生产、消耗及能量
void CountEnergyLimit()
{
  //1. 备份当前轨道
  memcpy(trackBackup, track, sizeof(track));
  memcpy(momentBackup, moment, sizeof(moment));
  //2. 按自动布轨规则填充所有极限轨道（同时更新各秒能量值）
  ExpandLimit();
  //3. 还原轨道
  memcpy(track, trackBackup, sizeof(track));
  memcpy(moment, momentBackup, sizeof(moment));
}
//能量是否足够
bool IsEnergyEnough(int account, int currentCost, int thisTime)
{
  for (int t = thisTime; t <= maxTime; t++)
    if (energy[account][t] < currentCost)
      return false;
  return true;
}
//在账号account卡片order的Moment数组中插入时间t
void InsertMoment(int account, int order, int t)
{
  int times;
  for (times = 0; times < maxPlantTimes; times++)//查找种植次数
    if (moment[account][order][times + 1] == zero)//如果第times+1次种植时刻为0，说明只有times次种植
      break;
  int MaxTimes = times;//记录最大种植次数
  for (times = MaxTimes; times >= 1; times--)//从最后一次种植开始找
  {
    if (t * 10 >= moment[account][order][times])//如果已经找到位置，填写并结束循环
    {
      moment[account][order][times + 1] = t * 10;
      break;
    }
    else
      moment[account][order][times + 1] = moment[account][order][times];
  }
  if (times == zero)//如果循环完了还没找到小于t*10的Moment
    moment[account][order][1] = t * 10;//那就填在第一位
}
//从账号account卡片order的Moment数组中删除时间t
void RemoveMoment(int account, int order, int t)
{
  int times;
  int RemoveTimes;//需要删除的时间t所在位置
  for (times = 1; times < maxPlantTimes; times++)//查找种植次数
    if (moment[account][order][times] == zero || moment[account][order][times] / 10 == t)//找到时间t的位置或没有时间t，则结束
      break;
  RemoveTimes = times;
  for (times = RemoveTimes; times < maxPlantTimes; times++)
  {
    moment[account][order][times] = moment[account][order][times + 1];
    if (moment[account][order][times + 1] == zero)
      break;
  }
}
//为卡槽order添加轨道t，若t已有轨道则按冷却向右延长一个轨道（仅限空间足够）
void AddTrack(int account, int order, int currentTime)
{
  if (currentTime > maxTime) //时间超过960不添加
    return;
  if (track[account][order][currentTime] == zero)//没有轨道，就在此处添加轨道
  {
    track[account][order][currentTime] = 1;
    InsertMoment(account, order, currentTime);
  }
  else //如果已有轨道
  {
    if (slot[account][order].cd == zero) //如果cd为0，增加1层轨道
    {
      track[account][order][currentTime]++;
      InsertMoment(account, order, currentTime);
    }
    else //否则尝试在一个冷却周期后添加轨道
    {
      //检查t+1到t+cd-1范围内是否已经有轨道
      for (int t = currentTime + 1; t <= maxTime; t++)
      {
        //如果距离是cd的整数倍
        if ((t - currentTime) % slot[account][order].cd == 0)
        {
          if (track[account][order][t] == 0) //此处没有轨道，则添加轨道，结束函数
          {
            AddTrack(account, order, t);
            return;
          }
          //否则继续往后查找
        }
        else //距离不是cd的整数倍
        {
          if (track[account][order][t])//此处有轨道，则无法添加，结束函数
            return;
          //否则继续往后查找
        }
      }
    }
  }
}
//删除这张卡的全部阵型
void DeleteMap(int account, int order);
//删除卡片order第t秒轨道
void DeleteTrack(int account, int order, int t)
{
  if (track[account][order][t] >= 1)//已有轨道，就执行删除轨道
  {
    track[account][order][t]--;
    RemoveMoment(account, order, t);
    //如果轨道全空，删除终止条件
    if (moment[account][order][1] == 0)
    {
      slot[account][order].endNum = 0;
      slot[account][order].endTime = 0;
    }
  }
}
//删除卡片order全部轨道
void DeleteTrack(int account, int order)
{
  memset(track[account][order], 0, sizeof(track[account][order]));
  memset(moment[account][order], 0, sizeof(moment[account][order]));
  slot[account][order].endNum = 0;
  slot[account][order].endTime = 0;
}
//删除卡片order的阵型和轨道
void DeleteMapAndTrack(int account, int order)
{
  DeleteMap(account, order);
  DeleteTrack(account, order);
}
//清空阵型
void ClearMap(int account)
{
  memset(map[account], 0, sizeof(map[account]));
  memset(location[account], 0, sizeof(location[account]));
  for (int order = 0; order <= maxSlotNum + 1; order++)
  {
    slot[account][order].repair = 0;
    slot[account][order].search = 0;
  }
}
//清空轨道
void ClearTrack(int account)
{
  memset(track[account], 0, sizeof(track[account]));
  memset(moment[account], 0, sizeof(moment[account]));
  for (int order = 0; order <= maxSlotNum + 1; order++)
  {
    slot[account][order].lock = false;
    slot[account][order].limit = false;
    slot[account][order].independent = false;
    slot[account][order].retain = false;
    memset(&slot[account][order].delay, 0, sizeof(ConditionType));//清空放卡条件
  }
}
//清空阵型和轨道（保留卡槽）
void ClearMapAndTrack(int account)
{
  ClearMap(account);
  ClearTrack(account);
}
//删除地图某行某列某层级某深度的卡片
void DeleteFromGrid(int account, int row, int column, int level, int depth)
{
  //删除后，Loc数组要向前递补，该卡后面的所有次数都要改变。
  int oldDepth = map[account][row][column].depth[level];//该层级原有的卡片数量
  int oldTimes = map[account][row][column].times[level][depth];//被删除卡的种植次序
  if (depth < oldDepth)  //如果被删深度属于[0,OriginDepth-1]，才执行删除
  {
    int order = GetOrder(account, map[account][row][column].name[level][depth]);//获得被删除的卡片编号
    /*1.全图同名卡，大于times的次数都要减1。例如删了海星15，则海星16-海星30（末尾）都要减1*/
    for (int times = oldTimes + 1; times <= GetLocTimes(account, order); times++)//从海星16到海星30
    {
      int row0 = location[account][order][times] / 10;
      int column0 = location[account][order][times] % 10;//获得第times次海星的位置
      for (int d = 0; d < map[account][row0][column0].depth[level]; d++)//在该格子的海星层级内搜索每一个深度
        if (GetOrder(account, map[account][row0][column0].name[level][d]) == order && map[account][row0][column0].times[level][d] == times)
          //如果该深度的卡片正是这一次的海星，说明要调的就是这个深度
        {
          map[account][row0][column0].times[level][d]--;//该海星的次序号减1
          break;
        }
    }
    /*2.地图该层级该深度的卡删除，后续深度向前递补，最后一个深度清空，总深度-1*/
    for (int d = depth; d <= oldDepth - 2; d++)
    {
      strcpy_s(map[account][row][column].name[level][d], map[account][row][column].name[level][d + 1]);
      map[account][row][column].times[level][d] = map[account][row][column].times[level][d + 1];
    }
    strcpy_s(map[account][row][column].name[level][oldDepth - 1], "");
    map[account][row][column].times[level][oldDepth - 1] = 0;
    map[account][row][column].depth[level]--;
    /*3.从Loc数组中删除本次种植位置，后续位置向前递补*/
    for (int times = oldTimes; times <= maxPlantTimes; times++)
    {
      location[account][order][times] = location[account][order][times + 1];
      if (location[account][order][times + 1] == zero)
        break;//后续位置为0代表删完了
    }
  }
}
void DeleteFromGridLevel(int account, int row, int column, int level)//删除地图某行某列某层级选中深度的卡片
{
  DeleteFromGrid(account, row, column, level, map[account][row][column].depthSelected[level]);
}
void AddToGrid(int account, int order, int row, int column)//将order号卡槽添加到地图某行某列
{//自动辨认层级，且添加到最高深度，本步骤不执行深度排列
  int times;
  int level = slot[account][order].level;//获得层级
  int depth = map[account][row][column].depth[level];//本层已有卡数，新卡添加到第[depth]位
  //如果该层级未满或所加卡为冰沙，且该卡种植次数小于上限
  if (depth < maxStack && GetLocTimes(account, order) < maxPlantTimes)
  {
    times = GetLocTimes(account, order);//获取order号卡已种植次数
    location[account][order][times + 1] = row * 10 + column;//向Loc数组中记录种植行列
    strcpy_s(map[account][row][column].name[level][depth], slot[account][order].name);
    map[account][row][column].times[level][depth] = times + 1;  //向地图格子该层级中填写卡片名称、种植次数
    map[account][row][column].depth[level]++;
  }
}
void AddEmptyToSlot(int account, const char *name)//向卡槽中添加名称为name的空卡
{
  char *CardName[5][3] = {
    { "木盘子", "棉花糖", "软糖" }, { "瓜皮", "处女座", "赫拉" }, { "大火炉", "火焰牛", "大炮" },
    { "冰沙", "冰激凌", "" }, { "幻幻鸡", "", "" } };
  if (slotNum[account] < maxSlotNum)
  {
    slotNum[account]++;
    slot[account][slotNum[account]] = emptyCard;//各项参数为0的空卡
    strcpy_s(slot[account][slotNum[account]].name, name);
    slot[account][slotNum[account]].level = 3;
    for (int i = 0; i <= 4; i++)
      for (int j = 0; j <= 2; j++)
        if (strcmp(name, CardName[i][j]) == zero)
        {
          if (i <= 1)
            slot[account][slotNum[account]].level = i + 1;
          else
            slot[account][slotNum[account]].level = i + 2;
        }
  }
}
//删除铲子地图和轨道
void DeleteShovel(int account)
{
  int row, column, depth;
  while (location[account][0][1] > zero) //如果有种植记录，全部删去（通过DeleteFromGrid函数，既删地图又删Loc）
  {
    row = location[account][0][1] / 10;
    column = location[account][0][1] % 10;
    for (depth = map[account][row][column].depth[0] - 1; depth >= zero; depth--) //从高深度往低深度删除
      if (GetOrder(account, map[account][row][column].name[0][depth]) == zero)//该深度的卡编号为order才删除
        DeleteFromGrid(account, row, column, 0, depth);
  }
  memset(track[account][0], 0, sizeof(track[account][0]));
  memset(moment[account][0], 0, sizeof(moment[account][0]));
  DecreaseToEmptyDepth();//选择深度高于LUMO，则降至LUMO
}
void AutoShovel(int account)//自动布置铲子位置和轨道
{
  int row, column, level;
  int times[22] = {};//每张卡的已种植次数
  int MapOccupied[8][10][7];
  memset(MapOccupied, 0, sizeof(MapOccupied));//地图占用设为0
  DeleteShovel(account);//清空铲子地图和轨道
  int MaxTrack = GetMaxTrack(account);//最大轨道时间
  for (int t = minTime; t <= MaxTrack; t++)
    for (int order = 1; order <= slotNum[account]; order++)
    {
      if (slot[account][order].level == 5)//冰激凌和冰沙轨道不用布铲
        continue;
      if (track[account][order][t] >= 1) //如果这个时刻这张卡有种植
      {
        times[order]++;
        row = location[account][order][times[order]] / 10;
        column = location[account][order][times[order]] % 10;//获取这张卡本次种植的位置
        if (slot[account][order].level == 6)//如果是幻鸡轨道
        {
          int MagicTarget = GetMagicTarget(account, t);//获得幻幻鸡复制的目标卡，没找到返回0
          if (MagicTarget > zero)
            level = slot[account][MagicTarget].level;//获取复制目标的层级
          else
            continue;//没有复制目标，不予考虑
        }
        else //如果不是幻鸡轨道，则是1-4层级
          level = slot[account][order].level;//获取这张卡的层级
        if (MapOccupied[row][column][level] == zero)//如果该层级没有占用
          MapOccupied[row][column][level] = 1;//则设为已占用
        else if (row > zero) //如果该层级已占用，添加铲子
        {
          if (t - 1 >= 2)
          {
            AddToGrid(account, 0, row, column);
            AddTrack(account, 0, t - 1);
          }
        }
      }
    }
}
//自动添加卡片order的n个轨道
void AutoTrack(int account, int order, int n)
{
  if (n <= 0)
    return;
  SlotType &slot0 = slot[account][order];
  //继承卡不布轨
  if (slot0.retain)
    return;

  CountEnergyNormal();//计算全局生产
  int startTime = times[account][order] == 0 ? minTime : moment[account][order][times[account][order]] / 10 + 2;
  for (int t = startTime; t <= maxTime; t++)
  {
    if (IsCold(account, order, t)) //如果冷却完毕
    {
      //本次放卡消耗
      int currentCost = slot0.cost < 1000 ? slot0.cost : slot0.cost - 1000 + 50 * times[account][order];
      //如果能量够用，就放一张
      if (IsEnergyEnough(account, currentCost, t))
      {
        AddTrack(account, order, t);//添加轨道
        if (slot0.style >= 1 && slot0.style <= 6)//如果放下的是生产卡或炎焱兔，需要重新计算产火
          CountEnergyNormal();
        else //否则更新times[account][order]和能量值即可
        {
          times[account][order]++;
          for (int tempT = t; tempT <= maxTime; tempT++)
            energy[account][tempT] -= currentCost;
        }
        n--;
        if (n <= 0)
          return;
      }
    }
  }
}
//自动布置全局轨道
void AutoTrack()
{
  //第一步：保留轨道，计算全局生产，填写已放次数
  CountEnergyNormal();
  //如果已经缺火，放弃布轨
  for (int account = 0; account < accountNum; account++)
    for (int t = minTime; t <= maxTime; t++)
      if (energy[account][t] < 0)
        return;

  //第二步：记录无需布轨的卡片编号和每张卡所需布轨的次数
  int innateStove[2] = {};//初始火
  int innateCup[2] = {};//初始灯
  int magic[2] = {};//幻幻鸡
  int ice[2] = {};//冰激凌
  int sorbet[2] = {};//冰沙
  int totalTimes[2][maxSlotNum + 1] = {};//每张卡所需布轨次数（阵型中的数量）
  for (int account = 0; account < accountNum; account++)
  {
    innateStove[account] = GetOrder(account, "初始火");
    innateCup[account] = GetOrder(account, "初始灯");
    magic[account] = GetOrder(account, "幻幻鸡");
    ice[account] = GetOrder(account, "冰激凌");
    sorbet[account] = GetOrder(account, "冰沙");
    for (int order = 1; order <= slotNum[account]; order++)//获得每张卡的种植次数（需布轨次数）
    {
      if (order == magic[account] || order == ice[account] || order == sorbet[account]
        || order == innateStove[account] || order == innateCup[account])
        totalTimes[account][order] = 0;
      else
        totalTimes[account][order] = GetLocTimes(account, order);
    }
  }

  //第三步：从第1秒开始检查检查每张卡是否可放置，放置时更新总能量
  for (int t = minTime; t <= maxTime; t++)
    for (int account = 0; account < accountNum; account++)
      for (int order = 1; order <= slotNum[account]; order++)
      {
        SlotType &slot0 = slot[account][order];
        //锁定卡、继承卡、布轨已完成的卡、幻鸡、冰激凌、冰沙、初始火、初始灯不布轨
        if (slot0.lock || slot0.retain || times[account][order] >= totalTimes[account][order])
          continue;
        if (IsCold(account, order, t)) //如果冷却完毕
        {
          //本次放卡消耗
          int currentCost = slot0.cost < 1000 ? slot0.cost : slot0.cost - 1000 + 50 * times[account][order];
          //如果能量够用，就放一张
          if (IsEnergyEnough(account, currentCost, t))
          {
            AddTrack(account, order, t);//添加轨道
            if (slot0.style >= 1 && slot0.style <= 6)//如果放下的是生产卡或炎焱兔，需要重新计算产火
              CountEnergyNormal();
            else //否则更新times[account][order]和能量值即可
            {
              times[account][order]++;
              for (int tempT = t; tempT <= maxTime; tempT++)
                energy[account][tempT] -= currentCost;
            }
            //如果这张卡是冰幻核心卡……
          }
        }
      }

  //第四步：如果铲子没有锁定轨道，自动布置铲子
  for (int account = 0; account < accountNum; account++)
    if (!slot[account][0].lock && !slot[account][0].retain)
      AutoShovel(account);
}

/*录制程序*/
int recordStage;//录制阶段
int headWidth, cardNameWidth, tailWidth, recordLength;//录制轨道时的文字长度
RecordType Record[maxPlantTimes];//录制的每一张卡的信息
int recordNum;//已录制卡片数量
char *WaveColorString[19] = { "99d5fa", "a6e9ff", "8dc8fc", "6bdfff", "92f7ff", "aee4ff", "79fbff", "77dfff",
"64daee", "7ec4ff", "19252e", "000d21", "417687", "5a9ba0", "716a5d", "4d2a14", "4b1900", "000000", "c47931" };
COLORREF WaveColor[19];
HWND hWnd;//录制轨道时的游戏窗口
RECT GameRect;
DWORD StartTick, CurrentTick;
int StartX = 875, StartY = 453;//“开始”按s钮
int RoleWidth = 112, RoleHeight = 91;//人物角色按钮
int SlotX = 192, SlotY = 10, SlotWidth = 53, SlotHeight = 68;//卡槽1横、纵坐标，宽度、高度。卡组小于等于12张时右移21
int ShovelX = 866, ShovelY = 12, ShovelWidth = 84, ShovelHeight = 84;//铲子
int LShovelX = 113, LShovelY = 90, LShovelWidth = 80, LShovelHeight = 84;//20-21槽的铲子
int MapX = 302, MapY = 105, MapWidth = 60, MapHeight = 64;//地图1行1列横、纵坐标，地图格子宽度、高度
int ExitCount;//计数达到3结束录制
/*图像显示*/
int isMapCatched = 1;//是否已经抓取地图
IMAGE mapImage;//地图图像
int isMapImageCatched;//地图图像是否已经载入

const int maxBackupNum = 256;
char backupList[maxBackupNum][maxPath];
//清理自动备份
void ClearBackup()
{
  int directNum = GetFileList("自动备份\\轨道文件\\*", backupList, maxBackupNum);
  for (int order = 0; order < directNum; order++)
  {
    char direct[maxPath];
    sprintf_s(direct, "自动备份\\轨道文件\\%s\\", backupList[order]);
    DeleteFolder(direct);
  }
}
//清理本编辑器的自动备份文件夹
void ClearThisBackup()
{
  char direct[maxPath];
  sprintf_s(direct, "自动备份\\轨道文件\\%s\\", runTimeString);
  DeleteFolder(direct);
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
/*对话框函数*/
struct InputBoxParam
{
  char *dest;//输入对象
  const char *prompt, *title, *defaultStr;//提示语，标题，初始文本
  int maxLength;//最大长度
};
//输入框过程函数
INT_PTR CALLBACK InputDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
//设置对话框控件数值（如果数值是0则空置）及最大输入长度length
void SetItemValue(HWND hDlg, int idItem, int value, int length)
{
  if (value == 0)
    SetDlgItemText(hDlg, idItem, "");
  else
  {
    char message[20];
    sprintf_s(message, "%d", value);
    SetDlgItemText(hDlg, idItem, message);
  }
  SendMessage(GetDlgItem(hDlg, idItem), EM_LIMITTEXT, length, 0);//设置长度限制
}
//设置对话框控件文本（如果是nullptr则空置）及最大输入长度length
void SetItemText(HWND hDlg, int idItem, const char *text, int length)
{
  if (text == nullptr)
    SetDlgItemText(hDlg, idItem, "");
  else
  {
    if (idItem == idWave && strcmp(text, "0") == 0) //波次为0不显示
      SetDlgItemText(hDlg, idItem, "");
    else //否则原样显示
      SetDlgItemText(hDlg, idItem, text);
  }
  SendMessage(GetDlgItem(hDlg, idItem), EM_LIMITTEXT, length, 0);//设置长度限制
}
//从输入框获取数值
int GetItemValue(int *dest, const char *itemName, HWND hDlg, int idItem, int length, int minimum, int maximum)
{
  if (!dest)
  {
    char message[100];
    sprintf_s(message, "【%s】指针为空", itemName);
    MessageBox(hDlg, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return 0;
  }

  char content[20] = {};
  GetDlgItemText(hDlg, idItem, content, length + 1);
  int value = atoi(content);
  if (value >= minimum && value <= maximum)
  {
    *dest = value;
    return 1;
  }
  char message[100];
  sprintf_s(message, "【%s】范围为%d~%d", itemName, minimum, maximum);
  MessageBox(hDlg, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
  return 0;
}
//从输入框获取文本
template <size_t size>
int GetItemText(char(&dest)[size], const char *itemName, HWND hDlg, int idItem, int length)
{
  if (!dest)
  {
    char message[100];
    sprintf_s(message, "【%s】指针为空", itemName);
    MessageBox(hDlg, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return 0;
  }

  char condition[20] = {};//输入的条件
  GetDlgItemText(hDlg, idItem, condition, length + 1);
  if (idItem == idWave) //波次：只能是m.n格式
  {
    if (!IsWaveLegal(condition)) //波次输入不合格
    {
      MessageBox(hDlg, waveErrorString, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
      return 0;
    }
    int mainWave = 0, smallWave = 0;
    int scannedNum = sscanf_s(condition, "%d.%d", &mainWave, &smallWave);
    if (smallWave > 0)
      sprintf_s(dest, "%d.%d", mainWave, smallWave);
    else if (mainWave > 0)
      sprintf_s(dest, "%d", mainWave);
    else
      sprintf_s(dest, "");
    return 1;
  }
  else if (idItem == idImage)//图像：不超过6个字
  {
    if (strlen(condition) > 8)
    {
      MessageBox(hDlg, "【图像】长度不能超过4个汉字", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
      return 0;
    }
    if (strcmp(condition, "（无）") == 0)
      strcpy_s(dest, "");
    else
      strcpy_s(dest, condition);
    return 1;
  }
  else if (idItem == idRow)
  {
    const char *tip = "【索敌行数】应为以下四种格式之一：\n"
      "4（固定放在第4行）\n"
      "a（和图像放在同一行）\n"
      "a+2（放在图像下方2行）\n"
      "a-1（放在图像上方1行）\n";
    bool isLegal = true;
    int length = strlen(condition);
    if (length != 0 && length != 1 && length != 3)
    {
      MessageBox(hDlg, tip, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
      return 0;
    }
    if (length == 1) //a或一位数（1-7）
    {
      if (!((condition[0] >= '1' && condition[0] <= '7') || condition[0] == 'a'))
      {
        MessageBox(hDlg, tip, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        return 0;
      }
    }
    else if (length == 3) //a±n（n=1-6）
    {
      if (!(condition[0] == 'a' && (condition[1] == '+' || condition[1] == '-') && condition[2] >= '1' && condition[2] <= '6'))
      {
        MessageBox(hDlg, tip, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        return 0;
      }
    }
    strcpy_s(dest, condition);
    return 1;
  }
  else if (idItem == idColumn)
  {
    const char *tip = "【索敌列数】应为以下四种格式之一：\n"
      "4（固定放在第4列）\n"
      "b（和图像放在同一列）\n"
      "b+2（放在图像右边2列）\n"
      "b-1（放在图像左边1列）\n";
    bool isLegal = true;
    int length = strlen(condition);
    if (length != 0 && length != 1 && length != 3)
    {
      MessageBox(hDlg, tip, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
      return 0;
    }
    if (length == 1) //b或一位数（1-9）
    {
      if (!((condition[0] >= '1' && condition[0] <= '9') || condition[0] == 'b'))
      {
        MessageBox(hDlg, tip, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        return 0;
      }
    }
    else if (length == 3) //b±n（n=1-9）
    {
      if (!(condition[0] == 'b' && (condition[1] == '+' || condition[1] == '-') && condition[2] >= '1' && condition[2] <= '9'))
      {
        MessageBox(hDlg, tip, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        return 0;
      }
    }
    strcpy_s(dest, condition);
    return 1;
  }
  char message[100];
  sprintf_s(message, "【%s】未添加检查程序", itemName);
  MessageBox(hDlg, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
  return 0;
}
const int maxItemNum = 256;//最大图像个数
char itemList[maxItemNum][maxPath] = {};
int itemNum;//图像个数
char itemName[maxItemNum][10] = {};//图像名称
//载入所有图像名称
void LoadItem()
{
  char path[maxPath] = {};
  int filesNum = GetFileList("用户参数\\自定图像\\*.png", itemList, maxItemNum);//查找所有png文件
  itemNum = 0;
  for (int i = 0; i < filesNum && i < maxItemNum; i++)
  {
    //记录长度不超过4个汉字的图像名称
    if (IsItemPathLegal(itemList[i], itemName[itemNum]))
      itemNum++;
  }
}
void ResetComboBox(HWND hDlg, int idItem)
{
  HWND hComboCard = GetDlgItem(hDlg, idItem);//组合框句柄
  ComboBox_SetCurSel(hComboCard, 0);//设置默认选中
}
//用当前卡组初始化组合框，cardSelected为当前选中的卡片名
void InitComboBox(HWND hDlg, int idItem, const char *cardSelected)
{
  char boxString[10] = {};
  HWND hComboCard = GetDlgItem(hDlg, idItem);//组合框句柄
  ComboBox_AddString(hComboCard, "（无）");
  int orderSelected = 0;//当前选中的卡片
  for (int order = 1; order <= slotNum[curAccount]; order++)
  {
    sprintf_s(boxString, "%02d %s", order, slot[curAccount][order].name);
    ComboBox_AddString(hComboCard, boxString);
    if (strcmp(cardSelected, slot[curAccount][order].name) == 0)
      orderSelected = order;
  }
  ComboBox_SetCurSel(hComboCard, orderSelected);//设置默认选中
}
//获得组合框当前选中的卡片名称
void GetComboBox(HWND hDlg, int idItem, char(&cardSelected)[10])
{
  HWND hComboCard = GetDlgItem(hDlg, idItem);
  int selectedCard = ComboBox_GetCurSel(hComboCard);
  if (selectedCard == 0)
    strcpy_s(cardSelected, "");
  else
    strcpy_s(cardSelected, slot[curAccount][selectedCard].name);
}
//条件编辑框过程函数
INT_PTR CALLBACK LimitDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    ConditionType *params = (ConditionType *)lParam;// 获取传入的参数
    RECT Desktop, Dialog;
    GetWindowRect(GetDesktopWindow(), &Desktop);
    GetWindowRect(hDlg, &Dialog);
    int xPos = (Desktop.left + Desktop.right - (Dialog.right - Dialog.left)) / 2;
    int yPos = (Desktop.top + Desktop.bottom - (Dialog.bottom - Dialog.top)) / 2;
    //设置对话框位置
    SetWindowPos(hDlg, HWND_TOP, xPos, yPos, 0, 0, SWP_NOSIZE);
    //设置对话框标题
    SetWindowText(hDlg, "极限成阵附加条件");

    //设置卡片组合框
    InitComboBox(hDlg, idCard, params->card);

    //设置跟随卡片组合框
    InitComboBox(hDlg, idFollow, params->follow);

    //设置图像组合框
    if (params->image[0]) //设置初始文本
      SetItemText(hDlg, idImage, params->image, 8);
    else
      SetItemText(hDlg, idImage, "（无）", 8);
    HWND hComboImage = GetDlgItem(hDlg, idImage);
    ComboBox_AddString(hComboImage, "（无）");
    int originImage = -1;//原来选中的图像
    LoadItem();//载入所有图像
    for (int i = 0; i < itemNum; i++)
    {
      ComboBox_AddString(hComboImage, itemName[i]);
      if (strcmp(params->image, itemName[i]) == 0)
        originImage = i;
    }
    if (originImage > 0)
      ComboBox_SetCurSel(hComboImage, originImage + 1);//设置默认选中
    HWND hEdit = FindWindowEx(hComboImage, NULL, "EDIT", NULL);
    if (hEdit)
      SendMessage(hEdit, EM_LIMITTEXT, 8, 0);//设置长度限制

    //设置其他文本框
    SetItemValue(hDlg, idDelay, params->delay, 3);
    if (params->triggerMode)
      CheckDlgButton(hDlg, idRetain, BST_CHECKED);
    else
      CheckDlgButton(hDlg, idRetain, BST_UNCHECKED);
    SetItemValue(hDlg, idEnergy, params->energy, 5);
    SetItemText(hDlg, idWave, params->wave, 5);
    SetItemValue(hDlg, idCardNum, params->cardNum, 2);
    SetItemValue(hDlg, idImageNum, params->imageNum, 2);
    SetItemText(hDlg, idRow, params->hunterRow, 3);
    SetItemText(hDlg, idColumn, params->hunterColumn, 3);
    SetItemText(hDlg, idFollow, params->follow, 6);
    SetItemValue(hDlg, idEndNum, params->endNum, 2);
    SetItemValue(hDlg, idEndTime, params->endTime, 3);
    SetWindowLongPtr(hDlg, DWLP_USER, (LONG_PTR)params);//设置用户数据，以便在后面的消息中使用
  }
  return TRUE;
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK) // 点击了“确定”按钮
    {
      // 获取编辑框中的内容
      ConditionType *params = (ConditionType *)GetWindowLongPtr(hDlg, DWLP_USER);
      ConditionType temp = {};

      if (!GetItemValue(&temp.delay, "延迟", hDlg, idDelay, 3, 0, 960))
        break;
      temp.triggerMode = GetItemCheck(hDlg, idRetain);
      if (!GetItemValue(&temp.energy, "火苗", hDlg, idEnergy, 5, 0, 15000))
        break;
      if (!GetItemText(temp.wave, "波次", hDlg, idWave, 5))
        break;

      //获得卡片条件
      HWND hComboCard = GetDlgItem(hDlg, idCard);
      int selectedCard = ComboBox_GetCurSel(hComboCard);
      if (selectedCard == 0)
        strcpy_s(temp.card, "");
      else
        strcpy_s(temp.card, slot[curAccount][selectedCard].name);

      if (!GetItemValue(&temp.cardNum, "卡片数量", hDlg, idCardNum, 2, 0, 63))
        break;
      if (!GetItemText(temp.image, "图像", hDlg, idImage, 8))
        break;
      if (!GetItemValue(&temp.imageNum, "图像数量", hDlg, idImageNum, 2, 0, 63))
        break;
      if (!GetItemText(temp.hunterRow, "索敌行数", hDlg, idRow, 3))
        break;
      if (!GetItemText(temp.hunterColumn, "索敌列数", hDlg, idColumn, 3))
        break;

      //获得跟随条件
      HWND hComboFollow = GetDlgItem(hDlg, idFollow);
      int selectedFollow = ComboBox_GetCurSel(hComboFollow);
      if (selectedFollow == 0)
        strcpy_s(temp.follow, "");
      else
        strcpy_s(temp.follow, slot[curAccount][selectedFollow].name);

      if (!GetItemValue(&temp.endNum, "放满", hDlg, idEndNum, 2, 0, 63))
        break;
      if (!GetItemValue(&temp.endTime, "终止时间", hDlg, idEndTime, 3, 0, 960))
        break;
      if (temp.card[0] && temp.cardNum == 0)
        temp.cardNum = 1;
      if (temp.image[0] && temp.imageNum == 0)
        temp.imageNum = 1;
      memcpy(params, &temp, sizeof(temp));
      EndDialog(hDlg, IDOK);// 结束对话框
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL) // 点击了“取消”按钮
    {
      EndDialog(hDlg, IDCANCEL);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDRESET)
    {
      ConditionType *params = (ConditionType *)GetWindowLongPtr(hDlg, DWLP_USER);
      memset(params, 0, sizeof(ConditionType));
      EndDialog(hDlg, IDRESET);
      return TRUE;
    }
    break;
  }
  return FALSE;
}
//弹出放卡条件对话框
int EditCondition(int account, int order)
{
  return DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_LIMIT),
    hWndEditor, LimitDialogProc, (LPARAM)&slot[account][order].delay);
}
//初始化自动布轨对话框控件内容
void InitAutoDialog(HWND hDlg)
{
  //设置卡片组合框
  for (int i = 0; i < maxConstraintNum; i++)
  {
    InitComboBox(hDlg, idCard0 + i, autoTable.card[i]);
    SetItemValue(hDlg, idMin0 + i, autoTable.minTime[i], 3);
    SetItemValue(hDlg, idMax0 + i, autoTable.maxTime[i], 3);
  }
  InitComboBox(hDlg, idCardMin, autoTable.minCard);
}
//重设自动布轨对话框控件
void ResetAutoDialog(HWND hDlg)
{
  //设置卡片组合框
  for (int i = 0; i < maxConstraintNum; i++)
  {
    ResetComboBox(hDlg, idCard0 + i);
    SetItemValue(hDlg, idMin0 + i, autoTable.minTime[i], 3);
    SetItemValue(hDlg, idMax0 + i, autoTable.maxTime[i], 3);
  }
  ResetComboBox(hDlg, idCardMin);
}
//自动布轨对话框过程函数
INT_PTR CALLBACK AutoDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    RECT desktop, dialog;
    GetWindowRect(GetDesktopWindow(), &desktop);
    GetWindowRect(hDlg, &dialog);
    int xPos = (desktop.left + desktop.right - (dialog.right - dialog.left)) / 2;
    int yPos = (desktop.top + desktop.bottom - (dialog.bottom - dialog.top)) / 2;
    //设置对话框位置
    SetWindowPos(hDlg, HWND_TOP, xPos, yPos, 0, 0, SWP_NOSIZE);
    //设置对话框的初始文本
    SetWindowText(hDlg, "自动布轨附加条件");
    //设置卡片组合框
    InitAutoDialog(hDlg);
  }
  return TRUE;
  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK) // 点击了“确定”按钮
    {
      // 获取编辑框中的内容
      AutoTableType temp = {};
      for (int i = 0; i < maxConstraintNum; i++)
      {
        if (!GetItemValue(&temp.minTime[i], "最小时间", hDlg, idMin0 + i, 3, 0, maxProgrammingTime))
          return TRUE;
        if (!GetItemValue(&temp.maxTime[i], "最大时间", hDlg, idMax0 + i, 3, 0, maxProgrammingTime))
          return TRUE;
        if (temp.maxTime[i] > 0 && temp.minTime[i] > temp.maxTime[i])
        {
          MessageBox(hDlg, "最小时间不能超过最大时间", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
          return TRUE;
        }
      }
      for (int i = 0; i < maxConstraintNum; i++)
        GetComboBox(hDlg, idCard0 + i, temp.card[i]);
      GetComboBox(hDlg, idCardMin, temp.minCard);
      memcpy(&autoTable, &temp, sizeof(temp));
      EndDialog(hDlg, IDOK);// 结束对话框
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL) // 点击了“取消”按钮
    {
      EndDialog(hDlg, IDCANCEL);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDRESET) // 点击了“重置”按钮
    {
      memset(&autoTable, 0, sizeof(autoTable));
      ResetAutoDialog(hDlg);
      return TRUE;
    }
    break;
  }
  return FALSE;
}
//弹出自动布轨对话框
int EditAutoTrack()
{
  return DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_AUTO),
    hWndEditor, AutoDialogProc, 0);
}

//拖入轨道对话框过程函数
INT_PTR CALLBACK DragDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_INITDIALOG:
  {
    char *path = (char *)lParam;
    char shortPath[maxPath];
    GetShortPath(path, shortPath);
    RECT desktop, dialog;
    GetWindowRect(GetDesktopWindow(), &desktop);
    GetWindowRect(hDlg, &dialog);
    int xPos = (desktop.left + desktop.right - (dialog.right - dialog.left)) / 2;
    int yPos = (desktop.top + desktop.bottom - (dialog.bottom - dialog.top)) / 2;
    //设置对话框位置
    SetWindowPos(hDlg, HWND_TOP, xPos, yPos, 0, 0, SWP_NOSIZE);
    //设置对话框文本
    SetDlgItemText(hDlg, IDC_TEXT, shortPath);
    SetForegroundWindow(hDlg);
  }
  return TRUE;
  case WM_COMMAND:
    if (LOWORD(wParam) == ID_1P) // 点击了“确定”按钮
    {
      EndDialog(hDlg, ID_1P);// 结束对话框
      return TRUE;
    }
    else if (LOWORD(wParam) == ID_2P) // 点击了“取消”按钮
    {
      EndDialog(hDlg, ID_2P);
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
//弹出拖入轨道文件对话框
int DragDialog(const char *path)
{
  return DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DRAG),
    hWndEditor, DragDialogProc, (LPARAM)path);
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
  return DialogBoxParamA(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_INPUT), hWndEditor, InputDialogProc, (LPARAM)&params);
}
//输入自然数到Num，直到满足范围Min~Max为止。输入成功则返回1，否则返回0
int InputNum(int *pNum, int mininum, int maximun, const char *prompt = "", const char *title = "输入")
{
  char illegalPrompt[1000];//输入不规范的提示
  sprintf_s(illegalPrompt, "您的输入不规范，请重新输入。\n%s", prompt);

  char maxString[11];
  sprintf_s(maxString, "%d", maximun);
  int maxLength = strlen(maxString) + 1;//获得最大输入长度

  char defaultStr[11];
  sprintf_s(defaultStr, "%d", *pNum);//初始文本：自然数的原值

  int currentNum = 0;
  char dest[11];
  int inputButton = NewInputBox(dest, maxLength, prompt, title, defaultStr);//输入自然数到dest
  while (inputButton == IDOK) //如果点击了确定，则检查输入
  {
    currentNum = StrToNat(dest);//将dest转化为自然数
    if (currentNum >= mininum && currentNum <= maximun)//输入在要求范围内，则记录输入
    {
      *pNum = currentNum;
      return 1;
    }
    inputButton = NewInputBox(dest, maxLength, illegalPrompt, title, defaultStr);//否则继续输入
  }
  return 0;//取消输入
}
//输入字符串到dest，直到isLegal(输入)为真。输入成功则返回1，否则返回0
template <size_t size>
int InputWaveString(char(&dest)[size], int isLegal(const char *str), const char *prompt = "", const char *title = "输入")
{
  char illegalPrompt[500];//输入不规范的提示
  sprintf_s(illegalPrompt, "您的输入不规范，请重新输入（示例：1.5）。\n%s", prompt);
  char existPrompt[500];//波次已存在的提示
  sprintf_s(existPrompt, "您输入的波次已存在，请重新输入。\n%s", prompt);
  const char *errorPrompt[2] = { illegalPrompt, existPrompt };//错误提示

  char destCopy[100] = {};
  strcpy_s(destCopy, dest);//初始文本：自然数的原值

  int inputButton = NewInputBox(destCopy, size, prompt, title, destCopy);//输入文本到destCopy
  while (inputButton == IDOK) //如果点击了确定，则检查输入
  {
    int error = isLegal(destCopy);
    if (error == 0)//输入在要求范围内，则记录输入
    {
      strcpy_s(dest, destCopy);
      return 1;
    }
    inputButton = NewInputBox(destCopy, size, errorPrompt[error - 1], title, destCopy);//否则继续输入
  }
  return 0;//取消输入
}
void PrintToClipboard(const char *AnsiStr)//将字符串AnsiStr复制到剪贴板
{
  if (!OpenClipboard(NULL))//打开剪贴板，打不开就结束
    return;

  //将ANSI转换为UTF-16字符串
  int UnicodeSize = MultiByteToWideChar(CP_ACP, 0, AnsiStr, -1, NULL, 0);
  wchar_t *UnicodeStr = (wchar_t *)malloc(UnicodeSize * sizeof(wchar_t));
  MultiByteToWideChar(CP_ACP, 0, AnsiStr, -1, UnicodeStr, UnicodeSize);
  EmptyClipboard();//清空剪贴板

  HGLOBAL AnsiClipBuffer, UnicodeClipBuffer;
  char *AnsiBuffer;
  wchar_t *UnicodeBuffer;

  //Unicode写入剪贴板
  UnicodeClipBuffer = GlobalAlloc(GMEM_DDESHARE, (UnicodeSize + 1) * sizeof(wchar_t));
  UnicodeBuffer = (wchar_t *)GlobalLock(UnicodeClipBuffer);
  wcscpy_s(UnicodeBuffer, UnicodeSize + 1, UnicodeStr);
  GlobalUnlock(UnicodeClipBuffer);
  SetClipboardData(CF_UNICODETEXT, UnicodeClipBuffer);

  //ansi写入剪贴板
  AnsiClipBuffer = GlobalAlloc(GMEM_DDESHARE, (strlen(AnsiStr) + 1) * sizeof(char));
  AnsiBuffer = (char *)GlobalLock(AnsiClipBuffer);
  strcpy_s(AnsiBuffer, strlen(AnsiStr) + 1, AnsiStr);
  GlobalUnlock(AnsiClipBuffer);
  SetClipboardData(CF_TEXT, AnsiClipBuffer);

  CloseClipboard();//关闭剪贴板
  free(UnicodeStr);// 释放内存
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
//卡片order第times次种植在地图的什么位置
void GetMapLoc(int account, int order, int times, int &row, int &column, int &level, int &depth)
{
  row = location[account][order][times] / 10;
  column = location[account][order][times] % 10;
  level = slot[account][order].level;
  for (depth = 0; depth < map[account][row][column].depth[level]; depth++)
    if (order == GetOrder(account, map[account][row][column].name[level][depth]) && times == map[account][row][column].times[level][depth])
      break;
}
//切换到账号account
void SwitchToAccount(int account)
{
  curAccount = account;
  //如果切换后当前波次不存在，则将当前波次设为0
  if (totalWaveExist[curAccount][curTotalWave[curAccount]] == 0)
    curTotalWave[curAccount] = 0;
  SelectDepth(1);
  if (bagMode >= 0)
    bagMode = -1;
  firstTrackSlot = -1;
  commonPage = 1;
}
int IsUndoAllowed()
{
  return currentBackup > 1;
}
int IsRepeatAllowed()
{
  return currentBackup < backupNum;
}
void VerticalLine(int x, int y1, int y2, int thick)
{
  setfillcolor(getlinecolor());
  solidrectangle(x - thick, y1, x + thick, y2);
  setfillcolor(defaultFillColor);
}
void HorizontalLine(int x1, int x2, int y, int thick)
{
  setfillcolor(getlinecolor());
  solidrectangle(x1, y - thick, x2, y + thick);
  setfillcolor(defaultFillColor);
}
//判断鼠标点击位置(x,y)在哪个区域
int GetArea(int originX, int originY)
{
  //0=空白区;101-121=卡槽，200-229=常用卡组，300=人物，1x11-1x79=地图（x=层级）,114514=轨道格子14卡槽514秒
  //1=阵型编辑按钮;2=轨道编辑按钮;400=翻页按钮;501=503=设置人物时间、继续作战时间、等待时间
  //600=自动布轨，601-603=布轨选项

  if (mode == 1 || mode == 2)
  {
    if (originX == 9999 && originY == 9999)
      return 3;//3：截图指令
    if (originX == 9999 && originY == 9998)
      return 40;//40：添加2P轨道指令
  }

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
      return 92;
    else if (x > modeX + modeWidth / 2)
      return 93;
  }

  if (mode == 1)//阵型编辑模式
  {
    if (x >= bagOptionX && x < bagOptionX + 2 * bagOptionWidth && y >= bagOptionY && y < bagOptionY + bagOptionHeight)
      return 10 + (x - bagOptionX) / bagOptionWidth;//10=防御卡背包，11=生产信息
    if (x >= playerX && x < playerX + accountNum * playerDistance && y < playerHeight) //1P/2P轨道切换
    {
      int account = (x - playerX) / playerDistance;
      int dx = x - playerX - account * playerDistance;
      if (dx < playerTitleWidth)
        return 20 + account;//选择xP
      else
        return 30 + account;//复制xP路径
    }
    if (accountNum == 1 && x >= playerX + playerDistance && x < playerX + playerDistance + 110 && y < playerHeight)
      return 40; //添加2P
    if (accountNum == 2 && x >= playerX + 2 * playerDistance && x < playerX + 2 * playerDistance + playerOptionWidth && y < playerHeight) //1P/2P轨道切换
      return 50; //移除2P
    if (x >= roleSlotX && x < roleSlotX + 16 * roleSlotWidth && y >= mapGridY + 7 * mapGridHeight)
      return 60; //编辑注释
    if (accountNum == 1 && x >= skillX && x < skillX + skillWidth && y >= skillY
      && y < skillY + skillHeight)
      return 70; //刷技能勾选框

    if (x >= undoX && x < undoX + undoWidth && y >= undoY && y < undoY + 2 * undoHeight)
      return 90 + (y - undoY) / undoHeight;//90=撤销，91=恢复

    if (x >= mapSlotX && x < mapSlotX + 15 * mapSlotWidth && y >= mapSlotY && y < mapSlotY + mapSlotHeight)
    {
      int n = (x - mapSlotX) / mapSlotWidth;
      if (n > slotNum[curAccount])
        return 0;
      if ((n + 1) * mapSlotWidth - (x - mapSlotX) < 20 && y - mapSlotY < 26)
        return 400 + n;//400-414：卡槽的补阵模式
      else if ((n + 1) * mapSlotWidth - (x - mapSlotX) < 38 && y - mapSlotY < 26)
        return 500 + n;//500-514：卡槽的索敌模式
      else
        return 100 + n;//100=铲子,101-114=卡槽
    }
    if (x >= mapSlotX + 14 * mapSlotWidth && x < mapSlotX + 15 * mapSlotWidth && y >= mapSlotY + mapSlotHeight && y < mapSlotY + 8 * mapSlotHeight)
    {
      int n = (y - mapSlotY) / mapSlotHeight + 14;
      if (n > slotNum[curAccount])
        return 0;
      if (15 * mapSlotWidth - (x - mapSlotX) < 20 && y - mapSlotY - (n - 14) * mapSlotHeight < 26)
        return 400 + n;//415-421：卡槽的补阵模式
      else if (15 * mapSlotWidth - (x - mapSlotX) < 38 && y - mapSlotY - (n - 14) * mapSlotHeight < 26)
        return 500 + n;//515-521：卡槽的索敌模式
      else
        return 100 + n;//115-121=卡槽
    }
    if (bagMode == -1)//防御卡选取模式
    {
      if (x >= bagX && x < bagX + 2 * bagWidth && y >= bagY && y < bagY + 14 * bagHeight)
      {
        int row = (y - bagY) / bagHeight;
        int column = (x - bagX) / bagWidth;
        int common = row * 2 + column;
        int result = 200 + row * 2 + column;//200-225=常用卡组，226=新增卡片
        if (result == 227) //227=背包向左翻页 228=背包向右翻页
        {
          if (x - bagX - bagWidth < 18)
            return 227;
          else if (bagX + 2 * bagWidth - x < 18)
            return 228;
        }
        else
          return result;
      }
    }
    else if (bagMode == -2) //生产信息编辑模式
    {
      if (x >= productionX + 2 * productionWidth - 18 && x < productionX + 2 * productionWidth && y >= productionY && y < productionY + 12 * productionHeight)
      {
        int n = (y - productionY) / productionHeight;
        if (y - productionY - n * productionHeight <= productionHeight / 2) return 2000 + n;//数值上调
        else return 2100 + n;//数值下调
      }
    }
    else //防御卡编辑模式
    {
      if (x >= bagX + 2 * bagWidth - 18 && x < bagX + 2 * bagWidth && y >= bagY && y < bagY + 8 * bagHeight)
      {
        int n = (y - bagY) / bagHeight;
        if (y - bagY - n * bagHeight <= bagHeight / 2) return 2200 + n;//数值上调1
        else return 2300 + n;//数值下调1
      }
      if (x >= bagX + bagWidth && x < bagX + bagWidth + 18 && y >= bagY && y < bagY + 8 * bagHeight)
      {
        int n = (y - bagY) / bagHeight;
        if (y - bagY - n * bagHeight <= bagHeight / 2) return 2400 + n;//数值上调10
        else return 2500 + n;//数值下调10
      }
      if (x >= bagX + bagWidth + 18 && x < bagX + 2 * bagWidth - 18 && y >= bagY && y < bagY + 8 * bagHeight)
      {
        int n = (y - bagY) / bagHeight;
        return 2600 + n;//点击数值本身
      }
    }
    if (x >= roleSlotX && x < roleSlotX + roleSlotWidth && y >= roleSlotY && y < roleSlotY + roleSlotHeight)
    {
      if (y >= roleSlotY + roleSlotHeight / 2)
        return 310;//点击人物槽下半部分选中人物2
      else
        return 300;//否则选中人物1
    }
    //魔塔、地图、波次
    if (x >= waveX && x < waveX + 4 * waveWidth)
    {
      if (y >= waveY && y < waveY + 3 * waveHeight)
        return 600 + (y - waveY) / waveHeight * 4 + (x - waveX) / waveWidth;//600-611=波次按钮
      else if (y >= buttonY && y < buttonY + buttonHeight)
      {
        if (x - buttonX < 22)
          return 615;//抓取地图
        else if (buttonX + buttonWidth - x <= 22)
          return 616;//删除地图
      }
    }
    //800-808=深度0-8
    if (x >= stackX && x < stackX + stackWidth && y >= stackY && y < stackY + maxViewStack * stackHeight)
      return 800 + (y - stackY) / stackHeight;
    if (x >= stackX && x < stackX + stackWidth && y >= stackY + maxViewStack * stackHeight && y < stackY + 10 * stackHeight)
    {
      if (x <= stackX + stackWidth / 3)
        return 809;
      else if (x >= stackX + stackWidth - stackWidth / 3)
        return 811;
      else
        return 810;
    }
    if (x >= mapGridX && x < mapGridX + 9 * mapGridWidth && y >= mapGridY && y < mapGridY + 7 * mapGridHeight)
    {
      int row = (y - mapGridY) / mapGridHeight;
      int column = (x - mapGridX) / mapGridWidth;
      int height = y - mapGridY - row * mapGridHeight;//剩余高度
      int width = x - mapGridX - column * mapGridWidth;//剩余宽度
      int level = 0;
      if (height <= 9)//最上层
      {
        if (width < 10)
          level = 6;//左：幻鸡
        else if (width < 25)
          level = 0;//中：铲子
        else
          level = 5;//右：冷却层
      }
      else if (height <= 28)
        level = 4;//第二：升级层
      else if (height <= 47)
        level = 3;//第三：主层
      else if (height <= 66)
        level = 2;//第四：护罩层
      else
        level = 1;//最下：承载层
      return 1011 + level * 100 + row * 10 + column;//1x11-1x79=地图（x=层级）
    }
    return 0;//不属于上述情况，视为空白区，返回0
  }
  else if (mode == 2)//轨道编辑模式
  {
    if (x >= modeX && x < modeX + modeWidth && y >= modeY && y < modeY + modeHeight)
      return 1;//阵型编辑切换按钮

    if (x >= playerX && x < playerX + accountNum * playerDistance && y < playerHeight) //1P/2P轨道切换
    {
      int account = (x - playerX) / playerDistance;
      int dx = x - playerX - account * playerDistance;
      if (dx < playerTitleWidth)
        return 20 + account;//选择xP
      else
        return 30 + account;//xP火苗显示
    }

    if (accountNum == 1 && x >= playerX + playerDistance && x < playerX + playerDistance + 110 && y < playerHeight)
      return 40; //添加2P
    if (accountNum == 2 && x >= playerX + 2 * playerDistance && x < playerX + 2 * playerDistance + playerOptionWidth && y < playerHeight) //1P/2P轨道切换
      return 50; //移除2P

    if (x >= undoX && x < undoX + undoWidth && y >= undoY && y < undoY + 2 * undoHeight)
      return 90 + (y - undoY) / undoHeight;//90=撤销，91=恢复

    //轨道卡槽
    if (x >= trackSlotX && x < trackSlotX + trackSlotWidth
      && y >= trackSlotY && y < trackSlotY + min(slotNum[curAccount] + 2, trackSlotNumPerPage + 1) * trackSlotHeight)//卡槽
    {
      int floor = (y - trackSlotY) / trackSlotHeight;
      int lineOrder = firstTrackSlot + floor;
      int order = lineOrder == -1 ? maxSlotNum + 1 : lineOrder;

      int height = (y - trackSlotY) % trackSlotHeight;
      if (x - trackSlotX < 15 && height < 14)
        return 200 + order;//200+=继承（左上角）
      else if (x - trackSlotX < 15 && height > trackSlotHeight - 14)
        return 300 + order;//300+=独立计时（左下角）
      else if (x - trackSlotX > trackSlotWidth - 15 && height < 14)
        return 400 + order;//400+=锁定轨道（右上角）
      else if (x - trackSlotX > trackSlotWidth - 15 && height > trackSlotHeight - 14)
        return 500 + order;//500+=极限成阵（右下角）
      else
        return 100 + order;//100+=卡槽本体
    }
    //600-611=波次按钮
    if (x >= waveX && x < waveX + 4 * waveWidth)
    {
      if (y >= waveY && y < waveY + 3 * waveHeight)
        return 600 + (y - waveY) / waveHeight * 4 + (x - waveX) / waveWidth;
    }
    //612=录制轨道，613=自动布轨，614=魔塔，615=退出时间
    if (x >= buttonX && x < buttonX + buttonWidth && y >= buttonY && y < buttonY + 4 * buttonHeight)
    {
      int n = (y - buttonY) / buttonHeight;
      return 612 + n;
    }

    //712=优先队列标题，700-711=第0-11张卡
    if (x >= queueX && x < queueX + 12 * queueWidth && y >= queueY && y < queueY + queueHeight)
      return 700 + (x - queueX) / queueWidth;
    if (x >= queueTitleX && x < queueTitleX + queueTitleWidth && y >= queueY && y < queueY + queueHeight)
      return 712;

    //801-810=翻页按钮
    if (x >= pageX && x < pageX + 4 * pageWidth && y >= pageY && y < pageY + 4 * pageHeight)
      return 801 + (y - pageY) / pageHeight * 4 + (x - pageX) / pageWidth;

    //900+=箭头
    if (x >= moveX && x < moveX + 3 * moveWidth && y >= moveY && y < moveY + moveHeight)
    {
      int n = (x - moveX) / moveWidth;//n=0~2
      if (n == zero)
        return 900;
      if (n == 2)
        return 901;//900-901=左右
      n = (y - moveY) / (moveHeight / 2);
      if (n == zero)
        return 902;
      if (n == 1)
        return 903;//902-903=上下
    }

    //2000-2600=时间轴
    if (x >= trackX && x < trackX + trackNum * timelineWidth && y >= playerHeight && y < queueY)
      return 2000 + (x - trackX) / timelineWidth;

    //100000+=轨道
    if (x >= trackX && x < trackX + trackNum * timelineWidth && y >= trackY
      && y < trackY + min(slotNum[curAccount] + 2, trackSlotNumPerPage + 1) * trackHeight)
    {
      int floor = (y - trackY) / trackHeight;
      int lineOrder = firstTrackSlot + floor;
      int order = lineOrder == -1 ? maxSlotNum + 1 : lineOrder;
      return 100000 + order * 1000 + (x - trackX) / timelineWidth;//轨道：114050=14号卡槽050秒（第2页的110秒区域也是050）
    }
    return 0;
  }
  else if (mode == 3)//录制轨道模式
  {
    if (recordStage == 2)
    {
      if (x >= recordX + recordLength + 13 && x < recordX + recordLength + 29
        && y >= recordY - 5 && y < recordY + 20 * recordHeight - 5)
        return 100 + (y + 5 - recordY) / recordHeight;//100+删除/恢复卡的本页编号
      if (y >= recordPageCY - 12 && y < recordPageCY + 12)
      {
        if (x >= recordPageCX - 42 - 12 && x <= recordPageCX - 42 + 12)
          return 201;//左翻页
        if (x >= recordPageCX + 42 - 12 && x <= recordPageCX + 42 + 12)
          return 202;//右翻页
      }
    }
    if (x >= recordEndCX - 50 && x <= recordEndCX + 50 && y >= recordEndCY - 20 && y <= recordEndCY + 20)
      return 300;//结束录制
    return 0;
  }
  return 0;
}
//账号account的常用卡common是否已选入卡槽
bool isCommonSelected(int account, int common)
{
  for (int order = 1; order <= slotNum[account]; order++)
    if (strcmp(slot[account][order].name, bag[account][common].name) == 0)
      return true;
  return false;
}
//向卡槽中添加常用卡组中的第common张卡片
void AddToSlot(int account, int common)
{
  if (isCommonSelected(account, common) == false && slotNum[account] < maxSlotNum)//如果该卡未被添加，且卡槽有空位
  {
    slotNum[account]++;
    slot[account][slotNum[account]] = bag[account][common];
  }
}
//删除这张卡的全部阵型
void DeleteMap(int account, int order)
{
  int row, column, depth;
  int level = slot[account][order].level;
  while (location[account][order][1] > zero) //如果有种植记录，全部删去（通过DeleteFromGrid函数，既删地图又删Loc）
  {
    row = location[account][order][1] / 10;
    column = location[account][order][1] % 10;
    for (depth = map[account][row][column].depth[level] - 1; depth >= zero; depth--) //从高深度往低深度删除
      if (GetOrder(account, map[account][row][column].name[level][depth]) == order)//该深度的卡编号为order才删除
        DeleteFromGrid(account, row, column, level, depth);
  }
  DecreaseToEmptyDepth();//选择深度高于LUMO，则降至LUMO
}

//向队列添加卡片
void AddQueue(int account, int order)
{
  if (order == 0 || order == maxSlotNum + 1)
    return;
  if (queueSize[account] >= 12)
    return;
  strcpy_s(queue[account][queueSize[account]++], slot[account][order].name);
}
//删除队列第qOrder张卡片
void DeleteQueue(int account, int qOrder)
{
  for (int i = qOrder; i < queueSize[account] - 1; i++)
    strcpy_s(queue[account][i], queue[account][i + 1]);
  queueSize[account]--;
}
//向队列第qOrder个位置插入卡片slotName
void InsertToQueue(int account, int qOrder, const char *slotName)
{
  for (int i = queueSize[account] - 1; i >= qOrder; i--)
    strcpy_s(queue[account][i + 1], queue[account][i]);
  strcpy_s(queue[account][qOrder], slotName);
  queueSize[account]++;
}
//将队列中第qOrder1张卡插入到qOrder2位置
void InsertQueue(int account, int qOrder1, int qOrder2)
{
  char slotName[10] = {};
  strcpy_s(slotName, queue[account][qOrder1]);
  DeleteQueue(account, qOrder1);
  InsertToQueue(account, qOrder2, slotName);
}
//删除卡槽中的第order张卡片（如果有种植记录也会清空）；删除针对此卡的卡片数量和跟随条件
void DeleteFromSlot(int account, int order)
{
  //删除针对此卡的卡片数量和跟随条件
  for (int i = 0; i <= slotNum[account]; i++)
  {
    if (strcmp(slot[account][i].card, slot[account][order].name) == 0)
      strcpy_s(slot[account][i].card, "");
    if (strcmp(slot[account][i].follow, slot[account][order].name) == 0)
      strcpy_s(slot[account][i].follow, "");
  }

  DeleteMap(account, order);
  for (int i = order; i < slotNum[account]; i++)//后面的卡槽向前递补
  {
    slot[account][i] = slot[account][i + 1];//卡片信息递补
    for (int times = 0; times <= maxPlantTimes; times++)//卡片种植位置递补
      location[account][i][times] = location[account][i + 1][times];
    for (int t = 0; t <= maxTime; t++)//卡片轨道递补
      track[account][i][t] = track[account][i + 1][t];
    for (int times = 0; times < maxPlantTimes; times++)
      moment[account][i][times] = moment[account][i + 1][times];
  }
  for (int times = 0; times <= maxPlantTimes; times++)
    location[account][slotNum[account]][times] = 0;//空出来的那个卡槽，种植位置全部设为0
  for (int t = 0; t <= maxTime; t++)
    track[account][slotNum[account]][t] = 0;//空出来的那个卡槽，轨道信息全部设为0
  for (int times = 0; times <= maxPlantTimes; times++)
    moment[account][slotNum[account]][times] = 0;//空出来的那个卡槽，种植时刻全部设为0
  slotNum[account]--;
  if (firstTrackSlot + trackSlotNumPerPage > slotNum[account] && firstTrackSlot > -1)
    firstTrackSlot--;
}
void ChangeSlot(int account, int order, int common)//将卡槽与常用卡组对换
{
  //如果换的是本卡，将卡槽卡片的层级、冷却赋值成该卡即可
  if (strcmp(bag[account][common].name, slot[account][order].name) == 0)
  {
    slot[account][order].cd = bag[account][common].cd;
    slot[account][order].level = bag[account][common].level;
    return;
  }
  //如果换的是一张已经携带的卡，直接退出
  if (isCommonSelected(account, common))
    return;
  //如果换的是一张未携带的卡，先删除针对原卡的卡片数量和跟随条件
  for (int i = 0; i <= slotNum[account]; i++)
  {
    if (strcmp(slot[account][i].card, slot[account][order].name) == 0)
      strcpy_s(slot[account][i].card, "");
    if (strcmp(slot[account][i].follow, slot[account][order].name) == 0)
      strcpy_s(slot[account][i].follow, "");
  }

  for (int row = 1; row <= 7; row++)
    for (int column = 1; column <= 9; column++)
      for (int level = 1; level <= 6; level++)
        for (int depth = 0; depth < maxStack; depth++)
          if (strcmp(map[account][row][column].name[level][depth], slot[account][order].name) == zero)
            strcpy_s(map[account][row][column].name[level][depth], bag[account][common].name);
  strcpy_s(slot[account][order].name, bag[account][common].name);
  slot[account][order].cd = bag[account][common].cd;
  slot[account][order].level = bag[account][common].level;
  slot[account][order].cost = bag[account][common].cost;
  slot[account][order].style = bag[account][common].style;
  slot[account][order].star = bag[account][common].star;
  slot[account][order].sun = bag[account][common].sun;
  slot[account][order].sunNum = bag[account][common].sunNum;
  slot[account][order].transfer = bag[account][common].transfer;
}
//交换两个卡槽的卡片（不涉及地图信息的修改）
void ExchangeSlot(int account, int order0, int order1)
{
  SlotType tempSlot = slot[account][order0];
  slot[account][order0] = slot[account][order1];
  slot[account][order1] = tempSlot;
  int temp;
  for (int times = 0; times <= maxPlantTimes; times++)
  {
    temp = location[account][order0][times];
    location[account][order0][times] = location[account][order1][times];
    location[account][order1][times] = temp;
  }
  for (int times = 0; times <= maxPlantTimes; times++)
  {
    temp = moment[account][order0][times];
    moment[account][order0][times] = moment[account][order1][times];
    moment[account][order1][times] = temp;
  }
  for (int t = 0; t <= maxTime; t++)
  {
    temp = track[account][order0][t];
    track[account][order0][t] = track[account][order1][t];
    track[account][order1][t] = temp;
  }
}
//将卡槽order0插入到order1位置
void InsertSlot(int account, int order0, int order1)
{
  if (order0 < order1)
    for (int order = order0; order < order1; order++)
      ExchangeSlot(account, order, order + 1);
  else
    for (int order = order0; order > order1; order--)
      ExchangeSlot(account, order, order - 1);
}
//写入错误信息到info，返回出错的行数line
int WriteInfo(char(&info)[1000], FILE *f, const char *path, int line, char *content, const char *tip)
{
  fclose(f);//关闭文件
  if (path == NULL) //没有填写path，则不需要记录info
    return line;
  int maxlen = 20;
  char shortPath[100], shortContent[100];
  int length = strlen(path);
  if (length > maxlen)
  {
    if (IsChineseSecond(path, length - maxlen))
      sprintf_s(shortPath, "...%s", path + length - maxlen + 1);//只显示最后39字节
    else
      sprintf_s(shortPath, "...%s", path + length - maxlen);//显示最后40字节
  }
  else
    strcpy_s(shortPath, path);//完整显示
  length = strlen(content);
  if (length > maxlen)
  {
    if (IsChineseFirst(content, maxlen - 1))
    {
      strncpy_s(shortContent, content, maxlen - 1);
      shortContent[maxlen - 1] = 0;
    }
    else
    {
      strncpy_s(shortContent, content, maxlen);
      shortContent[maxlen] = 0;
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
//检查账号account的轨道文件有哪些波次，填写WaveExist。
int GetWaveExist(int account)
{
  memset(totalWaveExist[account], 0, sizeof(totalWaveExist[account]));
  totalWaveExist[account][0] = 1;//第0波默认存在

  FILE *f;
  fopen_s(&f, fullTrackPath[curAccount], "r");
  char s[maxLineSize];
  int wave = 0, smallWave = 0;
  while (!feof(f))
  {
    fgets(f, s);
    if (strlen(s) == 0)
      break;
    if (ScanWave(s, &wave, &smallWave) == 1)
      totalWaveExist[account][wave * maxSmallWave + smallWave] = 1;
  }
  fclose(f);
  return 0;
}
//清除地图数据，但保留选中深度
void ClearMap_RetainDepth(int account)
{
  int depth[7][9][7] = {};
  for (int row = 1; row <= 7; row++) //记录选中深度
    for (int column = 1; column <= 9; column++)
      memcpy(depth[row - 1][column - 1], map[account][row][column].depthSelected, sizeof(depth[0][0]));
  memset(map[account], 0, sizeof(map[account]));//地图清空
  for (int row = 1; row <= 7; row++) //还原选中深度
    for (int column = 1; column <= 9; column++)
      memcpy(map[account][row][column].depthSelected, depth[row - 1][column - 1], sizeof(depth[0][0]));
}
//删除账号account的波次信息
void ClearWave(int account)
{
  memset(slot[account], 0, sizeof(slot[account]));
  memset(location[account], 0, sizeof(location[account]));
  ClearMap_RetainDepth(account);//地图清空
  memset(moment[account], 0, sizeof(moment[account]));
  memset(track[account], 0, sizeof(track[account]));
  slotNum[account] = 0;
  slot[account][0] = shovel;//卡槽0设为铲子
  slot[account][maxSlotNum + 1] = jewel;//最后一个卡槽设为宝石
}
bool isCupUsed;//酒杯灯是否使用过
//bool isWaterUsed;//开水壶是否使用过
//bool isPuddingUsed;//布丁是否使用过
//bool isPuddingInSlot12;
//从文件当前位置读取波次信息到账号account的内存中，指定读取slotNum张卡片。返回出错的行数
int LoadCurrentWave(int account, FILE *f, int slotNum0, const char *path, int &line, char(&info)[1000])
{
  ClearWave(account);//清空波次信息（卡槽数、地图轨道）
  char s[maxLineSize], segment[maxLineSize];//从文件读取的字符串，用来分割的字符串
  char tip[100];
  char *name = nullptr, *nextName = nullptr;
  //读取接下来的slotNum+2行。originOrder = -1代表宝石，0代表铲子
  for (int originOrder = -1; originOrder <= slotNum0; originOrder++)
  {
    fgets(f, s);
    line++;
    int order = originOrder == -1 ? maxSlotNum + 1 : originOrder;

    //如果卡槽还没读完，就遇到了空行或--第x波--，说明卡槽数填多了
    if (strlen(s) == 0 || IsWaveTitle(s))
    {
      sprintf_s(tip, "卡槽数=%d，但只读取到%d个卡槽", slotNum0, originOrder - 1);
      return WriteInfo(info, f, path, line, s, tip);
    }
    //第一行如果不是宝石，则从铲子（order=0）开始读
    if (originOrder == -1 && strstr(s, "宝石") == nullptr)
      order = originOrder = 0;
    SlotType &slot0 = slot[account][order];

    strcpy_s(segment, s);
    if (segment[0] == '\t')//没有卡片名称
      WriteInfo(info, f, path, line, s, "卡片名称不能为空");
    name = strtok_s(segment, "\t", &nextName);//从子串0读取卡牌名称（非空）
    if (order >= 1 && order <= maxSlotNum)//铲子和宝石无需添加到卡槽，其他卡需要添加
    {
      if (strlen(name) > 6)
        return WriteInfo(info, f, path, line, s, "卡片名称不能超过3个汉字");
      bool isCommonCard = false;//这张卡是否为背包中的卡
      for (int common = 0; common < bagNum[account]; common++)//通过名称查找是哪张常用卡
        if (strcmp(bag[account][common].name, name) == zero)
        {
          AddToSlot(account, common);//从卡组中向卡槽添加这张卡（包括名称、冷却时间、层级）
          isCommonCard = true;
          break;
        }
      if (!isCommonCard) //如果没有在常用卡中找到该卡
        AddEmptyToSlot(account, name);//向卡组中添加只有名称、其他信息为0的卡
    }

    //子串1：冷却|补极独索继|放卡条件
    name = strtok_s(NULL, "\t", &nextName);
    if (name == NULL || strstr(name, "|") != NULL)
      return WriteInfo(info, f, path, line, s, "第二项应为冷却时间和轨道状态");
    //按逗号分割"7[3]补极,火苗=120,血量=80,图像=hello"
    char *nextCondition = nullptr;
    char *condition = NewStrTok(name, ",", &nextCondition);//第一项：7[3]补极
    slot0.prior = strstr(condition, "绝") != nullptr;
    slot0.lock = strstr(condition, "x") != nullptr;
    slot0.repair = strstr(condition, "补") != nullptr;
    slot0.limit = strstr(condition, "极") != nullptr;
    slot0.independent = strstr(condition, "独") != nullptr;
    slot0.search = strstr(condition, "索") != nullptr;
    slot0.retain = strstr(condition, "继") != nullptr;
    sscanf_s(condition, "%d[%d]", &slot0.cd, &slot0.level);//冷却和层级
    if (order == zero && (slot0.cd < zero || slot0.cd > 320))
      return WriteInfo(info, f, path, line, s, "铲子cd范围0~320s");
    if (order != zero && (slot0.cd < 2 || slot0.cd > 320))
      return WriteInfo(info, f, path, line, s, "卡片cd范围2~320s");

    //第2-n项：13种极限条件
    memset(&slot0.delay, 0, sizeof(ConditionType));
    for (int i = 0; i < 13; i++)
    {
      condition = NewStrTok(nullptr, ",", &nextCondition);
      if (!condition)
        break;
      switch (i)
      {
      case 0:
        slot0.delay = atoi(condition);
        if (slot0.delay < 0 || slot0.delay > 960)
          return WriteInfo(info, f, path, line, s, "【延迟】范围为0~960");
        break;
      case 1:
        slot0.triggerMode = atoi(condition) ? 1 : 0;
        break;
      case 2:
        slot0.energy = atoi(condition);
        if (slot0.energy < 0 || slot0.energy > 15000)
          return WriteInfo(info, f, path, line, s, "【火苗】范围为0~15000");
        break;
      case 3: //波次：只能是m.n格式
      {
        if (!IsWaveLegal(condition)) //波次输入不合格
          return WriteInfo(info, f, path, line, s, waveErrorString);
        int mainWave = 0, smallWave = 0;
        int scannedNum = sscanf_s(condition, "%d.%d", &mainWave, &smallWave);
        if (smallWave > 0)
          sprintf_s(slot0.wave, "%d.%d", mainWave, smallWave);
        else if (mainWave > 0)
          sprintf_s(slot0.wave, "%d", mainWave);
        else
          sprintf_s(slot0.wave, "");
      }
      break;
      case 4:
        if (strcmp(condition, "NULL") == 0)
          strcpy_s(slot0.card, "");
        else
        {
          if (strlen(condition) > 6)
            return WriteInfo(info, f, path, line, s, "【卡片】长度不能超过3个汉字");
          strcpy_s(slot0.card, condition);
        }
        break;
      case 5:
        slot0.cardNum = atoi(condition);
        if (slot0.cardNum < 0 || slot0.cardNum > 63)
          return WriteInfo(info, f, path, line, s, "【卡片数量】范围为0~63");
        break;
      case 6:
        if (strcmp(condition, "NULL") == 0)
          strcpy_s(slot0.image, "");
        else
        {
          if (strlen(condition) > 8)
            return WriteInfo(info, f, path, line, s, "【图像】长度不能超过4个汉字");
          strcpy_s(slot0.image, condition);
        }
        break;
      case 7:
        slot0.imageNum = atoi(condition);
        if (slot0.imageNum < 0 || slot0.imageNum > 63)
          return WriteInfo(info, f, path, line, s, "【图像数量】范围为0~63");
        break;
      case 8://行数:a,4,a+4,a-4
        if (strcmp(condition, "NULL") == 0)
          strcpy_s(slot0.hunterRow, "");
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
          strcpy_s(slot0.hunterRow, condition);
        }
        break;
      case 9:
        if (strcmp(condition, "NULL") == 0)
          strcpy_s(slot0.hunterColumn, "");
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
          strcpy_s(slot0.hunterColumn, condition);
        }
        break;
      case 10:
        if (strcmp(condition, "NULL") == 0)
          strcpy_s(slot0.image, "");
        else
        {
          if (strlen(condition) > 6)
            return WriteInfo(info, f, path, line, s, "【跟随卡片】长度不能超过3个汉字");
          strcpy_s(slot0.follow, condition);
        }
        break;
      case 11:
        slot0.endNum = atoi(condition);
        if (slot0.endNum < 0 || slot0.endNum > 63)
          return WriteInfo(info, f, path, line, s, "【终止数量】范围为0~63");
        break;
      case 12:
        slot0.endTime = atoi(condition);
        if (slot0.endTime < 0 || slot0.endTime > 960)
          return WriteInfo(info, f, path, line, s, "【终止时间】范围为0~960");
        break;
      }
    }

    int times = 0;//种植次数
    int loc = 0, second = 0, decisecond = 0;//种植时刻和种植位置
    name = strtok_s(NULL, "\t", &nextName);
    while (name)
    {
      if (strlen(name) > 8 || strchr(name, '|') == NULL)
        return WriteInfo(info, f, path, line, s, "放卡格式应为ttt(.t)|xy，以Tab键分隔");

      times++;//读到了ttt.s|xy型轨道，则次数+1
      if (sscanf_s(name, "%d.%d|%d", &second, &decisecond, &loc) == 3)//有小数点就按小数读取
      {
        while (decisecond >= 10)
          decisecond = decisecond / 10;//只保留1位小数
      }
      else //没有小数点就按整数读取
      {
        sscanf_s(name, "%d|%d", &second, &loc);
        decisecond = 0;
      }

      //记录种植时刻
      moment[account][order][times] = second * 10 + decisecond;
      if ((moment[account][slotNum[account]][times] != zero && moment[account][slotNum[account]][times] < 10) || moment[account][slotNum[account]][times] > 9600)
        return WriteInfo(info, f, path, line, s, "放卡时间范围为1~960");
      if (second > zero)//如果时刻不是000
        track[account][order][second]++;//记入轨道

      //记录种植位置
      if (strcmp(slot0.name, "冰沙") == 0) //冰沙位置强制设为0
        loc = 0;
      int row = loc / 10, column = loc % 10;
      if (loc != zero)//如果位置不是00
      {
        if (row < 1 || row > 7 || column == zero)
          return WriteInfo(info, f, path, line, s, "行数范围为1~7，列数范围为1~9");
        AddToGrid(account, order, row, column);//向地图添加这次种植位置
      }
      name = strtok_s(NULL, "\t", &nextName);
    }

    //酒杯灯开启了极限成阵或有轨道，视为使用过
    if (strcmp(slot0.name, "酒杯灯") == 0 && (slot0.limit || moment[account][order][1]))
      isCupUsed = true;
  }
  return 0;
}
int LoadCurrentWave(int account, FILE *f, int slotNum0)
{
  int line = 0;
  char info[1000];
  return LoadCurrentWave(account, f, slotNum0, nullptr, line, info);
}
//在文件f中输入"--第%d(.%d)波--"并换行
void fprintWave(FILE *f, int totalWave)
{
  char waveString[10] = {};
  GetWaveString(waveString, totalWave);
  fprintf_s(f, "--第%s波--\n", waveString);
}
//读取轨道文件并检查。读取前所有信息归零。文件不存在返回-1，读取成功返回0，否则报告错误信息
//当checkAllWave开启时，检查每一波轨道
int LoadTrackFrom(int account, const char *path, char(&info)[1000], bool checkAllWave = false)
{
  char *name = nullptr, *nextName = nullptr;
  char s[maxLineSize];//每行读入的原始字符串，用来分割的字符串
  char Tip[100];
  /*待读信息归零*/
  slotNum[account] = 0;//预设参数和状态清零
  tower[account] = 0;

  if (!FileExist(path))//如果文件不存在，则不进行读取
  {
    strcpy_s(info, "轨道文件不存在。");
    return -1;
  }
  FILE *f;
  if (fopen_s(&f, path, "r"))
  {
    strcpy_s(info, "轨道文件无法打开。");
    return -2;
  }
  //至此轨道文件已成功打开

  int line = 0;//读取到的行数
  if (checkAllWave) //如果需要检查全部波次，则先看看有哪些波次
  {
    memset(totalWaveExist[account], 0, sizeof(totalWaveExist[account]));
    totalWaveExist[account][0] = 1;//第0波默认存在
    int existedWaveNum = 1;//轨道中的波次数量
    int wave = 0, smallWave = 0;
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
        totalWaveExist[account][wave * maxSmallWave + smallWave] = 1;
      }
    }
    fclose(f);
    fopen_s(&f, path, "r");
  }

  line = 0;//读取到的行数
  fgets(f, s);//读取第1行：轨道版本号
  line++;

  if (strstr(s, "轨道版本号=") == s) //有版本号则再读一行
  {
    fgets(f, s);//读取第2行：人物1位置
    line++;
  }

  strcpy_s(notation[account], "");//注释清空
  if (strstr(s, "注释=") == s) //有注释则读取注释，再读一行
  {
    strcpy_s(notation[account], s + 5);
    fgets(f, s);//读取第2行：人物1位置
    line++;
  }

  char roleString[100];
  if (sscanf_s(s, "人物1位置=%s", roleString, sizeof(roleString)) != 1) //不能读取人物1位置直接报错
    return WriteInfo(info, f, path, line, s, "缺少人物1位置");
  else //否则读取各个人物位置
  {
    nextName = nullptr;
    name = strtok_s(roleString, ",", &nextName);
    roleLocNum[account] = 0;//人物1个数预设为0
    while (name)
    {
      int loc = atoi(name);
      if (loc / 10 < 1 || loc / 10 > 7 || loc % 10 == zero)//人物1位置超出范围
        return WriteInfo(info, f, path, line, s, "行数范围为1~7，列数范围为1~9");
      roleLocNum[account]++;//人物1个数+1
      roleLoc[account][roleLocNum[account]] = loc;//记录人物1位置
      name = strtok_s(nullptr, ",", &nextName);
    }
  }
  fgets(f, s);//读取下一行：人物2位置/魔塔层数/卡槽数
  line++;

  if (sscanf_s(s, "人物2位置=%s", roleString, sizeof(roleString)) == 1)//如果读到了人物2位置，检查并再读一行
  {
    if (accountNum == 1) //只有单轨模式需要从轨道中读取人物2位置
    {
      nextName = nullptr;
      name = strtok_s(roleString, ",", &nextName);
      roleLocNum[1 - account] = 0;//人物2个数预设为0
      while (name)
      {
        int loc = atoi(name);
        if (loc / 10 < 1 || loc / 10 > 7 || loc % 10 == zero)//人物1位置超出范围
          return WriteInfo(info, f, path, line, s, "行数范围为1~7，列数范围为1~9");
        roleLocNum[1 - account]++;//人物2个数+1
        roleLoc[1 - account][roleLocNum[1 - account]] = loc;//记录人物2位置
        name = strtok_s(nullptr, ",", &nextName);
      }
    }
    fgets(f, s);
    line++;
  }

  if (sscanf_s(s, "魔塔层数=%d", &tower[account]) == 1)//如果读到了就再读一行
  {
    if (tower[account] < zero || tower[account] > 165)
      return WriteInfo(info, f, path, line, s, "魔塔层数范围为0~165");
    fgets(f, s);
    line++;
  }

  if (sscanf_s(s, "退出时间=%d", &quitTime[account]) == 1)//如果读到了就再读一行
  {
    if (quitTime[account] < zero || quitTime[account] > 960)
      return WriteInfo(info, f, path, line, s, "退出时间范围为0~960");
    fgets(f, s);
    line++;
  }

  //读取刷技能模式
  if (strstr(s, "刷技能模式=") == s)
  {
    isSkillMode = atoi(s + 11) != 0;
    fgets(f, s);
    line++;
  }

  //读取优先队列
  queueSize[account] = 0;
  if (strstr(s, "优先队列=") == s)
  {
    char *context = nullptr;
    char *name = strtok_s(s + 9, ",", &context);
    while (name)
    {
      strcpy_s(queue[account][queueSize[account]++], name);
      name = strtok_s(nullptr, ",", &context);
    }
    fgets(f, s);
    line++;
  }

  //如果有宝石波次，开启极限释放，再读一行
  int isJewelWaveExist = 0;
  if (strstr(s, "宝石波次=") == s)
  {
    if (strcmp(s, "宝石波次=") != 0)
      isJewelWaveExist = 1;
    fgets(f, s);
    line++;
  }

  //读取卡槽数
  int slotNum0;//临时卡槽数
  if (sscanf_s(s, "卡槽数=%d", &slotNum0) != 1)
    return WriteInfo(info, f, path, line, s, "缺少卡槽数");
  else
  {
    if (slotNum0 < 0 || slotNum0 > 21)
      return WriteInfo(info, f, path, line, s, "卡槽数范围为0~21");
  }

  if (checkAllWave == false) //正常读取，不检查轨道
  {
    if (curTotalWave[account] > 0)//要读的波次大于0，则跳到指定波次那一行
    {
      char waveString[20];
      GetWaveString(waveString, curTotalWave[account]);
      char waveTitle[20];
      sprintf_s(waveTitle, "--第%s波--", waveString);
      if (fskip(f, waveTitle) == 0)//跳到对应波数
      {
        char tip[30];
        sprintf_s(tip, "没有找到第%s波信息", waveString);
        return WriteInfo(info, f, path, line, s, tip);
      }
    }
    LoadCurrentWave(account, f, slotNum0);//读取当前波次的卡槽信息（不记录错误信息）
  }
  else //需要检查每一波轨道
  {
    for (int totalWave = 0; totalWave < maxTotalWave; totalWave++)
      if (totalWaveExist[account][totalWave] == 1)
      {
        //读取本波，如果出错则结束读取，返回行数（info已在LoadCurrentWave中填好）
        if (LoadCurrentWave(account, f, slotNum0, path, line, info) != 0)
          return line;
        //每读完一个波次再读一行
        fgets(f, s);
        line++;
        if (strlen(s) > 0 && !IsWaveTitle(s))
        {
          sprintf_s(Tip, "卡槽数=%d，但读完%d个卡槽仍未结束", slotNum0, slotNum0);
          return WriteInfo(info, f, path, line, s, Tip);
        }
      }
  }
  fclose(f);
  if (isJewelWaveExist == 1)
    slot[account][maxSlotNum + 1].limit = 1;
  return 0;
}
int LoadTrackFrom(int account, char *path)
{
  char info[1000];
  return LoadTrackFrom(account, path, info);
}
//载入所有账号的轨道。载入前会填写WaveExist，并调整curWave
void LoadTrack()
{
  for (int account = 0; account < accountNum; account++)
  {
    GetWaveExist(account);
    //如果当前波次不存在，则当前波次设为0
    if (totalWaveExist[account][curTotalWave[account]] == 0)
      curTotalWave[account] = 0;
    LoadTrackFrom(account, fullTrackPath[account]);
  }
}
//第order张卡的条件数量
int GetConditionNum(int account, int order)
{
  SlotType &slot0 = slot[account][order];
  int conditionNum = 0;
  if (slot0.endTime)
    conditionNum = 13;
  else if (slot0.endNum)
    conditionNum = 12;
  else if (strlen(slot0.follow))
    conditionNum = 11;
  else if (strlen(slot0.hunterColumn))
    conditionNum = 10;
  else if (strlen(slot0.hunterRow))
    conditionNum = 9;
  else if (slot0.imageNum)
    conditionNum = 8;
  else if (strlen(slot0.image))
    conditionNum = 7;
  else if (slot0.cardNum)
    conditionNum = 6;
  else if (strlen(slot0.card))
    conditionNum = 5;
  else if (strlen(slot0.wave))
    conditionNum = 4;
  else if (slot0.energy)
    conditionNum = 3;
  else if (slot0.triggerMode)
    conditionNum = 2;
  else if (slot0.delay)
    conditionNum = 1;
  return conditionNum;
}
void SaveCurrentWave(int account, FILE *f)
{
  for (int originOrder = -1; originOrder <= slotNum[account]; originOrder++)
  {
    int order = originOrder == -1 ? maxSlotNum + 1 : originOrder;
    SlotType &slot0 = slot[account][order];
    fprintf(f, "%s\t%d[%d]", slot0.name, slot0.cd, slot0.level);
    //写入卡牌名称、冷却、层级
    if (slot0.lock)
      fprintf(f, "x");//锁定轨道
    if (slot0.search)
      fprintf(f, "索");//索敌
    if (slot0.repair)
      fprintf(f, "补");//补阵
    if (slot0.limit)
      fprintf(f, "极");//极限成阵
    if (slot0.prior)
      fprintf(f, "绝");//绝对优先
    if (slot0.independent)
      fprintf(f, "独");//独立计时
    if (slot0.retain)
      fprintf(f, "继");//继承上一波
    //写入极限条件
    int conditionNum = GetConditionNum(account, order);
    if (conditionNum >= 1)
      fprintf(f, ",%d", slot0.delay);
    if (conditionNum >= 2)
      fprintf(f, ",%d", slot0.triggerMode);
    if (conditionNum >= 3)
      fprintf(f, ",%d", slot0.energy);
    if (conditionNum >= 4)
      fprintf(f, ",%s", slot0.wave);
    if (conditionNum >= 5)
      fprintf(f, ",%s", slot0.card);
    if (conditionNum >= 6)
      fprintf(f, ",%d", slot0.cardNum);
    if (conditionNum >= 7)
      fprintf(f, ",%s", slot0.image);
    if (conditionNum >= 8)
      fprintf(f, ",%d", slot0.imageNum);
    if (conditionNum >= 9)
      fprintf(f, ",%s", slot0.hunterRow);
    if (conditionNum >= 10)
      fprintf(f, ",%s", slot0.hunterColumn);
    if (conditionNum >= 11)
      fprintf(f, ",%s", slot0.follow);
    if (conditionNum >= 12)
      fprintf(f, ",%d", slot0.endNum);
    if (conditionNum >= 13)
      fprintf(f, ",%d", slot0.endTime);

    int times = 1;
    char TimeString[10], LocString[10];
    while (moment[account][order][times] > zero || location[account][order][times] > zero)//种植时刻和位置有一个不为0，就记录到文件中
    {
      int second = moment[account][order][times] / 10;//第times次种植的时刻
      int decisecond = moment[account][order][times] % 10;
      if (decisecond == zero)
        sprintf_s(TimeString, "%03d", second);
      else
        sprintf_s(TimeString, "%03d.%d", second, decisecond);
      int loc = location[account][order][times];
      sprintf_s(LocString, "%02d", loc);
      fprintf(f, "\t%s|%s", TimeString, LocString);
      times++;
    }
    fprintf(f, "\n");
  }
}
//保存头部信息
void SaveHead(int account, FILE *fout, int SlotNumVariation = 0)
{
  fprintf(fout, "轨道版本号=%s\n", version);
  fprintf(fout, "注释=%s\n", notation[account]);
  fprintf(fout, "人物1位置=");
  for (int times = 1; times <= roleLocNum[account]; times++)
  {
    fprintf(fout, "%d", roleLoc[account][times]);
    if (times < roleLocNum[account])
      fprintf(fout, ",");
    else
      fprintf(fout, "\n");
  }
  //存在2P位置时记入1P轨道
  if (account == 0 && roleLocNum[1] > 0)
  {
    fprintf(fout, "人物2位置=");
    for (int times = 1; times <= roleLocNum[1 - account]; times++)
    {
      fprintf(fout, "%d", roleLoc[1 - account][times]);
      if (times < roleLocNum[1 - account])
        fprintf(fout, ",");
      else
        fprintf(fout, "\n");
    }
  }
  fprintf(fout, "魔塔层数=%d\n", tower[account]);
  fprintf(fout, "退出时间=%d\n", quitTime[account]);
  fprintf(fout, "刷技能模式=%d\n", isSkillMode ? 1 : 0);

  fprintf(fout, "优先队列=");
  for (int i = 0; i < queueSize[account]; i++)
  {
    if (i > 0)
      fprintf(fout, ",");
    fprintf(fout, "%s", queue[account][i]);
  }
  fprintf(fout, "\n");

  fprintf(fout, "卡槽数=%d\n", slotNum[account] + SlotNumVariation);
}
//提示轨道文件已删除或移动，然后退出程序
void ReportNoTrack()
{
  PopMessage(hWndEditor, "轨道文件已删除或移动，\n请重新打开该文件再操作。");
  exit(0);
}
const int ADD_TO_SLOT = 0;
const int DELETE_FROM_SLOT = 1;
const int EXCHANGE_SLOT = 2;
const int CHANGE_SLOT = 3;
const int INSERT_SLOT = 4;
//四大卡槽修改操作 operation: 0=AddToSlot,1=DeleteFromSlot,2=ExchangeSlot,3=ChangeSlot
void SlotOperation(int account, int operation, int para1, int para2 = 0)
{
  if (!Copy(fullTrackPath[account], tempTrackForSave)) //当前轨道复制到临时文件
    ReportNoTrack();
  FILE *fin, *fout;
  fopen_s(&fin, tempTrackForSave, "r");//输入轨道：复制的临时文件
  fopen_s(&fout, fullTrackPath[account], "w");//输出轨道：TrackFile[account]

  char s[maxLineSize];
  int OriginSlotNum = slotNum[account];//记录修改前的卡槽数
  int SlotNumVariation = 0;//操作后卡槽数的改变量

  if (operation == ADD_TO_SLOT)
    SlotNumVariation = 1;
  else if (operation == DELETE_FROM_SLOT)
    SlotNumVariation = -1;

  //删除、更换卡槽时，删除队列中的此卡
  if (operation == DELETE_FROM_SLOT || operation == CHANGE_SLOT)
    for (int i = queueSize[account] - 1; i >= 0; i--)
      if (strcmp(queue[account][i], slot[account][para1].name) == 0)
        DeleteQueue(account, i);

  SaveHead(account, fout, SlotNumVariation);//写入文件头，卡槽数比原来多1
  fskip(fin, "卡槽数=");//跳过原文件头

  for (int totalWave = 0; totalWave < maxTotalWave; totalWave++)//为每一波添加卡槽
    if (totalWaveExist[account][totalWave] == 1)//如果波次存在
    {
      if (totalWave > 0)
      {
        fgets(fin, s);//跳过波次抬头
        fprintWave(fout, totalWave);//写入波次抬头
      }
      LoadCurrentWave(account, fin, OriginSlotNum);//读取波次信息
      if (operation == ADD_TO_SLOT)
        AddToSlot(account, para1);//添加卡槽
      else if (operation == DELETE_FROM_SLOT)
        DeleteFromSlot(account, para1);
      else if (operation == EXCHANGE_SLOT)
        ExchangeSlot(account, para1, para2);
      else if (operation == INSERT_SLOT)
        InsertSlot(account, para1, para2);
      else if (operation == CHANGE_SLOT)
        ChangeSlot(account, para1, para2);
      SaveCurrentWave(account, fout);//写入波次信息
    }
  fclose(fin);
  fclose(fout);
  remove(tempTrackForSave);
  LoadTrack();
}
//在轨道文件中添加卡槽
void AddToSlot_File(int account, int common)
{
  if (isCommonSelected(account, common) == false && slotNum[account] < maxSlotNum)
    SlotOperation(account, ADD_TO_SLOT, common);
}
//在轨道文件中删除卡槽
void DeleteFromSlot_File(int account, int order)
{
  SlotOperation(account, DELETE_FROM_SLOT, order);
  SelectDepth(1);//选中最高深度
}
//在轨道文件中交换卡槽
void ExchangeSlot_File(int account, int order1, int order2)
{
  SlotOperation(account, EXCHANGE_SLOT, order1, order2);
}
//在轨道文件中连续交换卡槽
void InsertSlot_File(int account, int order1, int order2)
{
  SlotOperation(account, INSERT_SLOT, order1, order2);
}
//在轨道文件中替换卡槽
void ChangeSlot_File(int account, int order, int common)
{
  SlotOperation(account, CHANGE_SLOT, order, common);
  SelectDepth(1);//选中最高深度
}
//保存当前波次轨道
void SaveTrackTo(int account, char *path)
{
  if (!Copy(fullTrackPath[account], tempTrackForSave)) //当前轨道复制到临时文件
    ReportNoTrack();
  FILE *fin, *fout;
  fopen_s(&fin, tempTrackForSave, "r");//输入轨道：复制的临时文件
  fopen_s(&fout, path, "w");//输出轨道：path
  SaveHead(account, fout);//写入文件头
  fskip(fin, "卡槽数=");//跳过文件头

  char s[maxLineSize];
  //如果curTotalWave > 0，curTotalWave之前的波数原封不动地复制过来（包括"--第curTotalWave波--"这一行）
  if (curTotalWave[account] > 0)
  {
    char waveString[10];
    GetWaveString(waveString, curTotalWave[account]);
    char waveTitle[20];
    sprintf_s(waveTitle, "--第%s波--", waveString);
    do
    {
      fgets(fin, s);
      fputs(fout, s);
    } while (strcmp(s, waveTitle) != 0);
  }
  SaveCurrentWave(account, fout);//把当前波的卡组和轨道写入文档
  if (fskip(fin, "--第", s))//跳到下一个波数（如果存在），将这一行写入s
  {
    do //后续内容原封不动地复制，直到文件结束
    {
      fputs(fout, s);
      fgets(fin, s);
    } while (strlen(s) > 0);
  }

  fclose(fin);
  fclose(fout);
  remove(tempTrackForSave);
}
//轨道是否改变
bool IsTrackChanged()
{
  //获取最新备份中的轨道数量和轨道路径
  int backupAccountNum = 0;//最新备份中的轨道数量
  char backupTrackPath[2][maxPath] = {};//最新备份中记载的轨道路径
  char path[3][maxPath] = {};//1P备份文件，2P备份文件，路径文件
  sprintf_s(path[2], "自动备份\\轨道文件\\%s\\%d[path].txt", runTimeString, currentBackup);
  FILE *f;
  if (fopen_s(&f, path[2], "r")) //不存在路径文件，视为相同
    return true;
  fscanf_s(f, "轨道数量=%d\n", &backupAccountNum); //读取轨道数量
  for (int account = 0; account < backupAccountNum; account++) //读取轨道路径
    fgets(f, backupTrackPath[account]);
  fclose(f);

  if (backupAccountNum != accountNum)
    return true;//备份轨道数量和当前轨道数量不一致，视为已改变
  for (int account = 0; account < backupAccountNum; account++)
    if (strcmp(backupTrackPath[account], fullTrackPath[account]) != 0)
      return true; //轨道路径有变化，视为已改变

  //如果轨道内容有变，视为已改变
  for (int account = 0; account < accountNum; account++)
  {
    sprintf_s(path[account], "自动备份\\轨道文件\\%s\\%d[%d].txt", runTimeString, currentBackup, account);
    if (!IsFileEqual(fullTrackPath[account], path[account]))
      return true;
  }
  return false;
}
//保存轨道完整路径
void SaveTrackPathTo(const char *path)
{
  FILE *f;
  fopen_s(&f, path, "w");
  fprintf(f, "轨道数量=%d\n", accountNum);
  for (int account = 0; account < accountNum; account++)
    fprintf(f, "%s\n", fullTrackPath[account]);
  fclose(f);
}
//保存编辑器句柄及当前打开的轨道路径
void SaveOpenedTrackPath()
{
  char path[maxPath] = {};
  sprintf_s(path, "自动备份\\轨道文件\\%s\\当前打开轨道.txt", runTimeString);
  FILE *f;
  fopen_s(&f, path, "w");
  fprintf(f, "编辑器句柄=%d\n", (int)hWndEditor);
  fprintf(f, "轨道数量=%d\n", accountNum);
  for (int account = 0; account < accountNum; account++)
    fprintf(f, "%s\n", fullTrackPath[account]);
  fclose(f);
}
//保存备份（1P轨道、2P轨道、轨道路径）
void SaveBackup()
{
  char path[4][maxPath] = {};//1P备份文件，2P备份文件，路径文件，当前打开轨道.txt
  if (IsTrackChanged())//如果轨道有变，进行备份
  {
    currentBackup++;
    backupNum = currentBackup;
    for (int account = 0; account < accountNum; account++)
    {
      sprintf_s(path[account], "自动备份\\轨道文件\\%s\\%d[%d].txt", runTimeString, currentBackup, account);
      SaveTrackTo(account, path[account]);
    }
    sprintf_s(path[2], "自动备份\\轨道文件\\%s\\%d[path].txt", runTimeString, currentBackup);
    SaveTrackPathTo(path[2]);
  }
}
//保存所有账号的轨道文件。如有变化，保存备份。
void SaveTrack()
{
  for (int account = 0; account < accountNum; account++)
    SaveTrackTo(account, fullTrackPath[account]);
  SaveBackup();//如果轨道改变，保存备份文件
  SaveOpenedTrackPath();//保存打开轨道的路径
}
//载入某一个备份
void LoadBackup(int offset)
{
  //获取最新备份中的轨道数量和轨道路径
  currentBackup = currentBackup + offset;
  char path[3][maxPath] = {};//1P备份文件，2P备份文件，路径文件
  sprintf_s(path[2], "自动备份\\轨道文件\\%s\\%d[path].txt", runTimeString, currentBackup);//路径文件
  FILE *f;
  if (fopen_s(&f, path[2], "r"))//打开路径文件
    return;
  fscanf_s(f, "轨道数量=%d\n", &accountNum); //读取轨道数量
  for (int account = 0; account < accountNum; account++) //读取轨道路径
    fgets(f, fullTrackPath[account]);
  fclose(f);
  if (accountNum == 1)//如果2P不存在，把当前轨道设为1P
    curAccount = 0;

  for (int account = 0; account < accountNum; account++)
  {
    sprintf_s(path[account], "自动备份\\轨道文件\\%s\\%d[%d].txt", runTimeString, currentBackup, account);
    Copy(path[account], fullTrackPath[account]);//将备份文件复制到当前轨道路径
  }
  LoadTrack();//读取当前波数轨道
  timeSelected = queueSelected = trackSlotSelected = state = 0;
  SelectDepth(1);//选中最高深度
}
//撤销操作
void Undo()
{
  LoadBackup(-1);
}
//恢复操作
void Repeat()
{
  LoadBackup(1);
}
//波次操作（operation 0=创建 1=复制 2=删除）
void WaveOperation(int account, int targetTotalWave, int operation)
{
  if (!Copy(fullTrackPath[account], tempTrackForSave)) //当前轨道复制到临时文件
    ReportNoTrack();
  FILE *fin, *fout;
  fopen_s(&fin, tempTrackForSave, "r");//输入轨道：复制的临时文件
  fopen_s(&fout, fullTrackPath[account], "w");//输出轨道：TrackFile[account]

  char s[maxLineSize];
  int wave = 0, smallWave = 0;
  while (1) //复制targetTotalWave波次之前的内容
  {
    fgets(fin, s);
    //如果targetTotalWave已存在（说明本波需要被删除或覆盖），跳过这个波次的内容
    if ((strstr(s, "卡槽数=") && targetTotalWave == 0)
      || (ScanWave(s, &wave, &smallWave) == 1 && targetTotalWave == wave * maxSmallWave + smallWave))
    {
      do
        fgets(fin, s);
      while (strlen(s) > 0 && !strstr(s, "--第"));
    }
    if (strlen(s) == 0 || (ScanWave(s, &wave, &smallWave) == 1 && wave * maxSmallWave + smallWave > targetTotalWave))
      break; //如果读到了文件末尾或大于targetTotalWave的波次，退出循环
    fputs(fout, s);
  }
  if (operation == 0 || operation == 1) //如果是创建波或复制波，写下目标波次的内容（移动波也会先复制）
  {
    if (targetTotalWave == 0)
      fprintf_s(fout, "卡槽数=%d\n", slotNum[account]); //写下卡槽数
    else
      fprintWave(fout, targetTotalWave); //写下目标波次
    totalWaveExist[account][targetTotalWave] = 1;
    curTotalWave[account] = targetTotalWave;//切换当前波
    if (operation == 0) //创建波要清空地图轨道再保存，复制波则直接保存
      ClearMapAndTrack(account);//清空地图和轨道，保留卡槽
    SaveCurrentWave(account, fout);//保存当前波
  }
  else //如果是删除波，则不用写入内容，直接将波次存在记为0
    totalWaveExist[account][targetTotalWave] = 0;

  if (strlen(s) > 0) //如果刚才没有读到文件结束，则需要继续复制后续内容；否则轨道已保存完成
  {
    fputs(fout, s);//写上第wave波
    while (1) //复制第targetWave波之后的内容
    {
      fgets(fin, s);
      if (strlen(s) == 0)
        break; //如果读到了文件末尾，退出循环
      fputs(fout, s);
    }
  }
  fclose(fin);
  fclose(fout);
  remove(tempTrackForSave);
  LoadTrack();//读取对应波次轨道到内存
}
//切换当前波次
void SelectWave(int account, int targetTotalWave)
{
  if (curTotalWave[account] == targetTotalWave)
    return; //不允许原地切换
  if (totalWaveExist[account][targetTotalWave] == 1)
  {
    curTotalWave[account] = targetTotalWave; //当前波次设为targetTotalWave
    LoadTrack();//读取对应波次轨道到内存
    SelectDepth(1);//选择最高深度
  }
}
//新建空波次
void AddWave(int account, int targetTotalWave)
{
  if (totalWaveExist[account][targetTotalWave])
    return; //不允许新建已存在的波次
  WaveOperation(account, targetTotalWave, 0);
  SelectDepth(1);//选择最高深度
}
//复制波次
void CopyWave(int account, int targetTotalWave)
{
  if (curTotalWave[account] == targetTotalWave)
    return; //不允许原地复制

  //继承信息是否需要删除
  bool isRetainCleared = (curTotalWave[account] == 0 && targetTotalWave > 0)
    || (curTotalWave[account] > 0 && targetTotalWave == 0);

  WaveOperation(account, targetTotalWave, 1);

  if (isRetainCleared) //删除继承信息
    for (int order = 0; order <= maxSlotNum + 1; order++)
      slot[account][order].retain = 0;
}
//删除波次
void DeleteWave(int account, int targetTotalWave)
{
  if (targetTotalWave == 0 || !totalWaveExist[account][targetTotalWave])
    return; //不允许删除第0波或不存在的波次
  WaveOperation(account, targetTotalWave, 2);
}
//移动波次
void MoveWave(int account, int targetTotalWave)
{
  if (curTotalWave[account] == 0 || curTotalWave[account] == targetTotalWave)
    return; //不允许移动第0波，也不允许原地移动

  //继承信息是否需要删除
  bool isRetainCleared = (curTotalWave[account] == 0 && targetTotalWave > 0)
    || (curTotalWave[account] > 0 && targetTotalWave == 0);

  int oldTotalWave = curTotalWave[account];
  CopyWave(account, targetTotalWave);
  DeleteWave(account, oldTotalWave);

  if (isRetainCleared)  //删除继承信息
    for (int order = 0; order <= maxSlotNum + 1; order++)
      slot[account][order].retain = 0;
}
//从文件中读取任务参数
void LoadParameter()
{
  FILE *f;
  char path[100], s[1000];

  zoom = 100;//缩放默认为100%
  strcpy_s(path, "用户参数\\编辑器参数.txt");
  if (!fopen_s(&f, path, "r"))//打开文件
  {
    fgets(f, s);//读取第1行
    if (strstr(s, "缩放比例") != NULL)
      zoom = atoi(s + 9);//读取缩放比例
    fclose(f);
  }
}
void SaveParameter()//把任务参数保存到文件
{
  FILE *f;
  char path[100];

  strcpy_s(path, "用户参数\\编辑器参数.txt");
  fopen_s(&f, path, "w");//打开文件
  fprintf(f, "缩放比例=%d\n", zoom);
  fclose(f);
}
//读取常用卡片的名称和冷却时间
int LoadCommonCard(FILE *f, char *path, int &line, int account, char(&info)[1000])
{
  const char NewTitle[200] = "名称\t冷却\t层级\t耗能\t生产特性\t星级\t单朵火苗\t火苗朵数\t转职";//未使用
  const char OldTitle[200] = "名称\t冷却\t层级\t耗能\t星级\t单朵火苗\t火苗朵数";
  char IsOldFile = 0;
  char s[1000], seg[1000];
  char *name = nullptr, *nextName = nullptr;

  bagNum[account] = 0;
  fgets(f, s);//第一行：标题行
  line++;
  if (strcmp(s, OldTitle) == zero)//旧版本
    IsOldFile = 1;
  while (!feof(f))
  {
    fgets(f, s);//读取每一行
    line++;
    if ((int)strlen(s) == zero || strstr(s, "----------"))
      break;
    strcpy_s(seg, s);
    RemoveBlank(seg);//空格改Tab
    if (seg[0] == '\t')
      return WriteInfo(info, f, path, line, s, "卡片名称不能为空");

    name = strtok_s(seg, "\t", &nextName);//第1项：名称
    if (strlen(name) > 6)
      return WriteInfo(info, f, path, line, s, "卡片名称不能超过3个汉字或6个字母");
    strcpy_s(bag[account][bagNum[account]].name, name);//名称

    name = strtok_s(NULL, "\t", &nextName);//第2项：冷却
    if (name != NULL)
    {
      int cd = atoi(name);
      if (cd < 2)
        bag[account][bagNum[account]].cd = 2;
      else
        bag[account][bagNum[account]].cd = cd;
    }
    else
      bag[account][bagNum[account]].cd = 7;

    name = strtok_s(NULL, "\t", &nextName);//第3项：层级
    bag[account][bagNum[account]].level = 3;//预设为普通层级
    for (int level = 1; level <= 6; level++)
    {
      if (strcmp(name, levelName[level]) == zero)
      {
        bag[account][bagNum[account]].level = level;
        break;
      }
    }

    name = strtok_s(NULL, "\t", &nextName);//第4项：耗能，没填就是NULL
    if (name != NULL)
    {
      int cost = atoi(name);
      if (cost < zero || cost > 999)
        return WriteInfo(info, f, path, line, s, "耗能范围为0~999，可以带+");
      if (name[strlen(name) - 1] == '+')//如果耗能带+
        bag[account][bagNum[account]].cost = cost + 1000;//150+记为1150
      else//如果不带+
        bag[account][bagNum[account]].cost = cost;//直接记录耗能，没填就是0
    }
    else
      bag[account][bagNum[account]].cost = 0;

    if (IsOldFile == zero) //只有新版本有第5项
    {
      name = strtok_s(NULL, "\t", &nextName);//第5项：生产特性：非生产/无秒产/火炉型/酒杯型/四转太/单轮型/炎焱兔/初始火/初始灯
      if (name != NULL)
      {
        bag[account][bagNum[account]].style = 0;//生产特性预设为0
        for (int style = 1; style <= 5; style++)//符合第几项就设为几
          if (strcmp(name, styleName[style]) == zero)
            bag[account][bagNum[account]].style = style;
      }
      else
        bag[account][bagNum[account]].style = 0;//没填就是0
      if (strcmp(bag[account][bagNum[account]].name, "炎焱兔") == zero)
        bag[account][bagNum[account]].style = 6;
      if (strcmp(bag[account][bagNum[account]].name, "初始火") == zero)
        bag[account][bagNum[account]].style = 7;
      if (strcmp(bag[account][bagNum[account]].name, "初始灯") == zero)
        bag[account][bagNum[account]].style = 8;
    }

    name = strtok_s(NULL, "\t", &nextName);//第6项：星级。
    if (name != NULL)
    {
      int star = atoi(name);
      if (star < zero || star>16)
        return WriteInfo(info, f, path, line, s, "星级范围为0~16");
      bag[account][bagNum[account]].star = star;
    }
    else
      bag[account][bagNum[account]].star = 0;//没填就是0星

    name = strtok_s(NULL, "\t", &nextName);//第7项：单朵产量，没填就是0
    if (name != NULL)
    {
      int sun = atoi(name);
      if (sun < zero)
        return WriteInfo(info, f, path, line, s, "单朵产量不能小于0");
      bag[account][bagNum[account]].sun = sun;
    }
    else
      bag[account][bagNum[account]].sun = 0;

    name = strtok_s(NULL, "\t", &nextName);//第8项：火苗数量，没填就是0
    if (name != NULL)
    {
      int num = atoi(name);
      if (num < zero || num > 8)
        return WriteInfo(info, f, path, line, s, "火苗朵数范围为0~8");
      bag[account][bagNum[account]].sunNum = num;
    }
    else
      bag[account][bagNum[account]].sunNum = 0;

    if (IsOldFile == zero)//只有新版本有备注
    {
      name = strtok_s(NULL, "\t", &nextName);//第9项：备注，没填就是0
      if (name != NULL)
      {
        if (strcmp(name, "一转") == zero)
          bag[account][bagNum[account]].transfer = 1;
        else if (strcmp(name, "二转") == zero)
          bag[account][bagNum[account]].transfer = 2;
        else
          bag[account][bagNum[account]].transfer = 0;
      }
      else
        bag[account][bagNum[account]].transfer = 0;
    }
    else //对于旧版本
    {
      char *FlashCard[3][3] = { { "小火炉", "大火炉", "火焰牛" }, { "酒杯灯", "", "" }, { "幻紫牛", "四转太", "" } };//火炉型2，酒杯型3，四转太4
      if (bag[account][bagNum[account]].sun > zero)
      {
        bag[account][bagNum[account]].style = 1;
        for (int i = 0; i <= 2; i++)
        {
          for (int j = 0; j <= 2; j++)
            if (strcmp(bag[account][bagNum[account]].name, FlashCard[i][j]) == zero)//属于秒产卡
            {
              bag[account][bagNum[account]].style = i + 2;
              break;
            }
          if (bag[account][bagNum[account]].style >= 2)
            break;
        }
      }
      else
        bag[account][bagNum[account]].style = 0;
    }
    bagNum[account]++;
    if (bagNum[account] >= maxCommonNum)
      break;
  }
  return 0;
}
int LoadCommonCard(char(&info)[1000])
{
  int line = 0;
  char *path = "防御卡背包.txt";
  FILE *f;
  if (fopen_s(&f, path, "r"))
  {
    sprintf_s(info, "【%s】无法打开。", path);
    return -2;
  }
  for (int account = 0; account < 2; account++)
  {
    int LoadResult = LoadCommonCard(f, path, line, account, info);
    if (LoadResult != zero)
      return LoadResult;
  }
  fclose(f);
  return 0;
}
void LoadCommonCard()
{
  char info[1000];
  if (LoadCommonCard(info) != 0)
  {
    MessageBox(NULL, info, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    exit(0);
  }
}
//保存常用卡片的名称和冷却时间
void SaveCommonCard()
{
  FILE *f;
  fopen_s(&f, "防御卡背包.txt", "w");
  for (int account = 0; account < 2; account++)
  {
    fprintf(f, "名称\t冷却\t层级\t耗能\t生产特性\t星级\t单朵火苗\t火苗朵数\t转职\n");
    for (int i = 0; i < bagNum[account]; i++)
    {
      /*名称，冷却，层级*/
      fprintf(f, "%s\t%ds\t%s\t", bag[account][i].name, bag[account][i].cd, levelName[bag[account][i].level]);
      /*耗能*/
      if (bag[account][i].cost >= 1000)
        fprintf(f, "%d+", bag[account][i].cost - 1000);//耗能带+
      else
        fprintf(f, "%d", bag[account][i].cost);//耗能不带+
      /*生产特性*/
      if (bag[account][i].style == 5 || bag[account][i].style == 6) //花火龙/炎焱兔
      {
        fprintf(f, "\t%s\t%d\t%d\t%d", styleName[bag[account][i].style], bag[account][i].star, 0, bag[account][i].sunNum);
        if (bag[account][i].style == 5)
          fprintf(f, "技能");
        fprintf(f, "\t");
        if (bag[account][i].transfer == 1)
          fprintf(f, "一转\n");
        else if (bag[account][i].transfer == 2)
          fprintf(f, "二转\n");
        else
          fprintf(f, "不转\n");
      }
      else if (bag[account][i].style == zero)//不是生产卡
        fprintf(f, "\n");//直接换行
      else if (bag[account][i].style >= 7) //同小火/同酒杯
        fprintf(f, "\t%s\n", styleName[bag[account][i].style]);
      else
        fprintf(f, "\t%s\t%d\t%d\t%d\n", styleName[bag[account][i].style], bag[account][i].star, bag[account][i].sun, bag[account][i].sunNum);
    }
    if (account == 0)
      fprintf(f, "-----------------------------------\n");
  }
  fclose(f);
}
void DeleteCommonCard()//删除正在编辑的防御卡
{
  for (int i = bagMode; i <= bagNum[curAccount] - 2; i++)
    bag[curAccount][i] = bag[curAccount][i + 1];
  memset(&bag[curAccount][bagNum[curAccount] - 1], 0, sizeof(bag[curAccount][bagNum[curAccount] - 1]));
  bagNum[curAccount]--;
  int MaxCommonPage = (bagNum[curAccount] - 1) / 26 + 1;
  if (commonPage > MaxCommonPage)
    commonPage = MaxCommonPage;
}
//读取生产信息
template<size_t size> int LoadProduction(FILE *f, char *path, int &line, int account, char(&info)[size])
{
  char s[1000], tip[100], format[100];
  char *option[8] = { "回火", "产能", "生产", "光能", "神佑", "疾风", "转化", "榴弹类型" };
  int OptionValue[8];
  int MinValue[8] = { -1, -1, -1, -1, -1, -1, -1, 1 };
  int MaxValue[8] = { 15, 15, 15, 15, 15, 15, 15, 2 };
  do //跳到“回火”那一行
  {
    fgets(f, s);
    line++;
    if (strlen(s) == 0)//如果文档已经读完了
      return WriteInfo(info, f, path, line, s, "文档中缺少“回火”项");
  } while (strstr(s, "回火=") == 0);
  for (int i = 0; i <= 7; i++)
  {
    sprintf_s(format, "%s%s", option[i], "=%d");
    if (sscanf_s(s, format, &OptionValue[i]) != 1) //不能读取直接报错
    {
      sprintf_s(tip, "本行应为：%s", option[i]);
      return WriteInfo(info, f, path, i + 1, s, tip);
    }
    else if (OptionValue[i] < MinValue[i] || OptionValue[i] > MaxValue[i]) //读取了还要检查范围
    {
      sprintf_s(tip, "%s范围为%d~%d", option[i], MinValue[i], MaxValue[i]);
      return WriteInfo(info, f, path, i + 1, s, tip);
    }
    if (i < 7)
    {
      fgets(f, s);
      line++;
    }
  }
  for (int i = 0; i <= 7; i++)//七颗宝石的星级和榴弹类型
    weapon[account][i] = OptionValue[i];
  return 0;
}
//读取生产信息
template<size_t size>
int LoadProduction(char(&info)[size])
{
  char s[1000];
  int line = 0;
  FILE *f;
  char *path = "生产信息.txt";
  fopen_s(&f, path, "r");
  if (f == NULL)
  {
    sprintf_s(info, "【%s】无法打开。", path);
    return -2;
  }

  char tip[100], format[100];
  int *value[4] = { &initialEnergy, &InnateFire, &InnateCup, &isDay };
  int maxValue[4] = { 10000, 30, 30, 1 };
  char *option[4] = { "初始火苗", "开局小火数量", "开局酒杯数量", "是否白天" };
  for (int i = 0; i < 4; i++)
  {
    fgets(f, s);
    line++;
    sprintf_s(format, "%s%s", option[i], "=%d");
    if (sscanf_s(s, format, value[i]) != 1)
    {
      sprintf_s(tip, "本行应为：%s", option[i]);
      return WriteInfo(info, f, path, line, s, tip);
    }
    else if (*value[i] < zero || *value[i] > maxValue[i])
    {
      sprintf_s(tip, "%s范围为0~%d", option[i], maxValue[i]);
      return WriteInfo(info, f, path, line, s, tip);
    }
  }

  int LoadResult = LoadProduction(f, path, line, 0, info);//读取1P生产信息
  if (LoadResult != zero)
    return LoadResult;
  fgets(f, s);
  line++;
  if (strstr(s, "------")) //如果读完1P后下一行是“------”
  {
    LoadResult = LoadProduction(f, path, line, 1, info);//读取2P生产信息
    if (LoadResult != zero)
      return LoadResult;
  }
  else //否则说明只有1P，将默认值赋给2P
  {
    for (int i = 0; i <= 6; i++)
      weapon[1][i] = -1;
    weapon[1][7] = 2;
  }
  fclose(f);
  return 0;
}
void LoadProduction()
{
  char info[1000];
  if (LoadProduction(info) != 0)
  {
    MessageBox(NULL, info, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    exit(0);
  }
}
void SaveProduction()//保存生产信息
{
  char WeaponString[8][13] = { "回火", "产能", "生产", "光能", "神佑", "疾风", "转化", "榴弹类型" };
  char *path = "生产信息.txt";
  FILE *f;
  fopen_s(&f, path, "w");
  fprintf(f, "初始火苗=%d\n", initialEnergy);
  fprintf(f, "开局小火数量=%d\n", InnateFire);
  fprintf(f, "开局酒杯数量=%d\n", InnateCup);
  fprintf(f, "是否白天=%d\n", isDay);
  for (int account = 0; account < 2; account++)
  {
    fprintf(f, "--------------------\n");
    for (int i = 0; i <= 7; i++)//七颗宝石的星级和榴弹类型
      fprintf(f, "%s=%d\n", WeaponString[i], weapon[account][i]);
  }
  fclose(f);
}
//显示缩放按钮
void ViewZoom()
{
  settextcolor(RGB(255, 255, 255));
  PaintHorizontalAdjust(zoomX, zoomY, zoomWidth, zoomHeight); //显示缩放比例
  char zoomString[10];
  sprintf_s(zoomString, "%d %%", zoom);
  CenterView(zoomString, zoomX + zoomWidth / 2, zoomY + zoomHeight / 2);
}
//模式：阵型编辑和轨道编辑
void ViewMode()
{
  PaintGrid(2, 1, modeX, modeY, modeWidth, modeHeight);
  int i;
  for (i = 0; i <= 1; i++)
  {
    if (mode == i + 1)
      settextcolor(RGB(255, 0, 0));
    else
      settextcolor(RGB(255, 255, 255));
    CenterView(modeString[i], modeX + modeWidth / 2, modeY + (2 * i + 1) * modeHeight / 2);
  }
  ViewZoom();//显示缩放按钮
}
void ViewMapSlotCard(int order)//显示卡槽卡片
{
  int left, top;
  COLORREF slotColor = levelColor[slot[curAccount][order].level];//卡片颜色
  if (order <= 14)
  {
    left = mapSlotX + order * mapSlotWidth;
    top = mapSlotY;
  }
  else
  {
    left = mapSlotX + 14 * mapSlotWidth;
    top = mapSlotY + (order - 14) * mapSlotHeight;
  }
  if (state - 100 == order)//如果选中了该卡片
  {
    setlinecolor(RGB(255, 0, 0));
    PaintGrid(1, 1, left, top, mapSlotWidth, mapSlotHeight);
    setlinecolor(RGB(255, 255, 255));
    settextcolor(RGB(255, 0, 0));
  }
  else
    settextcolor(slotColor);

  //显示卡槽序号
  CenterView(order, left + mapSlotWidth / 2 - 20, top + mapSlotHeight / 2 - 30);
  //显示卡片名称
  CenterView(slot[curAccount][order].name, left + mapSlotWidth / 2, top + mapSlotHeight / 2);
  //显示卡片CD
  char cdString[10];
  sprintf_s(cdString, "%ds", slot[curAccount][order].cd);
  CenterView(cdString, left + mapSlotWidth / 2, top + mapSlotHeight / 2 + 30);

  //显示补阵和索敌
  int size = GetFontSize();
  SetFontSize(16);

  COLORREF currentColor = gettextcolor();

  settextcolor(slot[curAccount][order].repair ? currentColor : RGB(128, 128, 128));
  CenterView("补", left + mapSlotWidth / 2 + 22, top + 14);

  settextcolor(slot[curAccount][order].search ? currentColor : RGB(128, 128, 128));
  CenterView("索", left + mapSlotWidth / 2 + 6, top + 14);

  SetFontSize(size);
}
void ViewRoleSlot()//显示人物槽
{
  int size = GetFontSize();
  line(roleSlotX, roleSlotY + roleSlotHeight / 2, roleSlotX + roleSlotWidth, roleSlotY + roleSlotHeight / 2);//分隔线
  for (int i = 0; i < GetRoleNum(); i++)
  {
    if ((state - 300) % 10 == 0 && i == (state - 300) / 10)
    {
      setlinecolor(RGB(255, 0, 0));
      PaintGrid(1, 1, roleSlotX, roleSlotY + i * roleSlotHeight / 2, roleSlotWidth, roleSlotHeight / 2);
      setlinecolor(RGB(255, 255, 255));
      settextcolor(RGB(255, 0, 0));
    }
    else
      settextcolor(roleColor);
    char RoleString[10];
    sprintf_s(RoleString, "%dP人物", i + 1);
    CenterView(RoleString, roleSlotX + roleSlotWidth / 2, roleSlotY + (2 * i + 1) * roleSlotHeight / 4);
  }
  SetFontSize(24);
  settextcolor(roleColor);
  if (GetRoleNum() == 1)
    CenterView("+", roleSlotX + roleSlotWidth / 2, roleSlotY + 3 * roleSlotHeight / 4);
  SetFontSize(size);
}
void ViewMapSlotCard()//显示全部卡槽卡片
{
  int order;
  PaintGrid(1, 1, roleSlotX, roleSlotY, roleSlotWidth, roleSlotHeight);//移动人物按钮框
  PaintGrid(1, 15, mapSlotX, mapSlotY, mapSlotWidth, mapSlotHeight);//卡槽框
  PaintGrid(7, 1, mapSlotX + 14 * mapSlotWidth, mapSlotY + mapSlotHeight, mapSlotWidth, mapSlotHeight);//卡槽框
  ViewRoleSlot();
  for (order = 0; order <= slotNum[curAccount]; order++)
    ViewMapSlotCard(order);
}
void ViewCommonCard(int common)//显示常用卡片
{
  int size;
  if (isCommonSelected(curAccount, common) == false)
    settextcolor(levelColor[bag[curAccount][common].level]);
  else
    settextcolor(RGB(128, 128, 128));
  int row = common % 26 / 2, column = common % 26 % 2;
  CenterView(bag[curAccount][common].name, bagX + (2 * column + 1) * bagWidth / 2 - 19, bagY + (2 * row + 1) * bagHeight / 2);
  char CdString[10];
  sprintf_s(CdString, "%ds", bag[curAccount][common].cd);
  CenterView(CdString, bagX + (2 * column + 1) * bagWidth / 2 + 33, bagY + (2 * row + 1) * bagHeight / 2);

  if (bag[curAccount][common].style >= 1 && bag[curAccount][common].style <= 5)
  {
    size = GetFontSize();
    SetFontSize(17);
    outtextxy(bagX + (column + 1) * bagWidth - 15, bagY + row * bagHeight, "★");
    SetFontSize(size);
  }

}
void ViewCommonCard()//显示全部常用卡片
{
  int order;
  for (int i = 0; i < 26; i++)
  {
    order = (commonPage - 1) * 26 + i;
    if (order < bagNum[curAccount])
      ViewCommonCard(order);
  }
  settextcolor(levelColor[0]);
  CenterView("【新增卡片】", bagX + bagWidth / 2, bagY + 27 * bagHeight / 2);
  char PageString[10];
  int MaxCommonPage = (bagNum[curAccount] - 1) / 26 + 1;
  sprintf_s(PageString, "%d/%d", commonPage, MaxCommonPage);
  CenterView(PageString, bagX + 3 * bagWidth / 2, bagY + 27 * bagHeight / 2);
  PaintHorizontalAdjust(bagX + bagWidth, bagY + 13 * bagHeight, bagWidth, bagHeight);
}
//根据isDark计算暗化颜色
COLORREF DarkColor(COLORREF origin, int isDark)
{
  if (isDark == 0)
    return origin;
  return RGB(GetRValue(origin) / 2, GetGValue(origin) / 2, GetBValue(origin) / 2);
}
//显示地图卡片,num为下标数字
void ViewGridCard(int account, int row, int column, int level, const char *name, int times, COLORREF color, int num)
{
  int height[5] = { 0, 33, 14, -5, -24 };//1-4层级高度
  settextcolor(color);
  CenterView(name, mapGridX + (2 * column - 1) * mapGridWidth / 2 - 13, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level]);
  //显示名称
  int size = GetFontSize();
  SetFontSize(18);
  if (times >= 10 && num >= 10)
    CenterView(times, mapGridX + (2 * column - 1) * mapGridWidth / 2 + 25, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level]);
  else
    CenterView(times, mapGridX + (2 * column - 1) * mapGridWidth / 2 + 26, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level]);
  //显示种植次序
  if (map[account][row][column].depth[level] >= 2)//至少有两张卡，才显示下标
  {
    SetFontSize(12);
    if (times >= 10 && num >= 10)
      CenterView(num, mapGridX + (2 * column - 1) * mapGridWidth / 2 + 40, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level] + 4);
    else
      CenterView(num, mapGridX + (2 * column - 1) * mapGridWidth / 2 + 39, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level] + 4);
  }
  //显示叠卡数量
  SetFontSize(size);
}
void ViewGridCard(int account, int row, int column, int level)//显示地图某行某列某层级的卡片（多深度下只显示选中深度）
{
  int isDark = 0;//是否使用暗色
  if (account != curAccount)
  {
    if (map[curAccount][row][column].depth[level] > 0)
      return;
    isDark = 1;
  }
  int Times, size, t, order;
  int height[5] = { 0, 33, 14, -5, -24 };
  char MagicString[10];
  int MagicTargetLevel;//幻幻鸡复制目标的层级
  int depth = map[account][row][column].depth[level] - 1;//最高深度
  int DepthSelected = map[account][row][column].depthSelected[level];//选中深度
  if (depth < zero)
    return;//该层级没卡，则不显示
  if (1000 + level * 100 + row * 10 + column == state)//如果该层级被选中，红色强调
  {
    settextcolor(RGB(255, 0, 0));//红色
    setlinecolor(RGB(255, 0, 0));
  }
  else
  {
    setlinecolor(DarkColor(levelColor[level], isDark));//层级颜色
    settextcolor(DarkColor(levelColor[level], isDark));
  }
  /*对幻幻鸡的特殊处理：先查询其复制的卡牌和层级，再执行显示*/
  if (level == 6) //如果是幻鸡层级，左上角显示蓝色实心圆点，空层级时显示空心圆点
  {
    //只有模仿层级没有普通卡时，才显示幻幻鸡。
    if (DepthSelected == depth + 1) //如果选中了最高深度上面的空深度
      rectangle(mapGridX + (column - 1) * mapGridWidth + 2, mapGridY + (row - 1) * mapGridHeight + 2, mapGridX + (column - 1) * mapGridWidth + 8, mapGridY + (row - 1) * mapGridHeight + 8);
    else
    {
      setfillcolor(getlinecolor());
      solidrectangle(mapGridX + (column - 1) * mapGridWidth + 2, mapGridY + (row - 1) * mapGridHeight + 2, mapGridX + (column - 1) * mapGridWidth + 8, mapGridY + (row - 1) * mapGridHeight + 8);
      setfillcolor(defaultFillColor);
      order = GetOrder(account, "幻幻鸡");//得到幻鸡在卡槽中的编号
      t = GetTime(account, order, map[account][row][column].times[level][DepthSelected]);
      //得到幻鸡这次种植的时刻t。如果没有轨道，返回MaxTime+1
      if (t == zero)//如果本次幻幻鸡没有轨道
      {
        strcpy_s(MagicString, "幻幻鸡");//显示原名
        Times = map[account][row][column].times[level][DepthSelected];
        MagicTargetLevel = 3;//在普通层级显示
      }
      else//如果找到了幻幻鸡轨道
      {
        order = GetMagicTarget(account, t);//寻找幻幻鸡复制的卡片编号
        if (order == zero)//如果没有复制卡
        {
          strcpy_s(MagicString, "幻幻鸡");//显示原名
          Times = map[account][row][column].times[level][DepthSelected];
          MagicTargetLevel = 3;//在普通层级显示
        }
        else
        {
          strcpy_s(MagicString, slot[account][order].name);//显示被复制的名称
          Times = map[account][row][column].times[level][DepthSelected];//次数
          MagicTargetLevel = slot[account][order].level;//在复制目标卡片所属层级显示
        }
      }
      if (map[account][row][column].depth[MagicTargetLevel] == zero)//如果幻鸡目标层级没有卡片，才显示幻鸡文字
        ViewGridCard(account, row, column, MagicTargetLevel, MagicString, Times, gettextcolor(), DepthSelected + 1);
    }
  }
  else if (level == zero) //如果是铲子：次左上角显示黄色数字
  {
    if (DepthSelected == depth + 1) //如果选中了最高深度上面的空深度
      rectangle(mapGridX + (column - 1) * mapGridWidth + 10, mapGridY + (row - 1) * mapGridHeight + 2, mapGridX + (column - 1) * mapGridWidth + 24, mapGridY + (row - 1) * mapGridHeight + 8);
    else
    {
      size = GetFontSize();
      SetFontSize(14);
      CenterView(map[account][row][column].times[level][DepthSelected], mapGridX + (column - 1) * mapGridWidth + 17, mapGridY + (row - 1) * mapGridHeight + 6);
      SetFontSize(size);
    }
  }
  else if (level == 5) //如果是冰激凌，画蓝线+显示数字
  {
    if (DepthSelected == depth + 1) //如果选中了最高深度上面的空深度
      PaintGrid(1, 1, mapGridX + (column - 1) * mapGridWidth + 26, mapGridY + (row - 1) * mapGridHeight + 2, mapGridWidth - 28, 6);
    else
    {
      HorizontalLine(mapGridX + (column - 1) * mapGridWidth + 26, mapGridX + column * mapGridWidth - 18, mapGridY + (row - 1) * mapGridHeight + 5, 1);
      size = GetFontSize();
      SetFontSize(14);
      CenterView(map[account][row][column].times[level][DepthSelected], mapGridX + column * mapGridWidth - 9, mapGridY + (row - 1) * mapGridHeight + 6);
      SetFontSize(size);
    }
  }
  else //如果是普通层级，显示选中深度的卡名和次序，选中空深度显示一个框
  {
    if (DepthSelected == depth + 1) //如果选中了最小空深度，显示矩形框
      PaintGrid(1, 1, mapGridX + (column - 1) * mapGridWidth + 4, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level] - 8, mapGridWidth - 8, 14);
    else
    {
      if (level == 1)//如果要显示的是承载卡, 第2,3层级种植时间<=承载卡种植时间，则以蓝色标注
      {
        int TrayTime = GetTime(account, GetOrder(account, map[account][row][column].name[1][0]), map[account][row][column].times[1][0]);//承载卡种植时间
        if (TrayTime > zero)//如果承载卡有种植时间，则考虑是否需要报错
          for (int Level = 2; Level <= 3; Level++)//对该格的的2,3层级，若存在卡片，统计第0深度的种植时间
            if (map[account][row][column].depth[Level] > zero)
            {
              int PlantTime = GetTime(account, GetOrder(account, map[account][row][column].name[Level][0]), map[account][row][column].times[Level][0]);
              if (PlantTime <= TrayTime && PlantTime > zero)//如果第2,3层级种植时间<=承载卡种植时间
              {
                setfillcolor(RGB(0, 0, 255));//蓝色
                solidrectangle(mapGridX + (column - 1) * mapGridWidth + 4, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level] - 10,
                  mapGridX + column * mapGridWidth - 4, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level] + 8);//背景涂蓝
                setfillcolor(defaultFillColor);
                break;
              }
            }
      }
      if (1000 + level * 100 + row * 10 + column == state)//如果该层级被选中，红色文字强调
      {
        PaintGrid(1, 1, mapGridX + (column - 1) * mapGridWidth + 3, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level] - 9, mapGridWidth - 5, 18);
        ViewGridCard(account, row, column, level, map[account][row][column].name[level][DepthSelected], map[account][row][column].times[level][DepthSelected], RGB(255, 0, 0), DepthSelected + 1);
      }
      else
        ViewGridCard(account, row, column, level, map[account][row][column].name[level][DepthSelected], map[account][row][column].times[level][DepthSelected], DarkColor(levelColor[level], isDark), DepthSelected + 1);
    }
  }
  setlinecolor(RGB(255, 255, 255));
}
//判断row行column列人物显示层级
int RoleLevel(int row, int column)
{
  int level[4] = { 3, 4, 2, 1 };
  for (int i = 0; i < 4; i++) //如果某个层级没有卡片，就判断人物显示在这个层级
    if (map[curAccount][row][column].depth[level[i]] == 0)
      return level[i];
  return 3;//都有卡片就显示在3层级
}
//显示单个人物
void ViewRole(int account, int times)
{
  int height[5] = { 0, 33, 14, -5, -24 };
  int row = roleLoc[account][times] / 10;
  int column = roleLoc[account][times] % 10;
  int level = RoleLevel(row, column);
  char roleString[10] = {};
  sprintf_s(roleString, "%dP人物", account + 1);
  //选中的人物变成红色
  if (state == 300 + 10 * account + times)
  {
    setlinecolor(RGB(255, 0, 0));
    PaintGrid(1, 1, mapGridX + (column - 1) * mapGridWidth + 3, mapGridY + (2 * row - 1) * mapGridHeight / 2 - 9 + height[level], mapGridWidth - 5, 18);
    setlinecolor(RGB(255, 255, 255));
    settextcolor(RGB(255, 0, 0));
  }
  else
    settextcolor(roleColor);

  CenterView(roleString, mapGridX + (2 * column - 1) * mapGridWidth / 2 - 13, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level]);
  int size = GetFontSize();
  SetFontSize(18);
  CenterView(times, mapGridX + (2 * column - 1) * mapGridWidth / 2 + 26, mapGridY + (2 * row - 1) * mapGridHeight / 2 + height[level]);
  SetFontSize(size);
}
void ViewGridCard(int account)//显示全部地图卡片（按Loc显示）
{
  int row, column, level;
  for (row = 1; row <= 7; row++)
    for (column = 1; column <= 9; column++)
      for (level = 0; level <= 6; level++)
        ViewGridCard(account, row, column, level);
}
void ViewGridCard()//显示全部地图卡片（按Loc显示）
{
  //显示两个账号地图卡片
  ViewGridCard(0);
  if (accountNum == 2)
    ViewGridCard(1);
  //显示每个人物位置
  for (int account = 0; account < 2; account++)
    for (int times = 1; times <= roleLocNum[account]; times++)
      ViewRole(account, times);
}
//获取卡片显示位置
int GetSlotFloor(int order)
{
  return order > maxSlotNum ? -1 - firstTrackSlot : order - firstTrackSlot;
}
void ViewTrackSlotCard(int order)//显示轨道卡片
{
  int floor = GetSlotFloor(order);//显示位置
  if (trackSlotSelected - 100 == order)
  {
    settextcolor(RGB(255, 0, 0));
    setlinecolor(RGB(255, 0, 0));
    PaintGrid(1, 1, trackSlotX, trackSlotY + floor * trackSlotHeight, trackSlotWidth, trackSlotHeight);
    setlinecolor(RGB(255, 255, 255));
  }
  else
    settextcolor(levelColor[slot[curAccount][order].level]);
  /*卡片名称、冷却、编号*/
  CenterView(slot[curAccount][order].name, trackSlotX + trackSlotWidth / 2 - 19,
    trackSlotY + (2 * floor + 1) * trackSlotHeight / 2);
  char cdString[10];
  sprintf_s(cdString, "%ds", slot[curAccount][order].cd);
  CenterView(cdString, trackSlotX + trackSlotWidth / 2 + 35,
    trackSlotY + (2 * floor + 1) * trackSlotHeight / 2);
  int size = GetFontSize();
  SetFontSize(14);
  if (order == maxSlotNum + 1)
    outtextxy(trackSlotX + 1, trackSlotY + floor * trackSlotHeight, "-1");
  else
    outtextxy(trackSlotX + 1, trackSlotY + floor * trackSlotHeight, order);

  int locTimes = GetLocTimes(curAccount, order);
  int momentTimes = GetMomentTimes(curAccount, order);
  if (locTimes > 0 || momentTimes > 0)
  {
    char Message[20];
    sprintf_s(Message, "%d", locTimes);
    CenterView(Message, trackSlotX - textwidth(Message) / 2 - 5, trackSlotY + (2 * floor + 1) * trackSlotHeight / 2 - 8);
    sprintf_s(Message, "%d", momentTimes);
    CenterView(Message, trackSlotX - textwidth(Message) / 2 - 5, trackSlotY + (2 * floor + 1) * trackSlotHeight / 2 + 9);
  }

  COLORREF currentColor = gettextcolor();
  SetFontSize(size);


  size = GetFontSize();

  SetFontSize(17);
  settextcolor(slot[curAccount][order].prior ? currentColor : RGB(128, 128, 128));
  CenterView("★", trackSlotX + trackSlotWidth - 8, trackSlotY + (2 * floor + 1) * trackSlotHeight / 2 - 13);

  SetFontSize(16);
  settextcolor(slot[curAccount][order].independent ? currentColor : RGB(128, 128, 128));
  CenterView("in", trackSlotX + 8, trackSlotY + (2 * floor + 1) * trackSlotHeight / 2 + 15);

  SetFontSize(size);

  settextcolor(slot[curAccount][order].limit ? currentColor : RGB(128, 128, 128));
  CenterView("∞", trackSlotX + trackSlotWidth - 8, trackSlotY + (2 * floor + 1) * trackSlotHeight / 2 + 16);
}
void ViewTrackSlotCard()//显示全部轨道卡片
{
  PaintGrid(min(slotNum[curAccount] + 2, trackSlotNumPerPage + 1), 1,
    trackSlotX, trackSlotY, trackSlotWidth, trackSlotHeight);//卡槽框
  for (int order = firstTrackSlot; order <= slotNum[curAccount] && order <= firstTrackSlot + trackSlotNumPerPage; order++)
    ViewTrackSlotCard(order == -1 ? maxSlotNum + 1 : order);
}
//用颜色color填充轨道
void BarTrack(int order, int t, COLORREF color)
{
  int floor = GetSlotFloor(order);//显示位置：宝石=-1，卡片= order - TrackBegin
  setfillcolor(color);
  solidrectangle(trackX + t * trackWidth, trackY + floor * trackHeight,
    trackX + (t + 1) * trackWidth - 1, trackY + (floor + 1) * trackHeight - 1);
  setfillcolor(defaultFillColor);
}
//当value非零时：若dest为空，将value写入dest；否则在dest后面添加","和value
void strAdd(char(&dest)[50], const char *format, int value)
{
  if (value)
  {
    if (dest[0])
    {
      char newFormat[20] = {};
      char content[50] = {};
      sprintf_s(newFormat, ",%s", format);
      sprintf_s(content, newFormat, value);
      strcat_s(dest, content);
    }
    else
      sprintf_s(dest, format, value);
  }
}
void strAdd(char(&dest)[50], const char *format, char *value)
{
  if (value && value[0])
  {
    if (dest[0])
    {
      char newFormat[20] = {};
      char content[50] = {};
      sprintf_s(newFormat, ",%s", format);
      sprintf_s(content, newFormat, value);
      strcat_s(dest, content);
    }
    else
      sprintf_s(dest, format, value);
  }
}
void strAdd(char(&dest)[50], const char *format, char *str, int value)
{
  if (str && str[0] && value)
  {
    if (dest[0])
    {
      char newFormat[20] = {};
      char content[50] = {};
      sprintf_s(newFormat, ",%s", format);
      sprintf_s(content, newFormat, str, value);
      strcat_s(dest, content);
    }
    else
      sprintf_s(dest, format, str, value);
  }
}
//获得放卡条件显示
void GetConditionString(int account, int order, char(&upper)[50], char(&lower)[50])
{
  //上方：10000火,2.1波,小火炉x5
  //下方：轰隆隆x1,(a+2,7+4),@查克拉
  SlotType &slot0 = slot[account][order];
  char upperTemp[50] = {};
  strcpy_s(upperTemp, "");
  strcpy_s(lower, "");
  if (slot0.delay > 0)
    strAdd(upperTemp, "%ds", slot0.delay);
  if (slot0.energy > 0)
    strAdd(upperTemp, "%d火", slot0.energy);
  if (strcmp(slot0.wave, "0") != 0)
    strAdd(upperTemp, "%s波", slot0.wave);
  if (slot0.card[0] && slot0.cardNum > 0)
    strAdd(upperTemp, "%sx%d", slot0.card, slot0.cardNum);
  if (slot0.triggerMode)
    sprintf_s(upper, "★%s", upperTemp);
  else
    strcpy_s(upper, upperTemp);

  if (slot0.image[0] && slot0.imageNum > 0)
    strAdd(lower, "%sx%d", slot0.image, slot0.imageNum);

  //索敌显示
  char content[50] = {};
  if (slot0.hunterRow[0] && slot0.hunterColumn[0])
    sprintf_s(content, "(%s,%s)", slot0.hunterRow, slot0.hunterColumn);
  else if (slot0.hunterRow[0])
    sprintf_s(content, "(%s,-)", slot0.hunterRow);
  else if (slot0.hunterColumn[0])
    sprintf_s(content, "(-,%s)", slot0.hunterColumn);
  strcat_s(lower, content);
  if (slot0.follow[0])
    strAdd(lower, "@%s", slot0.follow);
}
//显示放卡优先队列
void ViewQueue()
{
  PaintGrid(1, 1, queueTitleX, queueY, queueTitleWidth, queueHeight);
  PaintGrid(1, 1, trackX, queueY, trackWidth, queueHeight);
  PaintGrid(1, maxQueueSize, queueX, queueY, queueWidth, queueHeight);

  int size = GetFontSize();
  SetFontSize(18);
  settextcolor(queueSelected == 712 ? RGB(255, 0, 0) : WHITE);
  CenterView("优先放卡队列", queueTitleX + queueTitleWidth / 2, queueY + queueHeight / 2);
  for (int i = 0; i < queueSize[curAccount]; i++)
  {
    settextcolor(queueSelected == 700 + i ? RGB(255, 0, 0) : WHITE);
    CenterView(queue[curAccount][i], queueX + (2 * i + 1) * queueWidth / 2, queueY + queueHeight / 2);
  }
  SetFontSize(size);
}
//显示轨道
void ViewTrack()
{
  //颜色：0=不显示，1=正常轨道，2=紧凑轨道间隙，3=冷却冲突轨道间隙
  int size = GetFontSize();
  int lastTrack;//上一个轨道值
  char momentString[10];//时间字符串，如001,020,205.6,205.6(12)
  char timesString[12];//次数字符串，如3,4-5,100-105(12)

  //填充10s整数倍
  setfillcolor(RGB(60, 60, 60));
  for (int t = 0; t < trackNum; t = t + 10)
    solidrectangle(trackX + t * timelineWidth, trackY, trackX + (t + 1) * timelineWidth,
      trackY + min(slotNum[curAccount] + 2, trackSlotNumPerPage + 1) * trackHeight);
  setfillcolor(defaultFillColor);

  //计算每个轨道要显示的颜色（对全轨进行计算，否则骑页间隙无法显示）
  int maxTrack[maxSlotNum + 2] = {};
  memset(trackType, 0, sizeof(trackType));
  for (int order = 0; order <= maxSlotNum + 1; order++)
  {
    if (order > slotNum[curAccount] && order <= maxSlotNum)
      continue;
    lastTrack = -1;
    maxTrack[order] = GetMaxTrack(curAccount, order);
    for (int t = minTime; t <= maxTrack[order]; t++)
      if (track[curAccount][order][t] >= 1)//检测到轨道记录
      {
        if (t == maxTrack[order] && slot[curAccount][order].limit == 1)
          trackType[order][t] = 7;//极限成阵的最后一个轨道
        else
          trackType[order][t] = 1;//标记为正常轨道
        if (lastTrack >= zero)  //如果存在上一个轨道值，就要对间隙进行判断
        {
          int gap = t - lastTrack;
          int type = 0;
          if (gap < slot[curAccount][order].cd)
            type = 3;//冷却冲突
          else if (gap == slot[curAccount][order].cd)
            type = 2;//冷却紧凑
          else
            type = 0;//冷却间断
          for (int i = lastTrack + 1; i < t; i++)
            trackType[order][i] = type;
        }
        lastTrack = t;
      }
    if (slot[curAccount][order].limit == 1)//标记极限轨道
    {
      int endTime = slot[curAccount][order].endTime > 0 ? slot[curAccount][order].endTime : maxTime;
      for (int t = max(minTime, maxTrack[order] + 1); t <= endTime; t++)
        trackType[order][t] = 4;
    }
    if (slot[curAccount][order].retain == 1)//标记继承
    {
      for (int t = minTime; t <= maxTime; t++)
        trackType[order][t] = 5;
    }
  }
  if (accountNum == 2)
    for (int order = 0; order <= slotNum[curAccount]; order++)//检查另一个号有没有同名卡
    {
      int anotherOrder = GetOrder(1 - curAccount, slot[curAccount][order].name);
      if (anotherOrder >= 0)//如果另一个账号存在同名卡
        for (int t = 0; t <= maxTime; t++)//某时刻本账号无轨道，另一个账号有轨道
          if (track[curAccount][order][t] == 0 && track[1 - curAccount][anotherOrder][t] >= 1)
            trackType[order][t] = 6;//标记为他号轨道
    }

  //显示本页轨道颜色
  for (int lineOrder = firstTrackSlot; lineOrder <= maxSlotNum; lineOrder++)
  {
    if (lineOrder > min(slotNum[curAccount], firstTrackSlot + trackSlotNumPerPage))
      continue;
    int order = lineOrder == -1 ? maxSlotNum + 1 : lineOrder;
    for (int t = minTime; t < trackNum; t++)
      switch (trackType[order][(page - 1) * 60 + t])
      {
      case 1:BarTrack(order, t, normalTrack); break;
      case 2:BarTrack(order, t, closeGap); break;
      case 3:BarTrack(order, t, errorGap); break;
      case 4:BarTrack(order, t, limitTrack); break;
      case 5:BarTrack(order, t, retainTrack); break;
      case 6:BarTrack(order, t, anotherTrack); break;
      case 7:BarTrack(order, t, indefiniteTrack); break;
      }
  }

  //绘制轨道栅格
  setlinecolor(RGB(80, 80, 80));
  PaintGrid(min(slotNum[curAccount] + 2, trackSlotNumPerPage + 1), trackNum,
    trackX, trackY, trackWidth, trackHeight);//暗色绘制轨道栅格
  setlinecolor(RGB(255, 255, 255));
  PaintGrid(min(slotNum[curAccount] + 2, trackSlotNumPerPage + 1), 1,
    trackX + timelineWidth, trackY, (trackNum - 1) * timelineWidth, trackHeight);//亮色绘制轨道外框线
  PaintGrid(min(slotNum[curAccount] + 2, trackSlotNumPerPage + 1), 1,
    trackX, trackY, timelineWidth, trackHeight);//亮色绘制轨道外框线

  //显示冰激凌作用线
  for (int account = 0; account < accountNum; account++)
  {
    int order = GetOrder(account, "冰激凌");
    if (order > zero)
    {
      int times = 0;
      for (int t = minTime; t <= maxTime; t++)//遍历整个时间轴
        if (track[account][order][t] == 1)//每找到一条轨道
        {
          times++;//IceTimes增加1
          if (location[account][order][times] > zero)//如果这次种植位置也有数据
          {
            int iceTarget = GetIceTarget(account, location[account][order][times] / 10, location[account][order][times] % 10, t);//获得冷却的目标
            if (account != curAccount)
              iceTarget = GetOrder(curAccount, slot[account][iceTarget].name);
            if (iceTarget > zero) //如果有冷却目标，画红绿线标记
            {
              setlinecolor(coldLine[account != curAccount]);//冰激凌轨道后1秒画线提示
              if (t + 1 > (page - 1) * 60 && t + 1 <= page * 60 + 1 && iceTarget >= firstTrackSlot
                && iceTarget <= firstTrackSlot + trackSlotNumPerPage)
                VerticalLine(trackX + (t + 1 - (page - 1) * 60) * trackWidth,
                  trackY + (iceTarget - firstTrackSlot) * trackHeight + 1,
                  trackY + (iceTarget - firstTrackSlot + 1) * trackHeight - 1, 1);
              setlinecolor(coldLine[account != curAccount]);//冰激凌轨道后3秒画线提示
              if (t + 3 > (page - 1) * 60 && t + 3 <= page * 60 + 1 && iceTarget >= firstTrackSlot
                && iceTarget <= firstTrackSlot + trackSlotNumPerPage)
                VerticalLine(trackX + (t + 3 - (page - 1) * 60) * trackWidth,
                  trackY + (iceTarget - firstTrackSlot) * trackHeight + 1,
                  trackY + (iceTarget - firstTrackSlot + 1) * trackHeight - 1, 1);
            }
          }
        }
      setlinecolor(RGB(255, 255, 255));
    }
  }

  //显示冰沙作用线
  for (int account = 0; account < accountNum; account++)
  {
    int order = GetOrder(account, "冰沙");
    if (order > zero)
    {
      for (int t = minTime; t <= trackNum; t++)
      {
        //冰沙轨道后2秒画红线提示
        setlinecolor(coldLine[account != curAccount]);
        if ((page - 1) * 60 + t - 2 > zero)
          if (track[account][order][(page - 1) * 60 + t - 2] == 1)
            VerticalLine(trackX + t * trackWidth, trackY + 1,
              trackY + min(slotNum[curAccount] + 2, trackSlotNumPerPage + 1) * trackHeight - 1, 1);
        //冰沙轨道后4秒画绿线提示
        setlinecolor(coldLine[account != curAccount]);
        if ((page - 1) * 60 + t - 4 > zero)//如果4秒前是非负时刻
          if (track[account][order][(page - 1) * 60 + t - 4] == 1) //且有冰沙轨道
            VerticalLine(trackX + t * trackWidth, trackY + 1,
              trackY + min(slotNum[curAccount] + 2, trackSlotNumPerPage + 1) * trackHeight - 1, 1);
      }
      setlinecolor(RGB(255, 255, 255));
    }
  }
  //显示轨道文字(需要遍历全轨道以获得要显示的次数)
  SetFontSize(18);
  for (int lineOrder = firstTrackSlot; lineOrder <= maxSlotNum; lineOrder++)
  {
    if (lineOrder > min(slotNum[curAccount], firstTrackSlot + trackSlotNumPerPage))
      continue;
    int order = lineOrder == -1 ? maxSlotNum + 1 : lineOrder;
    int floor = GetSlotFloor(order);//宝石（22号槽）显示位置为-1
    if (slot[curAccount][order].retain == 1)
    {
      char retainString[30] = "使用默认轨道";
      if (curTotalWave[curAccount] > 0)
      {
        int totalWaveRetained = 0;//被继承的波次
        //从当前波-1向前查找最近的存在波次
        for (int totalWave = curTotalWave[curAccount] - 1; totalWave >= 0; totalWave--)
          if (totalWaveExist[curAccount][totalWave] == 1)
          {
            totalWaveRetained = totalWave;
            break;
          }
        char waveString[10] = {};
        GetWaveString(waveString, totalWaveRetained);
        sprintf_s(retainString, "继承第%s波轨道", waveString);
      }
      settextcolor(RGB(255, 255, 255));
      outtextxy(trackX + 20, trackY + floor * trackHeight + 12, retainString);
    }
    else
    {
      if (slot[curAccount][order].limit == 1)
      {
        int startTrack = (page - 1) * 60;//本页轨道起始时间
        int conditionLoc = 0;//条件显示位置
        if (maxTrack[order] <= startTrack) //本页全绿
          conditionLoc = minTime;//从第1秒开始显示
        else if (maxTrack[order] <= startTrack + 55) //本页显示得下
          conditionLoc = maxTrack[order] - startTrack + 6;
        if (conditionLoc > 0)
        {
          char upper[50] = {}, lower[50] = {};
          GetConditionString(curAccount, order, upper, lower);
          settextcolor(RGB(255, 255, 0));
          outtextxy(trackX + conditionLoc * trackWidth + 2, trackY + floor * trackHeight + 3, upper);
          settextcolor(RGB(255, 255, 255));
          outtextxy(trackX + conditionLoc * trackWidth + 2, trackY + floor * trackHeight + 23, lower);
        }
      }
      int times = 0;
      for (int t = 0; t <= page * 60; t++)
        if (track[curAccount][order][t] >= 1)//如果有轨道
        {
          times += track[curAccount][order][t];
          if (t > (page - 1) * 60 && t <= page * 60)//只显示当前这一页的文字
          {
            if (t == maxTrack[order] && slot[curAccount][order].limit && slot[curAccount][order].endNum > 0)
            {
              if (track[curAccount][order][t] == 1)
                sprintf_s(timesString, "%d(%d)", times, slot[curAccount][order].endNum);
              else
                sprintf_s(timesString, "%d-%d(%d)", times - track[curAccount][order][t] + 1, times, slot[curAccount][order].endNum);
            }
            else
            {
              if (track[curAccount][order][t] == 1)
                sprintf_s(timesString, "%d", times);
              else
                sprintf_s(timesString, "%d-%d", times - track[curAccount][order][t] + 1, times);
            }

            settextcolor(RGB(255, 255, 0));
            outtextxy(trackX + (t - (page - 1) * 60) * trackWidth + 18, trackY + floor * trackHeight + 3, timesString);

            settextcolor(RGB(255, 255, 255));
            if (t == maxTrack[order] && slot[curAccount][order].limit && slot[curAccount][order].endTime > 0)
            {
              if (slot[curAccount][order].endTime <= t)
                settextcolor(RGB(255, 64, 0));
              if (moment[curAccount][order][times] % 10 == zero || slot[curAccount][order].independent == zero)
                sprintf_s(momentString, "%03d-%03d", t, slot[curAccount][order].endTime);
              else
                sprintf_s(momentString, "%03d.%d-%03d", t, moment[curAccount][order][times] % 10, slot[curAccount][order].endTime);
            }
            else
            {
              if (moment[curAccount][order][times] % 10 == zero || slot[curAccount][order].independent == zero)
                sprintf_s(momentString, "%03d", t);
              else
                sprintf_s(momentString, "%03d.%d", t, moment[curAccount][order][times] % 10);
            }
            outtextxy(trackX + (t - (page - 1) * 60) * trackWidth + 18, trackY + floor * trackHeight + 23, momentString);
          }
        }
    }
  }
  SetFontSize(size);
}
void ViewPage()
{
  int row, column, n;
  PaintGrid(1, 1, pageX, pageY - buttonHeight, buttonWidth, buttonHeight);//翻页框
  PaintGrid(4, 4, pageX, pageY, pageWidth, pageHeight);//翻页框
  settextcolor(RGB(255, 255, 255));
  CenterView("翻页", pageX + buttonWidth / 2, pageY - buttonHeight / 2);

  int size = GetFontSize();
  SetFontSize(16);
  for (row = 0; row < 4; row++)
    for (column = 0; column < 4; column++)
    {
      n = row * 4 + column + 1;
      if (n > 16)
        break;
      if (n == page)
        settextcolor(RGB(255, 0, 0));
      else
        settextcolor(RGB(255, 255, 255));
      CenterView(n, pageX + (2 * column + 1) * pageWidth / 2, pageY + (2 * row + 1) * pageHeight / 2);
    }
  SetFontSize(size);
}
//显示波次区
void ViewWave()
{
  PaintGrid(1, 1, buttonX, waveY - buttonHeight, buttonWidth, buttonHeight);//按钮
  PaintGrid(3, 4, waveX, waveY, waveWidth, waveHeight);//波次框

  settextcolor(RGB(255, 255, 255));
  CenterView("按波变阵", buttonX + buttonWidth / 2, waveY - buttonHeight / 2);

  int size = GetFontSize();
  SetFontSize(16);

  int existedWaveNum = 0;
  int row = 0, column = 0;
  char waveString[10] = {};
  for (int totalWave = 0; totalWave < maxTotalWave; totalWave++)
  {
    if (totalWaveExist[curAccount][totalWave])
    {
      row = existedWaveNum / 4;
      column = existedWaveNum % 4;
      if (totalWave == curTotalWave[curAccount])
        settextcolor(RGB(255, 0, 0));
      else
        settextcolor(RGB(255, 255, 255));
      GetWaveString(waveString, totalWave);
      if (strlen(waveString) >= 5)
        SetFontSize(12);
      else if (strlen(waveString) >= 4)
        SetFontSize(14);
      else
        SetFontSize(16);
      CenterView(waveString, waveX + (2 * column + 1) * waveWidth / 2, waveY + (2 * row + 1) * waveHeight / 2);
      existedWaveNum++;
    }
  }
  //如果存在的波次不满12个，显示"+"
  if (existedWaveNum < 12)
  {
    row = existedWaveNum / 4;
    column = existedWaveNum % 4;
    settextcolor(RGB(255, 255, 255));
    CenterView("+", waveX + (2 * column + 1) * waveWidth / 2, waveY + (2 * row + 1) * waveHeight / 2);
  }
  SetFontSize(size);
}
//【阵型编辑】显示波次和魔塔、地图按钮
void ViewMapButtons()
{
  ViewWave();
  PaintGrid(1, 1, buttonX, buttonY, buttonWidth, buttonHeight);//按钮
  settextcolor(RGB(255, 255, 255));
  CenterView("地图", buttonX + buttonWidth / 2, buttonY + buttonHeight / 2);
  PaintTargetCursor(buttonX + 12, buttonY + buttonHeight / 2);
  if (isMapImageCatched == 1)
    CenterView("×", buttonX + buttonWidth - 12, buttonY + buttonHeight / 2);
}
void ViewMove()
{
  int size;
  LOGFONT Font;
  PaintGrid(1, 3, trackSlotX, moveY, moveWidth, moveHeight);//移动轨道按钮
  PaintGrid(2, 1, trackSlotX + moveWidth, moveY, moveWidth, moveHeight / 2);//移动轨道按钮
  settextcolor(RGB(255, 255, 255));
  size = GetFontSize();
  SetFontSize(16);
  gettextstyle(&Font);
  CenterView("▲", moveX + moveWidth * 3 / 2, moveY + moveHeight / 4 + 1);
  CenterView("▼", moveX + moveWidth * 3 / 2, moveY + moveHeight * 3 / 4 + 4);
  Font.lfOrientation = 900;
  settextstyle(&Font);
  CenterView("▲", moveX + moveWidth / 2 - 13, moveY + moveHeight / 2 + 8);
  CenterView("▼", moveX + moveWidth * 5 / 2 - 11, moveY + moveHeight / 2 + 8);
  Font.lfOrientation = 0;
  settextstyle(&Font);
  SetFontSize(size);
}
//显示顶部信息（1P/2P轨道路径和火苗）
void ViewTopInfo()
{
  char message[1000];
  settextcolor(RGB(255, 255, 255));
  if (accountNum == 1) //单轨模式显示“添加2P轨道”和“刷技能”
  {
    outtextxy(playerX + playerDistance, 5, "[添加2P轨道]");
    outtextxy(playerX + 2 * playerDistance, 5, "刷技能");
    PaintCheckBox(skillX, skillY, skillWidth, skillHeight, 0);
    if (isSkillMode)
      PaintCheck(skillX, skillY, skillWidth, skillHeight, 0);
  }
  else
    CenterView("[移除]", playerX + playerDistance * 2 + playerOptionWidth / 2, 15);

  if (mode == 1)
  {
    outtextxy(roleSlotX, 5, "轨道路径：");
    for (int account = 0; account < accountNum; account++)
    {
      if (account == curAccount)
        settextcolor(RGB(255, 0, 0));
      else
        settextcolor(RGB(255, 255, 255));
      sprintf_s(message, "[%dP]", account + 1);
      outtextxy(playerX + playerDistance * account, 5, message);
      settextcolor(RGB(255, 255, 255));
      GetShortPath(fullTrackPath[account], shortTrackPath[account]);//获取相对路径
      Truncate(message, shortTrackPath[account], playerDistance - playerTitleWidth - 20);
      outtextxy(playerX + playerDistance * account + playerTitleWidth, 5, message);
    }
  }
  else if (mode == 2)
  {
    sprintf_s(message, "第%03d秒：", timeSelected);
    outtextxy(roleSlotX, 5, message);
    for (int account = 0; account < accountNum; account++)
    {
      CountEnergyLimit();//计算能量值
      if (account == curAccount)
        settextcolor(RGB(255, 0, 0));
      else
        settextcolor(RGB(255, 255, 255));
      sprintf_s(message, "[%dP]", account + 1);
      outtextxy(playerX + playerDistance * account, 5, message);
      sprintf_s(message, "%d (生产%d 消耗%d)",
        energy[account][timeSelected], totalCollect[account][timeSelected], totalCost[account][timeSelected]);
      int MessageWidth = textwidth(message);//字符串宽度
      settextcolor(RGB(255, 255, 255));
      outtextxy(playerX + playerDistance * account + playerTitleWidth, 5, message);
      for (int t = 0; t <= maxTime; t++)
        if (energy[account][t] < zero)
        {
          settextcolor(RGB(255, 128, 0));
          sprintf_s(message, " 第%03d秒缺火", t);
          outtextxy(playerX + playerDistance * account + playerTitleWidth + MessageWidth, 5, message);
          break;
        }
    }
  }
}
//显示底部信息
void ViewBottomInfo()
{
  settextcolor(WHITE);
#ifdef _DEBUG
  char message[1000];
  settextcolor(RGB(255, 255, 255));
  if (mode == 1 || mode == 2)
  {
    outtextxy(850, wndHeight - tipHeight, tracePrint);
    sprintf_s(message, "状态%d", state);//显示当前状态（调试用）
    outtextxy(950, wndHeight - tipHeight, message);
    sprintf_s(message, "区域%d", area);//显示当前点击区域（调试用）
    outtextxy(1050, wndHeight - tipHeight, message);
    sprintf_s(message, "备份%d/%d", currentBackup, backupNum);
    outtextxy(1150, wndHeight - tipHeight, message);
    if (state / 1000 == 1)
    {
      sprintf_s(message, "深度%d", map[curAccount][state % 100 / 10][state % 10].depthSelected[state % 1000 / 100]);
      outtextxy(1250, wndHeight - tipHeight, message);
    }
  }
  else if (mode == 3)
  {
    sprintf_s(message, "区域%d", area);
    outtextxy(180, 5, message);//显示当前点击区域（调试用）
    outtextxy(0, recordWndHeight - 30, tracePrint);
  }
#endif
}
//显示上一次提示信息
void ViewLastTip()
{
  settextcolor(WHITE);
  settextcolor(strstr(lastTip, "注释：") == lastTip ? WHITE : RGB(255, 255, 0));
  outtextxy(roleSlotX, wndHeight - tipHeight, lastTip);
  ViewBottomInfo();
}
//显示提示语句
void ViewTip(const char *tip)
{
  if (strcmp(lastTip, tip) == 0)
    return;

  strcpy_s(lastTip, tip);
  BeginBatchDraw();
  solidrectangle(0, wndHeight - tipHeight, wndWidth, wndHeight);
  ViewLastTip();
  EndBatchDraw();
}
//获取提示信息内容
int GetTip(int mode, int area, char(&tip)[200])
{
  if (area == 1)
    return strcpy_s(tip, "阵型编辑：用于设置放卡位置。右击此处清空阵型。");
  if (area == 2)
    return strcpy_s(tip, "轨道编辑：用于设置放卡时间。右击此处清空轨道。");
  for (int account = 0; account < 2; account++)
    if (area == 20 + account)
      return sprintf_s(tip, "单击切换到%dP轨道。", account + 1);
  if (area == 40)
    return strcpy_s(tip, "单击添加2P轨道。");
  if (area == 50)
    return strcpy_s(tip, "单击移除2P轨道。");
  if (area >= 600 && area <= 611)
    return strcpy_s(tip, "按波变阵：在关卡到达指定波次后变换阵型，详见使用手册4.3。");
  if (area == 90)
    return strcpy_s(tip, "撤销：返回上一步操作。");
  if (area == 91)
    return strcpy_s(tip, "恢复：恢复被撤销的操作。");

  if (mode == 1)
  {
    /*
    if (area == 10)
      return strcpy_s(tip, "防御卡背包：设置卡片生产（需要计算火苗时填写）、冷却、层级等信息。");
    if (area == 11)
      return strcpy_s(tip, "生产信息：设置武器和环境产火（需要计算火苗时填写）。");
      */
    for (int account = 0; account < 2; account++)
      if (area == 30 + account)
        return sprintf_s(tip, "%dP轨道路径（即保存位置）。单击可复制。", account + 1);
    if (area == 70)
      return sprintf_s(tip, "刷技能：勾选后，无需携带卡组，按住Ctrl点击放卡位置即可完成刷技能轨道制作。");
    if (area / 100 == 1)
      return strcpy_s(tip, "卡槽：单击选中，右击删除。选中后右击其他卡槽交换位置，右击本卡清空阵型。");
    if (area / 100 == 4)
      return strcpy_s(tip, "补阵：每次放卡优先放在阵型中第一个空缺位置。需设置轨道才能生效。");
    if (area / 100 == 5)
      return strcpy_s(tip, "索敌：每次放卡优先放在出怪的行数。需开启极限成阵才能生效。");
    if (area >= 200 && area <= 225)
      return strcpy_s(tip, "防御卡背包：单击添加到卡槽，右击编辑或删除卡片。");
    if (area == 300)
      return strcpy_s(tip, "1P人物：选中后可以往地图中添加多个人物位置，右击清空1P阵型和轨道。");
    if (area == 310)
    {
      if (roleLocNum[1] == 0)
        return strcpy_s(tip, "单击添加2P。");
      else if (accountNum == 2)
        return strcpy_s(tip, "2P人物：选中后可以往地图中添加多个人物位置，右击清空2P阵型和轨道。");
      else
        return strcpy_s(tip, "2P人物：选中后可以往地图中添加多个人物位置，右击删除2P。");
    }
    if (area == 615)
      return strcpy_s(tip, "将靶形光标拖至游戏窗口内可截取地图，方便观察地形。");
    if (area >= 800 && area <= 808)
      return strcpy_s(tip, "堆叠区：选中地图中的卡片后单击堆叠区空位，即可在这一格叠加卡片。");
    if (area == 809)
      return strcpy_s(tip, "切换到堆叠区最低层卡片。");
    if (area == 810)
      return strcpy_s(tip, "切换到堆叠区最高层卡片。");
    if (area == 811)
      return strcpy_s(tip, "切换到堆叠区空位（用于添加卡片）。");

    //if (area / 1000 == 1)
     // return strcpy_s(tip, "地图：单击卡片选中（选中后单击其他格子可进行移动），右击卡片删除。");
    return sprintf_s(tip, "注释：%s", notation[curAccount]);
  }
  else if (mode == 2)
  {
    for (int account = 0; account < 2; account++)
      if (area == 30 + account)
        return sprintf_s(tip, "%dP火苗计算值。", account + 1);
    if (area / 100 == 1)
      return strcpy_s(tip, "卡槽：选中后可以用左上角的箭头调节CD或移动轨道。双击可自动布轨。右击清空轨道。");
    if (area / 100 == 2)
      return strcpy_s(tip, "编号：单击开启轨道继承，详见使用手册4.3。");
    if (area / 100 == 3)
      return strcpy_s(tip, "独立计时：该卡片使用精确计时，适用于补云洞、放海盐等操作。");
    if (area / 100 == 4)
      return strcpy_s(tip, "绝对优先：该卡片的放置无视放卡优先队列的限制。");
    if (area / 100 == 5)
      return strcpy_s(tip, "极限成阵：卡槽亮起时立即放置。按住Ctrl后单击可设置放卡条件。");
    if (area == 612)
      return strcpy_s(tip, "录制轨道：将手动放卡操作录制成轨道文件。");
    if (area == 613)
      return strcpy_s(tip, "自动布轨：根据火苗计算结果自动安排放卡时间。需填写卡片数据和生产信息才可使用。");
    if (area == 614)
      return strcpy_s(tip, "魔塔：进入指定魔塔层数执行轨道。可用于刷技能。");
    if (area == 615)
      return strcpy_s(tip, "定时退出：到达指定时间后主动退出关卡。用于跨服刷威望等情形。");
    if (area / 100 == 8)
      return strcpy_s(tip, "翻页：点击数字快速翻页。一页为一分钟。");
    if (area == 900 || area == 901)
      return strcpy_s(tip, "左右箭头：1. 点击进行翻页。2. 选中卡槽后点击可整体平移轨道。");
    if (area == 902 || area == 903)
      return strcpy_s(tip, "上下箭头：1. 用于卡槽数≥14时滚动显示。2. 选中卡槽后点击可调节CD。");
    if (area / 1000 == 2)
      return strcpy_s(tip, "时间轴：点击刻度查看对应时间的火苗计算值。");
    return strcpy_s(tip, "");
  }
  return strcpy_s(tip, "");
}
//显示提示信息
void RepaintTip()
{
  //根据当前鼠标位置决定提示信息
  POINT point;
  GetCursorPos(&point);
  ScreenToClient(hWndEditor, &point);
  char tip[200];
  GetTip(mode, GetArea(point.x, point.y), tip);
  ViewTip(tip);
}
//显示轨道功能按钮
void ViewTrackButtons()
{
  ViewWave();
  PaintGrid(4, 1, buttonX, buttonY, buttonWidth, buttonHeight);//自动布轨按钮
  settextcolor(RGB(255, 255, 255));
  CenterView("录制轨道", buttonX + buttonWidth / 2, buttonY + buttonHeight / 2);
  CenterView("自动布轨", buttonX + buttonWidth / 2, buttonY + 3 * buttonHeight / 2);

  char TowerString[20];
  if (tower[curAccount] == zero)
    sprintf_s(TowerString, "魔塔：无");
  else
    sprintf_s(TowerString, "魔塔：%d", tower[curAccount]);
  CenterView(TowerString, buttonX + buttonWidth / 2, buttonY + 5 * buttonHeight / 2);

  char QuitString[20];
  if (quitTime[curAccount] == zero)
    sprintf_s(QuitString, "退出：无");
  else
    sprintf_s(QuitString, "退出：%ds", quitTime[curAccount]);
  CenterView(QuitString, buttonX + buttonWidth / 2, buttonY + 7 * buttonHeight / 2);
}
void ViewUndo()
{
  int i;
  settextcolor(RGB(255, 255, 255));
  const char *const UndoOption[2] = { "撤销", "恢复" };
  for (i = 0; i <= 1; i++)
    CenterView(UndoOption[i], undoX + undoWidth / 2, undoY + (2 * i + 1) * undoHeight / 2);
}
//显示生产信息编辑界面
void ViewEditProduction()
{
  const char *const option[12] = {
    "初始火苗", "开局小火数", "开局酒杯数", "是否白天",
    "回火", "产能", "生产", "光能", "神佑", "疾风", "转化", "榴弹类型" };
  int OptionValue[12] = { initialEnergy, InnateFire, InnateCup };
  char OptionString[12][20];
  int i;
  int size;
  for (i = 0; i <= 2; i++)
    sprintf_s(OptionString[i], "%d", OptionValue[i]);
  if (isDay == 1) sprintf_s(OptionString[3], "是");
  else sprintf_s(OptionString[3], "否");
  for (i = 0; i <= 6; i++)
  {
    if (weapon[curAccount][i] == -1) sprintf_s(OptionString[i + 4], "无");
    else sprintf_s(OptionString[i + 4], "Lv.%d", weapon[curAccount][i]);
  }
  if (weapon[curAccount][7] == 1) sprintf_s(OptionString[11], "普通");
  else sprintf_s(OptionString[11], "加强");
  size = GetFontSize();
  for (i = 0; i <= 11; i++)
  {
    if (i == zero) settextcolor(RGB(255, 255, 255));
    if (i == 4) settextcolor(productColor);
    CenterView(option[i], productionX + productionWidth / 2, productionY + (2 * i + 1) * productionHeight / 2);
    CenterView(OptionString[i], productionX + 3 * productionWidth / 2, productionY + (2 * i + 1) * productionHeight / 2);
    SetFontSize(16);
    CenterView("▲", productionX + 2 * productionWidth - 10, productionY + (2 * i + 1) * productionHeight / 2 - 7);
    CenterView("▼", productionX + 2 * productionWidth - 10, productionY + (2 * i + 1) * productionHeight / 2 + 10);
    SetFontSize(size);
  }
}
//显示防御卡编辑界面
void ViewEditBag()
{
  char option[9][13] = { "名称", "冷却", "层级", "耗能", "生产特性", "星级", "单朵产量", "火苗朵数", "" };
  int OptionValue[8] = { 0, bag[curAccount][bagMode].cd, 0, bag[curAccount][bagMode].cost, 0, bag[curAccount][bagMode].star, bag[curAccount][bagMode].sun, bag[curAccount][bagMode].sunNum };
  char OptionString[9][20] = {};
  if (strcmp(bag[curAccount][bagMode].name, "炎焱兔") == zero)
  {
    strcpy_s(option[6], "转职");
    strcpy_s(option[7], "增幅倍率");
  }
  if (bag[curAccount][bagMode].style == 5)
  {
    strcpy_s(option[6], "转职");
    strcpy_s(option[7], "技能");
    strcpy_s(option[8], "产量");
    if (bag[curAccount][bagMode].transfer == 1)
      sprintf_s(OptionString[8], "150+%d", fireworkYield[bag[curAccount][bagMode].star]);
    else if (bag[curAccount][bagMode].transfer == 2)
      sprintf_s(OptionString[8], "150+%d", 2 * fireworkYield[bag[curAccount][bagMode].star]);
    else
      sprintf_s(OptionString[8], "%d", fireworkYield[bag[curAccount][bagMode].star]);
  }

  sprintf_s(OptionString[0], "%s", bag[curAccount][bagMode].name);//名称
  sprintf_s(OptionString[1], "%ds", bag[curAccount][bagMode].cd);//冷却
  for (int i = 0; i <= 6; i++)
  {
    if (bag[curAccount][bagMode].level == i)
    {
      sprintf_s(OptionString[2], "%s", levelName[i]);//层级
      break;
    }
  }
  if (bag[curAccount][bagMode].cost >= 1000)
    sprintf_s(OptionString[3], "%d+", bag[curAccount][bagMode].cost - 1000);//冷却
  else
    sprintf_s(OptionString[3], "%d", bag[curAccount][bagMode].cost);//冷却
  for (int i = 0; i <= 8; i++)
  {
    if (bag[curAccount][bagMode].style == i)
    {
      sprintf_s(OptionString[4], "%s", styleName[i]);//生产特性
      break;
    }
  }
  for (int i = 5; i <= 7; i++)
    sprintf_s(OptionString[i], "%d", OptionValue[i]);

  if (bag[curAccount][bagMode].style == 5 || bag[curAccount][bagMode].style == 6) //花火龙/炎焱兔
  {
    const char TransferName[3][7] = { "不转", "一转", "二转" };
    strcpy_s(OptionString[6], TransferName[bag[curAccount][bagMode].transfer]);//转职名称
    if (bag[curAccount][bagMode].style == 6) //炎焱兔：显示增幅倍率
      if (bag[curAccount][bagMode].transfer >= 1)
        sprintf_s(OptionString[7], "%d.%02d", rabbitFactor[1][bag[curAccount][bagMode].star] / 100, rabbitFactor[1][bag[curAccount][bagMode].star] % 100);
      else
        sprintf_s(OptionString[7], "%d.%02d", rabbitFactor[0][bag[curAccount][bagMode].star] / 100, rabbitFactor[0][bag[curAccount][bagMode].star] % 100);
  }
  int size = GetFontSize();
  for (int i = 0; i < 9; i++)//9行参数
  {
    if (i == zero)
      settextcolor(levelColor[bag[curAccount][bagMode].level]);
    if (i == 4)
      settextcolor(productColor);
    if (i == 5)
      if (bag[curAccount][bagMode].style == zero || bag[curAccount][bagMode].style >= 7) //非生产、初始火、初始灯：没有后续选项
        break;
    CenterView(option[i], productionX + productionWidth / 2, productionY + (2 * i + 1) * productionHeight / 2);
    CenterView(OptionString[i], productionX + 3 * productionWidth / 2, productionY + (2 * i + 1) * productionHeight / 2);
    SetFontSize(16);
    //绘制右侧调节箭头。炎焱兔第7行、花火龙/炎焱兔第4行不用绘制
    if (i >= 1 && i <= 7)
    {
      bool isRightArrowRequired = true;
      if (bag[curAccount][bagMode].style == 6 && i == 7)
        isRightArrowRequired = false;
      if (bag[curAccount][bagMode].style >= 6 && i == 4)
        isRightArrowRequired = false;
      if (bag[curAccount][bagMode].level == 6 && i == 2)
        isRightArrowRequired = false;

      if (isRightArrowRequired)
      {
        CenterView("▲", productionX + 2 * productionWidth - 10, productionY + (2 * i + 1) * productionHeight / 2 - 7);
        CenterView("▼", productionX + 2 * productionWidth - 10, productionY + (2 * i + 1) * productionHeight / 2 + 10);
      }
    }
    //绘制左侧调节箭头。花火龙/炎焱兔的第6行不用绘制
    if (i == 1 || i == 3 || (i == 6 && bag[curAccount][bagMode].style <= 4))
    {
      CenterView("▲", productionX + productionWidth + 10, productionY + (2 * i + 1) * productionHeight / 2 - 7);
      CenterView("▼", productionX + productionWidth + 10, productionY + (2 * i + 1) * productionHeight / 2 + 10);
    }
    SetFontSize(size);
  }
}
void ViewStack()//显示堆栈内容（选中层级各个深度的卡片）
{
  line(stackX + stackWidth / 3, stackY + 9 * stackHeight, stackX + stackWidth / 3, stackY + 10 * stackHeight);
  line(stackX + stackWidth - stackWidth / 3, stackY + 9 * stackHeight, stackX + stackWidth - stackWidth / 3, stackY + 10 * stackHeight);
  PaintGrid(10, 1, stackX, stackY, stackWidth, stackHeight);//堆栈框
  int ButtonX[3] = { stackX + stackWidth / 6, stackX + stackWidth / 2, stackX + stackWidth - stackWidth / 6 };
  char *Button[3] = { "底", "顶", "空" };
  settextcolor(RGB(255, 255, 255));
  for (int i = 0; i <= 2; i++)
    CenterView(Button[i], ButtonX[i], stackY + 19 * stackHeight / 2);//显示名称
  if (state / 1000 != 1)
    return;
  int row = state % 100 / 10, column = state % 10, level = state % 1000 / 100;
  int depth;
  int size;
  int t, order;
  for (depth = 0; depth < maxViewStack; depth++)
  {
    if (depth == map[curAccount][row][column].depthSelected[level])//空层级显示方框
    {
      settextcolor(RGB(255, 0, 0));
      setlinecolor(RGB(255, 0, 0));
      PaintGrid(1, 1, stackX + 6, stackY + depth * stackHeight + 4, stackWidth - 12, stackHeight - 8);
      setlinecolor(RGB(255, 255, 255));
    }
    else
      settextcolor(levelColor[level]);
    if (depth < map[curAccount][row][column].depth[level]) //对有卡片的深度才显示名称和次序
    {
      if (level == 6)
      {
        order = GetOrder(curAccount, "幻幻鸡");
        t = GetTime(curAccount, order, map[curAccount][row][column].times[level][depth]);//得到幻鸡这次种植的时刻t。如果没有轨道，返回MaxTime+1
        if (t == zero)//如果本次幻幻鸡没有轨道
          CenterView("幻幻鸡", stackX + 40, stackY + (2 * depth + 1) * stackHeight / 2);//显示名称
        else//如果找到了幻幻鸡轨道
        {
          order = GetMagicTarget(curAccount, t);//寻找幻幻鸡复制的卡片编号
          if (order == zero)//如果没有复制卡
            CenterView("幻幻鸡", stackX + 40, stackY + (2 * depth + 1) * stackHeight / 2);//显示名称
          else
            CenterView(slot[curAccount][order].name, stackX + 40, stackY + (2 * depth + 1) * stackHeight / 2);//显示名称
        }
      }
      else
        CenterView(map[curAccount][row][column].name[level][depth], stackX + 40, stackY + (2 * depth + 1) * stackHeight / 2);//显示名称
      size = GetFontSize();
      SetFontSize(18);
      CenterView(map[curAccount][row][column].times[level][depth], stackX + 79, stackY + (2 * depth + 1) * stackHeight / 2);//显示种植次序
      if (map[curAccount][row][column].depth[level] >= 2)//至少有两张卡，才显示下标
      {
        SetFontSize(12);
        CenterView(depth + 1, stackX + 93, stackY + (2 * depth + 1) * stackHeight / 2 + 4);
      }
      SetFontSize(size);
    }
  }
}
//显示防御卡背包下方选项
void ViewBagOption()
{
  const char *const BagOption[2] = { "防御卡背包", "生产信息" };
  const char *const EditOption[2] = { "返回背包", "删除卡片" };
  PaintGrid(1, 2, bagOptionX, bagOptionY, bagOptionWidth, bagOptionHeight);//背包选项卡
  for (int i = 0; i <= 1; i++)
  {
    if (bagMode == -(i + 1)) settextcolor(RGB(255, 0, 0));
    else settextcolor(RGB(255, 255, 255));
    if (bagMode >= zero)
      CenterView(EditOption[i], bagOptionX + (2 * i + 1) * bagOptionWidth / 2, bagOptionY + bagOptionHeight / 2);
    else
      CenterView(BagOption[i], bagOptionX + (2 * i + 1) * bagOptionWidth / 2, bagOptionY + bagOptionHeight / 2);
  }
}
//重绘阵型编辑界面
void RepaintMap()
{
  BeginBatchDraw();
  solidrectangle(0, 0, wndWidth, wndHeight);

  if (isMapImageCatched)
    putimage(mapGridX, mapGridY, &mapImage);

  ViewRoleSlot();//移动人物按钮内容
  ViewMapSlotCard();//卡槽内容
  ViewStack();//显示堆栈内容
  PaintGrid(7, 9, mapGridX, mapGridY, mapGridWidth, mapGridHeight);//地图框
  ViewGridCard();//地图内容
  ViewBagOption();//显示背包选项卡
  if (bagMode == -1)
  {
    PaintGrid(14, 2, bagX, bagY, bagWidth, bagHeight);//常用卡组框
    ViewCommonCard();//常用卡组内容
  }
  else if (bagMode == -2)
  {
    PaintGrid(14, 2, productionX, productionY, productionWidth, productionHeight);//生产信息框
    ViewEditProduction();//生产信息
  }
  else
  {
    PaintGrid(14, 2, bagX, bagY, bagWidth, bagHeight);//防御卡编辑框
    ViewEditBag();//编辑内容
  }
  ViewMode();//选项卡内容
  ViewMapButtons();
  PaintGrid(2, 1, undoX, undoY, undoWidth, undoHeight);//撤销按钮
  ViewUndo();
  ViewTopInfo();
  ViewLastTip();
  EndBatchDraw();
  /*备份图像，减少重绘占用*/
  setaspectratio(1, 1);
  getimage(&imageBackup, 0, 0, (int)(wndWidth * ratio), (int)(wndHeight * ratio));
  setaspectratio(ratio, ratio);
}
void ViewTimeline()//绘制时间轴
{
  line(timelineX, timelineY, timelineX + (trackNum - 1) * timelineWidth, timelineY);//画轴线
  settextcolor(RGB(255, 255, 255));
  int t = 0;
  for (int x = timelineX; x <= timelineX + (trackNum - 1) * timelineWidth; x += timelineWidth)
  {
    line(x, timelineY - 10, x, timelineY);//画小刻度
    if (t % bigScale == zero)
    {
      VerticalLine(x, timelineY - 20, timelineY - 1, 1);//画大刻度，粗细为3
      CenterView(t + (page - 1) * 60, x, timelineY + 20);//标数字
    }
    t++;
  }
}
//重画轨道界面
void RepaintTrack()
{
  BeginBatchDraw();
  solidrectangle(0, 0, wndWidth, wndHeight);
  ViewTimeline();//绘制时间轴
  ViewQueue();//绘制优先队列
  ViewTrack();//轨道内容
  solidrectangle(buttonX, trackY, wndWidth, trackY + (trackSlotNumPerPage + 1) * trackHeight);//擦除出头的部分
  ViewTrackSlotCard();//卡槽内容
  PaintGrid(2, 1, modeX, modeY, modeWidth, modeHeight);//选项卡框
  ViewMode();//选项卡内容
  ViewTrackButtons();
  ViewPage();
  PaintGrid(2, 1, undoX, undoY, undoWidth, undoHeight);//撤销按钮
  ViewUndo();
  ViewMove();//移动轨道按钮
  ViewTopInfo();//显示火苗产量
  ViewLastTip();
  EndBatchDraw();
  /*备份图像，减少重绘占用*/
  setaspectratio(1, 1);
  getimage(&imageBackup, 0, 0, (int)(wndWidth * ratio), (int)(wndHeight * ratio));
  setaspectratio(ratio, ratio);
}

//对小火炉、酒杯灯进行调控的条件自动布轨
bool isConstraintsAchieved;//约束条件是否已满足
int endSearchTime;//搜索结束时间
int globalCollect[maxTime + 1];//两个号的总收火
struct OptProducerType
{
  bool isEnabled;//是否启用
  int order;//卡槽编号
  int cd;//冷却
  int cost;//耗能
  int period;//生产周期
  int yield;//产量（初始化时已经根据人物数量折算）
  int time[100];//100次放卡时间
  int timeNum;//放卡次数
  int bestTime[100];//最优解-放卡时间
  int bestTimeNum;//最优解-放卡次数
};
OptProducerType optProducer[2][2];//两个账号的小火和酒杯
//能促进第currentTime秒放卡的最大生产卡放置时间
int GetMaxControllableTime(int currentTime)
{
  return currentTime - 4 - (currentTime - 1) % 3;
}
//增加约束条件
void AddConstraint(int order, int targetTime)
{
  constraint[constraintNum].order = order;
  constraint[constraintNum].cost
    = slot[0][order].cost >= 1000 ? slot[0][order].cost - 1000 : slot[0][order].cost;
  constraint[constraintNum].maxTime = targetTime;
  constraint[constraintNum].maxControllableTime = GetMaxControllableTime(targetTime);
  constraintNum++;
}
//比较两个约束条件的目标时间
int CompareConstraint(const void *a, const void *b)
{
  ConstraintType &constraint1 = *(ConstraintType *)a;
  ConstraintType &constraint2 = *(ConstraintType *)b;
  return constraint1.maxTime - constraint2.maxTime;
}
int checkTimes;//检查次数
//清空数组内容，填写初始能量、目标卡耗能和生产卡数据
void InitConstrainedAutoTrack()
{
  checkTimes = 0;
  CountEnergyNormal();//先计算一遍能量
  memset(globalCollect, 0, sizeof(globalCollect));
  isConstraintsAchieved = false;//预设条件未达成
  endSearchTime = GetMaxControllableTime(maxProgrammingTime);//预设搜索结束时刻
  //填写生产卡信息
  memset(optProducer, 0, sizeof(optProducer));//默认不可用
  char *const producerName[2] = { "小火炉", "酒杯灯" };
  for (int account = 0; account < accountNum; account++)
    for (int order = 0; order < 2; order++)
    {
      OptProducerType &opt = optProducer[account][order];
      int slotOrder = GetOrder(account, producerName[order]);
      if (slotOrder > 0) //如果存在这个生产卡，进行记录
      {
        SlotType &slot0 = slot[account][slotOrder];
        opt.isEnabled = true;
        opt.order = slotOrder;
        opt.cd = slot0.cd;
        opt.cost = slot0.cost;
        opt.period = 25 - slot0.star;
        opt.yield = slot0.sunNum * (GetRoleNum() == 2 ? slot0.sun * 3 / 5 : slot0.sun);
      }
    }
  //记录约束条件
  constraintNum = 0;
  for (int i = 0; i < maxConstraintNum; i++)
  {
    int order = GetOrder(0, autoTable.card[i]);
    if (order > 0 && autoTable.maxTime[i] > 0)
      AddConstraint(order, autoTable.maxTime[i]);
  }
  //约束条件按目标时间从小到大排序
  qsort(constraint, constraintNum, sizeof(ConstraintType), CompareConstraint);
  for (int i = 1; i < constraintNum; i++)
    constraint[i].cost += constraint[i - 1].cost;

  //记录优化条件
  memset(&minCard, 0, sizeof(minCard));
  int order = GetOrder(0, autoTable.minCard);
  if (order > 0)
  {
    minCard.order = order;
    minCard.cost = slot[0][order].cost;
    minCard.minAchievingTime = maxProgrammingTime + 1;
  }

  //如果没有优化条件，只有约束条件，最大搜索时间为最后一个约束条件的可控时间
  if (minCard.order == 0)
  {
    //能影响约束条件达成的最大放卡时间
    int constraintControllableTime = constraintNum == 0 ? 0 :
      GetMaxControllableTime(constraint[constraintNum - 1].maxTime);
    if (constraintControllableTime < endSearchTime)
      endSearchTime = constraintControllableTime;
  }
}
//optProducer[account][order]是否已冷却好
bool IsColdOpt(int account, int order, int currentTime)
{
  OptProducerType &opt = optProducer[account][order];
  if (opt.timeNum == 0)
    return true;
  return currentTime - opt.time[opt.timeNum - 1] >= opt.cd;
}
//添加生产卡，填写产火轴和收火轴
void AddOptProducer(int currentTime, int account, int order)
{
  OptProducerType &opt = optProducer[account][order];
  opt.time[opt.timeNum++] = currentTime;
  energy[account][currentTime] -= opt.cost;
  //填写产火轴和收火轴
  int startTime = currentTime >= opt.period - 1 ? currentTime + 2 : currentTime + 2 + opt.period;
  for (int t = startTime; t <= endSearchTime + 6; t += opt.period)
  {
    int collectTime = t - t % 3 + 4;//收集时间
    if (collectTime <= endSearchTime + 6)
      globalCollect[collectTime] += opt.yield;
  }
}
//删除生产卡，填写产火轴和收火轴
void DeleteOptProducer(int currentTime, int account, int order)
{
  OptProducerType &opt = optProducer[account][order];
  opt.timeNum--;
  energy[account][currentTime] += opt.cost;//消耗的火苗加回来
  //删除产火轴和收火轴
  int startTime = currentTime >= opt.period - 1 ? currentTime + 2 : currentTime + 2 + opt.period;
  for (int t = startTime; t <= endSearchTime + 6; t += opt.period)
  {
    int collectTime = t - t % 3 + 4;//收集时间
    if (collectTime <= endSearchTime + 6)
      globalCollect[collectTime] -= opt.yield;
  }
}
//记录生产卡的最佳放置方式
void RecordBestPlant()
{
  //记录答案：生产卡的每次放置时间
  for (int account = 0; account < 2; account++)
    for (int order = 0; order < 2; order++)
    {
      OptProducerType &opt = optProducer[account][order];
      opt.bestTimeNum = opt.timeNum;
      for (int i = 0; i < opt.timeNum; i++)
        opt.bestTime[i] = opt.time[i];
    }
}
//检查限制条件是否达成
bool CheckConstraint(int currentTime)
{
  bool isConstraintMet = true;//限制条件是否满足
  bool isContinuable = true;//是否需要继续搜索
  //如果currentTime+1秒不管怎么排，都没有可能获得更好的解，则停止

  checkTimes++;

  for (int t = currentTime + 1; t <= endSearchTime + 6; t++)
    energy[0][t] = energy[0][t - 1] + globalCollect[t];//计算1P能量

  //不考虑优化目标，检查限制条件是否满足，不满足则退出函数
  //1. 如果第i个限制条件不能满足，判定条件未达成
  //2. 在此基础上，如果currentTime+1超过了第i个限制条件的最大可控时间，则剪枝
  for (int i = 0; i < constraintNum; i++)
    if (energy[0][constraint[i].maxTime] < constraint[i].cost)
    {
      isConstraintMet = false;
      if (currentTime >= constraint[i].maxControllableTime)
        isContinuable = false;
    }

  //如果没有优化目标
  if (minCard.order == 0)
  {
    //如果限制条件不满足了，退出函数
    if (!isConstraintMet)
      return isContinuable;
    //如果满足，记录答案
    isConstraintsAchieved = true;//问题已解决
    RecordBestPlant();//记录生产卡最佳放置方法
    return false;
  }

  //如果有优化目标，条件满足则尝试更新达成时间
  if (isConstraintMet)
  {
    int code = -1;//不能额外承担优化目标耗能的约束条件编号
    //如果第code个约束条件的时间不能额外满足优化目标的耗能，则只能在该时间节点之后找目标达成时间
    for (int i = constraintNum - 1; i >= 0; i--)
      if (energy[0][constraint[i].maxTime] < constraint[i].cost + minCard.cost)
      {
        code = i;
        break;
      }
    int startTime = code == -1 ? 2 : constraint[code].maxTime + 1;//起始搜索时间
    int constaintCost = code == -1 ? 0 : constraint[code].cost;//限制条件卡的耗能
    //搜索目标达成时间
    for (int t = startTime; t <= endSearchTime + 6; t++)
      if (energy[0][t] >= constaintCost + minCard.cost)
      {
        //如果刷新了最小达成时间，进行记录
        if (t < minCard.minAchievingTime)
        {
          minCard.minAchievingTime = t; //记录最小达成时间
          RecordBestPlant();//记录生产卡最佳放置方法
          int maxConstraintTime = constraintNum == 0 ? 0 : constraint[constraintNum - 1].maxTime;
          //更新终止搜索时间
          endSearchTime = GetMaxControllableTime(max(maxConstraintTime, minCard.minAchievingTime - 1));
        }
        break;
      }
  }
  //根据最小达成时间判断是否需要剪枝
  if (currentTime >= GetMaxControllableTime(minCard.minAchievingTime - 1))
    return false;
  return true;
}
struct MethodType
{
  bool isSet[2];//两张卡本秒是否放置
  bool isBanned[2];//两张卡未来是否禁放
};
//给定某种方法，按照此方法放置生产卡后，进行下一步搜索
void EnumMethod(int currentTime, MethodType &method0, MethodType &method1)
{
  void DFSAllMethod(int currentTime, bool(&originBan)[2][2]);//声明搜索函数
  MethodType method[2] = { method0, method1 };
  bool isBanned[2][2] = {};
  bool isAllBanned = true;
  //放置生产卡
  for (int account = 0; account < 2; account++)
    for (int order = 0; order < 2; order++)
    {
      isBanned[account][order] = method[account].isBanned[order];
      if (!isBanned[account][order])
        isAllBanned = false;
      if (method[account].isSet[order])
        AddOptProducer(currentTime, account, order);
    }
  if (isAllBanned)//卡全ban了，说明这一轮什么卡也没放，不用继续检查了
    return;
  //检查限制条件是否达成，如果需要继续检查，则搜索下一个时刻
  if (CheckConstraint(currentTime))
    DFSAllMethod(currentTime + 1, isBanned);
  if (isConstraintsAchieved) //限制条件已经达成，就不用搜索了
    return;
  //移除生产卡
  for (int account = 0; account < 2; account++)
    for (int order = 0; order < 2; order++)
      if (method[account].isSet[order])
        DeleteOptProducer(currentTime, account, order);
}
//添加方法
void AddMethod(MethodType(&method)[4], int &methodNum, const bool(&originBan)[2],
  const bool(&newBan)[2], const bool(&set)[2])
{
  for (int order = 0; order < 2; order++)
  {
    method[methodNum].isSet[order] = set[order];
    method[methodNum].isBanned[order] = originBan[order] || newBan[order];
  }
  methodNum++;
}
//搜索currentTime秒的各种放卡方式。originBan：两个账号的两张卡是否被强制禁放
void DFSAllMethod(int currentTime, bool(&originBan)[2][2])
{
  if (isConstraintsAchieved) //目标达成，搜索终止
    return;
  if (currentTime > endSearchTime) //时间超出范围，搜索终止
    return;
  for (int account = 0; account < accountNum; account++)//获得这一秒生产的火苗
    energy[account][currentTime] = energy[account][currentTime - 1] + globalCollect[currentTime];

  int isEnabled[2][2] = {};//两个账号两张生产卡是否可用
  int enabledNum[2] = {};//两个账号的可用生产卡数量
  for (int account = 0; account < 2; account++)
    for (int order = 0; order < 2; order++)
      if (optProducer[account][order].isEnabled && !originBan[account][order])
      {
        isEnabled[account][order] = true;
        enabledNum[account]++;
      }

  const bool none[2] = { false, false };
  const bool all[2] = { true, true };
  const bool single[2][2] = { { true, false }, { false, true } };//只放小火，只放酒杯

  //枚举16种放与不放的组合
  MethodType method[2][4] = {};//两个账号的至多4种方案
  int methodNum[2] = {};//两个账号的方案数量
  for (int account = 0; account < 2; account++)
  {
    if (enabledNum[account] == 0)//没有可用的生产卡，只能不放
      AddMethod(method[account], methodNum[account], originBan[account], none, none);
    else if (enabledNum[account] == 1)//有1张可用的生产卡
    {
      for (int order = 0; order < 2; order++)
        if (isEnabled[account][order])//如果是小火可用
        {
          //如果能量充足且已冷却好，可放可不放（2种方案）
          if (energy[account][currentTime] >= optProducer[account][order].cost && IsColdOpt(account, order, currentTime))
          {
            AddMethod(method[account], methodNum[account], originBan[account], none, single[order]);//放小火
            if (currentTime >= optProducer[account][order].period - 1)
              AddMethod(method[account], methodNum[account], originBan[account], single[order], none);//不放，以后禁放小火
            else
              AddMethod(method[account], methodNum[account], originBan[account], none, none);//不放，但也不禁放
          }
          else //否则只能不放（1种方案）
            AddMethod(method[account], methodNum[account], originBan[account], none, none);
        }
    }
    else if (enabledNum[account] == 2)//2张生产卡都可用
    {
      //两张卡是否已冷却好
      int isCold[2] = { IsColdOpt(account, 0, currentTime), IsColdOpt(account, 1, currentTime) };
      //如果能量够放两张卡，CD好的都要放，谁不放ban谁
      if (energy[account][currentTime] >= optProducer[account][0].cost + optProducer[account][1].cost)
      {
        for (int set0 = isCold[0]; set0 >= 0; set0--) //set0：0=不放小火，1=放小火
          for (int set1 = isCold[1]; set1 >= 0; set1--)
          {
            bool set[2] = { set0 != 0, set1 != 0 };//两张卡放置与否
            //两张卡ban与否
            bool ban[2] = { isCold[0] && !set0 && currentTime >= optProducer[account][0].period - 1,
              isCold[1] && !set1 && currentTime >= optProducer[account][1].period - 1 };
            AddMethod(method[account], methodNum[account], originBan[account], ban, set);
          }
      }
      //如果能量够放其中任意一张
      else if (energy[account][currentTime] >= max(optProducer[account][0].cost, optProducer[account][1].cost))
      {
        for (int set0 = isCold[0]; set0 >= 0; set0--) //set0：0=不放小火，1=放小火
          for (int set1 = isCold[1]; set1 >= 0; set1--)
          {
            bool set[2] = { set0 != 0, set1 != 0 };//两张卡放置与否
            //不能两个都放，因为火苗不够
            if (set0 && set1)
              continue;
            //如果两个都冷却好了，且两个都不放
            if (isCold[0] && !set0 && isCold[1] && !set1)
            {
              //过了首轮的卡要ban
              bool ban[2] = { currentTime >= optProducer[account][0].period - 1,
                currentTime >= optProducer[account][1].period - 1 };
              AddMethod(method[account], methodNum[account], originBan[account], ban, set);
            }
            else //其他情况不会ban卡
              AddMethod(method[account], methodNum[account], originBan[account], none, set);
          }
      }
      //如果能量只够放耗能低的那一张
      else if (energy[account][currentTime] >= min(optProducer[account][0].cost, optProducer[account][1].cost))
      {
        int order = optProducer[account][0].cost < optProducer[account][1].cost ? 0 : 1;//耗能低的卡片编号
        if (isCold[order]) //如果冷却好了可以放
          AddMethod(method[account], methodNum[account], originBan[account], none, single[order]);
        AddMethod(method[account], methodNum[account], originBan[account], none, none);
      }
      //如果能量不够放任何一张
      else
        AddMethod(method[account], methodNum[account], originBan[account], none, none);
    }
  }

  //遍历可行方案
  for (int i = 0; i < methodNum[0]; i++) //依次搜索每一个决策下的分支
    for (int j = 0; j < methodNum[1]; j++)
    {
      EnumMethod(currentTime, method[0][i], method[1][j]);
      if (isConstraintsAchieved) //限制条件已经达成，就不用搜索了
        return;
    }
}
//带约束条件的自动布轨
void ConstrainedAutoTrack()
{
  InitConstrainedAutoTrack();//初始化条件自动布轨
  bool originBan[2][2] = {};
  for (int account = 0; account < 2; account++)
    for (int order = 0; order < 2; order++)
      originBan[account][order] = !optProducer[account][order].isEnabled;
  DFSAllMethod(2, originBan);//从第2秒开始安排各种放卡方式
  char message[100];
  if (isConstraintsAchieved)
  {
    //输出答案：生产卡的每次放置时间
    for (int account = 0; account < 2; account++)
      for (int order = 0; order < 2; order++)
      {
        OptProducerType &opt = optProducer[account][order];
        for (int i = 0; i < opt.bestTimeNum; i++)
          AddTrack(account, opt.order, opt.bestTime[i]);
      }
    for (int i = 0; i < constraintNum; i++)
    {
      for (int t = constraint[i].maxTime; t >= minTime; t--)
        if (t == minTime || energy[0][t - 1] < constraint[i].cost)
        {
          AddTrack(0, constraint[i].order, t);
          break;
        }
    }
    sprintf_s(message, "求解成功，检查次数%d", checkTimes);
  }
  else
    sprintf_s(message, "求解失败，检查次数%d", checkTimes);
  MessageBox(hWndEditor, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
}
void MoveLevel(int account, int row0, int column0, int row, int column, int level0)//将层级移动到另一个空位
{//需要搬迁Map中的信息，并修改卡片这一次的种植位置（Loc），无需修改Track
  int order;
  if (map[account][row][column].depth[level0] >= maxStack) return;//如果目标格同层级已经有9张卡了，那就搬不成
  /*修改Loc*/
  int depth0 = map[account][row0][column0].depthSelected[level0];//获取源层级的选中深度
  order = GetOrder(account, map[account][row0][column0].name[level0][depth0]);//获取被搬迁的卡片序号
  location[account][order][map[account][row0][column0].times[level0][depth0]] = row * 10 + column;//该卡该次种植的位置改成新位置
  /*搬Map*/
  /*把被搬深度添加到目标层级最高深度*/
  int depth = map[account][row0][column0].depth[level0];//获取目标层级的第一个空深度
  strcpy_s(map[account][row][column].name[level0][depth], map[account][row0][column0].name[level0][depth0]);//添加名字
  map[account][row][column].times[level0][depth] = map[account][row0][column0].times[level0][depth0];//添加次数
  map[account][row0][column0].depth[level0]++;//目标层级深度+1
  /*删除源层级被搬深度*/
  if (depth0 == map[account][row][column].depth[level0] - 1) //如果被搬深度是源层级的最高深度
  {
    strcpy_s(map[account][row0][column0].name[level0][depth0], "");//删除名字
    map[account][row0][column0].times[level0][depth0] = 0;//删除次数
    map[account][row][column].depth[level0]--;//源层级深度-1
  }
  else //否则，选中深度赋值为最高深度，最高深度清空，源层级深度-1
  {
    int MaxDepth = map[account][row][column].depth[level0] - 1;
    strcpy_s(map[account][row0][column0].name[level0][depth0], map[account][row0][column0].name[level0][MaxDepth]);//赋值名字
    map[account][row0][column0].times[level0][depth0] = map[account][row0][column0].times[level0][MaxDepth];//赋值次数
    strcpy_s(map[account][row0][column0].name[level0][MaxDepth], "");//删除名字
    map[account][row0][column0].times[level0][MaxDepth] = 0;//删除次数
    map[account][row][column].depth[level0]--;//源层级深度-1
  }
}
//把第times0个位置插入到第times1个位置
void InsertRole(int account, int times0, int times1)
{
  if (times0 > times1)
  {
    int loc = roleLoc[account][times0];
    for (int times = times0; times > times1; times--)
      roleLoc[account][times] = roleLoc[account][times - 1];
    roleLoc[account][times1] = loc;
  }
  else if (times0 < times1)
  {
    int loc = roleLoc[account][times0];
    for (int times = times0; times < times1; times++)
      roleLoc[account][times] = roleLoc[account][times + 1];
    roleLoc[account][times1] = loc;
  }
}
//同层级的卡位置插入
void InsertLevel(int account, int row0, int column0, int depth0, int row1, int column1, int depth1, int level0)
{
  int times, row, column;
  int mLoc;
  int order = GetOrder(account, map[account][row0][column0].name[level0][depth0]);//选中卡片序号

  //如果两个格子同层级选中深度不是同名卡，不能执行插入
  if (strcmp(map[account][row0][column0].name[level0][depth0], map[account][row1][column1].name[level0][depth1]) != zero)
    return;
  int times0 = map[account][row0][column0].times[level0][depth0];//选中卡片次序
  int times1 = map[account][row1][column1].times[level0][depth1];//目标卡片次序
  if (times0 < times1)//如果选中次序小于目标次序（1[234]56，2插4，变成1[423]56）
  {
    /*修改Map中的次序*/
    map[account][row0][column0].times[level0][depth0] = times1;//先把海星2赋值为海星4
    for (times = times0 + 1; times <= times1; times++)//对海星3-海星4，它们的数值都要减1
    {
      row = location[account][order][times] / 10;
      column = location[account][order][times] % 10;//查找海星times所在的格子
      for (int d = 0; d < map[account][row][column].depth[level0]; d++)//在该格子的海星层级内搜索每一个深度
        if (GetOrder(account, map[account][row][column].name[level0][d]) == order && map[account][row][column].times[level0][d] == times)
          //如果该深度的卡片正是海星times，说明要找的就是这个深度
        {
          map[account][row][column].times[level0][d]--;//该海星的次序号减1
          break;//无需再查找这一格的其他深度
        }
    }
    /*轮换Loc中的次序*/
    mLoc = location[account][order][times0];
    for (times = times0; times < times1; times++)
      location[account][order][times] = location[account][order][times + 1];
    location[account][order][times1] = mLoc;
  }
  else//如果选中次序大于目标次序（1[234]56，4插2，变成1[342]56）times0=4，times1=2
  {
    /*修改Map中的次序*/
    map[account][row0][column0].times[level0][depth0] = times1;//先把海星4赋值为海星2
    for (times = times0 - 1; times >= times1; times--)//对海星3~海星2，它们的数值都要加1
    {
      row = location[account][order][times] / 10;
      column = location[account][order][times] % 10;//查找海星times所在的格子
      for (int d = 0; d < map[account][row][column].depth[level0]; d++)//在该格子的海星层级内搜索每一个深度
        if (GetOrder(account, map[account][row][column].name[level0][d]) == order && map[account][row][column].times[level0][d] == times)
          //如果该深度的卡片正是海星times，说明要找的就是这个深度
        {
          map[account][row][column].times[level0][d]++;//该海星的次序号减1
          break;//无需再查找这一格的其他深度
        }
    }
    /*轮换Loc中的次序*/
    mLoc = location[account][order][times0];
    for (times = times0; times > times1; times--)
      location[account][order][times] = location[account][order][times - 1];
    location[account][order][times1] = mLoc;
  }
}
void InsertLevel(int account, int row0, int column0, int row1, int column1, int level0)//同层级的卡位置插入。仅对选中深度执行插入
{
  InsertLevel(account, row0, column0, map[account][row0][column0].depthSelected[level0], row1, column1, map[account][row1][column1].depthSelected[level0], level0);
}
void InsertTimes(int account, int order, int times0, int times1)//将卡片order的times0次种植插入到times1次之前
{
  int row0, column0, depth0, row1, column1, depth1, level;
  level = slot[account][order].level;
  row0 = location[account][order][times0] / 10;
  column0 = location[account][order][times0] % 10;
  for (depth0 = 0; depth0 < map[account][row0][column0].depth[level]; depth0++)//遍历所有深度
    if (order == GetOrder(account, map[account][row0][column0].name[level][depth0]) && times0 == map[account][row0][column0].times[level][depth0])
      break;//搜索order号卡第times0次种植在哪个深度
  row1 = location[account][order][times1] / 10;
  column1 = location[account][order][times1] % 10;
  for (depth1 = 0; depth1 < map[account][row1][column1].depth[level]; depth1++)//遍历所有深度
    if (order == GetOrder(account, map[account][row1][column1].name[level][depth1]) && times1 == map[account][row1][column1].times[level][depth1])
      break;//搜索order号卡第times1次种植在哪个深度
  InsertLevel(account, row0, column0, depth0, row1, column1, depth1, level);
}
//移动0.1格
void SmallMoveTrack(int account, int order)
{
}
//移动轨道distance格，但不能出界
void MoveTrack(int account, int order, int distance)
{
  if (distance == zero)
    return;
  if (distance > zero)//右移：轨道时间+1
  {
    if (track[account][order][maxTime] >= 1)
      return;//右边顶满，不准移动
    for (int t = maxTime; t >= 1; t--)
      track[account][order][t] = track[account][order][t - 1];
    track[account][order][0] = 0;
    for (int times = 1; times < maxPlantTimes && moment[account][order][times] > 0; times++)
      moment[account][order][times] = moment[account][order][times] + 10;
    MoveTrack(account, order, distance - 1);
  }
  else if (distance < zero)//左移：轨道时间-10
  {
    if (track[account][order][minTime] >= 1)
      return;//左边顶满，不准移动
    for (int t = 0; t <= maxTime - 1; t++)
      track[account][order][t] = track[account][order][t + 1];
    track[account][order][maxTime] = 0;
    for (int times = 1; times < maxPlantTimes; times++)
    {
      if (moment[account][order][times] == zero)
        break;
      moment[account][order][times] = moment[account][order][times] - 10;
    }
    MoveTrack(account, order, distance + 1);
  }
}
//获得slotName前一个卡槽的编号
int GetLastSlot(int account, const char *slotName)
{
  int slotOrder = GetOrder(account, slotName);
  if (slotOrder == -1)
    return -1;//卡组中没有slotName，返回-1
  //向前查找第一个普通卡
  for (int order = slotOrder - 1; order >= 1; order--)
    if (slot[account][order].level <= 4)
      return order;
  return -1;//slotName前面没有普通卡，返回-1
}
//将地图中的幻鸡复原成复制目标本体，幻鸡轨道清空
void MagicReturn(int account)
{
  int t, times;
  int row, column, depth, level;
  int magic = GetOrder(account, "幻幻鸡");
  if (magic == -1)
    return;//卡槽中没有幻幻鸡，无需操作
  int MagicTimes = GetLocTimes(account, magic);//获得幻幻鸡的总种植次数
  int target, TargetTimes;
  for (times = MagicTimes; times >= 1; times--)//按Loc数组倒序查找每一个幻幻鸡
  {
    t = GetTime(account, magic, times);//获得幻幻鸡第times次种植的时刻
    GetMagicTarget(account, t, target, TargetTimes);//获得这个幻幻鸡复制目标的编号和种植次数。如果该幻鸡没有轨道，或此前没有卡片，则target=0
    row = location[account][magic][times] / 10;
    column = location[account][magic][times] % 10;//获得幻幻鸡所在行列
    DeleteFromGridLevel(account, row, column, 6);//从地图中删除这个幻幻鸡
    if (target > zero) //如果存在目标卡，在幻鸡所在格子添加复制目标本地，并插入到下一次种植
    {
      AddToGrid(account, target, row, column);//添加幻鸡目标卡到地图，自动识别层级，添加到最高深度
      level = slot[account][target].level;//获得目标卡层级
      depth = map[account][row][column].depth[level] - 1;//获得目标层级最高深度（即刚添加的卡所在深度）
      if (map[account][row][column].times[level][depth] != TargetTimes + 1) //如果新增目标本体的种植序号不是原目标序号+1，则需要调整顺序
        InsertTimes(account, target, map[account][row][column].times[level][depth], TargetTimes + 1);
      //例如幻鸡复制的是海星20，现在新增的海星是海星26，26不等于21，故需要将海星26插入到海星21
    }
  }
  memset(track[account][magic], 0, sizeof(track[account][magic]));
  memset(moment[account][magic], 0, sizeof(moment[account][magic]));
}
void IceReturn(int account)//将冰激凌地图轨道全部删除
{
  int ice = GetOrder(account, "冰激凌");
  if (ice > zero)
  {
    for (int times = maxPlantTimes; times >= 1; times--)//删除冰激凌地图
      if (location[account][ice][times] > zero)
      {
        int row, column, level, depth;
        GetMapLoc(account, ice, times, row, column, level, depth);
        DeleteFromGrid(account, row, column, level, depth);
      }
    for (int t = 0; t <= maxTime; t++)//删除冰激凌轨道
      track[account][ice][t] = 0;
  }
}
void SorbetReturn(int account)//将冰沙地图轨道全部删除
{
  int sorbet = GetOrder(account, "冰沙");
  if (sorbet > zero)
    for (int t = maxTime; t >= zero; t--)//删除所有冰沙轨道
      if (track[account][sorbet][t] == 1)
        DeleteTrack(account, sorbet, t);
}
void IceMagicReturn(int account)//将地图中的幻幻鸡复原成复制目标本体，幻鸡冰沙冰激凌地图轨道全部删除
{
  IceReturn(account);
  SorbetReturn(account);
  MagicReturn(account);
}
//新增防御卡
void AddCommonCard()
{
  int commonOrder = bagNum[curAccount];
  memset(&bag[curAccount][commonOrder], 0, sizeof(bag[curAccount][commonOrder]));
  strcpy_s(bag[curAccount][commonOrder].name, "未命名");
  bag[curAccount][commonOrder].cd = 7;
  bag[curAccount][commonOrder].level = 3;
  commonPage = commonOrder / 26 + 1;
  bagNum[curAccount]++;
}
//编辑注释
void EditNotation()
{
  CreateThread(NULL, 0, AutoTransform, (void *)"注释", 0, NULL);//运行自动转化
  NewInputBox(notation[curAccount], 121, "可在此补充配置要求或提示信息，不超过60字。", "注释", notation[curAccount]);
  TransformStopSignal = 1;
}
//波次字符串m.n是否符合格式；额外检查“波次已存在”
int IsWaveLegalForEdit(const char *waveString)
{
  if (!IsWaveLegal(waveString))
    return 1;//不符合格式返回1
  int wave = 0, smallWave = 0;
  int scannedNum = sscanf_s(waveString, "%d.%d", &wave, &smallWave);
  if (totalWaveExist[curAccount][wave * maxSmallWave + smallWave])
    return 2;//波次已存在返回2
  return 0;//输入正常
}
//输入波次进行创建、复制或移动
void EditNewWave(SHORT ctrl, SHORT shift)
{
  const char *title = "新建波次";
  const char *prompt = "输入要新建的波次（m.n），关卡到达该波后使用该波轨道。\n大波m范围0~23，小波n范围0~11，可以只输入大波。";
  if (ctrl)
  {
    title = "复制到新波次";
    prompt = "将当前波次复制到输入波次（m.n）。\n大波m范围0~23，小波n范围0~11，可以只输入大波。";
  }
  else if (shift)
  {
    if (curTotalWave[curAccount] == 0)
    {
      PopMessage(hWndEditor, "第0波不允许移动。");
      return;
    }
    title = "移动到新波次";
    prompt = "将当前波次移动到输入波次（m.n）。\n大波m范围0~23，小波n范围0~11，可以只输入大波。";
  }
  char waveString[6] = {};
  int result = InputWaveString(waveString, IsWaveLegalForEdit, prompt, title);//输入波次
  if (result == 0)
    return;
  int mainWave = 0, smallWave = 0;
  int scannedNum = sscanf_s(waveString, "%d.%d", &mainWave, &smallWave);
  int totalWave = mainWave * maxSmallWave + smallWave;
  if (ctrl)
    CopyWave(curAccount, totalWave);
  else if (shift)
    MoveWave(curAccount, totalWave);
  else
    AddWave(curAccount, totalWave);
}
//编辑防御卡名称
void EditCardName()
{
  char OriginName[10];
  strcpy_s(OriginName, bag[curAccount][bagMode].name);

  CreateThread(NULL, 0, AutoTransform, (void *)"防御卡名称", 0, NULL);//运行自动转化
  NewInputBox(bag[curAccount][bagMode].name, 7, "长度为1-3个汉字", "防御卡名称", OriginName);
  while (strlen(bag[curAccount][bagMode].name) == zero)
    NewInputBox(bag[curAccount][bagMode].name, 7, "您的输入不规范，请重新输入。\n长度为1-3个汉字", "防御卡名称", OriginName);
  TransformStopSignal = 1;

  if (strcmp(bag[curAccount][bagMode].name, "炎焱兔") == zero)
    bag[curAccount][bagMode].style = 6;
  else if (strcmp(bag[curAccount][bagMode].name, "初始火") == zero)
    bag[curAccount][bagMode].style = 7;
  else if (strcmp(bag[curAccount][bagMode].name, "初始灯") == zero)
    bag[curAccount][bagMode].style = 8;
  else if (bag[curAccount][bagMode].style >= 6)
    bag[curAccount][bagMode].style = 0;
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
void SetRatio()//设置作图比例
{
  int nScreenWidth, nScreenHeight;
  nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
  nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
  ratio = min((float)nScreenWidth / 1382, (float)nScreenHeight / 864);
  setaspectratio(ratio, ratio);
}
//游戏里的区域：100-121=铲子、卡槽；11-79=地图，200=左上角退出
int GetGameArea(int x, int y)
{
  if (x < zero || y < zero || x >= 950 || y >= 596)
    return -1;//屏幕外点击无效
  /*退出的判定*/
  if (x < RoleWidth && y < RoleHeight)
    return 200;//退出标志
  /*铲子的判定*/
  if (realSlotNum[curAccount] <= 12)
  {
    if (x >= SlotX + realSlotNum[curAccount] * SlotWidth && x < SlotX + realSlotNum[curAccount] * SlotWidth + ShovelWidth && y < ShovelY + ShovelHeight)
      return 100;
  }
  else if (realSlotNum[curAccount] == 13)
  {
    if (x >= ShovelX && y >= SlotY + SlotHeight && y < SlotY + SlotHeight + ShovelHeight)
      return 100;
  }
  else if (realSlotNum[curAccount] >= 14 && realSlotNum[curAccount] <= 20)
  {
    if (x >= ShovelX && y >= SlotY + (realSlotNum[curAccount] - 13) * SlotHeight && y < SlotY + (realSlotNum[curAccount] - 13) * SlotHeight + ShovelHeight)
      return 100;
  }
  else
  {
    if (x >= LShovelX && x < LShovelX + LShovelWidth && y >= LShovelY && y < LShovelY + LShovelHeight)
      return 100;
  }
  /*卡槽的判定*/
  if (x >= SlotX && x < SlotX + 14 * SlotWidth && y >= SlotY && y < SlotY + SlotHeight)
  {
    int order = (x - SlotX) / SlotWidth + 1;//1-14槽
    if (order <= realSlotNum[curAccount])
      return 100 + order;
    else
      return 0;
  }
  if (x >= SlotX + 13 * SlotWidth && x < SlotX + 14 * SlotWidth && y >= SlotY + SlotHeight && y < SlotY + 8 * SlotHeight)
  {
    int order = (y - SlotY) / SlotHeight + 14;//15-21槽
    if (order <= realSlotNum[curAccount])
      return 100 + order;
    else
      return 0;
  }
  if (x >= MapX && x < MapX + 9 * MapWidth && y >= MapY && y < MapY + 7 * MapHeight)
  {
    int row = (y - MapY) / MapHeight + 1;
    int column = (x - MapX) / MapWidth + 1;
    return row * 10 + column;
  }
  return 0;
}
void GetMutiColor(int num, COLORREF *color, POINT *point)
{
  for (int i = 0; i < num; i++)//先把各点颜色预设为0
    color[i] = 0;
  if (!IsWindow(hWnd))//没有窗口就结束
    return;
  HDC hScreenDC = GetDC(hWnd);
  for (int i = 0; i < num; i++)//先用GetPixel尝试读取各点颜色
    color[i] = GetPixel(hScreenDC, point[i].x, point[i].y);
  //如果每个点颜色都读到了，就结束函数
  int success = 1;
  for (int i = 0; i < num; i++)//先用GetPixel尝试读取各点颜色
  {
    if (color[i] == -1) //如果颜色是-1，说明这个点没有读取成功。
    {
      success = 0;//有一个点没成功就需要PrintWindow
      break;
    }
  }
  if (success == 1) //如果每个点都读取成功
  {
    ReleaseDC(NULL, hScreenDC);//直接释放DC并退出函数
    return;
  }
  //如果有些点没读取成功，需要用PrintWindow再次读取
  HDC hMemDC = CreateCompatibleDC(hScreenDC);
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, 950, 596);
  SelectObject(hMemDC, hBitmap);
  PrintWindow(hWnd, hMemDC, NULL);//将窗口内容复制到内存DC
  InvalidateRect(hWnd, NULL, false);//重画
  for (int i = 0; i < num; i++)//从内存DC中获取颜色
    color[i] = GetPixel(hMemDC, point[i].x, point[i].y);
  DeleteObject(hBitmap);
  DeleteDC(hMemDC);
  ReleaseDC(NULL, hScreenDC);
}
//识图获得卡组数量
int GetSlotNum()
{
  COLORREF color[42];
  POINT point[42] = { { 217, 11 } };//13-21槽的情形
  point[21].x = point[0].x + 21;//point[21..32]对应1-12槽
  point[21].y = point[0].y;
  int i, num1, num2;
  for (i = 1; i <= 13; i++)
  {
    point[i].x = point[i - 1].x + SlotWidth;
    point[i].y = point[i - 1].y;
    point[21 + i].x = point[i].x + 21;
    point[21 + i].y = point[i].y;
  }
  for (i = 14; i <= 20; i++)
  {
    point[i].x = point[i - 1].x;
    point[i].y = point[i - 1].y + SlotHeight;
  }
  GetMutiColor(33, color, point);//0-20为21槽；21-32为12槽
  for (i = 0; i <= 20; i++)
    if (color[i] != 0xd9c7ca)
      break;
  num1 = i;
  for (i = 21; i <= 32; i++)
    if (color[i] != 0xd9c7ca)
      break;
  num2 = i - 21;
  return max(num1, num2);
}
//关卡进入：1=关卡中，2=加载，0=其他；波数0-13,14-18；弹窗信息：0=无，1=继续作战，2=掉线
void GetGameState(int &entry, int &wave, int &box)
{
  COLORREF color[4];
  POINT point[4] = { { 100, 45 }, { 100, 136 }, { 670, 552 }, { 475, 280 } };
  GetMutiColor(4, color, point);
  if (color[0] == 0xcaf3fc)//检测到房主号在关卡内
  {
    if (color[1] == 0xcaf3fc)//如果队友号也在关卡内
      entry = 2;//双人进入关卡
    else
      entry = 1;//单人进入关卡
  }
  else if (color[0] == zero)//识图失败，视为已进入关卡
    entry = 3;
  else if (color[0] == 0x72320f)
    entry = -1;//加载中
  else
    entry = 0;
  wave = -1;
  for (int i = 0; i <= 18; i++)
  {
    if (color[2] == WaveColor[i])
    {
      wave = i;
      break;
    }
  }
  if (color[3] == 0xf5c87e) //掉线
    box = 2;
  else if (color[3] == 0xacbad9) //继续作战
    box = 1;
  else
    box = 0;
}
//单击指定位置
BOOL Click(int x, int y)
{
  int x1 = x * GameRect.right / 950;//坐标缩放修正
  int y1 = y * GameRect.bottom / 596;
  BOOL result = PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x1, y1));
  PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(x1, y1));
  return result;
}
void ScreenToClientWithDPI(HWND hWnd, POINT *lpt)//转化为窗口内坐标
{
  ScreenToClient(hWnd, lpt);
  lpt->x = lpt->x * 950 / GameRect.right;
  lpt->y = lpt->y * 596 / GameRect.bottom;
}
//重绘录制信息
void RepaintRecord()
{
  char message[100];//信息
  char CardName[10];//卡片名称
  int LineY = textheight("小") / 2;
  int MaxPage = (recordNum - 1) / 20 + 1;//显示所需页数
  int RecordOrder;

  BeginBatchDraw();
  solidrectangle(0, 0, recordWndWidth, recordWndHeight);//清空画面
  settextcolor(RGB(255, 255, 255));
  setlinecolor(RGB(255, 255, 255));
  if (recordStage == zero)//录制前
  {
    outtextxy(recordX, 70, "请将鼠标指向游戏窗口，");
    outtextxy(recordX, 70 + recordHeight, "等待自动进入关卡。");
    CenterView("结束录制", recordEndCX, recordEndCY);
    CenterRectangle(recordEndCX, recordEndCY, 50, 20);
  }
  else if (recordStage == 1)//成功进入关卡
  {
    outtextxy(recordX, 70, "录制开始，请放置人物。");
    CenterView("结束录制", recordEndCX, recordEndCY);
    CenterRectangle(recordEndCX, recordEndCY, 50, 20);
  }
  else if (recordStage == 2) //成功放下人物后，显示人物位置和放卡信息
  {
    sprintf_s(message, "人物位置=%d  卡槽数=%d", roleLoc[curAccount][1], realSlotNum[curAccount]);
    outtextxy(recordX, 70, message);

    sprintf_s(message, "%d/%d", recordPage, MaxPage);
    CenterView(message, recordPageCX, recordPageCY);

    LOGFONT Font;
    int size = GetFontSize();
    SetFontSize(16);
    gettextstyle(&Font);
    Font.lfOrientation = 900;
    settextstyle(&Font);

    CenterView("▲", recordPageCX - 50 - 4, recordPageCY + 8);
    CenterView("▼", recordPageCX + 50 - 20, recordPageCY + 8);

    Font.lfOrientation = 0;
    settextstyle(&Font);
    SetFontSize(size);

    CenterView("结束录制", recordEndCX, recordEndCY);
    CenterRectangle(recordEndCX, recordEndCY, 50, 20);

    HorizontalLine(recordX - 3, recordWndWidth - recordX + 3, recordY - 11, 1);
    HorizontalLine(recordX - 3, recordWndWidth - recordX + 3, recordY + 20 * recordHeight - 1, 1);

    setlinecolor(RGB(128, 128, 128));
    for (int i = 0; i < 20; i++)
    {
      RecordOrder = (recordPage - 1) * 20 + i;
      if (RecordOrder < recordNum)
      {
        if (Record[RecordOrder].exist == 1)
        {
          settextcolor(levelColor[slot[curAccount][Record[RecordOrder].order].level]);
          if (Record[RecordOrder].loc > zero)
            outtextxy(recordX + recordLength + 15, recordY + i * recordHeight, "×");
        }
        else
        {
          settextcolor(RGB(128, 128, 128));
          outtextxy(recordX + recordLength + 13, recordY + i * recordHeight, "√");
          HorizontalLine(recordX, recordX + recordLength, recordY + i * recordHeight + LineY, 1);
        }

        /*显示序号*/
        sprintf_s(message, "%03d [ ", RecordOrder + 1);
        outtextxy(recordX, recordY + i * recordHeight, message);

        /*显示位置时间*/
        if (Record[RecordOrder].loc == zero)
          sprintf_s(message, " ]");
        else
          sprintf_s(message, " ] %03d.%d|%d", Record[RecordOrder].time / 10, Record[RecordOrder].time % 10, Record[RecordOrder].loc);
        outtextxy(recordX + headWidth + cardNameWidth, recordY + i * recordHeight, message);

        /*显示名称*/
        strcpy_s(CardName, slot[curAccount][Record[RecordOrder].order].name);
        outtextxy(recordX + headWidth + (cardNameWidth - textwidth(CardName)) / 2, recordY + i * recordHeight, CardName);
      }
    }
    setlinecolor(RGB(255, 255, 255));
  }
  ViewBottomInfo();
  EndBatchDraw();
}
//录制轨道：恢复放卡
void RecordRecoverPlant(int RecordOrder)
{
  Record[RecordOrder].exist = 1;
  int times;
  int order = Record[RecordOrder].order;
  int time = Record[RecordOrder].time;
  int loc = Record[RecordOrder].loc;
  for (times = 1; times <= maxPlantTimes; times++)
    if (moment[curAccount][order][times] > time || moment[curAccount][order][times] == zero)
      break;
  if (moment[curAccount][order][times] == zero)//如果没有晚于time的种植，则插入到最后
  {
    location[curAccount][order][times] = loc;
    moment[curAccount][order][times] = time;
    return;
  }
  int TargetTimes = times;//第一个晚于time的种植次数
  for (times = TargetTimes; times <= maxPlantTimes; times++)
    if (moment[curAccount][order][times] == zero)
      break;
  int MaxTimes = times;//向后找到第一个空位
  for (times = MaxTimes; times >= TargetTimes + 1; times--)
  {
    location[curAccount][order][times] = location[curAccount][order][times - 1];
    moment[curAccount][order][times] = moment[curAccount][order][times - 1];
  }
  location[curAccount][order][TargetTimes] = loc;
  moment[curAccount][order][TargetTimes] = time;
  SaveTrack();
  LoadTrack();
}
//录制轨道：删除放卡
void RecordDeletePlant(int RecordOrder)
{
  Record[RecordOrder].exist = 0;
  int times;
  int order = Record[RecordOrder].order;
  int time = Record[RecordOrder].time;
  for (times = 1; times <= maxPlantTimes; times++)
    if (moment[curAccount][order][times] == time)
      break;
  //找到要删除的那次种植
  int TargetTimes = times;
  for (times = TargetTimes; times <= maxPlantTimes; times++)
  {
    location[curAccount][order][times] = location[curAccount][order][times + 1];
    moment[curAccount][order][times] = moment[curAccount][order][times + 1];
    if (moment[curAccount][order][times] == zero)
      break;
  }
  SaveTrack();
  LoadTrack();
}
//录制轨道：选卡
void RecordChooseCard(int order)
{
  Record[recordNum].order = order;
  Record[recordNum].time = 0;
  Record[recordNum].loc = 0;
  Record[recordNum].exist = 1;
  recordNum++;
  recordPage = (recordNum - 1) / 20 + 1;//切换到最后一页
}
//录制轨道：取消选卡
void RecordReleaseCard()
{
  recordNum--;
  recordPage = (recordNum - 1) / 20 + 1;//切换到最后一页
}
//录制轨道：放下选中的卡
void RecordPlantCard(int time, int loc)
{
  Record[recordNum - 1].time = time;
  Record[recordNum - 1].loc = loc;
  Record[recordNum - 1].exist = 1;
  recordPage = (recordNum - 1) / 20 + 1;//切换到最后一页
}
//录制轨道：恢复到轨道编辑界面
void RecoverTrackEdit(RECT rect)
{
  /*还原窗口、取消置顶、恢复原位置*/
  solidrectangle(0, 0, recordWndWidth, recordWndHeight);//清空画面
  Resize(NULL, (int)(wndWidth * ratio), (int)(wndHeight * ratio));
  SetWindowPos(hWndEditor, HWND_NOTOPMOST, rect.left, rect.top, 0, 0, SWP_NOSIZE);//移动到左上角
  SetWindowText(hWndEditor, wndTitle);
  mode = 2;
  flushmessage(EX_MOUSE);
}
//第3模式：录制轨道模式
void RecordTrack()
{
  recordNum = 0;
  memset(Record, 0, sizeof(Record));//清空录制信息
  /*缩小窗口、置顶、靠边*/
  solidrectangle(0, 0, wndWidth, wndHeight);//清空画面
  Resize(NULL, (int)(recordWndWidth * ratio), (int)(recordWndHeight * ratio));//缩小窗口（缩小后绘图比例没问题）
  RECT rect;
  GetWindowRect(hWndEditor, &rect);
  POINT point = { 0, 0 };
  ClientToScreen(hWndEditor, &point);
  SetWindowPos(hWndEditor, HWND_TOPMOST, rect.left - point.x, 0, 0, 0, SWP_NOSIZE);//移动到左上角
  SetWindowText(hWndEditor, "准备录制");

  recordStage = 0;
  RepaintRecord();

  POINT pt, ptEditor;
  HWND hWndPointDown;//鼠标按下时指向的窗口
  int GameArea;
  int SlotSelected = 22;//选中的卡槽和铲子，-1=未选中，22=选中人物，人物无法取消选中
  int SelectionBeforePress = 1;//按下鼠标前是否有选中
  int CurrentTime;//当前时间
  int Times[maxSlotNum + 1];//每张卡的已放置次数
  memset(Times, 0, sizeof(Times));

  /*查找游戏窗口*/
  while (true)
  {
    Sleep(1);
    if (GetAsyncKeyState(VK_LBUTTON))//如果左键按下
    {
      GetCursorPos(&ptEditor);//获取按下时的鼠标位置
      hWndPointDown = WindowFromPoint(ptEditor);//获取鼠标指向的窗口
      if (hWndPointDown == hWndEditor)//如果点击了编辑器内部
      {
        ScreenToClient(hWndEditor, &ptEditor);//转化为编辑器客户区坐标
        area = GetArea(ptEditor.x, ptEditor.y);//获取编辑器区域
        if (area == 300)
          return RecoverTrackEdit(rect);//恢复轨道编辑并退出
      }
      while (GetAsyncKeyState(VK_LBUTTON))//等待鼠标左键松开
        Sleep(1);
    }
    hWnd = MousePoint();
    if (IsWindow(hWnd))
    {
      char ClassName[100];
      GetClassName(hWnd, ClassName, 256);//获取窗口类名
      if (strcmp(ClassName, "WebPluginView") == zero || strcmp(ClassName, "NativeWindowClass") == zero)
        break;
    }
  }
  GetClientRect(hWnd, &GameRect);

  //进入关卡
  int Entry, Wave, Box;
  int StartTick = GetTickCount();
  int CurrentTick;
  int iColor = 0;
  while (true)
  {
    Sleep(1);//每个循环等0.1秒
    if (GetAsyncKeyState(VK_LBUTTON))//如果左键按下
    {
      GetCursorPos(&ptEditor);//获取按下时的鼠标位置
      hWndPointDown = WindowFromPoint(ptEditor);//获取鼠标指向的窗口
      if (hWndPointDown == hWndEditor)//如果点击了编辑器内部
      {
        ScreenToClient(hWndEditor, &ptEditor);//转化为编辑器客户区坐标
        area = GetArea(ptEditor.x, ptEditor.y);//获取编辑器区域
        if (area == 300)
          return RecoverTrackEdit(rect);//恢复轨道编辑并退出
      }
      while (GetAsyncKeyState(VK_LBUTTON))//等待鼠标左键松开
        Sleep(1);
    }
    CurrentTick = GetTickCount();
    if (CurrentTick - StartTick > 100)
    {
      iColor++;
      StartTick = CurrentTick;
      GetGameState(Entry, Wave, Box);//读取关卡进入状态，Box=2意味着弹出提示
      if (Entry > zero)//已经进入关卡，则跳出循环
        break;
      if (iColor % 10 == zero)
        if (Click(StartX, StartY) == false) //单击开始，如果点击失败则弹窗提示并返回轨道编辑界面
        {
          MessageBox(hWndEditor, "权限不足。请先关闭当前窗口，然后\n右键编辑器选择“以管理员身份运行”。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
          return RecoverTrackEdit(rect);
        }
    }
  }

  StartTick = GetTickCount();//设置开始时间
  /*成功进入关卡*/
  realSlotNum[curAccount] = GetSlotNum();//记录游戏中卡槽数量
  /*若轨道中卡槽数小于卡组数，增添cd7s的卡组*/
  char NewSlotName[7];
  for (int order = slotNum[curAccount] + 1; order <= realSlotNum[curAccount]; order++)
  {
    sprintf_s(NewSlotName, "卡片%d", order);
    AddEmptyToSlot(curAccount, NewSlotName);
    slot[curAccount][order].cd = 7;
  }
  /*所有卡槽打开独立计时，关闭索敌、补阵和极限成阵*/
  for (int order = 0; order <= realSlotNum[curAccount]; order++)
  {
    slot[curAccount][order].independent = 1;
    slot[curAccount][order].search = 0;
    slot[curAccount][order].repair = 0;
    slot[curAccount][order].limit = 0;
  }
  if (realSlotNum[curAccount] <= 12)//根据卡槽数量确定卡槽左端位置
    SlotX = 213;
  else
    SlotX = 192;

  recordStage = 1;
  RepaintRecord();//反馈录制开始，提示放人物
  SetWindowText(hWndEditor, "正在录制");

  /*录制鼠标操作*/
  while (1)
  {
    if (GetAsyncKeyState(VK_LBUTTON))
    {
      /*按下鼠标时检测：无选中时，立即选中卡槽；有选中时，此坐标不生效*/
      GetCursorPos(&pt);//获取按下时的鼠标位置
      hWndPointDown = WindowFromPoint(pt);//获取鼠标指向的窗口
      if (hWndPointDown == hWndEditor)//如果点击了编辑器内部
      {
        ptEditor = pt;//转移坐标值到ptEditor，因为pt后面还要用
        ScreenToClient(hWndEditor, &ptEditor);//转化为编辑器客户区坐标
        area = GetArea(ptEditor.x, ptEditor.y);//获取编辑器区域

        if (SlotSelected != 22)//根据阶段和点击区域执行操作
        {
          if (area / 100 == 1)
          {
            int RecordOrder = area % 100 + (recordPage - 1) * 20;
            if (RecordOrder < recordNum)
              if (Record[RecordOrder].loc > zero)
              {
                if (Record[RecordOrder].exist == 1)
                  RecordDeletePlant(RecordOrder);
                else
                  RecordRecoverPlant(RecordOrder);
              }
          }
          if (area == 201)
          {
            if (recordPage > 1)
              recordPage--;
          }
          if (area == 202)
          {
            if (recordPage < (recordNum - 1) / 20 + 1)
              recordPage++;
          }
          RepaintRecord();
        }

        if (area == 300)
          return RecoverTrackEdit(rect);//恢复轨道编辑并退出
      }

      if (IsWindow(hWnd))
        ScreenToClientWithDPI(hWnd, &pt);//转化为窗口内坐标
      else
      {
        MessageBox(hWndEditor, "游戏窗口已关闭，录制结束", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        return RecoverTrackEdit(rect);//恢复轨道编辑并退出
      }
      if (hWndPointDown == hWnd)//如果点击发生在窗口内
        GameArea = GetGameArea(pt.x, pt.y);//获得鼠标点击区域
      else
        GameArea = -1;//否则视为无效点击
      if (GameArea == 200)//连点两下人物槽结束录制
      {
        ExitCount++;
        if (ExitCount >= 2)
        {
          MessageBox(hWndEditor, "录制成功", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
          return RecoverTrackEdit(rect);//恢复轨道编辑并退出
        }
      }
      else
        ExitCount = 0;
      if (SlotSelected == -1)//无选中
      {
        SelectionBeforePress = 0;//记录：按下鼠标时未选中
        if (GameArea >= 100 && GameArea <= 121)//如果点击卡槽或铲子
        {
          SlotSelected = GameArea - 100;//选中卡槽
          RecordChooseCard(SlotSelected);//记录选卡
          RepaintRecord();//输出录制内容
        }
      }
      else
        SelectionBeforePress = 1;//记录：按下鼠标时有选中

      while (GetAsyncKeyState(VK_LBUTTON))//等待鼠标左键松开
        Sleep(1);

      /*松开鼠标时检测：按下鼠标时无选中，无判定；按下鼠标时有选中，种植或取消选中*/
      GetCursorPos(&pt);//获取鼠标位置
      if (IsWindow(hWnd))
        ScreenToClientWithDPI(hWnd, &pt);//转化为窗口内坐标
      else
      {
        MessageBox(hWndEditor, "游戏窗口已关闭，录制结束", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        return RecoverTrackEdit(rect);//恢复轨道编辑并退出
      }

      if (hWndPointDown == hWnd)//如果按下时在窗口内
        GameArea = GetGameArea(pt.x, pt.y);//获得鼠标点击区域
      else
        GameArea = -1;//否则视为无效点击

      if (SelectionBeforePress == 1)
      {
        if (GameArea >= 11 && GameArea <= 79)//在地图上松开鼠标
        {
          CurrentTick = GetTickCount();//记录当前时间
          CurrentTime = (CurrentTick - StartTick + 500) / 100;
          if (SlotSelected == 22)
          {
            roleLocNum[curAccount] = 1;//记录人物位置
            roleLoc[curAccount][1] = GameArea;//记录人物位置
            recordStage = 2;
            RepaintRecord();//输出录制内容
          }
          else
          {
            if (CurrentTime < 20)
              CurrentTime = 20;//放卡时间至少为第2秒
            Times[SlotSelected]++;
            location[curAccount][SlotSelected][Times[SlotSelected]] = GameArea;
            moment[curAccount][SlotSelected][Times[SlotSelected]] = CurrentTime;
            SaveTrack();
            LoadTrack();
            RecordPlantCard(CurrentTime, GameArea);//记录放卡
            RepaintRecord();//输出录制内容
          }
          SlotSelected = -1;//取消选中
        }
        else if (GameArea >= zero)//在地图外（游戏窗口内）松开鼠标
        {
          if (SlotSelected != 22)
          {
            RecordReleaseCard();//记录取消选卡
            RepaintRecord();//输出录制内容
            SlotSelected = -1;//取消选中
          }
        }
      }
    }
    Sleep(1);
    if (!IsWindow(hWnd))
    {
      MessageBox(hWndEditor, "游戏窗口已关闭，录制结束", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
      return RecoverTrackEdit(rect);//恢复轨道编辑并退出
    }
  }
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
void SetState(int value)
{
  state = value;
  isStateRetained = 1;
}
//抓取地图，返回抓取的句柄
HWND GrabWindow()
{
  SetSystemCursor(crossCursor, (DWORD)IDC_ARROW);//将箭头和十字光标替换
  while (GetAsyncKeyState(VK_LBUTTON))//等待鼠标左键松开
    Sleep(10);
  SetSystemCursor(crossCursor, (DWORD)IDC_ARROW);//光标恢复正常
  return MousePoint();
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
//载入地图图像
int LoadMapImage(IMAGE *pGridImage)
{
  char path[] = "用户参数\\地图.png";
  if (!FileExist(path)) //如果不存在地图文件
    return 0;
  loadimage(pGridImage, path, mapGridWidth * 9, mapGridHeight * 7);
  DWORD *grid = GetImageBuffer(pGridImage);
  for (int i = 0; i < mapGridWidth * 9 * mapGridHeight * 7; i++)//每个点RGB减半
    grid[i] = RGB(GetRValue(grid[i]) / 2, GetGValue(grid[i]) / 2, GetBValue(grid[i]) / 2);
  return 1;
}
//抓取地图，抓取成功返回1，失败返回0
int GrabMap()
{
  HWND hWndGame = NULL;
  HWND hWndPointed = GrabWindow();//抓取游戏窗口句柄
  if (!IsWindow(hWndPointed)) //不是窗口，返回0
    return 0;
  char ClassName[256];
  GetClassName(hWndPointed, ClassName, 256);//获取窗口类名

  if (strcmp(ClassName, "ApolloRuntimeContentWindow") == 0 || strcmp(ClassName, "DUIWindow") == 0)
    hWndGame = GetActiveGameWindow(hWndPointed);
  else if (strcmp(ClassName, "NativeWindowClass") == 0 || strcmp(ClassName, "WebPluginView") == 0)
    hWndGame = hWndPointed;
  else if (strcmp(ClassName, "Chrome_RenderWidgetHostHWND") == 0)
    hWndGame = GetGameWindowFromServer(hWndPointed);
  else //不是游戏窗口则返回0
    return 0;
  WindowToBitmap(hWndGame, "用户参数\\地图.png", 304, 111, 536, 451, 0);//截取地图
  isMapImageCatched = LoadMapImage(&mapImage);//载入地图图像并记录
  return 1;
}
//返回第n个存在的波次，点击“+”返回-1，点击空位返回-2
int getExistedTotalWave(int account, int n)
{
  for (int totalWave = 0; totalWave < maxTotalWave; totalWave++)
    if (totalWaveExist[account][totalWave])
    {
      if (n == 0)
        return totalWave;
      n--;
    }
  if (n == 0)
    return -1;//点击加号
  return -2;//无效点击
}
//波次编辑：左键复制、移动、新增、选择；右键删除
void EditWave(int buttons, int &area, int minArea, int maxArea)
{
  //左键：复制、移动、新增、选择
  if (buttons == 1)
  {
    if (area >= minArea && area <= maxArea)
    {
      int code = area - minArea;//区域编号
      int targetTotalWave = getExistedTotalWave(curAccount, code);//获取点击的波次
      GetAsyncKeyState(VK_CONTROL);
      GetAsyncKeyState(VK_SHIFT);
      if (targetTotalWave == -1)//点击+号：弹出对话框输入波次，进行新建、复制或移动
        EditNewWave(GetAsyncKeyState(VK_CONTROL), GetAsyncKeyState(VK_SHIFT));
      else if (targetTotalWave >= 0) //点击已有波次
      {
        if (GetAsyncKeyState(VK_CONTROL)) //复制波次
        {
          sprintf_s(tracePrint, "复制%d->%d", curTotalWave[curAccount], targetTotalWave);
          CopyWave(curAccount, targetTotalWave);
        }
        else if (GetAsyncKeyState(VK_SHIFT))
        {
          sprintf_s(tracePrint, "移动%d->%d", curTotalWave[curAccount], targetTotalWave);
          MoveWave(curAccount, targetTotalWave);
        }
        else //切换波次
        {
          sprintf_s(tracePrint, "切换到%d", targetTotalWave);
          SelectWave(curAccount, targetTotalWave);
        }
      }
      state = 0;
    }
  }
  //右键：删除波次
  else if (buttons == 2)
  {
    if (area >= minArea && area <= maxArea)//右击波次区：删除波次
    {
      int code = area - minArea;//区域编号
      int targetTotalWave = getExistedTotalWave(curAccount, code);//获取点击的波次
      if (targetTotalWave > 0)//不是第0波才能删
      {
        if (curTotalWave[curAccount] == targetTotalWave)//如果删除的是当前波次，选择第0波
          SelectWave(curAccount, 0);
        DeleteWave(curAccount, targetTotalWave);
      }
      state = 0;
    }
  }
}
int AdjustZoom()
{
  if (Adjust(zoom, 50, 100, 10, 92, 93))//缩放比例
  {
    solidrectangle(0, 0, wndWidth, wndHeight);//清屏
    ratio = originRatio * zoom / 100;//改变窗口尺寸和显示比例，不影响点击位置判定
    setaspectratio(ratio, ratio);//改变显示比例
    imageBackup.Resize((int)(wndWidth * ratio), (int)(wndHeight * ratio));//修改备份图像大小

    RECT originRect, newRect;
    GetWindowRect(hWndEditor, &originRect);
    Resize(NULL, (int)(wndWidth * ratio), (int)(wndHeight * ratio));//改变窗口大小
    GetWindowRect(hWndEditor, &newRect);
    int newWidth = newRect.right - newRect.left;
    int newHeight = newRect.bottom - newRect.top;
    MoveWindow(hWndEditor, originRect.left + originRect.right - newRect.right, originRect.top, newWidth, newHeight, true);
    return 1;
  }
  return 0;
}
//移动人物
void MoveRole(int account, int times, int  row, int column)
{
  roleLoc[account][times] = row * 10 + column;
}
//查找row行column列level层级是否有人物，是则返回1，并填写是哪个账号第几次的人物
int FindRole(int row, int column, int *pAccount = nullptr, int *pTimes = nullptr)
{
  for (int account = 0; account < 2; account++)
    for (int times = 1; times <= roleLocNum[account]; times++)
      if (roleLoc[account][times] == row * 10 + column) //如果某个人物放在row行column列
      {
        if (pAccount)
          *pAccount = account;
        if (pTimes)
          *pTimes = times;
        return RoleLevel(row, column);
      }
  return 0;
}
//删除账号account的第times个人物
void DeleteRole(int account, int times)
{
  if (roleLocNum[account] <= 1) //只有一个人物位置不能删
    return;
  for (int i = times; i < roleLocNum[account]; i++)
    roleLoc[account][i] = roleLoc[account][i + 1];
  roleLocNum[account]--;
}
//账号account添加一个人物
bool AddRole(int account, int row = 0, int column = 0)
{
  if (roleLocNum[account] >= 9)//最多只能添加9个人物
    return false;
  char isGridOccupied[8][10] = { 0 };//地图格子是否被人物占用
  for (int acc = 0; acc < 2; acc++) //填写占用情况
    for (int times = 1; times <= roleLocNum[acc]; times++)
      isGridOccupied[roleLoc[acc][times] / 10][roleLoc[acc][times] % 10] = 1;
  if (row == 0) //如果选择添加到第一个空位置，则计算空位坐标
  {
    for (row = 1; row <= 7; row++)
    {
      for (column = 1; column <= 9; column++)
        if (isGridOccupied[row][column] == 0)
          break;
      if (column <= 9)
        break;
    }
  }
  if (isGridOccupied[row][column] == 1) //本格已有人物则不予添加
    return false;;
  roleLocNum[account]++;
  roleLoc[account][roleLocNum[account]] = row * 10 + column;
  return true;
}
//生成报错文件
void MakeErrorFile()
{
  //填写1P和2P文件名
  for (int account = 0; account < accountNum; account++)
    GetFileName(fullTrackPath[account], trackName[account]);
  //截图阵型和轨道
  char pngName[2][maxPath] = {};
  sprintf_s(pngName[0], "报错信息\\[%s]阵型.png", trackName[0]);
  sprintf_s(pngName[1], "报错信息\\[%s]轨道.png", trackName[0]);
  if (mode == 1)
  {
    saveimage(pngName[0], &imageBackup);
    mode = 2;
    RepaintTrack();
    saveimage(pngName[1], &imageBackup);
    mode = 1;
  }
  else if (mode == 2)
  {
    saveimage(pngName[1], &imageBackup);
    mode = 1;
    RepaintMap();
    saveimage(pngName[0], &imageBackup);
    mode = 2;
  }
  //复制1P和2P轨道文件
  char trackPath[maxPath] = {};
  for (int account = 0; account < accountNum; account++)
  {
    sprintf_s(trackPath, "报错信息\\%s", trackName[account]);
    Copy(fullTrackPath[account], trackPath);
  }
}
//向卡槽添加一张名为"卡片i"的level层级卡，不能与已携带的卡重名，添加成功返回true
bool AddNewToSlot(int account, int level)
{
  //卡槽已满或背包已满则不允许添加
  if (slotNum[account] >= maxSlotNum || bagNum[account] >= maxCommonNum)
    return false;

  //记录所有存在的"卡片i"
  bool isCodeUsed[maxSlotNum + 1] = {};//"卡片i"是否已存在
  int code = 0;
  for (int order = 1; order <= slotNum[account]; order++)
    if (sscanf_s(slot[account][order].name, "卡片%d", &code) == 1 && code >= 1 && code <= 21)
      isCodeUsed[code] = true;

  //记录要添加的卡片名称
  char cardName[10] = "卡片0";
  for (code = 1; code <= maxSlotNum; code++)
    if (!isCodeUsed[code])
    {
      sprintf_s(cardName, "卡片%d", code);
      break;
    }

  //创建一张名为"卡片i"的common卡
  int &num = bagNum[account];
  SlotType &common = bag[account][num];
  memset(&common, 0, sizeof(common));
  strcpy_s(common.name, cardName);
  common.cd = 7;
  common.level = level;
  AddToSlot_File(account, num);
  return true;
}
//去掉轨道名称结尾的"+"。成功返回true，失败返回false
bool RemovePlus(const char *path, char(&newPath)[maxPath])
{
  int length = strlen(path);
  if (length < 5) //长度小于5返回false
    return false;
  const char *plus = path + length - 5;//'+'的位置
  if (strcmp(plus, "+.txt") != 0) //结尾不是"+.txt"返回false
    return false;
  strcpy_s(newPath, path);
  newPath[length - 5] = 0;
  strcat_s(newPath, ".txt");
  return true;
}
//在轨道名称结尾添加"+"。成功返回true，失败返回false
bool AddPlusInTrack(const char *path, char(&newPath)[maxPath])
{
  int length = strlen(path);
  if (length < 4) //长度小于4返回false
    return false;
  const char *dot = path + length - 4;//'.'的位置
  if (strcmp(dot, ".txt") != 0) //结尾不是".txt"返回false
    return false;
  strcpy_s(newPath, path);
  newPath[length - 4] = 0;
  strcat_s(newPath, "+.txt");
  return true;
}
//编辑器类型（用于多开管理）
struct EditorType
{
  HWND hWnd;
  int accountNum;
  char trackPath[2][maxPath];
};
int editorNum;//已打开的编辑器数量
const int maxEditorNum = 50;//最大编辑器数量
EditorType editor[maxEditorNum];//编辑器信息
//读取当前打开的编辑器信息
void LoadEditor()
{
  memset(editor, 0, sizeof(editor));
  editorNum = GetFileList("自动备份\\轨道文件\\*", backupList, maxBackupNum);
  FILE *f;
  for (int order = 0; order < editorNum; order++)
  {
    char openingPath[maxPath];//"当前打开轨道.txt"
    sprintf_s(openingPath, "自动备份\\轨道文件\\%s\\当前打开轨道.txt", backupList[order]);
    if (fopen_s(&f, openingPath, "r") == 0)
    {
      fscanf_s(f, "编辑器句柄=%d\n", &editor[order].hWnd);
      fscanf_s(f, "轨道数量=%d\n", &editor[order].accountNum);
      for (int account = 0; account < editor[order].accountNum; account++)
        fgets(f, editor[order].trackPath[account]);
      fclose(f);
    }
  }
}
//轨道文件path是否已打开
bool isTrackOpened(const char *path)
{
  LoadEditor();//检查已打开的编辑器
  for (int order = 0; order < editorNum; order++)
    for (int account = 0; account < editor[order].accountNum; account++)
      if (strcmp(path, editor[order].trackPath[account]) == 0)
      {
        MessageBox(hWndEditor, "该轨道文件已在编辑器中打开。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        return true;
      }
  return false;
}
//根据选项(取消/1P/2P)打开path轨道
void OpenTrackWithChoice(const char *path, int choice)
{
  if (choice == IDCANCEL)
    return;
  else if (choice == ID_1P)
    curAccount = 0;
  else if (choice == ID_2P)
  {
    accountNum = 2;
    curAccount = 1;
  }
  char info[1000];
  //尝试读取全波轨道
  if (LoadTrackFrom(curAccount, path, info, true) != zero)//如果不能正常打开
  {
    LoadBackup(0);//用当前备份恢复轨道
    MessageBox(hWndEditor, info, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return;
  }
  //如果可以正常打开
  strcpy_s(fullTrackPath[curAccount], path);
  curTotalWave[curAccount] = 0;//当前波数设为0
  LoadTrackFrom(curAccount, fullTrackPath[curAccount]);//读取第0波轨道
  SwitchToAccount(curAccount);
  lastClickMessage = 100000000;//传入点击左上角的消息
}
//打开轨道文件path
void OpenTrack(const char *path)
{
  int fileType = CheckFileType(path);
  if (fileType != 1)
  {
    PopMessage(hWndEditor, "只能打开轨道文件。");
    return;
  }
  //如果待打开文件已打开，则拒绝打开
  if (isTrackOpened(path))
    return;
  //通过对话框选择作为1P轨道或2P轨道
  OpenTrackWithChoice(path, DragDialog(path));
}
//添加2P轨道
void OpenTrack2P(const char *path)
{
  int fileType = CheckFileType(path);
  if (fileType != 1)
  {
    PopMessage(hWndEditor, "只能打开轨道文件。");
    return;
  }
  //如果待打开文件已打开，则拒绝打开
  if (isTrackOpened(path))
    return;
  //打开2P轨道
  OpenTrackWithChoice(path, ID_2P);
}
//模块一：编辑阵型
void EditMap()
{
  int xPos, yPos, buttons;//鼠标按下
  RepaintMap();
  while (1)
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
        if (area == zero)
          state = 0;
        if (area == 2)//切换到轨道编辑模式
        {
          mode = 2;
          state = 0;
          return;
        }
        if (area == 3)
          MakeErrorFile();
        if (area == 10)//10=防御卡背包，11=生产信息编辑
        {
          bagMode = -1;
          state = 0;
        }
        if (area == 11)//10=防御卡背包，11=生产信息编辑
        {
          if (bagMode >= zero)//如果处于卡片编辑状态，执行删除卡片
          {
            DeleteCommonCard();
            SaveCommonCard();
            bagMode = -1;
          }
          else
            bagMode = -2;
          state = 0;
        }
        if (area == 20 || area == 21)//切换账号
        {
          int account = area - 20;
          if (curAccount != account)
            SwitchToAccount(account);
          state = 0;
        }
        if (area == 30 || area == 31)//复制轨道路径到剪贴板
        {
          int account = area - 30;
          char message[100];
          sprintf_s(message, "%dP轨道路径复制成功！", account + 1);
          GetShortPath(fullTrackPath[account], shortTrackPath[account]);
          PrintToClipboard(shortTrackPath[account]);
          MessageBox(hWndEditor, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
        }
        if (area == 40) //添加2P轨道
        {
          char trackPath2P[maxPath];
          AddPlusInTrack(fullTrackPath[0], trackPath2P);
          //如果trackPath2P（Track+.txt)不存在，生成初始轨道
          if (!FileExist(trackPath2P))
            OutputRes("初始轨道", "TEXT", trackPath2P);
          OpenTrack2P(trackPath2P);
          state = 0;
        }
        if (area == 50) //移除2P轨道（保留2P人物）
        {
          accountNum = 1;
          curAccount = 0;
          state = 0;
        }
        if (area == 60) //编辑注释
          EditNotation();
        if (area == 70) //切换刷技能模式
          isSkillMode = !isSkillMode;

        if (area / 100 == 2)//点击常用卡组：添加到卡槽
        {
          if (area == 226)//新增卡片
          {
            if (bagNum[curAccount] < maxCommonNum)
            {
              AddCommonCard();
              SaveCommonCard();
              bagMode = bagNum[curAccount] - 1;
            }
          }
          else if (area == 227) //向前翻页
          {
            if (commonPage > 1)
              commonPage--;
          }
          else if (area == 228) //向后翻页
          {
            if (commonPage < (bagNum[curAccount] - 1) / 26 + 1)
              commonPage++;
          }
          else //选中防御卡
          {
            int common = (commonPage - 1) * 26 + area - 200;
            if (common < bagNum[curAccount])
              AddToSlot_File(curAccount, common);
          }
          state = 0;
        }
        if (area / 100 == 4)//点击卡片右上角：补阵模式
        {
          slot[curAccount][area - 400].repair = !slot[curAccount][area - 400].repair;
          state = 0;
        }
        if (area / 100 == 5)//点击卡片右上角：索敌模式
        {
          slot[curAccount][area - 500].search = !slot[curAccount][area - 500].search;
          state = 0;
        }
        EditWave(buttons, area, 600, 613);

        if (area == 615) //抓取地图
          GrabMap();
        if (area == 616)
        {
          isMapImageCatched = 0;
          if (FileExist("用户参数\\地图.png"))
            remove("用户参数\\地图.png");
        }
        if (area == 90)//撤销按钮
          if (IsUndoAllowed())
            Undo();
        if (area == 91)//恢复按钮
          if (IsRepeatAllowed())
            Repeat();

        if (AdjustZoom())
          SaveParameter();

        if (area / 100 == 8)//点击堆叠区：显示深度切换到点击的深度
        {
          if (state / 1000 == 1)
          {
            int level0 = state % 1000 / 100;//记录选中的行列和层级
            int row0 = state % 100 / 10;
            int column0 = state % 10;
            if (area <= 808)
              map[curAccount][row0][column0].depthSelected[level0] = min(area - 800, map[curAccount][row0][column0].depth[level0]);
            else
              SelectDepth(row0, column0, level0, area - 809);
          }
          else if (area >= 809 || area <= 811)//“底”“顶”“空”深度调整
          {
            if (state / 100 == 1) //选中卡槽
              SelectDepth(slot[curAccount][state - 100].level, area - 809);
            else //无选中
              SelectDepth(area - 809);
          }
        }

        if (area / 100 == 20) //生产信息上调
        {
          int i = area - 2000;
          switch (i)
          {
          case 0:
            if (initialEnergy < 50)
              initialEnergy = 50;
            else if (initialEnergy < 500)
              initialEnergy = 500;
            else if (initialEnergy < 2000)
              initialEnergy = 2000;
            else if (initialEnergy < 3000)
              initialEnergy = 3000;
            break;
          case 1:
            if (InnateFire < 30)
              InnateFire++;
            break;
          case 2:
            if (InnateCup < 30)
              InnateCup++;
            break;
          case 3:
            isDay = 1 - isDay;
            break;
          case 4:case 5:case 6:case 7:case 8:case 9:case 10:
            if (weapon[curAccount][i - 4] < 15)
              weapon[curAccount][i - 4]++;
            break;
          case 11:
            weapon[curAccount][7] = 3 - weapon[curAccount][7];
          }
          SaveProduction();
        }
        if (area / 100 == 21) //生产信息下调
        {
          int i = area - 2100;
          switch (i)
          {
          case 0:
            if (initialEnergy > 3000)
              initialEnergy = 3000;
            else if (initialEnergy > 2000)
              initialEnergy = 2000;
            else if (initialEnergy > 500)
              initialEnergy = 500;
            else if (initialEnergy > 50)
              initialEnergy = 50;
            else
              initialEnergy = 0;
            break;
          case 1:
            if (InnateFire > 0)
              InnateFire--;
            break;
          case 2:
            if (InnateCup > 0)
              InnateCup--;
            break;
          case 3:
            isDay = 1 - isDay;
            break;
          case 4:case 5:case 6:case 7:case 8:case 9:case 10:
            if (weapon[curAccount][i - 4] > -1)
              weapon[curAccount][i - 4]--;
            break;
          case 11:
            weapon[curAccount][7] = 3 - weapon[curAccount][7];
          }
          SaveProduction();
        }
        if (area / 100 == 22) //防御卡数值上调1
        {
          int i = area - 2200;
          switch (i)
          {
          case 0:
            EditCardName();
            break;
          case 1:
            bag[curAccount][bagMode].cd = min(320, bag[curAccount][bagMode].cd + 1);
            break;
          case 2:
            if (bag[curAccount][bagMode].level < 5)
              bag[curAccount][bagMode].level++;
            break;
          case 3:
            if (bag[curAccount][bagMode].cost % 1000 < 999)
              bag[curAccount][bagMode].cost++;
            break;
          case 4://生产特性
            if (bag[curAccount][bagMode].style < 5)
              bag[curAccount][bagMode].style++;
            break;
          case 5://星级
            if (bag[curAccount][bagMode].style >= 1 && bag[curAccount][bagMode].style <= 6)//如果是生产卡或炎焱兔
              if (bag[curAccount][bagMode].star < 16)
                bag[curAccount][bagMode].star++;
            break;
          case 6://单朵产量
            if (bag[curAccount][bagMode].style >= 1 && bag[curAccount][bagMode].style <= 4)//如果是持续生产卡
              bag[curAccount][bagMode].sun++;//增加单朵火苗
            else if (bag[curAccount][bagMode].style == 5 || bag[curAccount][bagMode].style == 6)//炎焱兔/花火龙，增加转职形态
              if (bag[curAccount][bagMode].transfer < 2)
                bag[curAccount][bagMode].transfer++;
            break;
          case 7://火苗朵数
            if (bag[curAccount][bagMode].style >= 1 && bag[curAccount][bagMode].style <= 5)//如果是生产卡
              if (bag[curAccount][bagMode].sunNum < 8)
                bag[curAccount][bagMode].sunNum++;
            break;
          }
          SaveCommonCard();
        }
        if (area / 100 == 24) //防御卡数值上调10
        {
          int i = area - 2400;
          switch (i)
          {
          case 0:
            EditCardName();
            break;
          case 1:
            bag[curAccount][bagMode].cd = min(320, bag[curAccount][bagMode].cd + 10);
            break;
          case 3:
            if (bag[curAccount][bagMode].cost % 1000 < 975)
              bag[curAccount][bagMode].cost += 25;
            else
              bag[curAccount][bagMode].cost += 999 - bag[curAccount][bagMode].cost % 1000;
            break;
          case 6:
            if (bag[curAccount][bagMode].style >= 1 && bag[curAccount][bagMode].style <= 4)//如果是持续生产卡
              bag[curAccount][bagMode].sun += 10;
            break;
          }
          SaveCommonCard();
        }
        if (area / 100 == 23) //防御卡数值下调1
        {
          int i = area - 2300;
          switch (i)
          {
          case 0:
            EditCardName();
            break;
          case 1:
            if (bag[curAccount][bagMode].cd > 2)
              bag[curAccount][bagMode].cd--;
            break;
          case 2:
            if (bag[curAccount][bagMode].level > 1 && bag[curAccount][bagMode].level <= 5)
              bag[curAccount][bagMode].level--;
            break;
          case 3:
            if (bag[curAccount][bagMode].cost % 1000 > zero)
              bag[curAccount][bagMode].cost--;
            break;
          case 4:
            if (bag[curAccount][bagMode].style >= 1 && bag[curAccount][bagMode].style <= 5)//如果是生产卡
              bag[curAccount][bagMode].style--;
            break;
          case 5:
            if (bag[curAccount][bagMode].style >= 1 && bag[curAccount][bagMode].style <= 6)//如果是生产卡或炎焱兔
              if (bag[curAccount][bagMode].star > zero)
                bag[curAccount][bagMode].star--;
            break;
          case 6:
            if (bag[curAccount][bagMode].style >= 1 && bag[curAccount][bagMode].style <= 4)//如果是持续生产卡
            {
              if (bag[curAccount][bagMode].sun > zero)
                bag[curAccount][bagMode].sun--;
            }
            else if (bag[curAccount][bagMode].style == 5 || bag[curAccount][bagMode].style == 6)//炎焱兔/花火龙，降低转职形态
              if (bag[curAccount][bagMode].transfer > zero)
                bag[curAccount][bagMode].transfer--;
            break;
          case 7:
            if (bag[curAccount][bagMode].style >= 1 && bag[curAccount][bagMode].style <= 5)//如果是生产卡
              if (bag[curAccount][bagMode].sunNum > zero) bag[curAccount][bagMode].sunNum--;
            break;
          }
          SaveCommonCard();
        }
        if (area / 100 == 25) //防御卡数值下调10
        {
          int i = area - 2500;
          switch (i)
          {
          case 0:
            EditCardName();
            break;
          case 1:
            if (bag[curAccount][bagMode].cd > 12)
              bag[curAccount][bagMode].cd -= 10;
            else
              bag[curAccount][bagMode].cd = 2;
            break;
          case 3:
            if (bag[curAccount][bagMode].cost % 1000 > 25)
              bag[curAccount][bagMode].cost -= 25;
            else
              bag[curAccount][bagMode].cost -= bag[curAccount][bagMode].cost % 1000;
            break;
          case 6:
            if (bag[curAccount][bagMode].style >= 1 && bag[curAccount][bagMode].style <= 4)//如果是持续生产卡
            {
              if (bag[curAccount][bagMode].sun > 10) bag[curAccount][bagMode].sun -= 10;
              else bag[curAccount][bagMode].sun = 0;
            }
            break;
          }
          SaveCommonCard();
        }
        if (area / 100 == 26) //防御卡数值编辑
        {
          int i = area - 2600;
          switch (i)
          {
          case 0:
            EditCardName();
            break;
          case 3:
            if (bag[curAccount][bagMode].cost >= 1000)
              bag[curAccount][bagMode].cost -= 1000;
            else
              bag[curAccount][bagMode].cost += 1000;
            break;
          }
          SaveCommonCard();
        }

        if (area / 100 == 1 || area == 300 || area == 310)//点击卡槽/人物槽：选中卡片/人物
        {
          if (area == 310 && GetRoleNum() == 1) //单人模式下点击人物2位置"+"，必然是增加人物
          {
            AddRole(1);//添加一个2P位置
            state = 0;
          }
          else
          {
            if (area == state)//已经选中了本卡槽/人物槽：取消选中
              state = 0;
            else //否则选中点击位置
              state = area;
          }
        }

        //点击地图
        if (area / 1000 == 1)
        {
          int row = area % 100 / 10;
          int column = area % 10;
          int level = area % 1000 / 100;//记录点击的行、列、层级

          GetAsyncKeyState(VK_CONTROL);
          //按住Ctrl：无论状态是什么，点击空格则增加1个卡槽
          if (GetAsyncKeyState(VK_CONTROL))
          {
            //如果普通层无卡，增加普通卡
            if (map[curAccount][row][column].depth[3] == 0)
            {
              if (AddNewToSlot(curAccount, 3))//向卡槽添加普通层"卡片i"
                AddToGrid(curAccount, slotNum[curAccount], row, column);//放置在本格
            }
            else if (map[curAccount][row][column].depth[4] == 0)
            {
              if (AddNewToSlot(curAccount, 4))//向卡槽添加升级层"卡片i"
                AddToGrid(curAccount, slotNum[curAccount], row, column);//放置在本格
            }
            state = 0;
          }
          else //如果没有按住Ctrl
          {
            if (state == zero)//无选中状态：选中卡片/人物
            {
              if (map[curAccount][row][column].depth[level] > zero)
                state = area; //该层级有卡片：选中卡片
              else
              {
                int account, times;
                //如果点的是1~4层级，且该层级有人物
                if (level >= 1 && level <= 4 && level == FindRole(row, column, &account, &times))
                  state = 300 + 10 * account + times;
              }
            }
            else if (state / 100 == 1)//已选中卡槽：点击空格则放置卡片，否则选中所点击的层级
            {
              int order = state - 100;
              int level0 = slot[curAccount][order].level;//记录选中卡片的层级
              if (order == GetOrder(curAccount, "冰沙"))
                MessageBox(hWndEditor, "冰沙无需设置放卡位置。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
              else if (map[curAccount][row][column].depthSelected[level0] == map[curAccount][row][column].depth[level0])
              {
                //如果目标层级选中了空深度
                if (curTotalWave[curAccount] > 0 && slot[curAccount][order].retain)
                  MessageBox(hWndEditor, "启用继承的卡无法放置。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
                else
                  AddToGrid(curAccount, order, area % 100 / 10, area % 10);
              }
              else if (map[curAccount][row][column].depth[level] > zero)
                state = 1000 + level * 100 + row * 10 + column;//否则改为选中点击的层级
            }
            else if (state / 100 == 3) //选中人物槽或地图中的人物(300-319)
            {
              int account = (state - 300) / 10;
              int times = state % 10;
              if (times == 0) //选中人物槽：添加人物到本格
                AddRole(account, row, column);
              else //选中地图人物：若本格没有人物，移动人物到本格；否则改选本格物件
              {
                if (FindRole(row, column) == 0)
                  MoveRole(account, times, row, column);
                state = 0;
              }
            }
            else if (state / 1000 == 1) //已选中地图层级：移动到另一个层级
            {
              int level0 = state % 1000 / 100;//记录选中的行列和层级
              int row0 = state % 100 / 10;
              int column0 = state % 10;
              if (state != area)//如果点的是其他格子，或本格的其他层级
              {
                if (map[curAccount][row0][column0].depthSelected[level0] < map[curAccount][row0][column0].depth[level0]//选中层级选中的是有卡深度
                  && map[curAccount][row][column].depthSelected[level0] == map[curAccount][row][column].depth[level0])//且目标格同层级选中的是空深度
                {
                  MoveLevel(curAccount, row0, column0, row, column, level0);//移动层级
                  state = 0;
                }
                else if (map[curAccount][row][column].depth[level] > zero)//如果不能执行移动，且点的位置有卡
                  state = 1000 + level * 100 + row * 10 + column;//选中点击的层级
                else //如果点的位置没有卡
                  state = 0;//取消选中
              }
              else//如果点的是已选中的层级
                state = 0;//取消选中
            }
          }
        }
      }
      else if (buttons == 2)//如果按下的是右键
      {
        if (area == 1)//清空阵型
          ClearMap(curAccount);
        if (area == 2)//清空轨道
          ClearTrack(curAccount);
        if (area == 300 || area == 310)//右击人物槽：删除人物或清空轨道
        {
          if (area == 310 && accountNum == 1) //单轨模式下右击2P，删除2P
            roleLocNum[1] = 0;
          else
          {
            char Message[100];
            int account = (area - 300) / 10;
            sprintf_s(Message, "确定要清空%dP地图和轨道（保留卡槽）？", account + 1);
            int DeleteMessage = MessageBox(hWndEditor, Message,
              "提示", MB_OKCANCEL | MB_ICONINFORMATION | MB_SYSTEMMODAL);
            if (DeleteMessage == IDOK)
              ClearMapAndTrack(account);
          }
          state = 0;
        }
        EditWave(buttons, area, 600, 613);

        if (area / 100 == 1) //右击卡槽
        {
          if (state / 100 == 1) //选中卡槽状态
          {
            //右击其他卡槽，且两个卡槽都不是铲子，则交换卡槽位置
            if (area != state && area != 100 && state != 100)
            {
              GetAsyncKeyState(VK_SHIFT);
              if (GetAsyncKeyState(VK_SHIFT))
                InsertSlot_File(curAccount, state - 100, area - 100);
              else
                ExchangeSlot_File(curAccount, state - 100, area - 100);
            }
            //右击本卡槽，清空阵型
            if (area == state)
              DeleteMap(curAccount, state - 100);
          }
          else //其他状态
          {
            if (area - 100 == 0) //右击铲子：清空阵型轨道
              DeleteShovel(curAccount);
            else //右击其他卡片：删除卡槽
              DeleteFromSlot_File(curAccount, area - 100);
          }
          state = 0;
        }

        if (area / 100 == 2 && area <= 225)//右击背包卡片
        {
          int common = (commonPage - 1) * 26 + area - 200;
          if (state / 100 == 1) //选中卡槽：用背包卡替换卡槽
          {
            if (common < bagNum[curAccount] && state != 100)
              ChangeSlot_File(curAccount, state - 100, common);
          }
          else //其他状态：进入防御卡编辑状态
          {
            if (common < bagNum[curAccount])
              bagMode = common;
          }
          state = 0;
        }

        if (area / 1000 == 1)//右击地图
        {
          int row = area % 100 / 10;
          int column = area % 10;
          int level = area % 1000 / 100;
          if (state / 1000 == 1) //选中地图卡片：交换次序
          {
            int level0 = state % 1000 / 100;
            int row0 = state % 100 / 10;
            int column0 = state % 10;
            int depth0 = map[curAccount][row0][column0].depthSelected[level0];//选中层级的行列和选中深度
            int depth = map[curAccount][row][column].depthSelected[level0];//目标层级的行列和选中深度
            if (row0 != row || column0 != column)//如果右击的是另一个格子
            {
              if (depth0 < map[curAccount][row0][column0].depth[level0] && depth < map[curAccount][row][column].depth[level0])//两格选中深度均小于空深度
                if (strcmp(map[curAccount][row0][column0].name[level0][depth0], map[curAccount][row][column].name[level0][depth]) == zero) //且两格同层级选中深度为同名卡
                  InsertLevel(curAccount, row0, column0, row, column, level0);//执行插队
              state = 0;
            }
            else//右击的是本格
            {
              if (level == level0)//右击的是本层级，执行删除，如果删空了就取消选中状态
              {
                DeleteFromGridLevel(curAccount, row, column, level);
                if (map[curAccount][row][column].depth[level] == zero)
                  state = 0;
              }
              else
                state = 0;//否则取消选中状态
            }
          }
          else //其他状态：右击人物则删除人物，右击放卡则删除放卡
          {
            int account, times;
            //右击人物
            if (level >= 1 && level <= 4 && level == FindRole(row, column, &account, &times))
            {
              if (state / 100 == 3 && state % 10 > 0) //如果选中了某个人物位置
              {
                int times0 = state % 10;//选中的人物位置序数
                int account0 = (state - 300) / 10;//选中的人物所属账号
                if (account0 == account) //如果选中的人物与右击的人物账号一致，执行插入
                  InsertRole(account, times0, times);
                state = 0;
              }
              else
              {
                DeleteRole(account, times);
                if (state / 100 == 3 && state % 10 > 0) //如果选中了地图人物，则取消选中
                  state = 0;
              }
            }
            else //删除放卡
              DeleteFromGridLevel(curAccount, area % 100 / 10, area % 10, area % 1000 / 100);
          }
        }
      }
      SaveTrack();
      LoadTrack();
      RepaintMap();
    }
    else //没有鼠标按下，就重绘提示
      RepaintTip();
    sleepTime++;
    if (sleepTime % 10 == zero)
      Sleep(1);
    if (sleepTime >= 100)
    {
      RepaintOld();
      sleepTime = 0;
    }
  }
}
//所有账号的轨道是否全空
int isTrackEmpty()
{
  for (int account = 0; account < accountNum; account++)
    for (int order = 0; order <= slotNum[account]; order++)
      if (moment[account][order][1] > 0)
        return false;
  return true;
}
void EditTrack()//模块二：编辑轨道
{
  int xPos, yPos, buttons;//鼠标按下

  RepaintTrack();
  while (true)
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
        if (area == zero || area == 1)
          state = queueSelected = trackSlotSelected = 0;
        if (area == 1) //点击阵型编辑按钮：切换到阵型编辑模式
        {
          mode = 1;
          return;
        }
        if (area == 3)
          MakeErrorFile();
        if (area == 20 || area == 21)//切换账号
        {
          if (accountNum == 2)
          {
            int account = area - 20;
            if (account != curAccount)
              SwitchToAccount(account);
          }
        }
        //添加2P轨道
        if (area == 40)
        {
          char trackPath2P[maxPath];
          AddPlusInTrack(fullTrackPath[0], trackPath2P);
          //如果trackPath2P（Track+.txt)不存在，生成初始轨道
          if (!FileExist(trackPath2P))
            OutputRes("初始轨道", "TEXT", trackPath2P);
          OpenTrack2P(trackPath2P);
          state = 0;
        }
        //移除2P轨道（保留2P人物位置）
        if (area == 50)
        {
          accountNum = 1;
          curAccount = 0;
          state = 0;
        }
        if (area / 100 == 1) //点击卡槽：选中卡槽
        {
          if (trackSlotSelected == area) //已选中本卡槽：取消选中
          {
            if (GetTickCount() - trackSlotSelectionTick < 300) //双击
            {
              int order = trackSlotSelected - 100;
              AutoTrack(curAccount, order, GetLocTimes(curAccount, order) - GetMomentTimes(curAccount, order, maxTime));
            }
            trackSlotSelectionTick = 0;
          }
          else //未选中本卡槽
          {
            //如果选中了队列标题，加入队列
            if (queueSelected == 712)
            {
              isQueueSelectionRetained = true;
              AddQueue(curAccount, area - 100);
            }
            else //否则选中点击的卡槽
            {
              isTrackSlotSelectionRetained = true;
              trackSlotSelected = area;
              trackSlotSelectionTick = GetTickCount();
            }
          }
        }
        if (area / 100 == 2) //点击卡槽左上角：继承模式
        {
          int order = area - 200;
          if (slot[curAccount][order].retain)
            slot[curAccount][order].retain = false;
          else
          {
            slot[curAccount][order].retain = true;
            if (curTotalWave[curAccount] == 0)
              DeleteTrack(curAccount, order);//清空轨道
            else
              DeleteMapAndTrack(curAccount, order);//清空阵型和轨道
          }
        }
        if (area / 100 == 3) //点击卡槽左下角：独立计时
        {
          int order = area - 300;
          slot[curAccount][order].independent = !slot[curAccount][order].independent;
        }
        if (area / 100 == 4) //点击卡槽右上角：绝对优先
        {
          int order = area - 400;
          slot[curAccount][order].prior = !slot[curAccount][order].prior;
        }
        if (area / 100 == 5) //点击卡槽右下角：极限成阵（按住Ctrl编辑条件）
        {
          int order = area - 500;
          GetAsyncKeyState(VK_CONTROL);
          if (GetAsyncKeyState(VK_CONTROL)) //按住Ctrl点极限成阵：编辑条件
          {
            EditCondition(curAccount, order);
            slot[curAccount][order].limit = true;
            if (moment[curAccount][order][1] == 0 && (slot[curAccount][order].endNum > 0 || slot[curAccount][order].endTime > 0))
              AddTrack(curAccount, order, 1); //无轨道时设置了终止条件，则新增一个轨道
          }
          else
          {
            if (!slot[curAccount][order].limit)
              slot[curAccount][order].limit = true;
            else
              slot[curAccount][order].limit = false;
          }
        }
        if (area == 612)//录制轨道按钮
        {
          int RecordMessage = MessageBox(hWndEditor,
            "点击确定后，将鼠标指向选卡界面，\n程序将自动进入关卡并开始录制。\n移动地图请在2秒内放人，以免错位。",
            "录制轨道", MB_OKCANCEL | MB_ICONINFORMATION | MB_SYSTEMMODAL);
          if (RecordMessage == IDOK)
          {
            //清空地图、轨道，保留卡组
            memset(location[curAccount], 0, sizeof(location[curAccount]));
            memset(moment[curAccount], 0, sizeof(moment[curAccount]));
            memset(map[curAccount], 0, sizeof(map[curAccount]));
            memset(track[curAccount], 0, sizeof(track[curAccount]));
            //清空人物位置，再添加1个人物位置
            roleLocNum[curAccount] = 0;
            AddRole(curAccount);

            SaveTrack();
            mode = 3;//切换到录制模式
            return;//退出轨道编辑
          }
          state = 0;
        }
        EditWave(buttons, area, 600, 611);

        if (area == 613)//自动布轨按钮
        {
          GetAsyncKeyState(VK_CONTROL);
          if (GetAsyncKeyState(VK_CONTROL)) //按住Ctrl点自动布轨：约束自动布轨
          {
            /*
            if (EditAutoTrack() == IDOK)
              ConstrainedAutoTrack();
              */
          }
          else
          {
            if (trackSlotSelected == zero)//全局布轨
              AutoTrack();
            else
            {
              AutoTrack(curAccount, trackSlotSelected - 100, 1);//选中了卡槽，就布轨一个
              isTrackSlotSelectionRetained = true;
            }
          }
          state = 0;
        }
        if (area == 614)//魔塔
          InputNum(&tower[curAccount], 0, 165,
            "输入要刷的魔塔层数（1~165）。\n输入0表示普通关卡。", "魔塔选项");
        if (area == 615)//退出时间
          InputNum(&quitTime[curAccount], 0, 960,
            "到达指定时间（1~960秒）后主动退出关卡，仅高级任务有效。\n输入0表示不主动退出。", "退出时间");

        if (area == 90)//撤销按钮
          if (IsUndoAllowed())
            Undo();
        if (area == 91)//恢复按钮
          if (IsRepeatAllowed())
            Repeat();

        if (AdjustZoom()) //92-93：缩放
          SaveParameter();

        //单击队列标题/队列卡：切换或取消选中
        if (area / 100 == 7)
        {
          if (queueSelected != area)
          {
            if (area == 712 || area - 700 < queueSize[curAccount])
            {
              isQueueSelectionRetained = true;
              queueSelected = area;
            }
          }
        }

        if (area / 100 == 8) //点击翻页按钮：切换页数
        {
          isTrackSlotSelectionRetained = true;
          if (area - 800 <= 16)
            page = area - 800;
        }
        if (area / 100 == 9) //点击方向键
        {
          if (trackSlotSelected > zero)//如果选中了卡片
          {
            isTrackSlotSelectionRetained = true;
            int order = trackSlotSelected - 100;
            if (area == 900)//左右：移动轨道
              MoveTrack(curAccount, order, -1);
            else if (area == 901)
              MoveTrack(curAccount, order, 1);
            else if (area == 902)//上下：调节冷却
            {
              if (order == maxSlotNum + 1)
                slot[curAccount][order].cd = min(320, slot[curAccount][order].cd + 5);
              else
                slot[curAccount][order].cd = min(320, slot[curAccount][order].cd + 1);
            }
            else if (area == 903)
            {
              if (order == maxSlotNum + 1)
                slot[curAccount][order].cd = max(60, slot[curAccount][order].cd - 5);
              else if ((order == zero && slot[curAccount][0].cd > zero) || slot[curAccount][order].cd > 2)
                slot[curAccount][order].cd--;
            }
          }
          else//如果没有选中卡片
          {
            if (area == 900)//左右：翻页
            {
              if (page > 1)
                page--;
            }
            else if (area == 901)
            {
              if (page < 16)
                page++;
            }
            else if (area == 902)//上下：上下滚动显示的内容
            {
              if (firstTrackSlot > -1)
                firstTrackSlot--;
            }
            else if (area == 903)
            {
              if (firstTrackSlot + trackSlotNumPerPage < slotNum[curAccount])
                firstTrackSlot++;
            }
          }
        }
        if (area / 1000 == 2)//点击时间轴：显示当前时间的武器产火和卡片产火
          timeSelected = area - 2000 + (page - 1) * 60;
        if (area / 100000 == 1)//点击轨道：设置轨道或延长轨道
        {
          int order = (area - 100000) / 1000;
          int t = area % 1000 + (page - 1) * 60;
          if (t != (page - 1) * 60)//点击每页非第0秒，添加轨道。
          {
            if (slot[curAccount][order].retain == 0) //没开启继承才能添加轨道
              AddTrack(curAccount, order, t);
            else
            {
              if (curTotalWave[curAccount] == 0)
                PopMessage(hWndEditor, "使用默认轨道的卡无法添加轨道。");
              else
                PopMessage(hWndEditor, "启用继承的卡无法添加轨道。");
            }
          }
          else if (order > zero) //点击第0秒，自动补满所有轨道
          {
            if (slot[curAccount][order].retain == 0) //没开启继承才能添加轨道
            {
              int MaxTrack = GetMaxTrack(curAccount, order);//获得order的最大轨道时间
              if (MaxTrack == zero)
              {
                AddTrack(curAccount, order, 1);
                MaxTrack = 1;
              }
              for (t = MaxTrack + slot[curAccount][order].cd; t <= maxTime; t = t + slot[curAccount][order].cd)
                AddTrack(curAccount, order, t);
            }
            else
              MessageBox(hWndEditor, "启用继承的卡无法添加轨道。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
          }
        }
      }
      else if (buttons == 2)//如果按下的是右键
      {
        if (area == 1)//清空阵型
          ClearMap(curAccount);
        if (area == 2)//清空轨道
          ClearTrack(curAccount);
        if (area / 100 == 1) //右击卡槽：删除该卡的全部轨道
        {
          if (trackSlotSelected == zero)
          {
            int order = area - 100;
            DeleteTrack(curAccount, order);
          }
        }

        EditWave(buttons, area, 600, 611);

        //右击队列标题/队列卡：切换或取消选中
        if (area / 100 == 7)
        {
          if (area == 712) //右击标题
            queueSize[curAccount] = 0;
          else if (area < 700 + queueSize[curAccount]) //右击队列卡
          {
            //如果已经选中一张队列卡，执行插入
            if (queueSelected >= 700 && queueSelected < 700 + queueSize[curAccount])
              InsertQueue(curAccount, queueSelected - 700, area - 700);
            else //没有选中队列卡，则删除右击的队列卡
              DeleteQueue(curAccount, area - 700);
          }
        }

        if (area / 100000 == 1)//右键点击轨道：删除轨道
        {
          int order = (area - 100000) / 1000;
          int t = area % 1000 + (page - 1) * 60;
          DeleteTrack(curAccount, order, t);
        }
      }

      if (!isTrackSlotSelectionRetained)
        trackSlotSelected = 0;
      isTrackSlotSelectionRetained = false;

      if (!isQueueSelectionRetained)
        queueSelected = 0;
      isQueueSelectionRetained = false;


      SaveTrack();
      LoadTrack();
      RepaintTrack();
    }
    else //没有鼠标按下，就重绘提示
      RepaintTip();
    sleepTime++;
    if (sleepTime % 10 == zero)
      Sleep(1);
    if (sleepTime >= 100)
    {
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
    ClearThisBackup();
    exit(0);
  case WM_DROPFILES: //添加2P轨道
  {
    HDROP hDrop = (HDROP)wParam;
    char path[maxPath];
    DragQueryFile(hDrop, 0, path, sizeof(path));
    DragFinish(hDrop);
    OpenTrack(path);
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
  }
  return 0;
}
const int maxAppendixNum = 100;
char appendix[maxAppendixNum][maxPath];
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
      char message[100];
      sprintf_s(message, "缺少依赖文件夹：\n%s\n请确保更新完整，且不要把软件移出文件夹。", appendix[i]);
      MessageBox(NULL, message, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
      exit(0);
    }
}
//创建本次自动备份文件夹
void CreateBackupFolder()
{
  time_t currentTime;
  char backupPath[maxPath] = {};
  while (true)
  {
    time(&currentTime);
    GetTimeStringForFolder(runTimeString, currentTime);//记录本次启动的时间
    sprintf_s(backupPath, "自动备份\\轨道文件\\%s", runTimeString);
    if (!FileExist(backupPath))//如果文件夹不存在，则退出循环
      break;
    Sleep(100);
  };
  CreatePath(backupPath);//创建本次自动备份文件夹
  sprintf_s(tempTrackForSave, "自动备份\\轨道文件\\%s\\temp.txt", runTimeString);
}

char specialTrack[100][maxPath];
int specialTrackNum;
const int maxFileNum = 128;
char fileList[maxFileNum][maxPath];

//重排特定轨道
void RemakeTrack(const char *track)
{
  int account = 0;
  //2P轨道判据：2槽是木盘子
  isCupUsed = false;
  OpenTrackWithChoice(track, ID_1P);//打开轨道并检查每一波
  if (strcmp(slot[account][2].name, "木盘子") != 0) //2槽不是木盘子，退出
    return;
  bool maltoseExist = false;
  for (int order = 1; order <= slotNum[account]; order++)
    if (strcmp(slot[account][order].name, "麦芽糖") == 0 || strcmp(slot[account][order].name, "棉花糖") == 0)
      maltoseExist = true;
  //如果携带了麦芽糖/棉花糖或使用了酒杯灯，记录并退出
  if (maltoseExist || isCupUsed)
  {
    strcpy_s(specialTrack[specialTrackNum++], track);
    return;
  }
  //进行调整：1. 删除4酒杯灯 2. 木盘子2插入到冰激凌4 3. 插入麦芽糖到6号位置
  DeleteFromSlot_File(account, 4);
  InsertSlot_File(account, 2, 4);
  AddToSlot_File(account, 14);
  InsertSlot_File(account, slotNum[account], 5);
  SaveTrack();
}
//棉花糖换麦芽糖
void CottonToMaltose(const char *track)
{
  int account = 0;
  //2P轨道判据：2槽是木盘子
  isCupUsed = false;
  OpenTrackWithChoice(track, ID_1P);//打开轨道并检查每一波
  bool cottonExist = false;
  bool maltoseExist = false;
  int cottonOrder = -1;
  for (int order = 1; order <= slotNum[account]; order++)
  {
    if (strcmp(slot[account][order].name, "棉花糖") == 0)
    {
      cottonExist = true;
      cottonOrder = order;
    }
    if (strcmp(slot[account][order].name, "麦芽糖") == 0)
      maltoseExist = true;
  }
  bool onlyCotton = cottonExist && !maltoseExist;
  if (!onlyCotton)
  {
    strcpy_s(specialTrack[specialTrackNum++], track);
    return;
  }
  //进行调整：1. 棉花糖替换为麦芽糖
  ChangeSlot_File(account, cottonOrder, 14);
  SaveTrack();
}

//重排文件夹folder内的轨道卡组
void RemakeFolder(const char *folder)
{
  if (!FileExist(folder))
    return;
  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*.txt", folder);

  int filesNum = GetFileList(searchPath, fileList, maxFileNum);//查找所有txt文件
  char track[maxPath] = {};
  for (int i = 0; i < filesNum; i++)
  {
    sprintf_s(track, "%s\\%s", folder, fileList[i]);
    CottonToMaltose(track);
  }
}
const int maxFolderNum = 128;
char folderList[maxFolderNum][maxPath];
void RemakeBigFolder(const char *bigFolder)
{
  char searchPath[maxPath] = {};
  sprintf_s(searchPath, "%s\\*", bigFolder);
  int folderNum = GetFileList(searchPath, folderList, maxFolderNum);//查找所有文件夹
  char folder[maxPath] = {};
  for (int i = 0; i < folderNum; i++)
  {
    sprintf_s(folder, "%s\\%s", bigFolder, folderList[i]);
    RemakeFolder(folder);
  }
}
//重排通用轨道、美食大赛和公会任务
void RemakeAllTrack()
{
  RemakeBigFolder("预制轨道\\通用轨道");
  //RemakeBigFolder("预制轨道\\美食大赛");
  //RemakeFolder("预制轨道\\公会任务");
  char message[10000] = {};
  for (int i = 0; i < specialTrackNum; i++)
  {
    strcat_s(message, specialTrack[i]);
    strcat_s(message, "\n");
  }
  PopMessage(nullptr, message);
}
int main(int argc, char *argv[])
{
  DPI = SetDPIAware();//设置DPI感知并获取DPI

  /*获取程序所在目录*/
  char programDirect[maxPath];//当前目录
  strcpy_s(programDirect, argv[0]);//拷贝程序目录
  char *slash = strrchr(programDirect, '\\');//找到最右边的斜杠并设为0
  slash[0] = 0;//至此程序所在目录programDirect获取完毕
  if (argc > 1)
    _chdir(programDirect);//如果启动了其他文件，则修改工作目录到程序所在目录

  /*检查依赖文件完整性：防御卡背包和生产信息*/
  if (argc == 1) //直接启动编辑器，Track.txt必须存在，没有则创建初始轨道
    if (!FileExist("Track.txt"))
      OutputRes("初始轨道", "TEXT", "Track.txt");
  if (!FileExist("附加程序"))
    CreatePath("附加程序");
  if (!FileExist("用户参数"))
    CreatePath("用户参数");
  if (!FileExist("防御卡背包.txt"))
    OutputRes("防御卡背包", "TEXT", "防御卡背包.txt");
  if (!FileExist("生产信息.txt"))
    OutputRes("生产信息", "TEXT", "生产信息.txt");
  if (!FileExist("附加程序\\依赖文件.txt"))
    OutputRes("依赖文件", "TEXT", "附加程序\\依赖文件.txt");
  CheckAppendix();//检查依赖文件

  //获取轨道文件完整路径
  if (argc == 1)//直接启动时，完整轨道路径="程序所在目录\\Track.txt"
  {
    char workPath[maxPath];
    GetWorkPath(workPath);
    sprintf_s(fullTrackPath[0], "%sTrack.txt", workPath);
  }
  else if (argc == 2 && strlen(argv[1]) < maxPath - 4) //否则完整目录就是argv[1]
    strcpy_s(fullTrackPath[0], argv[1]);
  else
  {
    MessageBox(NULL, "文件路径不得长于260字符。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return 0;
  }

  int fileType = CheckFileType(fullTrackPath[0]);
  if (fileType != 1)
  {
    PopMessage(hWndEditor, "只能打开轨道文件。");
    return 0;
  }

  //创建互斥量
  HANDLE hMutex = CreateMutex(NULL, FALSE, "FORBID_REPEAT_EDITOR");//创建互斥量
  sprintf_s(wndTitle, versionString, version);//获取EasyX窗口标题WndTitle
  if (hMutex && GetLastError() == ERROR_ALREADY_EXISTS)//如果互斥量已经存在，说明已有编辑器打开
  {
    LoadEditor();//读取当前打开轨道信息
    //如果待打开文件已打开，则显示对应编辑器，结束程序
    for (int order = 0; order < editorNum; order++)
      for (int account = 0; account < editor[order].accountNum; account++)
        if (strcmp(fullTrackPath[0], editor[order].trackPath[account]) == 0)
        {
          CenterShow(editor[order].hWnd);
          SetForegroundWindow(editor[order].hWnd);
          MessageBox(editor[order].hWnd, "不能重复打开同一轨道文件。", "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
          return 0;
        }
    //如果待打开文件未打开，检查是否可以作为2P轨道
    char trackPath1P[maxPath] = {};
    bool isTrack2P = RemovePlus(fullTrackPath[0], trackPath1P);//是否2P轨道
    if (RemovePlus(fullTrackPath[0], trackPath1P)) //如果是2P轨道，检查去+轨道是否在1P中
    {
      for (int order = 0; order < editorNum; order++)
        // 如果某个编辑器只有1个轨道，且这个轨道就是待打开轨道的去+轨道，将待打开轨道附加到该编辑器
        if (editor[order].accountNum == 1 && strcmp(editor[order].trackPath[0], trackPath1P) == 0)
        {
          LeftClick(editor[order].hWnd, 9999, 9998);//发送“添加2P轨道”点击消息
          CenterShow(editor[order].hWnd);
          SetForegroundWindow(editor[order].hWnd);
          return 0;
        }
    }
    //其余情况以新编辑器打开轨道并作为1P
  }
  else //如果互斥量不存在，说明编辑器没打开
    ClearBackup();//清理之前的自动备份

  //至此程序已成功启动，没有重复打开轨道文件。下面获取轨道文件的完整路径

  crossCursor = LoadCursor(NULL, IDC_CROSS);//加载十字光标资源
  for (int i = 0; i <= 18; i++)
    WaveColor[i] = strtol(WaveColorString[i], NULL, 16);//将18种波数颜色按16进制转化为COLORREF（整数）

  CreateBackupFolder();//创建自动备份文件夹
  LoadParameter();
  SaveParameter();
  LoadCommonCard();
  LoadProduction();
  SaveCommonCard();
  SaveProduction();

  //RemakeAllTrack();
  //return 114514;

  //ExchangeGeneralDeck();
  //return 114;

  char info[1000];
  //检验每一波轨道并填写WaveExist（若检验成功，内存中是最后一波轨道，但curWave依然为0）
  int TestResult = LoadTrackFrom(curAccount, fullTrackPath[curAccount], info, true);
  if (TestResult != zero)
  {
    MessageBox(NULL, info, "提示", MB_ICONINFORMATION | MB_SYSTEMMODAL);
    return 0;
  }
  isMapImageCatched = LoadMapImage(&mapImage);//载入地图图像并记录

  PopUpdateNotice();//弹出更新公告
  OpenGraph_Fit();
  hWndEditor = GetHWnd();
  LoadTrack();//重新读取第0波轨道
  SaveTrack();//启动时保存一次，形成备份
  SelectDepth(1);//选中最高深度
  SetWindowLongPtr(hWndEditor, GWLP_WNDPROC, (LONG_PTR)MyWindowProc); // 设置自定义窗口过程

  EnableDragDropForHighIntegrity(hWndEditor);
  DragAcceptFiles(hWndEditor, TRUE);//允许窗口接收文件
  CenterShow(hWndEditor);
  SetForegroundWindow(hWndEditor);
  SetWindowText(hWndEditor, wndTitle);

  HINSTANCE hInstance = GetModuleHandle(NULL);
  HICON hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(101), IMAGE_ICON,
    GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
  SendMessage(hWndEditor, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
  SendMessage(hWndEditor, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
  UpdateWindow(hWndEditor);

  setfillcolor(defaultFillColor);
  setbkmode(TRANSPARENT);     //透明字体
  LOGFONT f;
  gettextstyle(&f);						//获取当前字体设置
  f.lfHeight = 20;						//设置字体高度
  f.lfWeight = 1000;						//设置字体厚度
  strcpy_s(f.lfFaceName, "等线");		//设置字体为等线
  f.lfQuality = ANTIALIASED_QUALITY;//设置输出效果为抗锯齿
  settextstyle(&f);						//设置字体样式

  headWidth = textwidth("000 [ ");
  cardNameWidth = textwidth("小火炉");
  tailWidth = textwidth(" ] 002.3|22");
  recordLength = headWidth + cardNameWidth + tailWidth;

  while (true)
    if (mode == 1)
      EditMap();
    else if (mode == 2)
      EditTrack();
    else if (mode == 3)
      RecordTrack();
  closegraph();
  return 0;
}