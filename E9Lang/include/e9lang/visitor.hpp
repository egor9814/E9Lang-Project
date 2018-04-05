//
// Created by egor9814 on 27.03.18.
//

#ifndef E9LANG_PROJECT_VISITOR_HPP
#define E9LANG_PROJECT_VISITOR_HPP

namespace e9lang {

    namespace ast {

        class Visitor;

        class Visitable {
        public:
            virtual void accept(Visitor* visitor) = 0;
        };

        template <class Node>
        struct Model {
            static Node* cast(Visitable* visitable){
                return reinterpret_cast<Node*>(visitable);
            }
            static void visit(Node* visitable, Visitor* visitor){}
        };

        class Visitor {
        public:
            template <class Node>
            void visit(Node* visitable) {
                Model<Node>::visit(Model<Node>::cast(visitable), this);
            }
        };
    }

}

#endif //E9LANG_PROJECT_VISITOR_HPP
