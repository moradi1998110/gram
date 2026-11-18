# gram — واجهات Telegram Desktop فوق بروتوكول XMPP

هذا المستودع نسخة (fork) من [Telegram Desktop](https://github.com/telegramdesktop/tdesktop)
يُضاف إليها خلفية XMPP مبنية على مكتبة [QXmpp](https://invent.kde.org/libraries/qxmpp)،
مع اعتماد [Gajim](https://gajim.org) مرجعاً للمميزات (XEPs). الدراسة الكاملة في
[feasibility-study.md](feasibility-study.md).

## ما أُضيف إلى كود تلكرام

| المسار | الغرض |
|---|---|
| `.github/workflows/linux.yml` | بناء لينكس على GitHub Actions (يبني QXmpp ثم التطبيق) |
| `.github/workflows-disabled/` | workflows فريق تلكرام (الروبوتات والنشر وبناء Windows وmacOS) معطّلة مؤقتاً |
| `Telegram/build/xmpp/docker_build.sh` | يُشغَّل داخل صورة البناء: QXmpp ثم بناء تلكرام العادي |
| `Telegram/ThirdParty/qxmpp` | مكتبة QXmpp (submodule، الإصدار v1.17.2) |
| `Telegram/build/xmpp/build_qxmpp.sh` | يبني QXmpp كمكتبة ثابتة باستخدام Qt الموجودة في صورة البناء |
| `Telegram/cmake/td_xmpp.cmake` | وحدة CMake باسم `td_xmpp` |
| `Telegram/SourceFiles/xmpp/` | كود خلفية XMPP (حالياً: `Xmpp::Client` غلاف فوق QXmppClient) |
| `Telegram/CMakeLists.txt` | سطران: تضمين `td_xmpp` وربطه بالتطبيق |

كل كود XMPP معزول في `SourceFiles/xmpp/` حتى يبقى دمج تحديثات تلكرام الأصلية سهلاً.

## البناء على GitHub Actions

- يعمل الـ workflow المسمى **Linux.** تلقائياً عند كل push يغيّر الكود، ويمكن تشغيله يدوياً من تبويب Actions عبر **Run workflow**.
- يستخدم أجهزة GitHub المجانية نفسها التي يبني عليها تلكرام نسخته الرسمية، والمستودع العام لا يُحتسب عليه حد للدقائق.
- **أول تشغيل يأخذ ساعات**، لأنه يبني كل المكتبات (Qt 6 وWebRTC وFFmpeg...) في صورة Docker. بعدها تُحفظ في cache فتصبح عمليات البناء التالية أسرع بكثير.
- الناتج (`Telegram` و`Updater`) يُحفظ كـ artifact في صفحة الـ run لمدة يوم واحد.
- بناء Windows وmacOS معطّل حالياً، ويُفعّل بعد إضافة QXmpp إلى `win.yml` و`mac.yml`.

## المرحلة الحالية

- [x] إدخال QXmpp في نظام البناء ووحدة `td_xmpp`
- [ ] شاشة تسجيل الدخول بـ JID وكلمة مرور (بدل رقم الهاتف) بنفس تصميم `intro/`
- [ ] عرض الـ Roster في قائمة المحادثات
- [ ] إرسال واستقبال الرسائل 1:1 في نافذة المحادثة الأصلية

## ملاحظات

- يجب تغيير اسم التطبيق وأيقونته الرئيسية قبل أي توزيع، لأن "Telegram" علامة تجارية.
- الرخصة: GPLv3 (كما في تلكرام)، وQXmpp بـ LGPL-2.1+.
