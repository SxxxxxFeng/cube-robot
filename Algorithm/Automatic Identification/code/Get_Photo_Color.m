%��ȡͼƬ��Ϣ������άͼ����ʾ����
%distance����ʾͼ�ε����ؼ��
function [hsv_classify_data,rgb_classify_data,reduce_rgb_data] = Get_Photo_Color(distance,pt_data)

%��ȡͼƬ��Ϣ
data=pt_data;



%������ʼ��
i_num=1;
j_num=1;
k_num=1;

%ѹ��ͼƬ����Ϣ��̫�󲻱�����ʾ��ʾ
for k=1:3
    for j=1:distance:size(data,2)
        for i=1:distance:size(data,1)
            reduce_rgb_data(i_num,j_num,k_num)=data(i,j,k);
            i_num=i_num+1;
        end
        j_num=j_num+1;
        i_num=1;%��Ҫ�����˳�ʼ��
    end
    k_num=k_num+1;
    j_num=1;%��Ҫ�����˳�ʼ��
    i_num=1;%��Ҫ�����˳�ʼ��
end

%��ʾѹ����ͼƬ
% figure;
% imshow(reduce_rgb_data);

%RGBɫ��ռ�
data_rgb=double(data);
%RGBת��HSVɫ��ռ�
data_hsv=rgb2hsv(data_rgb);

%������ɫֵ�ļӺ�
i_num=1;
j_num=1;
for j=1:distance:size(data,2)
    for i=1:distance:size(data,1)     
       hsv_classify_data(i_num,j_num)=data_hsv(i,j,1)+data_hsv(i,j,2)+data_hsv(i,j,3);
       rgb_classify_data(i_num,j_num)=data_rgb(i,j,1)+data_rgb(i,j,2)+data_rgb(i,j,3);
       i_num=i_num+1;
    end
   j_num=j_num+1;
   i_num=1;%��Ҫ�����˳�ʼ��
end

x_rgb=1:distance:size(data_rgb,1);
y_rgb=1:distance:size(data_rgb,2);
x_hsv=1:distance:size(data_hsv,1);
y_hsv=1:distance:size(data_hsv,2);
x_reduce=1:1:size(hsv_classify_data,1);
y_reduce=1:1:size(hsv_classify_data,2);

%���ɵ�Ϊx�У�y��
[X_rgb,Y_rgb]=meshgrid(y_rgb,x_rgb);
[X_hsv,Y_hsv]=meshgrid(y_hsv,x_hsv);
[X_reduce,Y_reduce]=meshgrid(y_reduce,x_reduce);

sum_data_rgb=data_rgb(:,:,1)+data_rgb(:,:,2)+data_rgb(:,:,3);
sum_data_hsv=data_hsv(:,:,1)+data_hsv(:,:,2)+data_hsv(:,:,3);

% figure;
% surf(X_rgb,Y_rgb,sum_data_rgb(x_rgb,y_rgb));
% title('����ǰRGBɫ��ռ�');
% 
% figure;
% surf(X_hsv,Y_hsv,sum_data_hsv(x_hsv,y_hsv));
% title('����ǰHSVɫ��ռ�');

%Find_Outline(x_redu_position,y_redu_position,hsv_classify_data);

%Draw_Outline(x_redu_position,y_redu_position,rgb_classify_data,reduce_rgb_data,200);
%Draw_line(x_redu_position,y_redu_position,reduce_rgb_data,200);
end


%�Һ����ĵ���ɫ��ͬ������
%�βΣ�dis_xΪx�����꣬dis_y��data�洢ͼƬ����
% function [] =Find_Outline(dis_x,dis_y,data)
%     x_max=size(data,1);
%     y_max=size(data,2);
%     book=zeros(x_max,y_max);
%     book(dis_x,dis_y)=1;
%     middle_data=data(dis_x,dis_y);
%     
%     dfs(dis_x,dis_y,data,middle_data);
%     
%     %�����������
% function [] =dfs(dis_x,dis_y,data,middle_data)
%         
%         next=[0,1;1,0;0,-1;-1,0];
% %         middle_data_sum=data(dis_x,dis_y);%��ʼ��
%         
%         %�õ�һ��ƽ�������ݣ����������ҵ�5�����ݵ�ƽ��ֵ
% %         for i=1:4
% %              middle_data_sum=middle_data_sum+data(dis_x+next(i,1),dis_y+next(i,2));
% %          end
% %          middle_data=uint16(middle_data_sum/5);
%   
%         %5��������Ϊƽ����
% %         data(dis_x,dis_y)=middle_data;
% %         for i=1:4
% %             data(dis_x+next(i,1),dis_y+next(i,2))=middle_data;
% %         end
%         
%         for k=1:4
%             tx=dis_x+next(k,1);
%             ty=dis_y+next(k,2);
%             
%             if (tx<1||tx>x_max||ty<1||ty>y_max)
%                 continue;   
%             end
%             
%             if book(tx,ty)==0
%                 data(tx,ty)=middle_data;
%                 book(tx,ty)=1;
%                 dfs(tx,ty,data,middle_data);
%             end
% 
%         end
%         
%      return ;
%     
%     end
    
%end





