#include "weather_gui.h"
#include "weather_image.h"

#include "lvgl.h"

LV_FONT_DECLARE(lv_font_ibmplex_115);
LV_FONT_DECLARE(lv_font_ibmplex_64);
LV_FONT_DECLARE(ch_font20);
static lv_style_t default_style;
static lv_style_t chFont_style;
static lv_style_t numberSmall_style;
static lv_style_t numberBig_style;
static lv_style_t btn_style;
static lv_style_t bar_style;
lv_obj_t* scr_1 = NULL;
lv_obj_t* weatherImg = NULL;
lv_obj_t* cityLabel = NULL;
lv_obj_t* btn = NULL, * btnLabel = NULL;
lv_obj_t* txtLabel = NULL;
lv_obj_t* clockLabel_1 = NULL, * clockLabel_2 = NULL;
lv_obj_t* dateLabel = NULL;
lv_obj_t* tempImg = NULL, * tempBar = NULL, * tempLabel = NULL;
lv_obj_t* humiImg = NULL, * humiBar = NULL, * humiLabel = NULL;
lv_obj_t* spaceImg = NULL;
lv_obj_t* scr_2 = NULL;
lv_obj_t* chart, * titleLabel;
lv_chart_series_t* ser1, * ser2;


// 天气图标路径的映射关系
const void* weaImage_map[] = { &weather_0, &weather_9, &weather_14, &weather_5, &weather_25, 
				&weather_30, &weather_26, &weather_11, &weather_23 };
// 太空人图标路径的映射关系
const void* manImage_map[] = { &man_0, &man_1, &man_2, &man_3, &man_4, &man_5, &man_6, &man_7, &man_8, &man_9 };
char weekDayCh[7][4] = {"日","一", "二", "三", "四", "五", "六"};
char airQualityCh[6][10] = {"优","良","轻度","中度","重度","严重"};

void weather_obj_del(void);

void weather_gui_init(void)
{
    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&default_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
    lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
    lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));

    scr_1 = lv_obj_create(NULL, NULL);
    lv_obj_add_style(scr_1, LV_BTN_PART_MAIN, &default_style);
    lv_style_init(&chFont_style);
    lv_style_set_text_opa(&chFont_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_text_color(&chFont_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&chFont_style, LV_STATE_DEFAULT, &ch_font20);
    lv_style_init(&numberSmall_style);
    lv_style_set_text_opa(&numberSmall_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_text_color(&numberSmall_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&numberSmall_style, LV_STATE_DEFAULT, &lv_font_ibmplex_64);
    lv_style_init(&numberBig_style);
    lv_style_set_text_opa(&numberBig_style, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_text_color(&numberBig_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&numberBig_style, LV_STATE_DEFAULT, &lv_font_ibmplex_115);
	lv_style_init(&btn_style);
	lv_style_set_border_width(&btn_style, LV_STATE_DEFAULT, 0); 
	lv_style_init(&bar_style);
	lv_style_set_bg_color(&bar_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_border_width(&bar_style, LV_STATE_DEFAULT, 2); 
	lv_style_set_border_color(&bar_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_pad_top(&bar_style, LV_STATE_DEFAULT, 1);//指示器到背景四周的距离
	lv_style_set_pad_bottom(&bar_style, LV_STATE_DEFAULT, 1);
	lv_style_set_pad_left(&bar_style, LV_STATE_DEFAULT, 1);
	lv_style_set_pad_right(&bar_style, LV_STATE_DEFAULT, 1);

	scr_2 = lv_obj_create(NULL, NULL);
    lv_obj_add_style(scr_2, LV_BTN_PART_MAIN, &default_style);
}

void display_curve_init(lv_scr_load_anim_t anim_type)
{
	lv_obj_t *act_obj = lv_scr_act(); 
    weather_obj_del();
    lv_obj_clean(act_obj); 

	titleLabel = lv_label_create(scr_2, NULL);
	lv_obj_add_style(titleLabel, LV_LABEL_PART_MAIN, &chFont_style);
	lv_label_set_recolor(titleLabel, true);
	lv_label_set_text(titleLabel, "查看更多天气");
	lv_obj_align(titleLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);

	chart = lv_chart_create(scr_2, NULL);
	lv_obj_set_size(chart, 220, 180);
	lv_chart_set_range(chart, 0, 40);
	lv_chart_set_point_count(chart, 7);
	lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0, 10);
	lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/
	ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
	ser2 = lv_chart_add_series(chart, LV_COLOR_BLUE);
	lv_obj_set_style_local_pad_left(chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, 40);
	lv_chart_set_y_tick_texts(chart, "40\n30\n20\n10\n0", 0, LV_CHART_AXIS_DRAW_LAST_TICK);

	if (LV_SCR_LOAD_ANIM_NONE != anim_type) {
        lv_scr_load_anim(scr_2, anim_type, 300, 300, false);
    } else {
        lv_scr_load(scr_2);
    }
}

void display_curve(short maxT[], short minT[])
{
	for (int Ti = 0; Ti < 7; ++Ti) {
		ser1->points[Ti] = maxT[Ti];
	}
	for (int Ti = 0; Ti < 7; ++Ti) {
		ser2->points[Ti] = minT[Ti];
	}
	lv_chart_refresh(chart);
}

void display_weather_init(lv_scr_load_anim_t anim_type)
{
    lv_obj_t *act_obj = lv_scr_act();   // 获取当前活动页
    if (act_obj == scr_1)
        return;
    weather_obj_del();
    lv_obj_clean(act_obj);              // 清空此前页面

    weatherImg = lv_img_create(scr_1, NULL);
	lv_img_set_src(weatherImg, weaImage_map[0]);
	lv_obj_align(weatherImg, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 10);

	cityLabel = lv_label_create(scr_1, NULL);
	lv_obj_add_style(cityLabel, LV_LABEL_PART_MAIN, &chFont_style);
	lv_label_set_recolor(cityLabel, true);
	lv_label_set_text(cityLabel, "上海");
	lv_obj_align(cityLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 15);

	btn = lv_btn_create(scr_1, NULL);
	lv_obj_add_style(btn, LV_BTN_PART_MAIN, &btn_style);
	lv_obj_set_pos(btn, 75, 15);
	lv_obj_set_size(btn, 50, 25);
	lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_ORANGE);
	btnLabel = lv_label_create(btn, NULL);
	lv_obj_add_style(btnLabel, LV_LABEL_PART_MAIN, &chFont_style);
	lv_label_set_text(btnLabel, "良");

	txtLabel = lv_label_create(scr_1, NULL);
	lv_obj_add_style(txtLabel, LV_LABEL_PART_MAIN, &chFont_style);
	lv_label_set_text(txtLabel, "最低气温12°C, ");
	lv_label_set_long_mode(txtLabel, LV_LABEL_LONG_SROLL_CIRC);
	lv_label_set_text(txtLabel, "最低气温12°C, 最高气温21°C, 西北风0 级.   ");
	lv_obj_align(txtLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 50);

	clockLabel_1 = lv_label_create(scr_1, NULL);
	lv_obj_add_style(clockLabel_1, LV_LABEL_PART_MAIN, &numberBig_style);
	lv_label_set_recolor(clockLabel_1, true);
	lv_label_set_text(clockLabel_1, "10#ffa500 52#");
	lv_obj_align(clockLabel_1, NULL, LV_ALIGN_IN_LEFT_MID, 0, 10);
	clockLabel_2 = lv_label_create(scr_1, NULL);
	lv_obj_add_style(clockLabel_2, LV_LABEL_PART_MAIN, &numberSmall_style);
	lv_label_set_recolor(clockLabel_2, true);
	lv_label_set_text(clockLabel_2, "00");
	lv_obj_align(clockLabel_2, NULL, LV_ALIGN_IN_LEFT_MID, 165, 9);

	dateLabel = lv_label_create(scr_1, NULL);
	lv_obj_add_style(dateLabel, LV_LABEL_PART_MAIN, &chFont_style);
	lv_label_set_text(dateLabel, "11月23日   周二");
	lv_obj_align(dateLabel, NULL, LV_ALIGN_IN_LEFT_MID, 10, 32);

	tempImg = lv_img_create(scr_1, NULL);
	lv_img_set_src(tempImg, &temp);
	lv_img_set_zoom(tempImg, 180);
	lv_obj_align(tempImg, NULL, LV_ALIGN_IN_LEFT_MID, 10, 70);
	tempBar = lv_bar_create(scr_1, NULL);
	lv_obj_add_style(tempBar, LV_BAR_TYPE_NORMAL, &bar_style);
	lv_bar_set_range(tempBar, 0, 40);
	lv_obj_set_size(tempBar, 60, 12);
	lv_obj_set_style_local_bg_color(tempBar,LV_BAR_PART_INDIC,LV_STATE_DEFAULT,LV_COLOR_RED);
	lv_bar_set_value(tempBar, 10, LV_ANIM_OFF);
	lv_obj_align(tempBar, NULL, LV_ALIGN_IN_LEFT_MID, 35, 70);
	tempLabel = lv_label_create(scr_1, NULL);
	lv_obj_add_style(tempLabel, LV_LABEL_PART_MAIN, &chFont_style);
	lv_label_set_text(tempLabel, "18°C");
	lv_obj_align(tempLabel, NULL, LV_ALIGN_IN_LEFT_MID, 100, 70);

	humiImg = lv_img_create(scr_1, NULL);
	lv_img_set_src(humiImg, &humi);
	lv_img_set_zoom(humiImg, 180);
	lv_obj_align(humiImg, NULL, LV_ALIGN_IN_LEFT_MID, 0, 100);
	humiBar = lv_bar_create(scr_1, NULL);
	lv_obj_add_style(humiBar, LV_BAR_TYPE_NORMAL, &bar_style);
	lv_bar_set_range(humiBar, 0, 100);
	lv_obj_set_size(humiBar, 60, 12);
	lv_obj_set_style_local_bg_color(humiBar,LV_BAR_PART_INDIC,LV_STATE_DEFAULT,LV_COLOR_BLUE);
	lv_bar_set_value(humiBar, 49, LV_ANIM_OFF);
	lv_obj_align(humiBar, NULL, LV_ALIGN_IN_LEFT_MID, 35, 100);
	humiLabel = lv_label_create(scr_1, NULL);
	lv_obj_add_style(humiLabel, LV_LABEL_PART_MAIN, &chFont_style);
	lv_label_set_text(humiLabel, "49%");
	lv_obj_align(humiLabel, NULL, LV_ALIGN_IN_LEFT_MID, 100, 100);

	spaceImg = lv_img_create(scr_1, NULL);
	lv_img_set_src(spaceImg, manImage_map[0]);
	lv_obj_align(spaceImg, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -10, -10);

	if (LV_SCR_LOAD_ANIM_NONE != anim_type) {
        lv_scr_load_anim(scr_1, anim_type, 300, 300, false);
    } else {
        lv_scr_load(scr_1);
    }
}

void display_weather(struct Weather weaInfo)
{
	lv_label_set_text(cityLabel, weaInfo.cityname);
	lv_label_set_text(btnLabel,airQualityCh[weaInfo.airQulity]);
	lv_label_set_text_fmt(txtLabel, "最低气温%d°C, 最高气温%d°C, %s%d 级.   ",
		weaInfo.minTemp,weaInfo.maxTmep,weaInfo.windDir,weaInfo.windLevel);
	lv_bar_set_value(tempBar, weaInfo.temperature, LV_ANIM_OFF);
	lv_label_set_text_fmt(tempLabel, "%2d°C",weaInfo.temperature);
	lv_img_set_src(weatherImg, weaImage_map[weaInfo.weather_code]);
}

void display_time(struct TimeStr timeInfo)
{
	lv_label_set_text_fmt(clockLabel_1, "%02d#ffa500 %02d#", timeInfo.hour, timeInfo.minute);
	lv_label_set_text_fmt(clockLabel_2, "%02d", timeInfo.second);
	lv_label_set_text_fmt(dateLabel, "%2d月%2d日   周%s",timeInfo.month, timeInfo.day, 
						weekDayCh[timeInfo.weekday]);
}

void weather_obj_del(void)
{
	if (weatherImg != NULL) {
		lv_obj_clean(weatherImg);
		lv_obj_clean(cityLabel);
		lv_obj_clean(btn);
		lv_obj_clean(btnLabel);
		lv_obj_clean(txtLabel);
		lv_obj_clean(clockLabel_1);
		lv_obj_clean(clockLabel_2);
		lv_obj_clean(dateLabel);
		lv_obj_clean(tempImg);
		lv_obj_clean(tempBar);
		lv_obj_clean(tempLabel);
		lv_obj_clean(humiImg);
		lv_obj_clean(humiBar);
		lv_obj_clean(humiLabel);
		lv_obj_clean(spaceImg);
		weatherImg = NULL;
		cityLabel = NULL;
		btn = NULL, btnLabel = NULL;
		txtLabel = NULL;
		clockLabel_1 = NULL, clockLabel_2 = NULL;
		dateLabel = NULL;
		tempImg = NULL, tempBar = NULL, tempLabel = NULL;
		humiImg = NULL, humiBar = NULL, humiLabel = NULL;
		spaceImg = NULL;
	}
	if (chart !=NULL) {
		lv_obj_clean(chart);
		lv_obj_clean(titleLabel);
		chart = NULL;
		titleLabel = NULL;
	}
}

void weather_gui_del(void)
{
    weather_obj_del();
	if(scr_1 != NULL) {
		lv_obj_clean(scr_1);
		scr_1 = NULL;
	} else if (scr_2 != NULL) {
		lv_obj_clean(scr_2);
		scr_2 = NULL;
	}
}

void display_space(void)
{
	static int _spaceIndex = 0;
    if (lv_scr_act() == scr_1) {
        lv_img_set_src(spaceImg, manImage_map[_spaceIndex]);
        _spaceIndex = (_spaceIndex + 1) % 10;
    }
}

int airQulityLevel(int q)
{
    if (q < 50) {
        return 0;
    } else if (q < 100) {
        return 1;
    } else if (q < 150) {
        return 2;
    } else if (q < 200) {
        return 3;
    } else if (q < 300) {
        return 4;
    } 
    return 5;
}