# gram — واجهات Telegram Desktop فوق بروتوكول XMPP

هذا المستودع نسخة (fork) من [Telegram Desktop](https://github.com/telegramdesktop/tdesktop)
يُضاف إليها خلفية XMPP مبنية على مكتبة [QXmpp](https://invent.kde.org/libraries/qxmpp)،
مع اعتماد [Gajim](https://gajim.org) مرجعاً للمميزات (XEPs). الدراسة الكاملة في
[feasibility-study.md](feasibility-study.md).

## ما أُضيف إلى كود تلكرام

| المسار | الغرض |
|---|---|
| `.gitlab-ci.yml` | بناء لينكس على GitLab CI |
| `Telegram/ThirdParty/qxmpp` | مكتبة QXmpp (submodule، الإصدار v1.17.2) |
| `Telegram/build/xmpp/build_qxmpp.sh` | يبني QXmpp كمكتبة ثابتة باستخدام Qt الموجودة في صورة البناء |
| `Telegram/cmake/td_xmpp.cmake` | وحدة CMake باسم `td_xmpp` |
| `Telegram/SourceFiles/xmpp/` | كود خلفية XMPP (حالياً: `Xmpp::Client` غلاف فوق QXmppClient) |
| `Telegram/CMakeLists.txt` | سطران: تضمين `td_xmpp` وربطه بالتطبيق |

كل كود XMPP معزول في `SourceFiles/xmpp/` حتى يبقى دمج تحديثات تلكرام الأصلية سهلاً.

## البناء على GitLab CI

1. **Runner:** Docker executor مع `privileged = true`، ذاكرة 16 GB على الأقل، 8 أنوية أو أكثر،
   مساحة 60 GB، واتصال بـ github.com وinvent.kde.org.
2. **المهلة:** من Settings > CI/CD > General pipelines اجعل Timeout = 12h.
3. **Container Registry:** يجب أن يكون مفعّلاً للمشروع.
4. أول pipeline يشغّل المهمة `env-image` التي تبني صورة Rocky Linux 8 بكل المكتبات
   (Qt 6 وWebRTC وFFmpeg...) وتأخذ عدة ساعات، ثم `linux-build` الذي يبني التطبيق.
   الـ pipelines التالية تعيد استخدام الصورة ولا تبنيها إلا إذا تغيّر
   `Telegram/build/docker/centos_env/`، أو عند التشغيل اليدوي مع المتغير `FORCE_ENV_IMAGE=1`.
5. الملف الناتج `artifact/gram` يُحفظ كـ artifact لمدة أسبوعين.

## المرحلة الحالية

- [x] إدخال QXmpp في نظام البناء ووحدة `td_xmpp`
- [ ] شاشة تسجيل الدخول بـ JID وكلمة مرور (بدل رقم الهاتف) بنفس تصميم `intro/`
- [ ] عرض الـ Roster في قائمة المحادثات
- [ ] إرسال واستقبال الرسائل 1:1 في نافذة المحادثة الأصلية

## ملاحظات

- يجب تغيير اسم التطبيق وأيقونته الرئيسية قبل أي توزيع، لأن "Telegram" علامة تجارية.
- الرخصة: GPLv3 (كما في تلكرام)، وQXmpp بـ LGPL-2.1+.
