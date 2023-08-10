//index.js
//获取应用实例
import {
  http
} from '../../utils/http';
const app = getApp()
Page({
  data: {
    uptownList: [],
    uptownIndex: -1,
    deviceMac: '',
    deviceUuid: '',
    deviceIndex: -1,
    filterNoName: false, // 是否过滤未知设备 true过滤
    connectedDeviceId: "", // 已连接设备uuid  
    services: [], // 连接设备的服务  
    serviceId: "", // 蓝牙设备服务的 uuid
    devicesList: [], // 找到的设备列表
    characteristics: "", // 连接设备的状态值  
    writeServicweId: "", // 可写服务uuid  
    writeCharacteristicsId: "", // 可写特征值uuid  
    readServicweId: "", // 可读服务uuid  
    readCharacteristicsId: "", // 可读特征值uuid  
    configServicweId: "", // 空中配置服务uuid  
    configCharacteristicsId: "", // 空中配置特征值uuid
    characteristics1: "", // 连接设备的状态值
    postData:2022,
    getData: '',
    status: "等待初始化"
  },
  
  onLoad: function () {
    this.getUptownList();
  },

  /** 页面隐藏 */
  onHide: function () {
    this.onStopDevice();
  },

  /** 页面卸载 */
  onUnload: function () {
    this.onStopDevice();
  },

  /** 小区列表 */
  getUptownList: function () {
    let that = this
    wx.get('/device/getUptownList').then(res => {
      if (res.code == 10000) {
        that.setData({
          uptownList: res.data,
        })
      } else that.showMsg(res.msg ? res.msg : '没有获取到小区数据');
    })
  },
 

  /** 获取设备MAC地址 */
  _getMAC: function (advertisData) {
    var buff = advertisData.slice(2, 8);
    var arrayBuff = Array.prototype.map.call(new Uint8Array(buff), x => ('00' + x.toString(16)).slice(-2)).join(':');
    return arrayBuff.toUpperCase();
  },

  //启动蓝牙 （初始化蓝牙模块-->获取本机蓝牙适配器状态-->开始搜寻附近的蓝牙外围设备-->监听寻找到新设备的事件）
  startBluetooth: function () {
    var that = this;
    that.setData({
      status: "初始化蓝牙模块"
    })
    //初始化蓝牙模块
    wx.openBluetoothAdapter({
      success(res) {
        //获取本机蓝牙适配器状态
        wx.getBluetoothAdapterState({
          success(res) {
            console.log('获取本机蓝牙适配器状态', res)
            //蓝牙适配器是否可用
            if (res.available) {
              //是否正在搜索设备
              if (res.discovering) {
                //监听寻找到新设备的事件
                that.onBluetoothDeviceFound()
              } else {
                //开始搜寻附近的蓝牙外围设备
                wx.startBluetoothDevicesDiscovery({
                  allowDuplicatesKey: true, //允许重复上报同一设备,更新RSSI值
                  success(res) {
                    that.onBluetoothDeviceFound()
                  },
                  fail(res) {
                    that.setData({
                      status: '搜寻失败：' + res.errCode
                    })
                    that.showMsg("搜寻失败，错误信息:" + res.errCode + " 错误编码:" + res.errCode)
                  }
                })
              }
            } else {
              that.setData({
                status: "蓝牙适配器不可用"
              })
              that.showMsg("蓝牙适配器不可用")
            }
          }
        })
      },
      fail(res) {
        // that.showMsg("初始化失败;错误信息:" + res.errCode + " 错误编码:" + res.errCode)
        if (res.errCode == 10001 || res.errCode == 10002) {
          //没有开启蓝牙
          that.setData({
            status: '初始化失败：请开启蓝牙',
          })
        } else {
          that.setData({
            status: "初始化失败：" + res.errMsg
          })
        }
      }
    })
  },

  //监听寻找新设备
  onBluetoothDeviceFound() {
    let that = this
    that.setData({
      status: "搜寻中......"
    })
    wx.onBluetoothDeviceFound(function (res) {
      // console.log('监听到的设备', res);
      res.devices.forEach(device => {
        console.log('设备信息', device);
        if (!device.name && !device.localName) {
          if (that.data.filterNoName) {
            return
          }
          device.name = '未知设备'
          return
        }
        const foundDevices = that.data.devicesList
        const idx = that.inArray(foundDevices, 'deviceId', device.deviceId)
        const data = {}
        if (idx === -1) {
          data[`devicesList[${foundDevices.length}]`] = device
        } else {
          data[`devicesList[${idx}]`] = device
        }
        that.setData(data)
        //是否有已连接设备
        wx.getConnectedBluetoothDevices({ //根据 uuid 获取处于已连接状态的设备
          services: device.advertisServiceUUIDs,
          success: function (res) {
            console.log('已连接的蓝牙设备:', res)
            console.log(JSON.stringify(res.devices));
            that.setData({
              connectedDeviceId: res.deviceId
            })
          }
        })
      })
    })
  },
  // 停止搜索
  onStopDevice: function () {
    wx.stopBluetoothDevicesDiscovery({
      success: (res) => {
        this.setData({
          status: '停止搜索...',
        })
      },
    })
  },

  /** 选择设备 */
  selectDev: function (e) {
    let {
      index,
      id,
    } = e.currentTarget.dataset;
    console.log("选择状态", e);
    let deviceInfo = this.data.devicesList[index];
    console.log(deviceInfo);
    console.log('解析mac', this._getMAC(deviceInfo.advertisData));
    console.log('uuid', deviceInfo.advertisServiceUUIDs[0]);
    this.setData({
      deviceMac: this._getMAC(deviceInfo.advertisData),
      deviceUuid: deviceInfo.advertisServiceUUIDs[0],
      deviceName: deviceInfo.name && deviceInfo.localName ? deviceInfo.name : '',
      deviceId: id,
      deviceIndex: index,
    })
  },

  //连接设备  
  connectTO: function () {
    var that = this;
    that.setData({
      status: "停止搜索..."
    })
    that.onStopDevice();
    wx.offBluetoothDeviceFound({ //先停止搜索周边设备
      success: function (res) {
        console.log('取消监听寻找到新设备的事件', res)
      }
    })
    wx.showLoading({
      title: '连接蓝牙设备中...',
    })
    if (!that.data.deviceId) {
      that.showMsg('没有选择蓝牙设备');
      return
    }
    that.setData({
      status: '连接设备中...',
    })
    wx.createBLEConnection({ //若小程序在之前已有搜索过某个蓝牙设备，并成功建立链接，可直接传入之前搜索获取的deviceId直接尝试连接该设备，无需进行搜索操作。
      deviceId: that.data.deviceId,
      success: function (res) {
        console.log('连接成功:', res)
        that.setData({
          //currentTarget: 事件绑定的元素
          connectedDeviceId: that.data.deviceId,
          status: "已连接" + that.data.deviceId,
        })
        that.getServices();
      },
      fail: function () {
        console.log("连接失败");
      },
      complete: function () {
        wx.hideLoading();
        console.log('已连接设备ID：' + that.data.connectedDeviceId);
        console.log("调用结束");
      }
    })
  },

  //获取已连接设备服务
  getServices: function () {
    var that = this;
    wx.getBLEDeviceServices({ //获取在小程序蓝牙模块生效期间所有已发现的蓝牙设备，包括已经和本机处于连接状态的设备
      // 这里的 deviceId 需要在上面的 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取  
      deviceId: that.data.connectedDeviceId,
      success: function (res) {
        //console.log('获取蓝牙设备所有服务成功：', res);
        that.data.services = res.services
        console.log('获取蓝牙设备所有服务成功：', that.data.services);
        that.setData({
          serviceId: that.data.services[0].uuid
        }, () => {
          console.log("服务uuid:", that.data.serviceId)
          //获取蓝牙设备某个服务中所有特征值(characteristic)。
          wx.getBLEDeviceCharacteristics({
            // 这里的 deviceId 需要在上面的 getBluetoothDevices 或 onBluetoothDeviceFound 接口中获取  
            deviceId: that.data.connectedDeviceId,
            // 这里的 serviceId 需要在上面的 getBLEDeviceServices 接口中获取  
            serviceId: that.data.serviceId, //-----注意是that.data.services[0].uuid
            success: function (res) {
              console.log('获取所有uuid:', res)

              var _suuid = that.data.services[0].uuid; // 服务uuid
              console.log('设备服务uuid:', _suuid)
              for (var i = 0; i < res.characteristics.length; i++) {
                var _tuuid = res.characteristics[i].uuid; //通道uuid
                var _notify = res.characteristics[i].properties.notify;
                var _write = res.characteristics[i].properties.write;
                var _read = res.characteristics[i].properties.read;
                //空中配置 FFF1 {notify: true, write: false, indicate: false, read: true}
                if (_notify) { //注意characteristic(特征值)信息,properties对象
                  that.setData({
                    readServicweId: _suuid,
                    readCharacteristicsId: _tuuid,
                    writeServicweId: _suuid,
                    writeCharacteristicsId: _tuuid,
                    configServicweId: _suuid,
                    configCharacteristicsId: _tuuid,
                  })
                  console.log("空中配置通道uuid", _tuuid)
                }
                //手机-->设备 写数据 FFF2 {notify: false, write: true, indicate: false, read: true}
                else if (_notify == false && _write && _read) {
                  that.setData({
                    writeServicweId: _suuid,
                    writeCharacteristicsId: _tuuid,
                  })
                  console.log("发数据通道uuid", _tuuid)
                  //设备-->手机 收数据 FFF3 {notify: true, write: true, indicate: false, read: true}
                } else if (_notify && _write == false && _read) {
                  that.setData({
                    readServicweId: _suuid,
                    readCharacteristicsId: _tuuid,
                  })
                  console.log("收数据通道uuid", _tuuid)
                }
              }
              // 启动监听低功耗蓝牙设备的特征值变化事件
              that.startBletNotify();
            },
            fail: function () {
              console.log("获取连接设备的所有特征值：", res);
            },
            // complete: function () {
            //   console.log("complete!");
            // }
          })
        })

      }
    })
  },

  //启用监听
  startBletNotify: function () {
    var that = this;
    wx.notifyBLECharacteristicValueChange({
      state: true, // 启用监听 notify
      //type:"notification",
      deviceId: that.data.connectedDeviceId,
      serviceId: "0000FFE0-0000-1000-8000-00805F9B34FB",
      characteristicId: "0000FFE1-0000-1000-8000-00805F9B34FB",
      success: function (res) {
        console.log('启用蓝牙监听：', res);
        that.setData({
          status: '启用监听',
        })
        //通知远程服务器蓝牙就绪
        /*
        wx.request({
            url: 'http://www.xxxx.com/test1.php', //仅为示例，并非真实的接口地址
            data: {x: '',y: ''},
            header: {'content-type': 'application/json' // 默认值},
            success (res) {
              console.log(res.data)
            }
        })
        */
      },
      fail: function (res) {
        console.log('启用蓝牙监听功能失败！', res);
        that.setData({
          status: '启用蓝牙监听功能失败！',
        })
      },
      complete: function () {
        console.log('监听设备ID：', that.data.connectedDeviceId)
        console.log('监听服务uuid：', that.data.readServicweId)
        console.log('监听通道uuid：', that.data.readCharacteristicsId)
      }
    })
  },

  //接收设备推送数据
  getMessagesData: function () {
    var that = this;
    // 这里的回调可以获取到 write 导致的特征值改变  
    wx.onBLECharacteristicValueChange(function (res) {
      console.log('监听接收数据：', res);
      console.log(that.ab2str(res.value))
      let hexStr = that.ab2hex(res.value)
      console.log('接收数据(十六进制)：', hexStr);
      hexStr = that.hexCharCodeToStr(hexStr);
      console.log('接收数据(十六进制转ASCII)：', hexStr);
      that.setData({
        getData: hexStr
      })
    })
  },


  /** 设置发送数据 */
  sendData: function (e) {
    let value = e.detail.value;
    // console.log('向设备发送字符：', value);
    // console.log('字符串内容', this.string2buffer(value));
    this.setData({
      postData: value,
    })
  },

  /**
   * 设备验证
   */
  setValidte:function() {
    let that = this
    that.setData({
      postData:'4',
    },() =>{
      that.sendMessages();
    })
  },
  setValidte1:function() {
    let that = this
    that.setData({
      postData:'1',
    },() =>{
      that.sendMessages();
    })
  },
  setValidte2:function() {
    let that = this
    that.setData({
      postData:'2',
    },() =>{
      that.sendMessages();
    })
  },
  setValidte0:function() {
    let that = this
    that.setData({
      postData:'0',
    },() =>{
      that.sendMessages();
    })
  },
  setValidteA:function() {
    let that = this
    that.setData({
      postData:'A',
    },() =>{
      that.sendMessages();
    })
  },
  setValidteT:function() {
    let that = this
    that.setData({
      postData:'T',
    },() =>{
      that.sendMessages();
    })
  },
  setValidteS:function() {
    let that = this
    that.setData({
      postData:'S',
    },() =>{
      that.sendMessages();
    })
  },
  setValidte3:function() {
    let that = this
    that.setData({
      postData:'3',
    },() =>{
      that.sendMessages();
    })
  },
  /**
   * 发送OPEN
   */
  sendOpen:function() {
    let that = this
    that.setData({
      postData:'6',
    },() =>{
      that.sendMessages();
    })
  },
  setValidte7:function() {
    let that = this
    that.setData({
      postData:'7',
    },() =>{
      that.sendMessages();
    })
  },
  setValidte8:function() {
    let that = this
    that.setData({
      postData:'8',
    },() =>{
      that.sendMessages();
    })
  },  setValidte9:function() {
    let that = this
    that.setData({
      postData:'9',
    },() =>{
      that.sendMessages();
    })
  },  setValidtea:function() {
    let that = this
    that.setData({
      postData:'a',
    },() =>{
      that.sendMessages();
    })
  },
  setValidtex:function() {
    let that = this
    that.setData({
      postData:'x',
    },() =>{
      that.sendMessages();
    })
  },
  //发送消息
  sendMessages: function (e) {
    var that = this;
    console.log('发送数据内容', that.data.postData);
    var buffer = that.string2buffer(this.data.postData);
    wx.writeBLECharacteristicValue({
      deviceId: that.data.connectedDeviceId,
      serviceId: "0000FFE0-0000-1000-8000-00805F9B34FB",
      characteristicId: "0000FFE1-0000-1000-8000-00805F9B34FB",
      value: buffer,
      success: function (res) {
        console.log('设备ID：', that.data.connectedDeviceId)
        console.log('写服务uuid:')
        console.log('写通道uuid:')
        console.log('发送成功：', res)
        that.getMessagesData();
      },
      fail: function (rev) {
        console.log('失败了')
        console.log(rev)
      },
      complete: function (rr) {
        console.log("操作执行完成")
        console.log(rr);
      }
    })
  },

  //接收消息  
  receiveMessages: function () {
    var that = this;
    //取消监听改为手动读取
    wx.offBLECharacteristicValueChange();
    that.getMessagesData();
    function ab2hex(buffer) {
        let hexArr = Array.prototype.map.call(
          new Uint8Array(buffer),
          function(bit) {
            return ('00' + bit.toString(16)).slice(-2)
          }
        )
        return hexArr.join('');
    }
    // 必须在这里的回调才能获取  
    wx.onBLECharacteristicValueChange(function (res) {
        console.log('接受数据:', characteristic);
        let hex = Array.prototype.map.call(new Uint8Array(characteristic.value), x => ('00' + x.toString(16)).slice(-2)).join('');
        console.log('hex',hex)
        console.log('接受内容：',ab2hex(characteristic.value))
    })
    console.log('接收消息服务uuid', "0000FEE7-0000-1000-8000-00805F9B34FB");
    console.log('接收消息通道uuid：',"0000FEC7-0000-1000-8000-00805F9B34FB");
    wx.readBLECharacteristicValue({
      deviceId: that.data.connectedDeviceId,
      serviceId: that.data.readServicweId,
      characteristicId: that.data.readCharacteristicsId,
      success: function (res) {
        console.log('接收消息成功', res);
      }
    })
  },

  //断开连接  
  closeBluetooth: function () {
    var that = this;
    wx.closeBLEConnection({
      deviceId: that.data.connectedDeviceId,
      success: function (res) {
        console.log('断开设备连接: ', that.data.connectedDeviceId)
        console.log(res)
        that.setData({
          connectedDeviceId: '', //currentTarget: 事件绑定的元素
          status: "已断开连接",
        })
      }
    })
  },

  // 判断是否存在数组内
  inArray(arr, key, val) {
    for (let i = 0; i < arr.length; i++) {
      if (arr[i][key] === val) {
        return i;
      }
    }
    return -1;
  },

  //将字符串转换成ArrayBufer
  string2buffer(str) {
    if (!str) return;
    var val = "";
    for (var i = 0; i < str.length; i++) {
      val += str.charCodeAt(i).toString(16);
    }
    str = val;
    val = "";
    let length = str.length;
    let index = 0;
    let array = []
    while (index < length) {
      array.push(str.substring(index, index + 2));
      index = index + 2;
    }
    val = array.join(",");
    // 将16进制转化为ArrayBuffer
    return new Uint8Array(val.match(/[\da-f]{2}/gi).map(function (h) {
      return parseInt(h, 16)
    })).buffer
  },

  //arraybuffer 转16进制
  ab2hex(buffer) {
    let hexArr = Array.prototype.map.call(
      new Uint8Array(buffer),
      function (bit) {
        return ('00' + bit.toString(16)).slice(-2)
      }
    )
    return hexArr.join('');
  },

  //arraybuffer转字符串
  ab2str(buf) {
    return String.fromCharCode.apply(null, new Uint16Array(buf));
  },

  hexCharCodeToStr(hexCharCodeStr) {
    var trimedStr = hexCharCodeStr.trim();
    var rawStr =
      trimedStr.substr(0, 2).toLowerCase() === "0x" ?
      trimedStr.substr(2) :
      trimedStr;
    var len = rawStr.length;
    if (len % 2 !== 0) {
      alert("Illegal Format ASCII Code!");
      return "";
    }
    var curCharCode;
    var resultStr = [];
    for (var i = 0; i < len; i = i + 2) {
      curCharCode = parseInt(rawStr.substr(i, 2), 16); // ASCII Code Value
      resultStr.push(String.fromCharCode(curCharCode));
    }
    return resultStr.join("");
  },

  showMsg: function (msg) {
    wx.showToast({
      icon: 'none',
      title: msg ? msg : '错误消息',
      duration: 2000,
    })
  },
})