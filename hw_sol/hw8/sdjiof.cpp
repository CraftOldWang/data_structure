



    // 删除节点
    avlnode* remove_helper(avlnode* node, int value, bool& shorter) {
        if (!node) {
            shorter = false; // 节点不存在
            return nullptr;
        }

        if (value < node->value) {
            node->left = remove_helper(node->left, value, shorter);
            if (shorter) {
                node->bf -= 1; // 左子树变短
            }
        } else if (value > node->value) {
            node->right = remove_helper(node->right, value, shorter);
            if (shorter) {
                node->bf += 1; // 右子树变短
            }
        } else { // 找到要删除的节点
            if (!node->left && !node->right) { // 叶子节点
                delete node;
                shorter = true;
                return nullptr;
            } else if (node->left && node->right) { // 有两个子树
                // 用中序后继替代
                avlnode* successor = findMin(node->right);
                node->value = successor->value;
                node->right = remove_helper(node->right, successor->value, shorter);
                if (shorter) {
                    node->bf += 1;
                }
            } else { // 只有一个子树
                avlnode* child = node->left ? node->left : node->right;
                delete node;
                shorter = true;
                return child;
            }
        }

        // 检查平衡因子并调整
        if (node->bf == 2) {
            if (node->left->bf >= 0) { // LL
                return rotateRight(node);
            } else { // LR
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        } else if (node->bf == -2) {
            if (node->right->bf <= 0) { // RR
                return rotateLeft(node);
            } else { // RL
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        if (node->bf == 0) {
            shorter = true;
        }

        return node;
    }

    // 查找最小值节点（用于中序后继）
    avlnode* findMin(avlnode* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }



    avlnode* rotateLeft(avlnode* node) {
        avlnode* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;

        // 更新平衡因子
        if (newRoot->bf == -1) {
            node->bf = 0;
            newRoot->bf = 0;
        } else {
            node->bf = -1;
            newRoot->bf = 1;
        }

        return newRoot;
    }

    // 右旋
    avlnode* rotateRight(avlnode* node) {
        avlnode* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;

        // 更新平衡因子
        if (newRoot->bf == 1) {
            node->bf = 0;
            newRoot->bf = 0;
        } else {
            node->bf = 1;
            newRoot->bf = -1;
        }

        return newRoot;
    }