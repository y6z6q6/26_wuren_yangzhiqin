import torch
import torch.nn as nn

class mixed_net(nn.Module):
    def __init__(self):
        super().__init__()
        self.juanji1 = nn.Conv2d(3,16,kernel_size=3,padding=1,stride=1)
        self.juanji2 = nn.Conv2d(16,32,kernel_size=3,padding=1,stride=1)
        self.chihua = nn.MaxPool2d(2,2)
        self.quanlianjie1 = nn.Linear(16*16*32,128)
        self.quanlianjie2 = nn.Linear(128,3)

    def forward(self,x):
        x = self.chihua(torch.relu(self.juanji1(x)))
        x = self.chihua(torch.relu(self.juanji2(x)))
        x = x.flatten(1)
        x = torch.relu(self.quanlianjie1(x))
        x = self.quanlianjie2(x)
        return x