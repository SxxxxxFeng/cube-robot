%compres_ratioΪͼƬѹ������
function [data,sum_num,middle_data,block,sort_data] = Find_Outline(compres_ratio,pt_data,mid_data)

%ȡ����
mid_data=round(mid_data);

[hsv_classify_data,rgb_classify_data,reduce_rgb_data] = Get_Photo_Color(compres_ratio,pt_data);

start_xy=mid_data;

data=hsv_classify_data;

x_num=size(data,1);
y_num=size(data,2);

x=1:x_num;
y=1:y_num;
[X,Y]=meshgrid(y,x);

data_buf=data(mid_data(1,1),mid_data(1,2));
data(mid_data(1,1),mid_data(1,2))=2000;

%��ʾ����ǰ��ͼƬ
figure;
surf(X,Y,data(x,y));
title('����ǰͼƬ');
data(mid_data(1,1),mid_data(1,2))=data_buf;


%���ù����������Ѱ��������ɫ����������
[deal_data,block,sum_num(5),middle_data(5,:)]=Find_Outline_Scope(data,start_xy);

%�㼯���������ߵ�˳�����������Է����������
sort_data=Block_Sort(block,sum_num(5));
 
%����kmeans�㷨Ѱ�������������Χ���ĸ��ض���
four_data=Find_Four_Side(sort_data,sum_num(5),middle_data(5,:));

%��������õ�һ������5����ɫ�����������λ��,2,6,4,8
[middle_data]=Compute_Five_Center(middle_data(5,:),four_data);

%
% Display_Deal_Photo(data,center_data,1000);
% title('�����ĵ�ȷ���������ĸ���');

%��ȷ���2,4,6,8
for i=2:2:8
   [deal_data,block_data,sum_num(i),middle_data(i,:)]=Find_Outline_Scope(deal_data,middle_data(i,:)); 
end

middle_data(1,:)=middle_data(2,:)+middle_data(4,:)-middle_data(5,:);
middle_data(3,:)=middle_data(2,:)+middle_data(6,:)-middle_data(5,:);
middle_data(7,:)=middle_data(4,:)+middle_data(8,:)-middle_data(5,:);
middle_data(9,:)=middle_data(6,:)+middle_data(8,:)-middle_data(5,:);

%��ȷ���1,3,7,9
for i=1:2:3
   [deal_data,block_data,sum_num(i),middle_data(i,:)]=Find_Outline_Scope(deal_data,middle_data(i,:)); 
end
for i=7:2:9
   [deal_data,block_data,sum_num(i),middle_data(i,:)]=Find_Outline_Scope(deal_data,middle_data(i,:)); 
end

figure;
surf(X,Y,deal_data(x,y));
title('�����ͼƬ');

end

%���ù�����������ҵ�������ɫ������������ĵ�����
%data���ͼƬ��Ϣ��start_position��������ʼλ��
%data_deal��Ŵ�����ͼƬ��Ϣ��block����������ݣ�sum_num����������Ŀ��middle_data�������ĵ�����
function [data_deal,block,sum_num,middle_data] = Find_Outline_Scope(data,start_position)

%ȡ����
start_position=round(start_position);

%���������ĸ�����
next=[0,1;1,0;0,-1;-1,0];

data_deal=data;

x_max=size(data_deal,1);
y_max=size(data_deal,2);

%��Ǽ�����������λ��
book=zeros(x_max,y_max);
 
%��һ�д�x���꣬�ڶ��д�y����
que=zeros(100000,2);%ռ�õ��ڴ�ϴ������ַ�������ʾ����

%���������Ŀ
sum_num=0;

%���������ɫ����������㣬��һ��Ϊx���꣬�ڶ���Ϊy����
block=zeros(2*(x_max+y_max),2);

head=1;
tail=1;
que(tail,1)=start_position(1,1);
que(tail,2)=start_position(1,2);
tail=tail+1;
book(start_position(1,1),start_position(1,2))=1;

    while head<tail
        for k=1:4
            tx=que(head,1)+next(k,1);
            ty=que(head,2)+next(k,2);
            
            if(tx<1||tx>x_max||ty<1||ty>y_max)
                continue;    
            end
            
            if data_deal(tx,ty)<(data_deal(start_position(1,1),start_position(1,2))/2)             
                sum_num=sum_num+1; 
                block(sum_num,1)=tx;
                block(sum_num,2)=ty;
                data_deal(tx,ty)=600; 
                book(tx,ty)=1;
            end        
            if (data_deal(tx,ty)>(data_deal(start_position(1,1),start_position(1,2))/2)&&book(tx,ty)==0)                 
                book(tx,ty)=1;
                que(tail,1)=tx;
                que(tail,2)=ty;
                tail=tail+1;
            end   
        end
        head=head+1;
    end
    
    %�����������ĵ�����
    sum_x=sum(block(:,1));
    sum_y=sum(block(:,2));
    middle_data(1,1)=round((sum_x/sum_num));
    middle_data(1,2)=round((sum_y/sum_num));
   data_deal(middle_data(1,1),middle_data(1,2))=1000;   
end

%�ú����ѵ㼯�ϰ��������ߵ�˳�����������Է����������
%block_dataΪ�������ݣ���һ�д��x�����ݣ��ڶ��д��y�����ݣ�block_data����ź����block_data������,data_numΪ��Ч���ݵ���Ŀ
function [sort_data]=Block_Sort(block_data,data_num)

%����
line_num=data_num;
%
book=zeros(line_num,1);

%��ʼ��������ź����block_data������,�����������
sort_data(1,1)=block_data(1,1);
sort_data(1,2)=block_data(1,2);
%sort_data(1,3)=1;
%��ʶ��ǰsort_data��װ�����ݵ���Ŀ
sort_datanum=1;
book(sort_datanum,1)=1;

min_dis=10000;

while sort_datanum<line_num %���һ�����ݱ��line_num-1
    
    for num=2:line_num
        if book(num,1)==1   %�ȽϹ���            
            continue;
        else
           dis_xy=(sort_data(sort_datanum,1)-block_data(num,1))^2+(sort_data(sort_datanum,2)-block_data(num,2))^2; 
            if min_dis>dis_xy 
                 min_dis=dis_xy;
                 min_sig=num;          
            end
            
        end

    end
    
  sort_datanum=sort_datanum+1;     
  sort_data(sort_datanum,1)=block_data(min_sig,1);
  sort_data(sort_datanum,2)=block_data(min_sig,2);  
 %sort_data(sort_datanum,3)=min_sig; 
  book(min_sig,1)=1;
  
  min_dis=10000;
  
end

end

%���������㼯�����ĵ�ľ���Ĵ�С���Ӵ�С����
%midlle_x�����ĵ��x���꣬midlle_y�����ĵ��y����
%data_sort������������
% function [data_sort]=Block_Sort_MaxMin(block_data,data_num,middle_x,middle_y)
%     data=zeros(data_num,1);
%     data_sort=zeros(data_num,1);
%     
%     for num=1:data_num
%         data(num,1)=(block_data(num,1)-middle_x)^2+(block_data(num,2)-middle_y)^2;
%     end
%     
%     %���մӴ�С����
%     [data_middle,index]=sort(data(:,1),'descend');
%    %[data_middle,index]=sort(data(:,1));
%     
%      for num=1:data_num
%         data_sort(num,1)=data(index(num));
%      end
%      
% end


%N������һ���ֶ�����
%data������Ĳ��������ŵ�����,u_initial���趨��ÿһ�����ĵĳ�ʼֵ
%u��ÿһ�������
%re�Ƿ��صĴ������ŵ�����
function [u re]=KMeans(data,N,u_initial)   
    [m n]=size(data);   %m�����ݸ�����n������ά��
    
    %����ֵ
    u=u_initial;
   
    while 1
        pre_u=u;            %��һ����õ�����λ��
        for i=1:N
            tmp{i}=[];      % ��ʽһ�е�x(i)-uj,Ϊ��ʽһʵ����׼��
            for j=1:m
                tmp{i}=[tmp{i};data(j,:)-u(i,:)];
            end
        end
        
        quan=zeros(m,N);
        for i=1:m           %��ʽһ��ʵ��
            c=[];
            for j=1:N
                c=[c norm(tmp{j}(i,:))];
            end
            [junk index]=min(c);
            quan(i,index)=norm(tmp{index}(i,:));           
        end
        
        %���ӵ�һ��
        for i=1:m
            for j=1:N
                if quan(i,j)>0
                    quan(i,j)=1;  
                end
            end
        end
        
        for i=1:N %��ʽ����ʵ��            
           for j=1:n
                u(i,j)=sum(quan(:,i).*data(:,j))/sum(quan(:,i));
           end           
        end
        
        if norm(pre_u-u)<1  %���ϵ���ֱ��λ�ò��ٱ仯
            break;
        end
    end
    
    re=[];
    for i=1:m
        tmp=[];
        for j=1:N
            tmp=[tmp norm(data(i,:)-u(j,:))];
        end
        [junk index]=min(tmp);
        re=[re;data(i,:) index];
    end
    
end


%���ܣ��ҵ���������ɫ����ĸ��ǵ�����λ��
%block_data�����յ����ڵ�ԭ���������ɫ�������㼯��midlle_data�����ĵ�����
%
function [u]=Find_Four_Side(block_data,data_num,middle_data)  

    middle_data=round(middle_data);
    data=zeros(data_num,1);
    data_sort=zeros(int16(data_num*0.5),2);
    initial_class_data=zeros(4,2);%kmens�㷨��ʼֵ
    
    data_dis=int16(data_num/4);
    
    for num=1:data_num
        data(num,1)=(block_data(num,1)-middle_data(1,1))^2+(block_data(num,2)-middle_data(1,2))^2;
    end
%     
%     figure;
%     plot(n,data(n,1));
    
    %�Ӵ�С��˳������
    %[data_middle,index]=sort(data(:,1),'descend');
    
    %��С�����˳������
    [data_out,index]=sort(data(:,1));
    

     %�������ݰ��յ����ĵ�ľ���Ӵ�С��˳������
     for num_data=1:int16(data_num*0.5)
        data_sort(num_data,:)=block_data(index(num_data),:);%�����㼯����
     end    
     
%      for nums=1:data_num
%         hold on;
%         pause(0.5);
%         plot(index(nums),data(index(nums)),'*');
%     end
     
     %����kmens�㷨��ʼֵ
     data_index=index(1);
     
     for i_num=1:4
         
         if data_index>data_num
             data_index=data_index-data_num;
         end
         
         initial_class_data(i_num,1)=block_data(data_index,1); 
         initial_class_data(i_num,2)=block_data(data_index,2); 
         %initial_class_data(i_num,3)=data_index; 
         data_index=data_index+data_dis;
     end
     
%k-means����
[u re]=KMeans(data_sort,4,initial_class_data);  %����������ŵ����ݣ�������������ݵ������˼���������ټ�һά��

u=int16(u);

[m n_data]=size(re);

%�����ʾ����������
% figure;
% hold on;
% for i=1:m 
%     if re(i,3)==1   
%          plot(re(i,1),re(i,2),'ro'); 
%          plot(u(1,1),u(1,2),'k+'); 
%     elseif re(i,3)==2
%          plot(re(i,1),re(i,2),'go'); 
%          plot(u(2,1),u(2,2),'k+'); 
%     elseif re(i,3)==3 
%          plot(re(i,1),re(i,2),'bo'); 
%          plot(u(3,1),u(3,2),'k+'); 
%     else
%          plot(re(i,1),re(i,2),'co'); 
%          plot(u(4,1),u(4,2),'k+');    
%     end
% end
    
end


%��������ɫ������ĵ������Χ�ĸ������������ɫ����ĸ���
%data�����Ŀ�����ĵ�λ�����ݣ�four_data������Χ���ĸ�������
%center_data�ǰ���һ��˳���ħ��һ�����ÿ����ɫ������ĵ�����
function [center_data]=Compute_Five_Center(data,four_data)
%�Ȱ���Χ�ĸ��㰴�մ��ϵ��¡������ҵ�˳������(��Ҫԭ���ǰ��յ�����λ�ô�С)

%�ֶ������õ��Ľ��
ratio_data=2.6;
%ratio_data(2)=ratio_data(1)*1.4;

sort_data=zeros(8,2);%���������ɫ����Χ��8��������
center_data=int16(zeros(9,2));%�����ɫ�����������

%������ɫ�������
center_data(5,:)=data(1,:);

[com_data,index]=sort(four_data(:,1),'descend');%y�����ݴӴ�С����

%�ȱȽ�y�����ݴ�С����ǰ��λ��
if four_data(index(1),2)>four_data(index(2),2)%�Ƚ�x�����ݵĴ�С
    sort_data(3,:)=four_data(index(2),:);
    sort_data(4,:)=four_data(index(1),:); 
else
    sort_data(3,:)=four_data(index(1),:);
    sort_data(4,:)=four_data(index(2),:);   
end

%�ٱȽ�y�����ݴ�С���ں���λ��
if four_data(index(3),2)>four_data(index(4),2)%�Ƚ�x�����ݵĴ�С
    sort_data(1,:)=four_data(index(4),:);
    sort_data(2,:)=four_data(index(3),:); 
else
    sort_data(1,:)=four_data(index(3),:);
    sort_data(2,:)=four_data(index(4),:);   
end

sort_data(5,:)=sort_data(1,:)+sort_data(2,:)-data(1,:);
sort_data(6,:)=sort_data(1,:)+sort_data(3,:)-data(1,:);
sort_data(7,:)=sort_data(2,:)+sort_data(4,:)-data(1,:);
sort_data(8,:)=sort_data(3,:)+sort_data(4,:)-data(1,:);

%2,6,4,8
center_data(2,:)=(1-ratio_data)*data(1,:)+ratio_data*sort_data(1,:);
center_data(6,:)=(1-ratio_data)*data(1,:)+ratio_data*sort_data(2,:);
center_data(4,:)=(1-ratio_data)*data(1,:)+ratio_data*sort_data(3,:);
center_data(8,:)=(1-ratio_data)*data(1,:)+ratio_data*sort_data(4,:);

end




function []=Display_Deal_Photo(photo_data,coor_data,set_data)

coor_data=round(coor_data);

line=size(coor_data,1);%����

for i=1:line
    photo_data(coor_data(i,1),coor_data(i,2))=set_data;
end

%��ʾͼƬ
x_num=size(photo_data,1);
y_num=size(photo_data,2);

x=1:x_num;
y=1:y_num;
[X,Y]=meshgrid(y,x);

%��ʾ����ǰ��ͼƬ
figure;
surf(X,Y,photo_data(x,y));
title('�����ͼƬ');

end