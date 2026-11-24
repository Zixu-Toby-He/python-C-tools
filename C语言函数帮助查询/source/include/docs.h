/* 文档字符串集中声明在这里。
   定义成数组常量而不是 char* 变量：PyModuleDef 的全局初始化式必须是
   常量表达式，只有数组名（地址常量）才满足，char* 变量不行。 */
extern const char putsdoc[];
extern const char systemdoc[];
extern const char qsortdoc[];
extern const char silentdoc[];
extern const char unsilentdoc[];
extern const char moduledoc[];
