#include "lucalendar.h"
#include "rtc.h"
#include "noli.h"

/*********************************************************************************
*************************MCU启明 STM32F407应用开发板******************************
**********************************************************************************
* 文件名称: lucalendar.c                                                         *
* 文件简述：RTC农历实现                                                          *
* 创建日期：2015.03.19                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：超强的日历，支持农历，24节气几乎所有日历的功能日历时间以1970年为元年,*
            用32bit的时间寄存器可以运行到2100年左右                              * 
**********************************************************************************
*********************************************************************************/

//下部分数据是农历部分要使用的
//月份数据表
u8  const day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
unsigned short const day_code2[3]={0x111,0x130,0x14e};
//u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
u8 const *sky[10]=  {"甲","乙","丙","丁","戊","己","庚","辛","壬","癸",};//天干
u8 const *earth[12]={"子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥",}; //地支
u8 const *monthcode[12]={"一","二","三","四","五","六","七","八","九","十","冬","腊",};//农历月份
u8 const *nongliday[4]={"初","十","廿","三",}; //农历日期  

///////////////////////////////////////////////////////////////////////
//支持从1900年到2099年的农历查询
//支持从2000年到2050年的节气查询
//子函数,用于读取数据表中农历月的大月或小月,如果该月为大返回1,为小返回0
u8 GetMoonDay(u8 month_p,unsigned short table_addr)
{
	switch (month_p)
	{
		case 1:
			if((year_code[table_addr]&0x08)==0)	return(0);
			else 								return(1); 
		case 2:
			if((year_code[table_addr]&0x04)==0)	return(0);
			else 								return(1);
		case 3:
			if((year_code[table_addr]&0x02)==0)	return(0);
			else 								return(1);
		case 4:
			if((year_code[table_addr]&0x01)==0)	return(0);
			else 								return(1);
		case 5:
			if((year_code[table_addr+1]&0x80)==0)	return(0);
			else 									return(1);
		case 6:
			if((year_code[table_addr+1]&0x40)==0)	return(0);
			else 									return(1);
		case 7:
			if((year_code[table_addr+1]&0x20)==0)	return(0);
			else 									return(1);
		case 8:
			if((year_code[table_addr+1]&0x10)==0)	return(0);
			else 									return(1);
		case 9:
			if((year_code[table_addr+1]&0x08)==0)	return(0);
			else 									return(1);
		case 10:
			if((year_code[table_addr+1]&0x04)==0)	return(0);
			else 									return(1);
		case 11:
			if((year_code[table_addr+1]&0x02)==0)	return(0);
			else 									return(1);
		case 12:
			if((year_code[table_addr+1]&0x01)==0)	return(0);
			else 									return(1);
		case 13:
			if((year_code[table_addr+2]&0x80)==0)	return(0);
			else 									return(1);
	}
	return(0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////// 
// 函数名称:GetChinaCalendar
//功能描述:公农历转换(只允许1901-2099年)
// 输　入:  year        公历年
//          month       公历月
//          day         公历日
//          p           储存农历日期地址
// 输　出:  1           成功
//          0           失败																			 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 GetChinaCalendar(u16  year,u8 month,u8 day,u8 *p)
{ 
	u8 temp1,temp2,temp3,month_p,yearH,yearL;	
	u8 flag_y;
	unsigned short temp4,table_addr;

	yearH=year/100;	yearL=year%100;//年份的高低两个字节 
	if((yearH!=19)&&(yearH!=20))return(0);//日期不在19xx ~ 20xx 范围内,则退出
	
	// 定位数据表地址  
	if(yearH==20)	table_addr=(yearL+100-1)*3;
	else  			table_addr=(yearL-1)*3;

	// 取当年春节所在的公历月份  
	temp1=year_code[table_addr+2]&0x60;	
	temp1>>=5;

	// 取当年春节所在的公历日  
	temp2=year_code[table_addr+2]&31; 

	// 计算当年春年离当年元旦的天数,春节只会在公历1月或2月  
	if(temp1==1) 	temp3=temp2-1; 
	else 			temp3=temp2+31-1; 

	// 计算公历日离当年元旦的天数  
	if (month<10) 	temp4=day_code1[month-1]+day-1;
	else  			temp4=day_code2[month-10]+day-1;
	// 如果公历月大于2月并且该年的2月为闰月,天数加1  
	if ((month>2)&&(yearL%4==0)) 	temp4++;

	// 判断公历日在春节前还是春节后  
	if (temp4>=temp3)
	{ 						
		temp4-=temp3;
		month=1;
		month_p=1;
							
		flag_y=0;
		if(GetMoonDay(month_p,table_addr)==0)	temp1=29; //小月29天
		else 									temp1=30; //大小30天
		// 从数据表中取该年的闰月月份,如为0则该年无闰月  
		temp2=year_code[table_addr]/16; 	
		while(temp4>=temp1)
		{
			temp4-=temp1;
			month_p++;
			if(month==temp2)
			{
				flag_y=~flag_y;
				if(flag_y==0)month++;
			}
			else month++;
			if(GetMoonDay(month_p,table_addr)==0)	temp1=29;
			else 									temp1=30;
		}
		day=temp4+1;
	}
	// 公历日在春节前使用下面代码进行运算  
	else
	{ 						
		temp3-=temp4;
		if (yearL==0)
		{
			yearL=100-1;
			yearH=19;
		}
		else yearL--;
		table_addr-=3;
		month=12;
		temp2=year_code[table_addr]/16; 	
		if (temp2==0)	month_p=12; 
		else 			month_p=13; 

		flag_y=0;
		if(GetMoonDay(month_p,table_addr)==0)	temp1=29; 
		else 									temp1=30; 
		while(temp3>temp1)
		{
			temp3-=temp1;
			month_p--;
			if(flag_y==0)		month--;
			if(month==temp2)	flag_y=~flag_y;
			if(GetMoonDay(month_p,table_addr)==0)	temp1=29;
			else 									temp1=30;
		}
		day=temp1-temp3+1;
	}

	*p++=yearH;
	*p++=yearL;
	*p++=month;
	*p=day;	
	return(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称:GetSkyEarth
// 功能描述:输入公历日期得到一个甲子年(只允许1901-2099年)
// 输　入:  year        公历年
//          p           储存星期地址
// 输　出:  无																							   
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetSkyEarth(u16 year,u8 *p)
{
	u8 x;
	
	if(year>=1984)
	{
		year=year-1984;
		x=year%60;				
	}
	else
	{
		year=1984-year;
		x=60-year%60;
	}
	*p=x;
}
//将指定字符source复制no个给target
void StrCopyss(u8 *target,u8 const *source,u8 no)
{
	u8 i;
	for(i=0;i<no;i++)
	{
 		 *target++=*source++;
	}
}

/********************************************************************************
*函数名称:void GetLunarCalendarStr(u16 year,u8 month,u8 day,u8 *str)
*功能描述:输入公历日期得到农历字符串	          
*入口参数: year        公历年
*          month       公历月
*          day         公历日
*          str         接收农历日期字符串地址    
*输出参数: 无	
*说    明：GetLunarCalendarStr(2015,03,15,str) 返回*str="乙未年正月廿五"
********************************************************************************/
void GetLunarCalendarStr(u16 year,u8 month,u8 day,u8 *str)
{
	u8 NLyear[4];
	u8 SEyear;
	
	StrCopyss(&str[0],(u8 *)"甲子年正月初一",15);
	if(GetChinaCalendar(year,month,day,(u8 *)NLyear)==0)	return;
	GetSkyEarth(NLyear[0]*100+NLyear[1],&SEyear);
	StrCopyss(&str[0],(u8 *)  sky[SEyear%10],2);	//  甲
	StrCopyss(&str[2],(u8 *)earth[SEyear%12],2);	//  子	
	
	if(NLyear[2]==1)	StrCopyss(&str[6],(u8 *)"正",2);
	else				StrCopyss(&str[6],(u8 *)monthcode[NLyear[2]-1],2);		
	
	if(NLyear[3]>10) 	StrCopyss(&str[10],(u8 *)nongliday[NLyear[3]/10],2);	
	else				StrCopyss(&str[10],(u8 *)"初",2);
	StrCopyss(&str[12],(u8 *)monthcode[(NLyear[3]-1)%10],2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称:GetJieQi
// 功能描述:输入公历日期得到本月24节气日期 day<15返回上半月节气,反之返回下半月	
//          如:GetJieQiStr(2007,02,08,str) 返回str[0]=4
// 输　入:  year        公历年
//          month       公历月
//          day         公历日
//          str         储存对应本月节气日期地址   1Byte
// 输　出:  1           成功
//          0           失败																			  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 GetJieQi(u16 year,u8 month,u8 day,u8 *JQdate)
{
	u8 bak1,value,JQ;

	if((year<2000)||(year>2050))     return 0;//节气表的范围限制
	if((month==0) ||(month>12))      return 0;
	JQ = (month-1) *2 ;		                        //获得节气顺序标号(0～23
	if(day >= 15) JQ++; 	                        //判断是否是上半月

	bak1=YearMonthBit[(year-2000)*3+JQ/8];        //获得节气日期相对值所在字节  
	value =((bak1<<(JQ%8))&0x80);                 //获得节气日期相对值状态

	*JQdate=days[JQ];								//得到基本节气日
	if( value != 0 )
	{
		//判断年份,以决定节气相对值1代表1,还是－1。
		if( (JQ== 1||JQ== 11||JQ== 18||JQ== 21)&&year< 2044)  (*JQdate)++;
		else                                                  (*JQdate)--;
	}
	return 1;
}

static u8 const MonthDayMax[]={31,28,31,30,31,30,31,31,30,31,30,31,};
/********************************************************************************
*函数名称:u8 GetJieQiStr(u16 year,u8 month,u8 day,u8 *str)
*功能描述:输入公历日期得到节气字符串	          
*入口参数: year        公历年
*          month       公历月
*          day         公历日
*          str         接收节气日期字符串地址   15Byte
*输出参数: 无	
*说    明：GetLunarCalendarStr(2015,06,07,str) 返回*str="离夏至还有15天"
********************************************************************************/
u8 GetJieQiStr(u16 year,u8 month,u8 day,u8 *str)
{
	u8 JQdate,JQ,MaxDay;

	if(GetJieQi(year,month,day,&JQdate)==0)	return 0;
 
	JQ = (month-1) *2 ;                             //获得节气顺序标号(0～23
	if(day >= 15) JQ++;                             //判断是否是上半月

	if(day==JQdate)                                 //今天正是一个节气日
	{
		StrCopyss(str,(u8 *)JieQiStr[JQ],5);
		return 1;
	}
                                              //今天不是一个节气日
	  StrCopyss(str,(u8 *)"离立冬还有??天",15);

	if(day<JQdate)                                  //如果今天日期小于本月的节气日期
	{
		StrCopyss(&str[2],(u8 *)JieQiStr[JQ],4);
		day=JQdate-day;
	} 

	else                                            //如果今天日期大于本月的节气日期
	{
		if((JQ+1) >23)  return 0;
		StrCopyss(&str[2],(u8 *)JieQiStr[JQ+1],4);
		if(day < 15)
		{
			GetJieQi(year,month,15,&JQdate);
			day=JQdate-day;
		}
		else                                        //翻月
		{
			MaxDay=MonthDayMax[month-1];
			if(month==2)                            //润月问题
			{
				if((year%4==0)&&((year%100!=0)||(year%400==0))) MaxDay++;
			}
			if(++month==13)	month=1;
			GetJieQi(year,month,1,&JQdate);
			day=MaxDay-day+JQdate;
		}
	}
	str[10]=day/10+'0';
	str[11]=day%10+'0';
	return 1;
}
