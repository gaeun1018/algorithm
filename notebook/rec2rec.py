import os
import os.path as osp
import sys
import mxnet as mx
from mxnet import ndarray as nd
import argparse
import numpy as np
from mxnet import io
from mxnet import recordio
import insightface
import numbers
from argparse import ArgumentParser, Namespace
import json

from insightface.app import MaskRenderer
from insightface.data.rec_builder import RecBuilder
from abc import ABC, abstractmethod
from argparse import ArgumentParser


class BaseInsightFaceCLICommand(ABC):
    @staticmethod
    @abstractmethod
    def register_subcommand(parser: ArgumentParser):
        raise NotImplementedError()

    @abstractmethod
    def run(self):
        raise NotImplementedError()
        
class RecBuilder():
    def __init__(self, path, result, mask, image_size=(112, 112)):
        self.path = path
        self.image_size = image_size
        self.last_label = 0
        self.widx = 1
        self.meta = []
        self.max_label = -1
        if not os.path.exists(path):
            os.makedirs(path)
#         if mask:
        self.writer = mx.recordio.MXIndexedRecordIO(
        os.path.join(path, result+'.idx'), os.path.join(path, result+'.rec'),
        'w')
#         else:
#             self.writer = mx.recordio.MXIndexedRecordIO(
#             os.path.join(path, 'complete.idx'), os.path.join(path, 'complete.rec'),
#             'w')
        self.label_stat = [-1, -1]
        self.identities = []
        self.labelnum = 0
    
    def add_exist(self):
        path_imgrec = osp.join(self.path, 'train.rec')
        path_imgidx = osp.join(self.path, 'train.idx')
        imgrec = mx.recordio.MXIndexedRecordIO(path_imgidx, path_imgrec, 'r')
        
        s = imgrec.read_idx(0)
        
        header, _ = recordio.unpack(s)
        label = 0
        
        if header.flag > 0:
            if len(header.label)==2:
                imgidx = list(range(1, int(header.label[0])))
            else:
                imgidx = list(list(imgrec.keys))
        else:
            imgidx = list(list(imgrec.keys))
        
        print('total exist : ',len(imgidx))
        idflag = [self.widx, -1]
        for i, idx in enumerate(imgidx):
#             if i==idx:
#                 idx+=1
            if i%10000==0:
                print('a processing', i, idx)
            s = imgrec.read_idx(idx)
            
            header, _ = mx.recordio.unpack(s)
            label = header.label
            if not isinstance(label, numbers.Number):
                label = label[0]
            image_meta = {'image_index': idx, 'image_classes': [label]}
            self.meta.append(image_meta)
            if label != self.last_label:
                idflag[1] = self.widx
                self.identities.append(idflag)
                if self.label_stat[0] < 0:
                    self.label_stat = [label-1, label-1]
                self.last_label = label
                idflag = [self.widx, -1]
            
            self.writer.write_idx(self.widx, s)
            self.widx += 1
        idflag[1] = self.widx
        self.identities.append(idflag)
        if self.label_stat[0] < 0:
            self.label_stat = [label, label]
        else:
            self.label_stat[0] = min(self.label_stat[0], label)
            self.label_stat[1] = max(self.label_stat[1], label)
        self.last_label = label
        self.labelnum = self.last_label
        
    def add(self, label, imgs):
        label = label + self.labelnum+1
        #img should be BGR
        assert label >= 0
        assert label > self.last_label
        assert len(imgs) > 0
        idflag = [self.widx, -1]
        
        for img in imgs:
            idx = self.widx
            self.widx += 1
            image_meta = {'image_index': idx, 'image_classes': [label]}
            self.meta.append(image_meta)
            header = mx.recordio.IRHeader(0, label, idx, 0)
            if isinstance(img, np.ndarray):
                s = mx.recordio.pack_img(header,
                                         img,
                                         quality=95,
                                         img_fmt='.png')
            else:
                s = mx.recordio.pack(header, img)
            self.writer.write_idx(idx, s)
        idflag[1] = self.widx
        self.identities.append(idflag)
        if self.label_stat[0] < 0:
            self.label_stat = [label, label]
        else:
            self.label_stat[0] = min(self.label_stat[0], label)
            self.label_stat[1] = max(self.label_stat[1], label)
        self.last_label = label
    
    
    def add_image(self, img, label):
        #!!! img should be BGR!!!!
        #assert label >= 0
        #assert label > self.last_label
        idx = self.widx
        header = mx.recordio.IRHeader(0, label, idx, 0)
        if isinstance(label, list):
            idlabel = label[0]
        else:
            idlabel = label
        image_meta = {'image_index': idx, 'image_classes': [idlabel]}
        if isinstance(img, np.ndarray):
            s = mx.recordio.pack_img(header,img,quality=95,img_fmt='.jpg')
        else:
            s = mx.recordio.pack(header, img)
        self.writer.write_idx(idx, s)
        self.meta.append(image_meta)
        self.max_label = max(self.max_label, idlabel)
    
    def add_param(self,path):
        tool = MaskRenderer()
        tool.prepare(ctx_id=0, det_size=(128,128))
        path_imgrec = osp.join(path, 'train.rec')
        path_imgidx = osp.join(path, 'train.idx')
        imgrec = mx.recordio.MXIndexedRecordIO(path_imgidx, path_imgrec, 'r')
        
        s = imgrec.read_idx(0)
        
        header, _ = recordio.unpack(s)
        
        if header.flag > 0:
            if len(header.label)==2:
                imgidx = np.array(range(1, int(header.label[0])))
            else:
                imgidx = np.array(list(self.imgrec.keys))
        else:
            imgidx = np.array(list(self.imgrec.keys))
        
        stat = [0,0]
        idflag = [self.widx, -1]
        
        
        label = 0
        
        for i, idx in enumerate(imgidx):
            if i%10000==0:
                print('a processing', i, idx)
            s = imgrec.read_idx(idx)
            header, img = mx.recordio.unpack(s)
            label = header.label
            if not isinstance(label, numbers.Number):
                label = label[0]
            
            if label+self.labelnum != self.last_label:
                idflag[1] = self.widx
                self.identities.append(idflag)
                if self.label_stat[0] < 0:
                    self.label_stat = [label-1, label-1]
                self.last_label = label+self.labelnum
                idflag = [self.widx, -1]
            sample = mx.image.imdecode(img).asnumpy()
            bgr = sample[:,:,::-1]
            params = tool.build_params(bgr)
            stat[1] += 1
            if params is None:
                wlabel = [label] + [-1.0]*236
                stat[0] += 1
            else:
                mask_label = tool.encode_params(params)
                wlabel = [label, 0.0]+mask_label # 237 including idlabel, total mask params size is 235
                if id==0:
                    print('param size:', len(mask_label), len(wlabel), label)
            assert len(wlabel)==237
            self.add_image(img, wlabel)
            self.widx += 1
        idflag[1] = self.widx
        self.identities.append(idflag)
        if self.label_stat[0] < 0:
            self.label_stat = [label, label]
        else:
            self.label_stat[0] = min(self.label_stat[0], label+self.labelnum+1)
            self.label_stat[1] = max(self.label_stat[1], label+self.labelnum+1)
        print('finished on', self.path+'complete_m.rec, failed:', stat[0])
            
        
    def close(self):
        id_idx = self.widx
        with open(osp.join(self.path, 'train.meta'), 'w') as pfile:
            for i in range(len(self.meta)):
                pfile.write(str(self.meta[i]))
#             pickle.dump(self.meta, pfile, protocol=pickle.HIGHEST_PROTOCOL)
        for id_flag in self.identities:
            idx = self.widx
            self.widx += 1
            _header = mx.recordio.IRHeader(0, id_flag, idx, 0)
            s = mx.recordio.pack(_header, b'')
            self.writer.write_idx(idx, s)
        print('id0:', (id_idx, self.widx))
        idx = 0
        _header = mx.recordio.IRHeader(0, (id_idx, self.widx), 0, 1)
        s = mx.recordio.pack(_header, b'')
        self.writer.write_idx(0, s)
        print('label stat:', self.label_stat)
        with open(os.path.join(self.path, 'property'), 'w') as f:
            f.write("%d,%d,%d\n" % (self.label_stat[1] + 1, self.image_size[0],
                                    self.image_size[1]))


def main(args):
    recrec = RecBuilder(args.input,args.result,args.mask)
    recrec.add_exist()
    
    if args.mask:
        recrec.add_param(args.plus)
        recrec.close()
        return
    
#     path_imgrec = osp.join(args.input, 'train.rec')
#     path_imgidx = osp.join(args.input, 'train.idx')
#     imgrec = mx.recordio.MXIndexedRecordIO(path_imgidx, path_imgrec, 'r')  # pylint: disable=redefined-variable-type
        
        
    path_imgrec2 = osp.join(args.plus, 'train.rec')
    path_imgidx2 = osp.join(args.plus, 'train.idx')
    imgrec2 = mx.recordio.MXIndexedRecordIO(path_imgidx2, path_imgrec2, 'r')
    
    
        
#     s = imgrec.read_idx(0)
    s2 = imgrec2.read_idx(0)
    
    
#     header, _ = recordio.unpack(s)
#     if header.flag > 0:
#         header0 = (int(header.label[0]), int(header.label[1]))
#         imgidx = list(range(1, int(header.label[0])))
#     else:
#         imgidx = list(imgrec.keys)
            
    header2, _ = recordio.unpack(s2)
    if header2.flag > 0:
        header1 = (int(header2.label[0]), int(header2.label[1]))
        imgidx2 = list(range(1, int(header2.label[0])))
    else:
        imgidx2 = list(imgrec2.keys)
    
    imglist = []
    lastlabel = 0
    print('total plus : ', len(imgidx2))
    for i, idx in enumerate(imgidx2):
        if i%10000==0:
            print('processing', i, idx)
        s = imgrec2.read_idx(idx)
        header, img = mx.recordio.unpack(s)
        label = header.label
        if not isinstance(label, numbers.Number):
            label = label[0]
        if label != lastlabel:
            recrec.add(lastlabel,imglist)
            lastlabel = label
            imglist = []
        imglist.append(img)
        
    recrec.add(lastlabel,imglist)
    recrec.close()
    
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='convert rec to shuffled rec')
    # general
    parser.add_argument('--input', default='', type=str, help='')
    parser.add_argument('--plus', default='', type=str, help='')
    parser.add_argument('--result', default='', type=str, help='')
    parser.add_argument('--mask', default='', action='store_true', help='')
    
    args = parser.parse_args()
    main(args)