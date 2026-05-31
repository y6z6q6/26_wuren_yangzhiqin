import torch
import copy
import os
from torch import nn
from torchvision import transforms,datasets
from torch.utils.data.dataloader import DataLoader
from torch.utils.data import random_split
import torch.optim as optim
import torch.nn.functional as F
#from torchinfo import summary
import os

class mixed_net(nn.Module):
    def __init__(self):
        super().__init__()
        self.juanji1=nn.Conv2d(3,16,kernel_size=3,padding=1,stride=1)
        self.juanji2=nn.Conv2d(16,32,kernel_size=3,padding=1,stride=1)
        self.chihua=nn.MaxPool2d(2,2)#每次2×2的尺寸，每次向前走2格
        self.quanlianjie1=nn.Linear(16*16*32,128)#池化两次，尺寸64——>尺寸16，每个格子包含32个信息；把这么多信息总结得到128个信息
        self.quanlianjie2=nn.Linear(128,3)#把128个特征进一步变成3种颜色的概率
    def forward(self,x):
        x=self.chihua(F.relu(self.juanji1(x)))#把输入的图片卷积后加入激活函数，使其特征具有非线性化的特点
        x=self.chihua(F.relu(self.juanji2(x)))
        x=x.flatten(1)#把特征变成一维，用于全连接层的操作
        x=F.relu(self.quanlianjie1(x))#继续非线性化
        x=self.quanlianjie2(x)
        return x#返回的就是每种颜色的概率，最大的就是结果

if __name__ == "__main__":
    #图像转换
    transforms = transforms.Compose(
        [
            transforms.Resize([64, 64]),
            transforms.ToTensor(),
            transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
        ]
    )
    
    #超参数设置
    BATCH_SIZE = 50
    EPOCH = 80
    train_ratio = 0.7

    #加载数据
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))
    data_root = os.path.join(BASE_DIR, "dataset", "train")
    full_dataset = datasets.ImageFolder(root=data_root, transform=transforms)

    print(f"总图片数量: {len(full_dataset)}")
    print(f"标签对应的ID: {full_dataset.class_to_idx}")

    # 按7:3划分数据集
    train_size = int(train_ratio * len(full_dataset))
    val_size = len(full_dataset) - train_size
    trainset, testset1 = random_split(full_dataset, [train_size, val_size])

    print(f"训练集图片数量: {len(trainset)}")
    print(f"测试集1图片数量: {len(testset1)}")
    
    train_loader = DataLoader(trainset, batch_size=BATCH_SIZE, shuffle=True, pin_memory=True)
    test_loader1 = DataLoader(testset1, batch_size=BATCH_SIZE, shuffle=True, pin_memory=True)
    #test_loader2 = DataLoader(testset2, batch_size=BATCH_SIZE, shuffle=True, pin_memory=True)

    #创建网络
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")#有独显就用独显，否则用cpu
    net = mixed_net().to(device)
    
    #打印网络信息
    #summary(net, input_size=(1, 3, 64, 64), device=device)
    #print(f'标签对应的ID: {trainset.class_to_idx}')

    #设置优化器、损失函数
    criterion = nn.CrossEntropyLoss()#计算真实值和预测值的差值用来优化权重参数
    optimizer =optim.Adam(net.parameters(), lr=0.001)#学习率为0.001，过大会震荡，过小训练速度会很慢

    #开始训练
    #PATH= r"C:\Users\杨智钦\Desktop\DL\pth"#模型路径
    #os.makedirs("pth", exist_ok=True)#创建保存模型的文件夹
    print("Start")
    max_correct=0
    for epoch in range(EPOCH):
        train_loss = 0.0#每一轮开始误差重置
        #print(epoch)
        
        for batch_id, (datas, labels) in enumerate(train_loader):#从头开始导入训练样本
            datas, labels = datas.to(device), labels.to(device)

            optimizer.zero_grad()#清空上一轮的梯度

            outputs = net(datas)#把每一轮新数据进行处理

            loss = criterion(outputs, labels)#计算差值

            loss.backward()#反推计算梯度

            optimizer.step()#用规定的Adam来优化参数

            train_loss += loss.item()#累加差值
        print(#打印测试数据
                f"epoch:{epoch + 1}\tbatch_id:{batch_id + 1}\taverage_loss:{(train_loss / len(train_loader.dataset)):.8f}\t"
            )
        if epoch > 10 and (epoch + 1) % 5 == 0:
            model=copy.deepcopy(net)
            model.eval()#变为评估模式
            model.to(device)

            #限定保存条件
            #清零
            correct1 = 0
            correct2 = 0
            total1 = 0
            total2 = 0

            #分别测试两个数据集
            with torch.no_grad():#不计算梯度
                for i ,(datas1, labels1) in enumerate(test_loader1):#测试test1里的数据
                    datas1, labels1 = datas1.to(device), labels1.to(device)
                    output_test1 = model(datas1)#得到测试数据
                    _, predicted1 = torch.max(output_test1.data, dim=1)#找到最大概率的颜色，保存下标
                    total1 += predicted1.size(0)#测试总数
                    correct1 += (predicted1 == labels1).sum()#测试正确总数

                #打印消息
                c1 = 0
                #c2 = 0
               # c2 = correct2 / total2 * 100
                c1 = correct1 / total1 * 100
                print(#打印测试数据
                #    f"epoch:{epoch + 1}\tbatch_id:{batch_id + 1}\taverage_loss:{(train_loss / len(train_loader.dataset)):.5f}\t"
                    f"correct1:{c1:.4f}%"
                )
                #保存每一次测试的模型，用于检测
                base1_dir=os.path.dirname(os.path.abspath(__file__))
                PATH=os.path.join(base1_dir, f"./model_epoch_{epoch}.pth")
                torch.save(net.state_dict(), PATH)
                if (c1 > max_correct):#要是正确率高于目前最高正确率，就保存当前的模型
                    max_correct = c1
                    base_dir=os.path.dirname(os.path.abspath(__file__))
                    MAX_PATH=os.path.join(base_dir, "model_best.pth")
                    print(f"save {MAX_PATH}")
                    torch.save(net.state_dict(),MAX_PATH)        

