grammar LogicGrammar;

program : orExpr EOF ;

orExpr
    : andExpr (OR_OP andExpr)*    # OrOp
    ;

andExpr
    : notExpr (AND_OP notExpr)*   # AndOp
    ;

notExpr
    : NOT_OP notExpr              # NotOp
    | baseExpr                    # NotBase
    ;

baseExpr
    : TRUE                        # TrueLiteral
    | FALSE                       # FalseLiteral
    | LPAREN orExpr RPAREN        # Parens
    ;

TRUE      : 'TRUE' ;
FALSE     : 'FALSE' ;
OR_OP     : '||' ;
AND_OP    : '&&' ;
NOT_OP    : '!' ;
LPAREN    : '(' ;
RPAREN    : ')' ;
WS        : [ \t\r\n]+ -> skip ;
COMMENT   : '//' ~[\r\n]* -> channel(HIDDEN) ;
