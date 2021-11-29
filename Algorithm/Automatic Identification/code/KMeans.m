
%N������һ���ֶ�����
%data������Ĳ��������ŵ�����
%u��ÿһ�������
%re�Ƿ��صĴ������ŵ�����
function [u re]=KMeans(data,N,initial_u)   
    [m n]=size(data);   %m�����ݸ�����n������ά��
%     ma=zeros(n);        %ÿһά������
%     mi=zeros(n);        %ÿһά��С����
    u=zeros(N,n);       %�����ʼ�������յ�����ÿһ�������λ��
%     for i=1:n
%        ma(i)=max(data(:,i));    %ÿһά������
%        mi(i)=min(data(:,i));    %ÿһά��С����
%        for j=1:N
%             u(j,i)=ma(i)+(mi(i)-ma(i))*rand();  %�����ʼ��������������ÿһά[min max]�г�ʼ����Щ
%        end      
%     end
    
        %����ֵ
    u=initial_u;
   
    while 1
        pre_u=u;            %��һ����õ�����λ��
        for i=1:N
            tmp{i}=[];      % ��ʽһ�е�x(i)-uj,Ϊ��ʽһʵ����׼��
            for j=1:m
                tmp{i}=[tmp{i};data(j,:)-u(i,:)];
            end
        end
        
        quan=zeros(m,N);
        for i=1:m        %��ʽһ��ʵ��
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
        
        for i=1:N            %��ʽ����ʵ��
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


% figure;
% plot(data_sort(idx==1,1),data_sort(idx==1,2),'r.','MarkerSize',12)
% hold on
% plot(data_sort(idx==2,1),data_sort(idx==2,2),'b.','MarkerSize',12)
% hold on
% plot(data_sort(idx==3,1),data_sort(idx==3,2),'g.','MarkerSize',12)
% hold on
% plot(data_sort(idx==4,1),data_sort(idx==4,2),'m.','MarkerSize',12)
% hold on
% plot(C(1,1),C(1,2),'kx','MarkerSize',15,'LineWidth',3);
% plot(C(2,1),C(2,2),'kx','MarkerSize',15,'LineWidth',3);
% plot(C(3,1),C(3,2),'kx','MarkerSize',15,'LineWidth',3);
% plot(C(4,1),C(4,2),'kx','MarkerSize',15,'LineWidth',3);
