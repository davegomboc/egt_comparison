#ifndef EGT_COMPARISON_BDD_FROM_BITS_BDD_NODE_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_BDD_NODE_TYPE_HPP


struct bdd_node_type {
    unsigned child[2];

    // next free, or next in hash table
    unsigned next;

    // 0 for terminals and freed nodes
    unsigned short level;

    // for mark and sweep
    unsigned short sequence;

    inline void copy(bdd_node_type const & N) {
        child[0] = N.child[0];
        child[1] = N.child[1];
        level = N.level;
    }

    inline bool is_duplicate(bdd_node_type const & N) {
        if (level != N.level) return false;
        if (child[0] != N.child[0]) return false;
        return child[1] == N.child[1];
    }

    inline void recycle(unsigned int free_list) {
        level = 0;
        next = free_list;
    }

    static inline void hash(unsigned int & H, unsigned short x) {
        H = 37 * H ^ (x >> 8);
        H = 37 * H ^ (x & 0x00ff);
    }

    static inline void hash(unsigned int & H, unsigned int x) {
        H = 37 * H ^ (x >> 24);
        H = 37 * H ^ ((x & 0x00ff0000) >> 16);
        H = 37 * H ^ ((x & 0x0000ff00) >> 8);
        H = 37 * H ^  (x & 0x000000ff);
    }

    inline unsigned int hash() const {
        unsigned int h = 0;
        hash(h, level);
        hash(h, child[0]);
        hash(h, child[1]);
        return h;
    }
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_BDD_NODE_TYPE_HPP
