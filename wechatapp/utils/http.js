const {
    baseUrl,
} = getApp().globalData;
const $http = (method, url, data, header = {}) => {
    let userInfo = wx.getStorageSync('userinfo');
    return new Promise(function (solve, reject) {
        wx.request({
            url: `${baseUrl}${url}`.replace(/[^:]\/\//, "/"),
            method: method,
            data: data,
            header: {
                Authorization: userInfo.access_token,
                ...header
            },
            dataType: 'json',
            success: function (res) {
                if (res.data.code === 401) {
                    getApp().userinfoController();
                } else if (res.statusCode === 200) {
                    solve(res.data, res.header)
                } else if (res.statusCode === 400) {
                    wx.showToast({
                        title: res.data.msg,
                        icon: "none",
                        duration: 2000
                    })
                } else {
                    reject(res);
                }
            },
            fail: function (err) {
                reject(err);
            },
            complete() {
                wx.hideLoading();
                wx.hideNavigationBarLoading() //完成停止加载
                wx.stopPullDownRefresh() //停止下拉刷新
            }
        })
    })
}
export const http = {
    get(url, data, header = {}) {
        return $http("GET", url, data, header)
    },
    post(url, data, header) {
        return $http("POST", url, data, header)
    },
    delete(url, data, header) {
        return $http("DELETE", url, data, header)
    },
    put(url, data, header) {
        return $http("PUT", url, data, header)
    }
}
export const getUser = () => {
    let user = wx.getStorageSync("userinfo");
    return user ? user : {}
}