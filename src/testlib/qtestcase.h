// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QTESTCASE_H
#define QTESTCASE_H

#include <QtTest/qttestglobal.h>

#include <QtCore/qstring.h>
#include <QtCore/qnamespace.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qtemporarydir.h>
#include <QtCore/qthread.h>

#include <string.h>

#ifndef QT_NO_EXCEPTIONS
#  include <exception>
#endif // QT_NO_EXCEPTIONS

QT_BEGIN_NAMESPACE

class qfloat16;
class QRegularExpression;

#define QVERIFY(statement) \
do {\
    if (!QTest::qVerify(static_cast<bool>(statement), #statement, "", __FILE__, __LINE__))\
        return;\
} while (false)

#define QFAIL(message) \
do {\
    QTest::qFail(static_cast<const char *>(message), __FILE__, __LINE__);\
    return;\
} while (false)

#define QVERIFY2(statement, description) \
do {\
    if (statement) {\
        if (!QTest::qVerify(true, #statement, static_cast<const char *>(description), __FILE__, __LINE__))\
            return;\
    } else {\
        if (!QTest::qVerify(false, #statement, static_cast<const char *>(description), __FILE__, __LINE__))\
            return;\
    }\
} while (false)

#define QCOMPARE(actual, expected) \
do {\
    if (!QTest::qCompare(actual, expected, #actual, #expected, __FILE__, __LINE__))\
        return;\
} while (false)


#ifndef QT_NO_EXCEPTIONS

#  define QVERIFY_THROWS_NO_EXCEPTION(...) \
    do { \
        QT_TRY { \
            __VA_ARGS__; \
            /* success */ \
        } QT_CATCH (const std::exception &e) { \
            QTest::qCaught(nullptr, e.what(), __FILE__, __LINE__); \
            return; \
        } QT_CATCH (...) { \
            QTest::qCaught(nullptr, nullptr, __FILE__, __LINE__); \
            QT_RETHROW; \
        } \
    } while (false) \
    /* end */

#if QT_DEPRECATED_SINCE(6, 3)
namespace QTest {
QT_DEPRECATED_VERSION_X_6_3("Don't use QVERIFY_EXCEPTION_THROWN(expr, type) anymore, "
                            "use QVERIFY_THROWS_EXCEPTION(type, expr...) instead")
inline void useVerifyThrowsException() {}
} // namespace QTest
#  define QVERIFY_EXCEPTION_THROWN(expression, exceptiontype) \
    QVERIFY_THROWS_EXCEPTION(exceptiontype, QTest::useVerifyThrowsException(); expression)
#endif

#  define QVERIFY_THROWS_EXCEPTION(exceptiontype, ...) \
    do {\
        QT_TRY {\
            QT_TRY {\
                __VA_ARGS__;\
                QTest::qFail("Expected exception of type " #exceptiontype " to be thrown" \
                             " but no exception caught", __FILE__, __LINE__);\
                return;\
            } QT_CATCH (const exceptiontype &) {\
                /* success */\
            }\
        } QT_CATCH (const std::exception &e) {\
            QTest::qCaught(#exceptiontype, e.what(), __FILE__, __LINE__);\
            return;\
        } QT_CATCH (...) {\
            QTest::qCaught(#exceptiontype, nullptr, __FILE__, __LINE__);\
            QT_RETHROW;\
        }\
    } while (false)

#else // QT_NO_EXCEPTIONS

/*
 * These macros check whether the expression passed throws exceptions, but we can't
 * catch them to check because Qt has been compiled without exception support. We can't
 * skip the expression because it may have side effects and must be executed.
 * So, users must use Qt with exception support enabled if they use exceptions
 * in their code.
 */
#  define QVERIFY_THROWS_EXCEPTION(...) \
    static_assert(false, "Support for exceptions is disabled")
#  define QVERIFY_THROWS_NO_EXCEPTION(...) \
    static_assert(false, "Support for exceptions is disabled")

#endif // !QT_NO_EXCEPTIONS

/* Ideally we would adapt qWaitFor(), or a variant on it, to implement roughly
 * what the following provides as QTRY_LOOP_IMPL(); however, for now, the
 * reporting of how much to increase the timeout to (if within a factor of two)
 * on failure and the check for QTest::currentTestFailed() go beyond
 * qWaitFor(). (We no longer care about the bug in MSVC < 2017 that precluded
 * using qWaitFor() in the implementation here, see QTBUG-59096.)
 */

// NB: not do {...} while (0) wrapped, as qt_test_i is accessed after it
#define QTRY_LOOP_IMPL(expr, timeoutValue, step) \
    if (!(expr)) { \
        QTest::qWait(0); \
    } \
    int qt_test_i = 0; \
    for (; qt_test_i < timeoutValue && !(expr); qt_test_i += step) { \
        QTest::qWait(step); \
    }

#define QTRY_TIMEOUT_DEBUG_IMPL(expr, timeoutValue, step)\
    if (!(expr)) { \
        QTRY_LOOP_IMPL((expr), 2 * (timeoutValue), step);   \
        if (expr) { \
            QFAIL(qPrintable(QTest::Internal::formatTryTimeoutDebugMessage(u8"" #expr, timeoutValue, timeoutValue + qt_test_i))); \
        } \
    }

#define QTRY_IMPL(expr, timeout)\
    const int qt_test_step = timeout < 350 ? timeout / 7 + 1 : 50; \
    const int qt_test_timeoutValue = timeout; \
    { QTRY_LOOP_IMPL(QTest::currentTestFailed() || (expr), qt_test_timeoutValue, qt_test_step); } \
    QTRY_TIMEOUT_DEBUG_IMPL(QTest::currentTestFailed() || (expr), qt_test_timeoutValue, qt_test_step)

// Will try to wait for the expression to become true while allowing event processing
#define QTRY_VERIFY_WITH_TIMEOUT(expr, timeout) \
do { \
    QTRY_IMPL((expr), timeout);\
    QVERIFY(expr); \
} while (false)

#define QTRY_VERIFY(expr) QTRY_VERIFY_WITH_TIMEOUT((expr), 5000)

// Will try to wait for the expression to become true while allowing event processing
#define QTRY_VERIFY2_WITH_TIMEOUT(expr, messageExpression, timeout) \
do { \
    QTRY_IMPL((expr), timeout);\
    QVERIFY2(expr, messageExpression); \
} while (false)

#define QTRY_VERIFY2(expr, messageExpression) QTRY_VERIFY2_WITH_TIMEOUT((expr), (messageExpression), 5000)

// Will try to wait for the comparison to become successful while allowing event processing
#define QTRY_COMPARE_WITH_TIMEOUT(expr, expected, timeout) \
do { \
    QTRY_IMPL(((expr) == (expected)), timeout);\
    QCOMPARE((expr), expected); \
} while (false)

#define QTRY_COMPARE(expr, expected) QTRY_COMPARE_WITH_TIMEOUT((expr), expected, 5000)

#define QSKIP_INTERNAL(statement) \
do {\
    QTest::qSkip(static_cast<const char *>(statement), __FILE__, __LINE__);\
    return;\
} while (false)

#define QSKIP(statement, ...) QSKIP_INTERNAL(statement)

#define QEXPECT_FAIL(dataIndex, comment, mode)\
do {\
    if (!QTest::qExpectFail(dataIndex, static_cast<const char *>(comment), QTest::mode, __FILE__, __LINE__))\
        return;\
} while (false)

#define QFETCH(Type, name)\
    Type name = *static_cast<Type *>(QTest::qData(#name, ::qMetaTypeId<typename std::remove_cv<Type >::type>()))

#define QFETCH_GLOBAL(Type, name)\
    Type name = *static_cast<Type *>(QTest::qGlobalData(#name, ::qMetaTypeId<typename std::remove_cv<Type >::type>()))

#define QTEST(actual, testElement)\
do {\
    if (!QTest::qTest(actual, testElement, #actual, #testElement, __FILE__, __LINE__))\
        return;\
} while (false)

#ifdef QT_TESTCASE_BUILDDIR

#ifndef QT_TESTCASE_SOURCEDIR
#define QT_TESTCASE_SOURCEDIR nullptr
#endif

# define QFINDTESTDATA(basepath)\
    QTest::qFindTestData(basepath, __FILE__, __LINE__, QT_TESTCASE_BUILDDIR, QT_TESTCASE_SOURCEDIR)
#else
# define QFINDTESTDATA(basepath)\
    QTest::qFindTestData(basepath, __FILE__, __LINE__)
#endif

# define QEXTRACTTESTDATA(resourcePath) \
    QTest::qExtractTestData(resourcePath)

class QObject;
class QTestData;

#define QTEST_COMPARE_DECL(KLASS)\
    template<> Q_TESTLIB_EXPORT char *toString<KLASS >(const KLASS &);

namespace QTest
{
    namespace Internal {

    Q_TESTLIB_EXPORT QString formatTryTimeoutDebugMessage(q_no_char8_t::QUtf8StringView expr, int timeout, int actual);

    template<typename T> // Output registered enums
    inline typename std::enable_if<QtPrivate::IsQEnumHelper<T>::Value, char*>::type toString(T e)
    {
        QMetaEnum me = QMetaEnum::fromType<T>();
        return qstrdup(me.valueToKey(int(e))); // int cast is necessary to support enum classes
    }

    template <typename T>
    inline typename std::enable_if<!QtPrivate::IsQEnumHelper<T>::Value && std::is_enum_v<T>, char*>::type toString(const T &e)
    {
        return qstrdup(QByteArray::number(static_cast<std::underlying_type_t<T>>(e)).constData());
    }

    template <typename T> // Fallback
    inline typename std::enable_if<!QtPrivate::IsQEnumHelper<T>::Value && !std::is_enum_v<T>, char*>::type toString(const T &)
    {
        return nullptr;
    }

    template<typename F> // Output QFlags of registered enumerations
    inline typename std::enable_if<QtPrivate::IsQEnumHelper<F>::Value, char*>::type toString(QFlags<F> f)
    {
        const QMetaEnum me = QMetaEnum::fromType<F>();
        return qstrdup(me.valueToKeys(int(f.toInt())).constData());
    }

    template <typename F> // Fallback: Output hex value
    inline typename std::enable_if<!QtPrivate::IsQEnumHelper<F>::Value, char*>::type toString(QFlags<F> f)
    {
        const size_t space = 3 + 2 * sizeof(unsigned); // 2 for 0x, two hex digits per byte, 1 for '\0'
        char *msg = new char[space];
        qsnprintf(msg, space, "0x%x", unsigned(f.toInt()));
        return msg;
    }

    } // namespace Internal

    template<typename T>
    inline char *toString(const T &t)
    {
        return Internal::toString(t);
    }

    template <typename T1, typename T2>
    inline char *toString(const QPair<T1, T2> &pair);

    template <typename T1, typename T2>
    inline char *toString(const std::pair<T1, T2> &pair);

    template <class... Types>
    inline char *toString(const std::tuple<Types...> &tuple);

    Q_TESTLIB_EXPORT char *toHexRepresentation(const char *ba, int length);
    Q_TESTLIB_EXPORT char *toPrettyCString(const char *unicode, int length);
    Q_TESTLIB_EXPORT char *toPrettyUnicode(QStringView string);
    Q_TESTLIB_EXPORT char *toString(const char *);
    Q_TESTLIB_EXPORT char *toString(const volatile void *);
    Q_TESTLIB_EXPORT char *toString(const void *); // ### FIXME: Qt 7: Remove
    Q_TESTLIB_EXPORT char *toString(const volatile QObject *);

    Q_TESTLIB_EXPORT void qInit(QObject *testObject, int argc = 0, char **argv = nullptr);
    Q_TESTLIB_EXPORT int qRun();
    Q_TESTLIB_EXPORT void qCleanup();

    Q_TESTLIB_EXPORT int qExec(QObject *testObject, int argc = 0, char **argv = nullptr);
    Q_TESTLIB_EXPORT int qExec(QObject *testObject, const QStringList &arguments);

    Q_TESTLIB_EXPORT void setMainSourcePath(const char *file, const char *builddir = nullptr);

    Q_TESTLIB_EXPORT bool qVerify(bool statement, const char *statementStr, const char *description,
                                 const char *file, int line);
    Q_DECL_COLD_FUNCTION
    Q_TESTLIB_EXPORT void qFail(const char *message, const char *file, int line);
    Q_TESTLIB_EXPORT void qSkip(const char *message, const char *file, int line);
    Q_TESTLIB_EXPORT bool qExpectFail(const char *dataIndex, const char *comment, TestFailMode mode,
                           const char *file, int line);
    Q_DECL_COLD_FUNCTION
    Q_TESTLIB_EXPORT void qCaught(const char *expected, const char *what, const char *file, int line);
#if QT_DEPRECATED_SINCE(6, 3)
    QT_DEPRECATED_VERSION_X_6_3("Use qWarning() instead")
    Q_TESTLIB_EXPORT void qWarn(const char *message, const char *file = nullptr, int line = 0);
#endif
    Q_TESTLIB_EXPORT void ignoreMessage(QtMsgType type, const char *message);
#if QT_CONFIG(regularexpression)
    Q_TESTLIB_EXPORT void ignoreMessage(QtMsgType type, const QRegularExpression &messagePattern);
#endif
    Q_TESTLIB_EXPORT void failOnWarning(const char *message);
#if QT_CONFIG(regularexpression)
    Q_TESTLIB_EXPORT void failOnWarning(const QRegularExpression &messagePattern);
#endif

#if QT_CONFIG(temporaryfile)
    Q_TESTLIB_EXPORT QSharedPointer<QTemporaryDir> qExtractTestData(const QString &dirName);
#endif
    Q_TESTLIB_EXPORT QString qFindTestData(const char* basepath, const char* file = nullptr, int line = 0, const char* builddir = nullptr, const char* sourcedir = nullptr);
    Q_TESTLIB_EXPORT QString qFindTestData(const QString& basepath, const char* file = nullptr, int line = 0, const char* builddir = nullptr, const char *sourcedir = nullptr);

    Q_TESTLIB_EXPORT void *qData(const char *tagName, int typeId);
    Q_TESTLIB_EXPORT void *qGlobalData(const char *tagName, int typeId);
    Q_TESTLIB_EXPORT void *qElementData(const char *elementName, int metaTypeId);
    Q_TESTLIB_EXPORT QObject *testObject();

    Q_TESTLIB_EXPORT const char *currentAppName();

    Q_TESTLIB_EXPORT const char *currentTestFunction();
    Q_TESTLIB_EXPORT const char *currentDataTag();
    Q_TESTLIB_EXPORT bool currentTestFailed();

    Q_TESTLIB_EXPORT Qt::Key asciiToKey(char ascii);
    Q_TESTLIB_EXPORT char keyToAscii(Qt::Key key);

    Q_TESTLIB_EXPORT bool compare_helper(bool success, const char *failureMsg,
                                         char *val1, char *val2,
                                         const char *actual, const char *expected,
                                         const char *file, int line);
    Q_TESTLIB_EXPORT void addColumnInternal(int id, const char *name);

    template <typename T>
    inline void addColumn(const char *name, T * = nullptr)
    {
        using QIsSameTConstChar = std::is_same<T, const char*>;
        static_assert(!QIsSameTConstChar::value, "const char* is not allowed as a test data format.");
        addColumnInternal(qMetaTypeId<T>(), name);
    }
    Q_TESTLIB_EXPORT QTestData &newRow(const char *dataTag);
    Q_TESTLIB_EXPORT QTestData &addRow(const char *format, ...) Q_ATTRIBUTE_FORMAT_PRINTF(1, 2);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    // kept after adding implementation of <T1, T2> out of paranoia:
    template <typename T>
    inline bool qCompare(T const &t1, T const &t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_helper(t1 == t2, "Compared values are not the same",
                              toString(t1), toString(t2), actual, expected, file, line);
    }
#endif

    Q_TESTLIB_EXPORT bool qCompare(qfloat16 const &t1, qfloat16 const &t2,
                    const char *actual, const char *expected, const char *file, int line);

    Q_TESTLIB_EXPORT bool qCompare(float const &t1, float const &t2,
                    const char *actual, const char *expected, const char *file, int line);

    Q_TESTLIB_EXPORT bool qCompare(double const &t1, double const &t2,
                    const char *actual, const char *expected, const char *file, int line);

    Q_TESTLIB_EXPORT bool qCompare(int t1, int t2, const char *actual, const char *expected,
                                   const char *file, int line);

#if QT_POINTER_SIZE == 8
    Q_TESTLIB_EXPORT bool qCompare(qsizetype t1, qsizetype t2, const char *actual, const char *expected,
                                   const char *file, int line);
#endif

    Q_TESTLIB_EXPORT bool qCompare(unsigned t1, unsigned t2, const char *actual, const char *expected,
                                   const char *file, int line);

    Q_TESTLIB_EXPORT bool qCompare(QStringView t1, QStringView t2,
                                   const char *actual, const char *expected,
                                   const char *file, int line);
    Q_TESTLIB_EXPORT bool qCompare(QStringView t1, const QLatin1StringView &t2,
                                   const char *actual, const char *expected,
                                   const char *file, int line);
    Q_TESTLIB_EXPORT bool qCompare(const QLatin1StringView &t1, QStringView t2,
                                   const char *actual, const char *expected,
                                   const char *file, int line);
    inline bool qCompare(const QString &t1, const QString &t2,
                         const char *actual, const char *expected,
                         const char *file, int line)
    {
        return qCompare(QStringView(t1), QStringView(t2), actual, expected, file, line);
    }
    inline bool qCompare(const QString &t1, const QLatin1StringView &t2,
                         const char *actual, const char *expected,
                         const char *file, int line)
    {
        return qCompare(QStringView(t1), t2, actual, expected, file, line);
    }
    inline bool qCompare(const QLatin1StringView &t1, const QString &t2,
                         const char *actual, const char *expected,
                         const char *file, int line)
    {
        return qCompare(t1, QStringView(t2), actual, expected, file, line);
    }

    inline bool compare_ptr_helper(const volatile void *t1, const volatile void *t2, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        return compare_helper(t1 == t2, "Compared pointers are not the same",
                              toString(t1), toString(t2), actual, expected, file, line);
    }

    inline bool compare_ptr_helper(const volatile QObject *t1, const volatile QObject *t2, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        return compare_helper(t1 == t2, "Compared QObject pointers are not the same",
                              toString(t1), toString(t2), actual, expected, file, line);
    }

    inline bool compare_ptr_helper(const volatile QObject *t1, std::nullptr_t, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        return compare_helper(t1 == nullptr, "Compared QObject pointers are not the same",
                              toString(t1), toString(nullptr), actual, expected, file, line);
    }

    inline bool compare_ptr_helper(std::nullptr_t, const volatile QObject *t2, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        return compare_helper(nullptr == t2, "Compared QObject pointers are not the same",
                              toString(nullptr), toString(t2), actual, expected, file, line);
    }

    inline bool compare_ptr_helper(const volatile void *t1, std::nullptr_t, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        return compare_helper(t1 == nullptr, "Compared pointers are not the same",
                              toString(t1), toString(nullptr), actual, expected, file, line);
    }

    inline bool compare_ptr_helper(std::nullptr_t, const volatile void *t2, const char *actual,
                                   const char *expected, const char *file, int line)
    {
        return compare_helper(nullptr == t2, "Compared pointers are not the same",
                              toString(nullptr), toString(t2), actual, expected, file, line);
    }

    Q_TESTLIB_EXPORT bool compare_string_helper(const char *t1, const char *t2, const char *actual,
                                      const char *expected, const char *file, int line);

    Q_TESTLIB_EXPORT char *formatString(const char *prefix, const char *suffix, size_t numArguments, ...);

#ifndef Q_QDOC
    QTEST_COMPARE_DECL(short)
    QTEST_COMPARE_DECL(ushort)
    QTEST_COMPARE_DECL(int)
    QTEST_COMPARE_DECL(uint)
    QTEST_COMPARE_DECL(long)
    QTEST_COMPARE_DECL(ulong)
    QTEST_COMPARE_DECL(qint64)
    QTEST_COMPARE_DECL(quint64)

    QTEST_COMPARE_DECL(float)
    QTEST_COMPARE_DECL(double)
    QTEST_COMPARE_DECL(qfloat16)
    QTEST_COMPARE_DECL(char)
    QTEST_COMPARE_DECL(signed char)
    QTEST_COMPARE_DECL(unsigned char)
    QTEST_COMPARE_DECL(bool)
#endif

    template <typename T1, typename T2>
    inline bool qCompare(const T1 &t1, const T2 &t2, const char *actual, const char *expected,
                         const char *file, int line)
    {
        return compare_helper(t1 == t2, "Compared values are not the same",
                              toString(t1), toString(t2), actual, expected, file, line);
    }

    inline bool qCompare(double const &t1, float const &t2, const char *actual,
                                 const char *expected, const char *file, int line)
    {
        return qCompare(qreal(t1), qreal(t2), actual, expected, file, line);
    }

    inline bool qCompare(float const &t1, double const &t2, const char *actual,
                                 const char *expected, const char *file, int line)
    {
        return qCompare(qreal(t1), qreal(t2), actual, expected, file, line);
    }

    template <typename T>
    inline bool qCompare(const T *t1, const T *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(t1, t2, actual, expected, file, line);
    }
    template <typename T>
    inline bool qCompare(T *t1, T *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(t1, t2, actual, expected, file, line);
    }

    template <typename T>
    inline bool qCompare(T *t1, std::nullptr_t, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(t1, nullptr, actual, expected, file, line);
    }
    template <typename T>
    inline bool qCompare(std::nullptr_t, T *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(nullptr, t2, actual, expected, file, line);
    }

    template <typename T1, typename T2>
    inline bool qCompare(const T1 *t1, const T2 *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(t1, static_cast<const T1 *>(t2), actual, expected, file, line);
    }
    template <typename T1, typename T2>
    inline bool qCompare(T1 *t1, T2 *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_ptr_helper(const_cast<const T1 *>(t1),
                static_cast<const T1 *>(const_cast<const T2 *>(t2)), actual, expected, file, line);
    }
    inline bool qCompare(const char *t1, const char *t2, const char *actual,
                                       const char *expected, const char *file, int line)
    {
        return compare_string_helper(t1, t2, actual, expected, file, line);
    }
    inline bool qCompare(char *t1, char *t2, const char *actual, const char *expected,
                        const char *file, int line)
    {
        return compare_string_helper(t1, t2, actual, expected, file, line);
    }

    /* The next two overloads are for MSVC that shows problems with implicit
       conversions
     */
    inline bool qCompare(char *t1, const char *t2, const char *actual,
                         const char *expected, const char *file, int line)
    {
        return compare_string_helper(t1, t2, actual, expected, file, line);
    }
    inline bool qCompare(const char *t1, char *t2, const char *actual,
                         const char *expected, const char *file, int line)
    {
        return compare_string_helper(t1, t2, actual, expected, file, line);
    }

    template <class T>
    inline bool qTest(const T& actual, const char *elementName, const char *actualStr,
                     const char *expected, const char *file, int line)
    {
        return qCompare(actual, *static_cast<const T *>(QTest::qElementData(elementName,
                       qMetaTypeId<T>())), actualStr, expected, file, line);
    }
}

#undef QTEST_COMPARE_DECL

#define QWARN(msg) QTest::qWarn(static_cast<const char *>(msg), __FILE__, __LINE__)

QT_END_NAMESPACE

#endif
