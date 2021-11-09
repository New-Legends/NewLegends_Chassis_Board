//
// Created by WSJ on 2021/11/5.
//

#ifndef CPROJECT_UI_H
#define CPROJECT_UI_H

#include "referee.h"
#include "struct_typedef.h"
#include <vector>
#include <string>

#pragma pack(1) //按1字节对齐

#define __FALSE 100

/****************************开始标志*********************/
#define UI_SOF 0xA5
/****************************CMD_ID数据********************/
#define UI_CMD_Robo_Exchange 0x0301
/****************************内容ID数据********************/
#define UI_Data_ID_Del 0x100
#define UI_Data_ID_Draw1 0x101
#define UI_Data_ID_Draw2 0x102
#define UI_Data_ID_Draw5 0x103
#define UI_Data_ID_Draw7 0x104
#define UI_Data_ID_DrawChar 0x110
/***************************删除操作***************************/
#define UI_Data_Del_NoOperate 0
#define UI_Data_Del_Layer 1
#define UI_Data_Del_ALL 2
/***************************图形配置参数__图形操作********************/
#define UI_Graph_ADD 1
#define UI_Graph_Change 2
#define UI_Graph_Del 3
/***************************图形配置参数__图形类型********************/
#define UI_Graph_Line 0      //直线
#define UI_Graph_Rectangle 1 //矩形
#define UI_Graph_Circle 2    //整圆
#define UI_Graph_Ellipse 3   //椭圆
#define UI_Graph_Arc 4       //圆弧
#define UI_Graph_Float 5     //浮点型
#define UI_Graph_Int 6       //整形
#define UI_Graph_Char 7      //字符型
/***************************图形配置参数__图形颜色********************/
#define UI_Color_Main 0 //红蓝主色
#define UI_Color_Yellow 1
#define UI_Color_Green 2
#define UI_Color_Orange 3
#define UI_Color_Purplish_red 4 //紫红色
#define UI_Color_Pink 5
#define UI_Color_Cyan 6 //青色
#define UI_Color_Black 7
#define UI_Color_White 8

/*帧头*/
typedef struct {
    uint8_t SOF;          //起始字节,固定0xA5
    uint16_t Data_Length; //帧数据长度
    uint8_t Seq;          //包序号
    uint8_t CRC8;         //CRC8校验值
    uint16_t CMD_ID;      //命令ID
} UI_header;
/*操作定义帧*/
typedef struct {
    uint16_t Data_ID;     //内容ID
    uint16_t Sender_ID;   //发送者ID
    uint16_t Receiver_ID; //接收者ID
} UI_Data_Operate;
/*删除图层帧*/
typedef struct {
    uint8_t Delete_Operate; //删除操作
    uint8_t Layer;          //删除图层
} UI_Data_Delete;
/*图形数据*/
typedef struct {
    uint8_t graphic_name[3];
    uint32_t operate_tpye: 3;
    uint32_t graphic_tpye: 3;
    uint32_t layer: 4;
    uint32_t color: 4;
    uint32_t start_angle: 9;
    uint32_t end_angle: 9;
    uint32_t width: 10;
    uint32_t start_x: 11;
    uint32_t start_y: 11;
    uint32_t radius: 10;
    uint32_t end_x: 11;
    uint32_t end_y: 11;
} Graph_Data;
/*浮点数据*/
typedef struct {
    uint8_t graphic_name[3];
    uint32_t operate_tpye: 3;
    uint32_t graphic_tpye: 3;
    uint32_t layer: 4;
    uint32_t color: 4;
    uint32_t start_angle: 9;
    uint32_t end_angle: 9;
    uint32_t width: 10;
    uint32_t start_x: 11;
    uint32_t start_y: 11;
    float graph_Float;
} Float_Data;
/*字符串数据*/
typedef struct {
    Graph_Data Graph_Control;
    uint8_t show_Data[30];
} String_Data;

static std::vector<Graph_Data> UI_Graph_v;
static std::vector<std::string> UI_ImageName_v;
static int Graph_Num = 1;


class UI : public referee {
public:
    UI() : referee() {};

    void Delete_Graph(uint8_t Del_Operate, uint8_t Del_Layer);

    static void Draw_Line(Graph_Data *image, std::string image_name, uint32_t Graph_Operate, uint32_t Graph_Layer,
                          uint32_t Graph_Color,
                          uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t End_x, uint32_t End_y);

    static int UI_ReFresh(Graph_Data imageData);

    static void Draw_Circle(Graph_Data *image, std::string image_name, uint32_t Graph_Operate, uint32_t Graph_Layer,
                            uint32_t Graph_Color,
                            uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t Graph_Radius);

    static void Draw_Rectangle(Graph_Data *image, const char *image_name, uint32_t Graph_Operate, uint32_t Graph_Layer,
                               uint32_t Graph_Color,
                               uint32_t Graph_Width, uint32_t Start_x, uint32_t Start_y, uint32_t End_x,
                               uint32_t End_y);

    static void Draw_Float(Float_Data *image, const char *image_name, uint32_t Graph_Operate, uint32_t Graph_Layer,
                           uint32_t Graph_Color,
                           uint32_t Graph_Size, uint32_t Graph_Digit, uint32_t Graph_Width, uint32_t Start_x,
                           uint32_t Start_y, float Graph_Float);

    static void Draw_Char(String_Data *image, const char *image_name, uint32_t Graph_Operate, uint32_t Graph_Layer,
                          uint32_t Graph_Color,
                          uint32_t Graph_Size, uint32_t Graph_Digit, uint32_t Graph_Width, uint32_t Start_x,
                          uint32_t Start_y,
                          char *Char_Data);

    int String_ReFresh(String_Data string_Data);

    void
    Draw_Arc(Graph_Data *image, char *image_name, uint32_t Graph_Operate, uint32_t Graph_Layer, uint32_t Graph_Color,
             uint32_t Graph_StartAngle, uint32_t Graph_EndAngle, uint32_t Graph_Width, uint32_t Start_x,
             uint32_t Start_y,
             uint32_t x_Length,
             uint32_t y_Length);

    static void UI_SendByte(unsigned char ch);

};

class UI_Line : public UI {
public:
    UI_Line() : UI() {};

    UI_Line(uint32_t Operate_, uint32_t Layer_, uint32_t Start_x_,
            uint32_t Start_y_, uint32_t End_x_, uint32_t End_y_, uint32_t Color_ = UI_Color_Yellow, uint32_t Width_ = 1)
            : UI() {
        Operate = Operate_;
        Layer = Layer_;
        Color = Color_;
        Width = Width_;
        Start_x = Start_x_;
        Start_y = Start_y_;
        End_x = End_x_;
        End_y = End_y_;
        image = *new Graph_Data{0};
        image_name = "G" + std::to_string(Graph_Num);
        Draw();
        UI_Graph_v.push_back(image);
        UI_ImageName_v.push_back(image_name);

    };
    uint32_t Operate{};
    uint32_t Layer{};
    uint32_t Color{};
    uint32_t Width{};
    uint32_t Start_x{};
    uint32_t Start_y{};
    uint32_t End_x{};
    uint32_t End_y{};
    Graph_Data image{};
    std::string image_name;

    void Draw();

    void Delete();

    void Refresh() const;
};

class UI_Circle : public UI {
public:
    UI_Circle() : UI() {};

    UI_Circle(uint32_t Operate_, uint32_t Layer_, uint32_t Start_x_, uint32_t Start_y_, uint32_t Radius_,
              uint32_t Color_ = UI_Color_Yellow,
              uint32_t Width_ = 1) {
        Operate = Operate_;
        Layer = Layer_;
        Start_x = Start_x_;
        Start_y = Start_y_;
        Radius = Radius_;
        Color = Color_;
        Width = Width_;
        image = *new Graph_Data{0};
        image_name = "G" + std::to_string(Graph_Num);
        Draw();
        UI_Graph_v.push_back(image);
        UI_ImageName_v.push_back(image_name);
    }

    Graph_Data image{};
    std::string image_name;
    uint32_t Operate{};
    uint32_t Layer{};
    uint32_t Color{};
    uint32_t Width{};
    uint32_t Start_x{};
    uint32_t Start_y{};
    uint32_t Radius{};

    void Draw();

    void Delete();

    void Refresh() const;


};

unsigned char Get_CRC8_Check_Sum_UI(unsigned char *pchMessage, unsigned int dwLength, unsigned char ucCRC8);

uint16_t Get_CRC16_Check_Sum_UI(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);

#endif //CPROJECT_UI_H
