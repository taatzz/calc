#include <iostream>

using namespace std;

enum Color
{
    RED,
    BLACK,
};

class node
{
    friend class RBTree;
    int value;
    node* _left;
    node* _right;
    node* _parent;
    Color col;

    node(int value) : value(value), _left(NULL), _right(NULL), _parent(NULL), col(RED){}

    // void left_rota(node*);
    // void right_rota(node*);
};

/*      pf                      pf 
        |                       |
        p                      pr(r)
       / \                     /  \
      pl  pr(r)   ->       p    rr
          / \                / \
         rl  rr             pl  rl
*/
/*
void node::left_rota(node* root)
{
    if(root != NULL)
    {
        // 当前节点连接到当前节点的右子树的左子树
        node* r = root->_right; 
        root->_right = r->_left; // p -> rl
        // 当前节点的右子树的左子树不为空时将此节点的父节点设置为当前节点
        if(r->_left != NULL)
        {
            r->_left->_parent= root; // rl
        }
        // 将当前节点的父节点设置为当前节点的右子树的父节点（将右子树往上移）
        r->_parent = root->_parent;
        // （将原根节点的父节点连接到新的根节点）切断当前节点与父节点的联系
        if(root->_parent == NULL)
        {
            root = r;
        }
        else if(root->_parent->_left == root)
        {
            root->_parent->_left = r;
        }
        else
        {
            root->_parent->_right = r;
        }
        // 将右子树设置成新的根节点(连接当前原根节点)
        r->_left = root;
        // 原根节点的父节点为新根节点
        root->_parent = r;
    }
}
*/
/*      pf                  pf
        |                    |
        p                    pl(l)
       / \                  / \
   (l)pl  pr               ll  p
     / \                      / \
    ll lr                    lr  pr
*/
/*
void node::right_rota(node* root)
{
    if(root != NULL)
    {
        node* l = root->_left;
        // 将当前节点的左子树设置为当前节点的左子树的右子树
        root->_left = l->_right;
        // 当前节点的左子树的右子树存在的时候将父节点设置为当前节点
        if(l->_right != NULL)
            l->_right->_parent = root;
        // 将当前节点的父节点设置为当前节点的右子树父节点
        l->_parent = root->_parent;
        // 将原根节点的父节点连接新的根节点
        if(root->_parent == NULL)// 当父节点为空时说明当前节点为(整个树的根节点）
            root = l;
        else if(root->_parent->_left == root)
            root->_parent->_left = l;
        else
            root->_parent->_right = l;
        // 新的根右子树连接当前节点（原根节点）
        l->_right = root;
        // 当前根节点（原根节点）的父节点设置为新的根节点
        root->_parent = l;
    }
}
*/


class RBTree
{
private:
    node* _root;
    void Destory(node*);
    void print(node*);
    void right_rota(node*); // 右旋
    void left_rota(node*); // 左旋
    bool check(node*, int, int); // 检查函数
    node* find_pre(node*);
    node* find_suc(node*);
    void balance(node*);
public:
    RBTree() : _root(NULL){}
    ~RBTree();

    bool Insert(int); // 插入
    void deletenode(int); // 删除

    bool is_balance(); // 判断红黑树是否平衡

    void midpost();
};

RBTree::~RBTree()
{
    Destory(_root);
}

void RBTree::Destory(node* n)
{
    if(n == NULL)
        return;

    Destory(n->_left);
    Destory(n->_right);
    delete n;
    n = NULL;
}

void RBTree::right_rota(node* root)
{
    if(root != NULL)
    {
        node* l = root->_left;
        root->_left = l->_right;
        if(l->_right != NULL)
            l->_right->_parent = root;
        l->_parent = root->_parent;
        if(root->_parent == NULL)
            _root = l;
        else if(root->_parent->_left == root)
            root->_parent->_left = l;
        else
            root->_parent->_right = l;
        l->_right = root;
        root->_parent = l;
    }
}

void RBTree::left_rota(node* root)
{
   if(root != NULL)
    {
        node* r = root->_right; 
        root->_right = r->_left; /* p -> rl*/
        if(r->_left != NULL)
        {
            r->_left->_parent/*rl*/ = root;
        }
        r->_parent = root->_parent;
        if(root->_parent == NULL)
        {
            _root = r;
        }
        else if(root->_parent->_left == root)
        {
            root->_parent->_left = r;
        }
        else
        {
            root->_parent->_right = r;
        }
        r->_left = root;
        root->_parent = r;
    }
}

node* RBTree::find_pre(node* n)
{
    if(n == NULL)
        return NULL;
    else
    {
        if(n->_left != NULL)
        {
            node* cur = n->_left;
            while(cur->_right)
            {
                cur = cur->_right;
            }
            return cur;
        }
        else
        {
            node* cur = n;
            node* parent = n->_parent;
            while(parent != nullptr && cur == parent->_left)
            {
                cur = parent;
                parent = parent->_parent;
            }
            return parent;
        }
    }
}

node* RBTree::find_suc(node* n)
{
    if(n == NULL)
    {
        return NULL;
    }
    else
    {
        if(n->_right != nullptr)
        {
            node* cur = n->_right;
            while(cur->_left != NULL)
            {
                cur = cur->_left;
            }
            return cur;
        }
        else
        {
            node* cur = n;
            node* parent = n->_parent;
            while(parent != NULL && cur == parent->_right)
            {
                cur = parent;
                parent = parent->_parent;
            }
            return parent;
        }
    }
}

void RBTree::balance(node* n)
{
    while(n != _root && n->col == BLACK)
    {
        if(n == n->_parent->_left)
        {
            node* bro = n->_parent->_right;

            // 找到真正的兄弟节点
            if(bro->col == RED)
            {
                bro->col = BLACK;
                n->_parent->col = RED;
                left_rota(n->_parent);

                bro = n->_parent->_right;
            }

            // 兄弟节点没有子节点
            if(bro->_left == nullptr && bro->_right == nullptr)
            {
                bro->col = RED;
                n = n->_parent;
            }
            else
            {
                // 兄弟节点有一个节点
                if(bro->_right == nullptr)
                {
                    bro->_left->col = BLACK;
                    bro->col = RED;
                    right_rota(bro);

                    bro = n->_parent->_right;
                }
                bro->col = bro->_parent->col;
                n->_parent->col = BLACK;
                bro->_right->col = BLACK;
                left_rota(n->_parent);

                n = _root;
            }
        }
        else
        {
            node* bro = n->_parent->_left;

            // 找到真正的兄弟节点
            if(bro->col == RED)
            {
                bro->col = BLACK;
                n->_parent->col = RED;
                right_rota(n->_parent);

                bro = n->_parent->_left;
            }

            if(bro->_left == nullptr && bro->_right == nullptr)
            {
                bro->col = RED;
                n = n->_parent;
            }
            else
            {
                if(bro->_left == nullptr)
                {
                    bro->_right->col = BLACK;
                    bro->col = RED;
                    left_rota(bro);

                    bro = n->_parent->_left;
                }
                bro->col = bro->_parent->col;
                n->_parent->col = BLACK;
                bro->_left->col = BLACK;
                right_rota(n->_parent);
                n = _root;
            }
        }
    }

    // 自己有子节点是直接让自己的子节点染成黑色
    n->col = BLACK;
}

bool RBTree::Insert(int num)
{
    if(_root == NULL)
    {
        _root = new node(num);
        _root->col = BLACK;
        return true;
    }
    node* parent = nullptr;
    node* cur = _root;
    // 找到每一个要插入节点的父节点
    while(cur)
    {
        if(cur->value > num)
        {
            parent = cur;
            cur = cur->_left;
        }
        else if(cur->value < num)
        {
            parent = cur;
            cur = cur->_right;
        }
        else
            return false;
    }

    cur = new node(num);
    cur->col = RED;
    // 连接父节点和子节点并且子节点保存父节点
    if(parent->value > num)
    {
        parent->_left = cur;
        cur->_parent = parent;
    }
    else
    {
        parent->_right = cur;
        cur->_parent = parent;
    }
    // 开始插入
    while(parent && parent->col == RED)
    {
        // 祖父节点
        node* grand = parent->_parent;
        // 父节点为祖父节点的左结点
        if(grand->_left == parent)
        {
            // 叔叔节点
            node* uncle = grand->_right;
            // 叔叔不为空且叔叔的颜色为红色
            if(uncle != NULL && uncle->col == RED)
            {
                uncle->col = BLACK;
                parent->col = BLACK;
                grand->col = RED;

                // 继续向上调整
                cur = grand;
                parent = cur->_parent;
            }
            else
            {
                /*    a(g)          b
                     b(p)    ->    a c
                    c(c)
                */         
               // 右旋祖父节点染色
                if(cur == parent->_left)
                {
                    right_rota(grand);

                    parent->col = BLACK;
                    grand->col = RED;
                }
                else
                {
                    /*  a (g)       a       c   
                       b (p)  ->   c   ->  b a              
                        c (c)     b
                    */
                    // 左旋父节点右旋祖父节点 然后染色
                    left_rota(parent);
                    right_rota(grand);

                    grand->col = RED;
                    cur->col = BLACK;
                }
                break;
            }
        }
        else
        {
            node* uncle = grand->_left;
            // 叔叔存在且叔叔节点的颜色为红色
            if(uncle != NULL && uncle->col == RED)
            {
                parent->col = BLACK;
                uncle->col = BLACK;
                grand->col = RED;

                // 继续向上调整
                cur = grand;
                parent = cur->_parent;
            }
            else
            {
                /*  a(g)        b
                     b(p)  ->  a c
                      c(c)
                */
               // 左旋祖父节点染色
                if(parent->_right == cur)
                {
                    left_rota(grand);

                    parent->col = BLACK;
                    grand->col = RED;
                }
                else
                {
                    /*  a(g)        a        b
                         b(p)  ->    b  ->  a c
                        c(c)          c
                    */
                   // 右旋父节点左旋祖父节点 然后染色
                    right_rota(parent);
                    left_rota(grand);

                    cur->col = BLACK;
                    grand->col = RED;
                }
                break;
            }
        }
    }
    
    // 根节点一定是黑色
    _root->col = BLACK;
    return true;
}

void RBTree::deletenode(int num)
{
    node* cur = _root;
    while(cur)
    {
        if(cur->value < num)
        {
            cur = cur->_right;
        }
        else if(cur->value > num)
        {
            cur = cur->_left;
        }
        else
        {
            break;
        }
    }
    if(cur == NULL)
    {
        cout << "num is not in tree" << endl;
        return;
    }


    if(cur->_left == nullptr && cur->_right == nullptr) // 叶子节点
    {
        if(cur->col == BLACK)
        {
            balance(cur);
        }
        node* parent = cur->_parent;
        if(parent->_left == cur)
        {
            parent->_left = nullptr;
            delete cur;
            cur = nullptr;
        }
        else
        {
            parent->_right = nullptr;
            delete cur;
            cur = nullptr;
        }
    }
    else if(cur->_left == nullptr || cur->_right == nullptr) // 一个子节点的节点
    {
        node* parent = cur->_parent;
        if(parent == NULL)
        {
            if(cur->_left != nullptr)
            {
                node* tmp = _root;
                _root = cur->_left;
                _root->col = BLACK;
                delete tmp;
                tmp = NULL;
            }
            else 
            {
                node* tmp = _root;
                _root = cur->_right;
                _root->col = BLACK;
                delete tmp;
                tmp = NULL;
            }
        }
        else if(parent->_left == cur)
        {
            if(cur->_left != nullptr)
            {
                node* tmp = cur;
                parent->_left = cur->_left;
                cur->_left->_parent = parent;
                if(cur->col == BLACK)
                    balance(cur->_left);
                delete cur;
                cur = NULL;
            }
            else
            {
                node* tmp = cur;
                parent->_left = cur->_right;
                cur->_right->_parent = parent;
                if(cur->col == BLACK)
                    balance(cur->_right);
                delete cur;
                cur = NULL;
            }
        }
        else 
        {
            if(cur->_left != nullptr)
            {
                node* tmp = cur;
                parent->_right = cur->_left;
                cur->_left->_parent = parent;
                if(cur->col == BLACK)
                    balance(cur->_left);
                delete cur;
                cur = NULL;
            }
            else
            {
                node* tmp = cur;
                parent->_right = cur->_right;
                cur->_right->_parent = parent;
                if(cur->col == BLACK)
                    balance(cur->_right);
                delete cur;
                cur = NULL;
            }
        }
    }
    else // 两个子节点的节点
    {
        node* pre = find_pre(cur);
        int tmp = pre->value;
        deletenode(pre->value);
        cur->value = tmp;
    }
    
}

void RBTree::midpost()
{
    print(_root);
}

void RBTree::print(node* root)
{
    if(root == NULL)
        return;

    print(root->_left);
    cout << root->value << " ";
    print(root->_right);
}

bool RBTree::is_balance()
{
    if(_root == NULL)
    {
        cout << "red black tree is null" << endl;
        return true;
    }
    if(_root->col == RED)
    {
        cout << "red black tree root is red"  << endl;
        return false;
    }

    // 统计一条路径上的黑色节点的数量
    int bf = 0;
    node* cur = _root;
    while(cur)
    {
        if(cur->col == BLACK)
            bf++;
        cur = cur->_left;
    }
    // 检查函数（检查每一条路径上的黑色节点数量）
    return check(_root, 0, bf);
}

bool RBTree::check(node* n, int blacknum, int bf)
{
    if(n == NULL)
    {
        // 一条路径上的黑色节点数量与统计的数量比较相同就说明这条路径平衡
        if(blacknum != bf)
        {
            cout << "blacknum is not equal" << endl;
            return false;
        }
        return true;
    }

    if(n->col == BLACK)
    {
        blacknum++;
    }
    // 不允许出现两个相连的红色节点
    if(n->col == RED && n->_parent->col == RED)
    {
        cout << "parent and child is red" << endl;
        return false;
    }
    // 递归检查每一条路径
    return (check(n->_left, blacknum, bf) && check(n->_right, blacknum, bf));
}

int main(int argc, char* argv[])
{
    int arr[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
    RBTree t;
    int size = sizeof(arr) / sizeof(arr[0]);

    for(auto e : arr)
    {
        t.Insert(e);
    }

    t.deletenode(7);
    t.midpost();
    cout << t.is_balance() << endl;

    return 0;
}
