import { indentNodeProp, continuedIndent, delimitedIndent, foldNodeProp, foldInside, LRLanguage, LanguageSupport } from '@codemirror/language';
import { parser } from './parser.js';
import { cHighlighting } from './highlight.js';

const configuredParser = parser.configure({
    props: [
        cHighlighting,
        indentNodeProp.add({
            CompoundStatement: delimitedIndent({ closing: "}" }),
            IfStatement: context => {
                if (context.matchCh("else"))
                    return context.continue();
                return undefined;
            },
            ForStatement: continuedIndent({ except: /^\s*\}/ }),
            SwitchStatement: continuedIndent({ except: /^\s*\}/ }),
            DoStatement: continuedIndent({ except: /^\s*\}/ }),
            WhileStatement: continuedIndent({ except: /^\s*\}/ }),
        }),
        foldNodeProp.add({
            CompoundStatement: foldInside,
            FieldDeclarationList: foldInside,
            EnumeratorList: foldInside,
            ParameterList: foldInside,
            ArgumentList: foldInside,
            InitializerList: foldInside,
            BlockComment: foldInside
        }),
    ],
});
const cLanguage = LRLanguage.define({
    parser: configuredParser,
    languageData: {
        commentTokens: {
            line: "//",
            block: { open: "/*", close: "*/" },
        },
        indentOnInput: /^\s*(\}|\)$|case |default:)$/,
        closeBrackets: {
            brackets: ["(", "[", "{", "'", '"'],
        },
        wordChars: "_",
    },
});
function c() {
    return new LanguageSupport(cLanguage);
}

export { c, cLanguage };
