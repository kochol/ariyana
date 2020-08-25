#include "Platform.hpp"
#include "core/log.h"
#include "io/Window.hpp"
#include "sokol_app.h"
#include <android/native_activity.h>

namespace ari::io
{
    struct AttachToVm
    {
        JavaVM* vm;
        JNIEnv** env;

        AttachToVm(JavaVM* _vm, JNIEnv** _env)
        {
            vm = _vm;
            env = _env;
            vm->AttachCurrentThread(_env, nullptr);
        }

        ~AttachToVm()
        {
            // Detach from vm
            vm->DetachCurrentThread();
        }
    };
    
    void getAndroidID(JavaVM* vm, jobject context, char* deviceId, int& dev_id_len)
    {
        if (!vm)
        {
            return;
        }
        JNIEnv* env;
        AttachToVm atovm(vm, &env);

        //Get the Setting.Secure class and the Context Class
        jclass c_settings_secure = env->FindClass("android/provider/Settings$Secure");
        jclass c_context = env->FindClass("android/content/Context");
        if(c_settings_secure == NULL || c_context == NULL){
            return;
        }
        //Get the getContentResolver method
        jmethodID m_get_content_resolver = env->GetMethodID(c_context, "getContentResolver",
                                                            "()Landroid/content/ContentResolver;");
        if(m_get_content_resolver == NULL){
            return;
        }

        //Get the Settings.Secure.ANDROID_ID constant
        jfieldID f_android_id = env->GetStaticFieldID(c_settings_secure, "ANDROID_ID", "Ljava/lang/String;");
        if(f_android_id == NULL){
            return;
        }
        jobject s_android_id =  env->GetStaticObjectField(c_settings_secure, f_android_id);

        //create a ContentResolver instance context.getContentResolver()
        jobject o_content_resolver = env->CallObjectMethod(context, m_get_content_resolver);
        if(o_content_resolver == NULL || s_android_id == NULL){
            return;
        }

        //get the method getString
        jmethodID m_get_string = env->GetStaticMethodID(c_settings_secure, "getString",
                                                        "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
        if(m_get_string == NULL){
            return;
        }

        //get the Android ID
        jstring android_id = (jstring)env->CallStaticObjectMethod(c_settings_secure,
                                                            m_get_string,
                                                            o_content_resolver,
                                                            s_android_id);

        dev_id_len = env->GetStringUTFLength(android_id);
        env->GetStringUTFRegion(android_id, 0, dev_id_len, deviceId);
        deviceId[dev_id_len] = '\0';
        log_debug("deviceId %s %d", deviceId, dev_id_len);
    }

    core::String GetMacAddress()
    {
        static core::String Result;
		if (Result.Length() > 0) 
			return Result;

   		auto nactivity = (ANativeActivity*)io::AndroidGetNativeActivity();        
        char dev_id[32];
        int dev_id_len = 0;
        getAndroidID(nactivity->vm, nactivity->clazz, dev_id, dev_id_len);
        Result.Assign(dev_id, 0, dev_id_len);

        return Result;
    }

} // namespace ari::io
