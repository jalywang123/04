#include "track.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


unsigned int posi_sigma[NUM_TEMPLATE_PIXEL];
unsigned int nega_sigma[NUM_TEMPLATE_PIXEL];
unsigned short int posi_templates[SIZE_POSITIVE];
unsigned short int nega_templates[SIZE_NEGATIVE];
unsigned short int particle_templates[SIZE_PARTICLE];
unsigned short int posi_mean[NUM_TEMPLATE_PIXEL];
unsigned short int nega_mean[NUM_TEMPLATE_PIXEL];
unsigned short int CHi_initial[TEMPLATE_SIZE_H];
unsigned short int CWi_initial[TEMPLATE_SIZE_W];
unsigned char CHf_initial[TEMPLATE_SIZE_H];
unsigned char CWf_initial[TEMPLATE_SIZE_W];
float float_map_templates[NUM_PARTICLE];
float particle_data[NUM_PARTICLE*4];
float random_matrix[NUM_PSEUDORANDOM];
float ncc_maxc[1]={0};
float ncc_mean=0;
float ncc_forw=1;
int small_ncc_num=0;
unsigned short int max_value_num;
float affsig[3] = {4, 4, 0.20};
float radioWH;

TargetData_TYPE TargetData_Parameter;
int nccx[20]={0};
int nccy[20]={0};
int x,y,sumx=0,sumy=0;

int num_frame; // 记录启动跟踪后，当前帧的帧数
IE ie;

unsigned char flagtrackerror;
//unsigned char num_still = 0;

// 更改

int i;
unsigned char numStill = 0; // 跟踪失效判断中，计数使用，备注：这个参数，在再次启动跟踪时，要清0
unsigned char flagForcast = 0;  // 启动预推的标志，0表示未启动，1表示启动
unsigned char numFindTarget = 0;  // 记录启动预推后，检测到目标的连续帧数，如果连续若干帧都检测到目标，则认为预推正确，目标确认
unsigned char ForcastNum = 0;  // 记录启动预推的帧数，若启动预推若干帧后，还确认不了目标，则认为跟踪丢失，启动检测
float Targetdx,Targetdy;
TargetData_TYPE LastTarget;  // 记录前一帧目标的坐标信息



//char track(unsigned char* pSource_data,char o,int dx,int dy,int ww,int hh)
char track(unsigned char* pSource_data,char o,int *dd)
{
    int i;
    TargetData_Parameter.track_state = 1;  // initialization
    TargetData_Parameter.TrackStatus = 1;  // initialization
    Gaussian_Distribution
    (
        random_matrix
        );
    if(o==1)
    {
        // 在前一帧目标的周围，挑选候选样本
        affine_training_particle(particle_data,particle_templates,random_matrix,affsig,CHf_initial,CWf_initial,CHi_initial,CWi_initial,pSource_data,&TargetData_Parameter);
        norm_vector(particle_templates,NUM_PARTICLE);  // 候选样本像素范围归一化到0-1之间
        ratio_classifier(nega_mean,posi_mean,particle_templates,float_map_templates); // 通过最近邻分类器，计算每个候选样本的NCC值
        max_value_num = max_address(ncc_maxc,&TargetData_Parameter,float_map_templates); // NCC值最大的候选样本即为本帧疑似目标
        
        // 更新
        TargetData_Parameter.TCenter_H = particle_data[max_value_num] + 0.5;
        TargetData_Parameter.TCenter_W = particle_data[NUM_PARTICLE + max_value_num] + 0.5;
        TargetData_Parameter.TSize_H = particle_data[2 * NUM_PARTICLE + max_value_num] + 0.5;
        TargetData_Parameter.TSize_W =  TargetData_Parameter.TSize_H * radioWH;
        //  TargetData_Parameter.TSize_W = particle_data[3 * NUM_PARTICLE + max_value_num] + 0.5;
        TargetData_Parameter.TSizeHalf_H = (TargetData_Parameter.TSize_H >> 1);
        TargetData_Parameter.TSizeHalf_W = (TargetData_Parameter.TSize_W >> 1);
        num_frame++;
        IEupd(ie,num_frame,TargetData_Parameter);
		
		// 记录前一帧目标与本帧目标的位置偏移量，用于后续的跟踪失效判断

        //display_rectangle (&TargetData_Parameter,pSource_data,0);
        printf("--[%d,%d]--\r\n",TargetData_Parameter.TCenter_W,TargetData_Parameter.TCenter_H);
            dd[0]=TargetData_Parameter.TCenter_W;
            dd[1]=TargetData_Parameter.TCenter_H;
            //dd[2]=TargetData_Parameter.

		// 如果目标处于边界处，则重新启动检测。
        if((TargetData_Parameter.TCenter_H < 50)||(TargetData_Parameter.TCenter_H > FRAME_H - 50)||(TargetData_Parameter.TCenter_W < 50)||(TargetData_Parameter.TCenter_W > FRAME_W))
           return 0;
        flagtrackerror = 0;

	   // BS08图像处理板上的跟踪失效
	   //   跟踪失效判断和预推
        IEpre(ie, ncc_maxc[0],num_frame,TargetData_Parameter);
		if (flagtrackerror == 1)  // 跟踪失效，启动检测
		{
			return 0;
		}
                printf("flagtrackerror = %d,NCC = %f \n", flagtrackerror,ncc_maxc[0]);
		
		//更新过程：更新负样本模型
		// 更新正负样本
		if( flagForcast == 0 && num_frame % 10 == 0 )
		{
			// 更新负样本//2016-负样本计算函数
			affine_training_nega(RANDOM_NEGAH64,RANDOM_NEGAW64,CHf_initial,CWf_initial,CHi_initial,CWi_initial,pSource_data,nega_templates,&TargetData_Parameter);
			norm_vector(nega_templates,NUM_NEGATIVE);//2016-负样本归一化函数
			nega_templates_initial(nega_mean,nega_templates,nega_sigma);//2016-负样本初始化函数
		}
	    return 1;  // 跟踪正常，继续跟踪
	    
	   
    }
    else
    {
 
		// 从检测 启动 跟踪时，参数初始化
        num_frame=1;

		
		// 目标的坐标信息
        TargetData_Parameter.TCenter_H = dd[1];
        TargetData_Parameter.TCenter_W = dd[0];
        TargetData_Parameter.TSize_H = dd[3];
        TargetData_Parameter.TSize_W = dd[2];
        TargetData_Parameter.TSizeHalf_H = (TargetData_Parameter.TSize_H >> 1);
        TargetData_Parameter.TSizeHalf_W = (TargetData_Parameter.TSize_W >> 1);

        // 记录目标的宽高比，如果目标发生尺度变化的话，可以保证宽高比不变
        radioWH = 1.0 * TargetData_Parameter.TSizeHalf_W / TargetData_Parameter.TSizeHalf_H;
        
		// 保存本帧目标的信息
         IEinit(ie,num_frame,TargetData_Parameter);
		
		// 在第一帧中，挑选正、负样本，并将所有样本的尺寸变到16 x 16大小
        affine_training_posi(RANDOM_POSIH32,RANDOM_POSIW32,CHf_initial,CWf_initial,CHi_initial,CWi_initial,pSource_data,posi_templates,&TargetData_Parameter);
        affine_training_nega(RANDOM_NEGAH64,RANDOM_NEGAW64,CHf_initial,CWf_initial,CHi_initial,CWi_initial,pSource_data,nega_templates,&TargetData_Parameter);
         
		// 正、负样本的像素值归一化到0-1之间，对光照变化鲁棒 
        norm_vector(posi_templates,NUM_POSITIVE);
        norm_vector(nega_templates,NUM_NEGATIVE);
		
		// 计算正、负样本的均值向量，构建最近邻分类器
        posi_templates_initial(posi_mean,posi_templates,posi_sigma);
        nega_templates_initial(nega_mean,nega_templates,nega_sigma);

        display_rectangle (&TargetData_Parameter,pSource_data,0);
     //   printf("datect h = %d, w = %d \n",hh,ww);

        return 1;
    }
}
