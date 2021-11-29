%Ѱ��ħ��ͼƬ��ɫ��߿����ù�����������㷨
function [sum,data] = Find_Outline_Scope(start_x,start_y,data)



% data=[
%     1,2,1,0,0,0,0,0,2,3;
%     3,0,2,0,1,2,1,0,1,2;
%     4,0,1,0,1,2,3,2,0,1;
%     3,2,0,0,0,1,2,4,0,0;
%     0,0,0,0,0,0,1,5,3,0;
%     0,1,2,1,0,1,5,4,3,0;
%     0,1,2,3,1,3,6,2,1,0;
%     0,0,3,4,8,9,7,5,0,0;
%     0,0,0,3,7,8,6,0,1,2;
%     0,0,0,0,0,0,0,0,1,0
% ];

x=1:size(data,1);
y=1:size(data,2);
[X,Y]=meshgrid(y,x);
figure;
surf(X,Y,data(x,y));
title('����ǰ');

book=zeros(2000,2000);
%��һ�д�x���꣬�ڶ��д�y����
que=zeros(4000000,2);%ռ�õ��ڴ�ϴ������ַ�������ʾ����
next=[0,1;1,0;0,-1;-1,0];

x_max=size(data,1);
y_max=size(data,2);


head=1;
tail=1;
que(tail,1)=start_x;
que(tail,2)=start_y;
tail=tail+1;
book(start_x,start_y)=1;
sum=1;

    while head<tail
        for k=1:4
            tx=que(head,1)+next(k,1);
            ty=que(head,2)+next(k,2);
            
            if(tx<1||tx>x_max||ty<1||ty>y_max)
                continue;    
            end
            
            if (data(tx,ty)>(data(start_x,start_y)/2)&&book(tx,ty)==0)
                sum=sum+1;
                data(tx,ty)=data(start_x,start_y);
                book(tx,ty)=1;
                que(tail,1)=tx;
                que(tail,2)=ty;
                tail=tail+1;
            end
            
        end
        head=head+1;

    end
    
    
    figure;
    surf(X,Y,data(x,y));
    title('�����');

end

