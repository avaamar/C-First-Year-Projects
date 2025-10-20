#include <iostream>
#include <vector>
#include <string>

struct TreeNode;
struct EdgeNode;
 
typedef std::string tree_t;
 
struct EdgeNode{
    tree_t val;
    TreeNode* subtree;
    EdgeNode* next;
};
 
struct TreeNode{
    tree_t val;
    EdgeNode* subtree_l;
};

class A3Tree {
    public: 
    //constructor: 2D vector of strings as input (headers, training data)
    A3Tree (const std::vector<std::vector<std::string>>&input) {
        header = input[0];
        t = allocate_tree_node(header[0]);//row starts with first attribute
        build_tree(input);
    }
    //Query: takes a list of attribute values and returns the corresponding label
    std::string query (const std::vector<std::string>& attributes) const {
        return query_recursive(t, header, attributes, 0);
    }
    //Query_recursive: helper that walks the tree based on matching edge labels
    std::string query_recursive(TreeNode* node, const std::vector<std::string>& headers,const std::vector<std::string>& attributes, int depth) const{
        if (!node){
            return "";
        }
        if (!node -> subtree_l){
            return node -> val;
        }
        std::string current_attr = headers[depth];
        //check all edges from this node to find a matching attribute value
        EdgeNode* edge = node -> subtree_l;
        while (edge){
            if (edge -> val == attributes[depth]){
                return query_recursive(edge -> subtree, headers, attributes, depth + 1);
            }
            edge = edge -> next;
        }
        return "";
    }
    //Return the total number of nodes in the tree
    int node_count() const {
        return count_nodes(t);
    }
    //Return the number of leaf nodes (with no children)
    int leaf_node_count() const {
        return count_leaves(t);
    }
    //Destructor: clean up dynamically allocated memory
    ~A3Tree() {
        deallocate_tree(t);
        leaf_pool.clear();
    }

private:
    TreeNode* t; //root of the tree
    std::vector<std::string> header; //to store attribute names
    void build_tree(const std::vector<std::vector<std::string>>& input){
        for (size_t i = 1; i < input.size(); i++) {
            insert_row(t, input[i], 0, input[0].size()-1);
        }
    }
    void insert_row(TreeNode* node, const std::vector<std::string>& row, int attr_idx, int label_idx) {
        if (attr_idx == label_idx) {
            //only assign if this node has no children
            if (!node -> subtree_l) {
                node -> val = row[label_idx];
            }
            return;
        }
        const std::string& current_val = row[attr_idx];
        EdgeNode* it = node -> subtree_l;
        EdgeNode* prev = NULL;
        while (it && it -> val != current_val) {
            prev = it;
            it = it -> next;
        }
        if (!it) {
            TreeNode* new_child;
            if (attr_idx + 1 == label_idx) {
                new_child = get_or_create_leaf(row[label_idx]);
            } else {
                new_child = allocate_tree_node(header[attr_idx + 1]);
            }
            it = cons_edge_node(current_val, new_child, NULL);
            if (!prev) {
                node -> subtree_l = it;
            } else {
                prev -> next = it;
            }
        }
        insert_row(it -> subtree, row, attr_idx + 1, label_idx);
    }
    TreeNode* allocate_tree_node(const std::string& attr_name) {
        TreeNode* node = new TreeNode;
        node -> val = attr_name;
        node -> subtree_l = NULL;
        return node;
    }
    EdgeNode* cons_edge_node(tree_t val, TreeNode* subtree, EdgeNode* next) {
        EdgeNode* edge = new EdgeNode;
        edge -> val = val;
        edge -> subtree = subtree;
        edge -> next = next;
        return edge;
    }
    int count_nodes(TreeNode* node) const {
        if (!node) {
            return 0;
        }
        int total = 1;
        EdgeNode* it = node -> subtree_l;
        while (it) {
            total += count_nodes(it -> subtree);
            it = it -> next;
        }
        return total;
    }
    int count_leaves(TreeNode* node) const {
        if (!node) {
            return 0;
        }
        if (!node -> subtree_l) {
            return 1;
        }
        int total = 0;
        EdgeNode* it = node -> subtree_l;
        while (it) {
            total += count_leaves(it -> subtree);
            it = it -> next;
        }
        return total;
    }
    //To cache reusable leaf nodes
    std::vector<TreeNode*> leaf_pool;
    TreeNode* get_or_create_leaf(const std::string& label) {
        for (size_t i=0; i < leaf_pool.size(); i++){
            TreeNode* leaf = leaf_pool[i];
            if (leaf -> val == label) {
                return leaf;
            }
        }
        TreeNode* new_leaf = allocate_tree_node(label);
        leaf_pool.push_back(new_leaf);
        return new_leaf;
    }
    void deallocate_tree(TreeNode* node) {
        if (!node) {
            return;
        }
        EdgeNode* it = node -> subtree_l;
        while (it) {
            EdgeNode* next_edge = it -> next;
            deallocate_tree(it -> subtree);
            delete it;
            it = next_edge;
        }
        delete node;
    }
};

int main() {

    // Functional test - weather data
    std::vector<std::vector<std::string>> input1 {
        {"temperature", "rain", "wind", "quality"}, 
        {"high", "yes", "light", "acceptable"},
        {"low", "yes", "light", "acceptable"},
        {"low", "no", "moderate", "good"},
        {"high", "yes", "strong", "poor"},
        {"high", "yes", "moderate", "acceptable"},
        {"high", "no", "moderate", "good"},
        {"low", "yes", "strong", "poor"},
        {"high", "no", "light", "good"},
        {"low", "yes", "moderate", "poor"},
        {"high", "no", "strong", "poor"}
    };

    A3Tree t1(input1);
    std::cout << "Functional Tests:" << std::endl;
    std::vector<std::string> query1 = {"high", "yes", "moderate"};
    std::cout << "Expected: acceptable | Got: " << t1.query(query1) << std::endl;

    std::vector<std::string> query2 = {"low", "no", "moderate"};
    std::cout << "Expected: good | Got: " << t1.query(query2) << std::endl;

    std::vector<std::string> query3 = {"high", "no", "strong"};
    std::cout << "Expected: poor | Got: " << t1.query(query3) << std::endl;

    std::cout << "\n Tree Composition:" << std::endl;
    std::cout << "Node count: " << t1.node_count() << std::endl;
    std::cout << "Leaf count: " << t1.leaf_node_count() << std::endl;

    /*
     * Expected:
     * Node count = 10 (may vary slightly depending on insertion order) but got 17
     * Leaf count = 6 (unique outcomes: acceptable, good, poor) but got 10
     * Note: this implementation always inserts attributes in column order
     * while this does not minimise the total node count, it ensures correctness
     * leaf node reuse is implemented to reduce unecessary duplication of labels
     */
    
    //Edge case: single row dataset
    std::cout <<"\n Edge case: single Row" << std::endl;
    std::vector<std::vector<std::string>> input2 {
        {"temp", "wind", "mood"},
        {"hot", "light", "happy"}
    };

    A3Tree t2(input2);
    std::vector<std::string> q2 = {"hot", "light"};
    std::cout << "Expected: happy | Got: " << t2.query(q2) << std::endl;
    std::cout << "Node count: " << t2.node_count() << " (should be 3)" << std::endl;
    std::cout << "Leaf count: " << t2.leaf_node_count() << " (should be 1)" << std::endl;

    //Generalisation to non-domain data
    std::cout <<"\n Generic Attribute Test" << std::endl;
    std::vector<std::vector<std::string>> input3 {
        {"featureA", "featureB", "output"},
        {"red", "circle", "match"},
        {"blue", "circle", "mismatch"},
        {"red", "square", "mismatch"}
    };

    A3Tree t3(input3);
    std::vector<std::string> q3 = {"red", "circle"};
    std::cout << "Expected: match | Got: " << t3.query(q3) << std::endl;

    std::vector<std::string> q4 = {"blue", "circle"};
    std::cout << "Expected: mismatch | Got: " <<t3.query(q4) << std::endl;

    std::vector<std::string> q5 = {"red", "square"};
    std::cout << "Expected: mismatch | Got: " <<t3.query(q5) << std::endl;

    std::cout << "Node count: " << t3.node_count() << std::endl;
    std::cout << "Leaf count: " << t3.leaf_node_count() << std::endl;

    /*
     * Notes:
     * Test 3 show that the tree works with any column names or values
     * Expected Leaf count: 2 ("match", "mismatch")
     * Tests for reuse of the "circle" attribute path
    */
}
