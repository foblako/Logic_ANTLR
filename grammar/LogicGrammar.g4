// ============================================================================
// LogicGrammar.g4 — чистая грамматика для логических выражений.
// Семантика реализуется отдельно через Visitor или Listener.
//
// Язык: логические выражения с операторами !, &&, || и скобками.
//   Терминалы: TRUE, FALSE
//   Операторы: ! (NOT), && (AND), || (OR)
//   Приоритеты: () → ! → && → ||
//
// Примеры:
//   TRUE
//   !TRUE
//   TRUE && FALSE
//   !TRUE && (!!FALSE)
//   (TRUE || FALSE) && !(!FALSE)
// ============================================================================
grammar LogicGrammar;

// Стартовое правило.
// EOF гарантирует, что парсер прочитает ВЕСЬ вход.
// Без EOF парсер может молча остановиться посередине файла при ошибке.
program : orExpr EOF ;

// Правила с метками альтернатив для генерации visit-методов.
// Порядок альтернатив определяет приоритет операторов.
orExpr
    : andExpr (OR_OP andExpr)*    # OrOp
    ;

andExpr
    : notExpr (AND_OP notExpr)*   # AndOp
    ;

// Правоассоциативный унарный оператор NOT.
notExpr
    : NOT_OP notExpr              # NotOp
    | baseExpr                    # NotBase
    ;

// Базовые выражения: константы и скобки.
baseExpr
    : TRUE                        # TrueLiteral
    | FALSE                       # FalseLiteral
    | LPAREN orExpr RPAREN        # Parens
    ;

// === Лексерные правила (ЗАГЛАВНЫЕ буквы) ===
// Строковые литералы в парсерных правилах — implicit tokens.
// ANTLR4 автоматически ставит их с приоритетом выше ID.

TRUE      : 'TRUE' ;
FALSE     : 'FALSE' ;
OR_OP     : '||' ;
AND_OP    : '&&' ;
NOT_OP    : '!' ;
LPAREN    : '(' ;
RPAREN    : ')' ;
WS        : [ \t\r\n]+ -> skip ;
COMMENT   : '//' ~[\r\n]* -> channel(HIDDEN) ;
