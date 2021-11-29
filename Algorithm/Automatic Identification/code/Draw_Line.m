%���������յ㣬���һ��ֱ��
function []=Draw_Line(x_start,y_start,x_end,y_end,data)
    x_diff=x_end-x_start;
    y_diff=y_end-y_start;

    %�ò��������Ϊ��׼
    if abs(x_diff)>abs(y_diff)
        time=abs(x_diff);
        per_x_go=abs(x_diff)/x_diff;
        per_y_go=y_diff/abs(x_diff);  
    else
        time=abs(y_diff);
        per_x_go=x_diff/abs(y_diff);     
        per_y_go=abs(y_diff)/y_diff;
    end
    
    for i=1:time
        current_x=uint16(x_start+i*per_x_go);
        current_y=uint16(x_start+i*per_y_go);
        data(current_x,current_y,1)=0;
        data(current_x,current_y,2)=0;
        data(current_x,current_y,3)=0;
    end
    
    %��ʾѹ����ͼƬ
    figure;
    imshow(data);  
end


