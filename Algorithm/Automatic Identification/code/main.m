distance=5;

%ͼƬ���Ŀ����������
xy_position(1,1)=390;
xy_position(1,2)=728;

%ѹ��ͼƬ������Ӧ������λ��
xy_redu_position(1,1)=round((xy_position(1,1)/distance));
xy_redu_position(1,2)=round((xy_position(1,2)/distance));

data=imread('1.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,xy_redu_position);

data=imread('2.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,middle_data(5,:));

data=imread('3.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,middle_data(5,:));

data=imread('4.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,middle_data(5,:));

data=imread('5.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,middle_data(5,:));

data=imread('6.jpg');
[data,sum_num,middle_data,block,sort_data] = Find_Outline(distance,data,middle_data(5,:));


