
function showStacks() {
    Java.perform(function () {
        send(Java.use("android.util.Log").getStackTraceString(Java.use("java.lang.Exception").$new()));
    });
}

function dump_obj(obj) {
    for(var key in obj){
        console.log('args_x.persons ----+++-----', key, obj[key])
    }
}

// obj含有size函数
function dump_obj_array(intent, obj) {
    var len = obj.size()
    var arr = obj.toArray()
    for(var i = 0; i < len; ++i){
        console.log('dump_obj_array =======', i, arr[i], intent.getExtra(arr[i]))
    }
}


function my_format(fmt, objs) {
    var cls_String = Java.use("java.lang.String");
    var fn_format = cls_String.format.overload('java.lang.String', '[Ljava.lang.Object;')

    return fn_format.call(cls_String, fmt, objs)
}


function my_hook_log(log_platformtools, fn, flag) {
    fn.implementation = function () {
        // dump_obj(arguments)
        console.log('[khz] fmt=   ', arguments[1])
        console.log('[khz]', flag, '>>>> ', arguments[0], '>> ', my_format(arguments[1], arguments[2]))
        fn.call(log_platformtools, arguments[0], arguments[1], arguments[2])
    }
}


// 微信com.tencent.mars.xlog.Xlog接口Hook
var filter_not_care = ["MicroMsg.MemoryWatchDog", "ThreadPool.Execute", "MicroMsg.SimcardService", "MicroMsg.BitmapTracer"]
var filter_care = ["MicroMsg.KVEasyReport", "MicroMsg.ConfigStorage", "MicroMsg.ReportManagerKvCheck", "MicroMsg.ReportService", "MicroMsg.EventCenter"]
function my_hook_log_arguments(log_platformtools, fn, flag) {
    fn.implementation = function () {
        // console.log(arguments["0"])
        if (filter_not_care.indexOf(arguments["0"]) != -1) {
            return
        }else if (filter_care.indexOf(arguments["0"]) != -1) {
            return
        } else if (arguments["0"].startsWith("HABBYGE-MALI.")) {
            return
        }

        // dump_obj(arguments)
        console.log('[khz] ', flag, JSON.stringify(arguments))
        // console.log('[khz]', flag, '>>>> ', arguments[0], '>> ', my_format(arguments[1], arguments[2]))
        // fn.call(log_platformtools, arguments[0], arguments[1], arguments[2])
    }
}


Java.perform(function(){
	console.log('【Java.perform】     ENTER');

    // Hook class
    var cls = Java.use("com.ninecents.MainActivity");

    cls._member_method.implementation = function () {
        console.log('【cls._member_method】       ENTER');
		
		// ## Hook打印参数、堆栈
        console.log('【cls._member_method】       打印参数：', arguments);
        console.log('【cls._member_method】       打印参数：', JSON.stringify(arguments));
        console.log('【cls._member_method】       打印参数：', arguments[0], arguments[0].value);
		showStacks();
		
		// ## 修改参数
		arguments[0] = "随便改改";
		// arguments[0].value = "随便改改，当类型是对象的时候，需要加上value属性";
		this._member_method(arguments[0])
		
		// ## 修改返回值
		return 1;
    };
	
	// ## Hook重载函数  overload
    cls._overload_method.overload('int').implementation = function () {
        console.log('【cls._overload_method】       ENTER');
		arguments[0] = 0;
		this._overload_method(arguments[0])
	}
	
	// ## Hook重载函数  overload
    cls._static_method.implementation = function () {
        console.log('【cls._static_method】       ENTER');
		
        console.log('【cls._member_method】       打印参数：', arguments);
        console.log('【cls._member_method】       打印参数：', JSON.stringify(arguments));
		showStacks();
	}
});

// 主动调用静态函数
function callStaticFun() { //定义导出函数
    Java.perform(function () { //找到隐藏函数并且调用
		console.log("Java.isMainThread() = ", Java.isMainThread());
        Java.choose("com.ninecents.MainActivity", {
            onMatch: function (instance) {
                console.log("Found instance: " + instance);
                console.log("Result of secret func: " + instance._static_method());
            },
            onComplete: function () { }
        });
    });
}
// Java.scheduleOnMainThread(callStaticFun)

// 主动调用成员函数
function callMemberFun() { //定义导出函数
    Java.perform(function () { //找到隐藏函数并且调用
		console.log("Java.isMainThread() = ", Java.isMainThread());
        Java.choose("com.ninecents.MainActivity", {
            onMatch: function (instance) {
                console.log("Found instance: " + instance);
                console.log("Result of secret func: " + instance._member_method("in js..."));
            },
            onComplete: function () { }
        });
    });
}

// Java.scheduleOnMainThread(callMemberFun)

// 主动调用点击事件
function call__onClick() { //定义导出函数
    Java.perform(function () { //找到隐藏函数并且调用
		console.log("Java.isMainThread() = ", Java.isMainThread());
        Java.choose("com.ninecents.MainActivity", {
            onMatch: function (instance) {
                console.log("Found instance: " + instance, JSON.stringify(instance));
                console.log("Result of secret func: ", JSON.stringify(instance.button_overload_method));
				
				var ClassView = Java.use("android.view.View");
				var instanceClassView = Java.cast(instance.button_overload_method.value, ClassView);
				
                instance.onClick(instanceClassView);
            },
            onComplete: function () { }
        });
    });
}

Java.scheduleOnMainThread(call__onClick)
