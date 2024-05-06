#ifndef DIRECTED_GRAPH_H
#define DIRECTED_GRAPH_H

#include <map>
#include <set>
#include <vector>
#include <tuple>
#include <stdexcept>

template<typename N, typename ND, typename E, typename ED, bool error_check = true>
class directed_graph {
private:
    std::map<N, std::set<E>> node_outbound {};
    std::map<N, std::set<E>> node_inbound {};
    std::map<N, ND> node_data {};
    std::map<E, std::tuple<N, N, ED>> edges {};
    
    template<typename K, typename V>
    void del_map_key(std::map<K, V> &map, const K& key) {
        auto iter = map.find(key);
        if (iter != map.end()) {
            map.erase(iter);
        }
    }

    template<typename V>
    void del_set(std::set<V> &set, const V& val) {
        auto iter = set.find(val);
        if (iter != set.end()) {
            set.erase(iter);
        }
    }

public:
    void insert_node(const N& node, ND&& data) {
        if constexpr (error_check) {
            if (has_node(node)) {
                throw std::runtime_error {"Node already exists"};
            }
        }
        this->node_outbound[node] = {};
        this->node_inbound[node] = {};
        this->node_data[node] = data;
    }
    
    void delete_node(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        std::set<E> outbound_copy = this->node_outbound[node]; // done to prevent concurrent modification problems
        for (const auto& edge_id : outbound_copy) {
            auto& [from_node, to_node, _] = this->edges[edge_id];
            del_set(this->node_outbound[from_node], edge_id);
            del_set(this->node_inbound[to_node], edge_id);
            del_map_key(this->edges, edge_id);
        }
        // _node_inbound mirrors _node_outbound, so you don"t have to do the above again for _node_inbound
        del_map_key(this->node_inbound, node);
        del_map_key(this->node_outbound, node);
        del_map_key(this->node_data, node);
    }
    
    void update_node_data(const N& node, ND&& data) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        this->node_data[node] = data;
    }
    
    ND& get_node_data(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        return this->node_data[node];
    }
    
    void insert_node_between_edge(const N& new_node, ND&& new_node_data, const E& existing_edge, const E& from_edge, ED&& from_edge_data, const E& to_edge, ED&& to_edge_data) {
        if constexpr (error_check) {
            if (has_node(new_node)) {
                throw std::runtime_error {"Node already exists"};
            }
            if (!has_edge(existing_edge)) {
                throw std::runtime_error {"Existing edge doesn't exist"};
            }
            if (has_edge(from_edge)) {
                throw std::runtime_error {"From edge already exists"};
            }
            if (has_edge(to_edge)) {
                throw std::runtime_error {"To edge already exists"};
            }
        }
        auto& [node_from, node_to, _] = this->get_edge(existing_edge);
        this->insert_node(new_node, new_node_data);
        this->delete_edge(existing_edge);
        this->insert_edge(from_edge, node_from, new_node, from_edge_data);
        this->insert_edge(to_edge, new_node, node_to, to_edge_data);
    }

    void insert_edge(const E& edge, const N& from_node, const N& to_node, ED&& data) {
        if constexpr (error_check) {
            if (!has_node(from_node)) {
                throw std::runtime_error {"From node doesn't exist"};
            }
            if (!has_node(to_node)) {
                throw std::runtime_error {"To node doesn't exist"};
            }
            if (has_edge(edge)) {
                throw std::runtime_error {"Edge already exists"};
            }
        }
        this->edges[edge] = std::tuple<N, N, ED> {from_node, to_node, data};
        this->node_inbound[to_node].insert(edge);
        this->node_outbound[from_node].insert(edge);
    }

    void delete_edge(const E& edge, bool remove_from_if_isolated = false, bool remove_to_if_isolated = false) {
        if constexpr (error_check) {
            if (!has_edge(edge)) {
                throw std::runtime_error {"Edge doesn't exist"};
            }
        }
        auto& [from_node, to_node, _] = this->edges[edge];
        if constexpr (error_check) {
            if (!has_node(from_node)) {
                throw std::runtime_error {"This should never happen"};
            }
            if (!has_node(to_node)) {
                throw std::runtime_error {"This should never happen"};
            }
        }
        del_set(this->node_outbound[from_node], edge);
        del_set(this->node_inbound[to_node], edge);
        // from and to may be the same -- if they are, and you"ve removed the from, make sure you don"t try to remove to
        // because form and to are the same... you can"t remove the same node twice
        bool dealing_with_same_node = (from_node == to_node);
        bool removed_from = false;
        //bool removed_to = false;
        if (remove_from_if_isolated
                && this->node_inbound[from_node].size() == 0
                && this->node_outbound[from_node].size() == 0) {
            this->delete_node(from_node);
            removed_from = true;
        }
        if (remove_to_if_isolated 
                && (not dealing_with_same_node or (dealing_with_same_node and not removed_from))
                && this->node_inbound[to_node].size() == 0
                && this->node_outbound[to_node].size() == 0) {
            this->delete_node(to_node);
            //removed_to = true;
        }
        del_map_key(this->edges, edge);
    }

    void update_edge_data(const E& edge, ED&& data) {
        if constexpr (error_check) {
            if (!has_edge(edge)) {
                throw std::runtime_error {"Edge doesn't exist"};
            }
        }
        auto& [from_node, to_node, _] = this->edges[edge];
        this->delete_edge(edge);
        this->insert_edge(edge, from_node, to_node, data);
    }

    ED& get_edge_data(const E& edge) {
        if constexpr (error_check) {
            if (!has_edge(edge)) {
                throw std::runtime_error {"Edge doesn't exist"};
            }
        }
        return std::get<2>(this->edges[edge]);
    }

    const N& get_edge_from(const E& edge) {
        if constexpr (error_check) {
            if (!has_edge(edge)) {
                throw std::runtime_error {"Edge doesn't exist"};
            }
        }
        return std::get<0>(this->edges[edge]);
    }

    const N& get_edge_to(const E& edge) {
        if constexpr (error_check) {
            if (!has_edge(edge)) {
                throw std::runtime_error {"Edge doesn't exist"};
            }
        }
        return std::get<1>(this->edges[edge]);
    }

    std::tuple<const N&, const N&, ED&> get_edge(const E& edge) {
        if constexpr (error_check) {
            if (!has_edge(edge)) {
                throw std::runtime_error {"Edge doesn't exist"};
            }
        }
        auto& ref = edges[edge];
        return std::tuple<const N&, const N&, ED&> {std::get<0>(ref), std::get<1>(ref), std::get<2>(ref)};
    }

    std::vector<std::reference_wrapper<const N>> get_root_nodes() {
        std::vector<std::reference_wrapper<const N>> ret {};
        for (const auto& n : this->get_nodes()) {
            if (this->get_in_degree(n) == 0) {
                ret.push_back(n);
            }
        }
        return ret;
    }

    const N& get_root_node() {
        std::vector roots = std::vector(this->get_root_nodes());
        if (roots.size() != 1) {
            throw std::runtime_error { "Exactly 1 root node required" };
        }
        return roots[0];
    }

    std::vector<std::reference_wrapper<const N>> get_leaf_nodes() {
        std::vector<std::reference_wrapper<const N>> ret {};
        for (const auto& n : this->get_nodes()) {
            if (this->get_out_degree(n) == 0) {
                ret.push_back(n);
            }
        }
        return ret;
    }

    std::vector<std::reference_wrapper<const N>> get_nodes() {
        std::vector<std::reference_wrapper<const N>> keys;
        for (const auto& pair : this->node_outbound) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    std::vector<std::reference_wrapper<const E>> get_edges() {
        std::vector<std::reference_wrapper<const E>> keys;
        for (const auto& pair : this->node_outbound) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    bool has_node(const N& node) {
        return this->node_outbound.contains(node);
    }

    bool has_edge(const E& edge) {
        return this->edges.contains(edge);
    }

    std::vector<std::tuple<const E&, const N&, const N&, ED&>> get_outputs_full(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        std::vector<std::tuple<const E&, const N&, const N&, ED&>> ret {};
        for (const auto& e : this->node_outbound[node]) {
            const auto& [from_node, to_node, edge_data] = this->get_edge(e);
            ret.push_back(std::tuple<> {e, from_node, to_node, edge_data});
        }
        return ret;
    }

    std::tuple<const E&, const N&, const N&, ED&> get_output_full(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        auto it = this->get_outputs_full(node).begin();
        const auto& ret = &it;
        it++;
        if (it != this->get_outputs_full(node).end()) {
            throw std::runtime_error {"More than one edge exists"};
        }
        return ret;
    }

    std::vector<std::tuple<const E&, const N&, const N&, ED&>> get_inputs_full(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        std::vector<std::tuple<const E&, const N&, const N&, ED&>> ret {};
        for (const auto& e : this->node_inbound[node]) {
            const auto& [from_node, to_node, edge_data] = this->get_edge(e);
            ret.push_back(std::tuple<> {e, from_node, to_node, edge_data});
        }
        return ret;
    }

    std::tuple<const E&, const N&, const N&, ED&> get_input_full(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        auto it = this->get_inputs_full(node).begin();
        const auto& ret = &it;
        it++;
        if (it != this->get_outputs_full(node).end()) {
            throw std::runtime_error {"More than one edge exists"};
        }
        return ret;
    }

    const std::set<E>& get_outputs(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        return this->node_outbound[node];
    }

    const E& get_output(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        auto it = this->get_outputs(node).begin();
        const auto& ret = *it;
        ++it;
        if (it != this->get_outputs_full(node).end()) {
            throw std::runtime_error {"More than one edge exists"};
        }
        return ret;
    }

    const std::set<E>& get_inputs(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        return this->node_inbound[node];
    }

    const E& get_input(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        auto it = this->get_inputs(node).begin();
        const auto& ret = *it;
        ++it;
        if (it != this->get_outputs_full(node).end()) {
            throw std::runtime_error {"More than one edge exists"};
        }
        return ret;
    }

    bool has_outputs(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        return this->get_outputs(node).begin() != this->get_outputs(node).end();
    }

    bool has_inputs(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        return this->get_inputs(node).begin() != this->get_inputs(node).end();
    }

    int get_out_degree(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        return this->node_outbound[node].size();
    }

    int get_in_degree(const N& node) {
        if constexpr (error_check) {
            if (!has_node(node)) {
                throw std::runtime_error {"Node doesn't exist"};
            }
        }
        return this->node_inbound[node].size();
    }

    std::string to_string() {
        std::string out {};
        for (const N& node : this->get_nodes()) {
            out += std::format("node {}: {}\n", node, this->get_node_data(node));
            for (const E& edge : this->get_outputs(node)) {
                auto [from_node, to_node, edge_data] = this->get_edge(edge);
                out += std::format("  edge {} pointing to node {}: {}\n", edge, to_node, edge_data);
            }
        }
        return out;
    }
};

// template<>
// struct std::formatter<directed_graph<std::string, std::string, std::string, std::string>> : std::formatter<std::string> {
//     auto format(directed_graph<std::string, std::string, std::string, std::string> g, format_context& ctx) {
//         std::string ret {"hi"};
//         // for (auto node : g.get_nodes()) {
//             // out += std::format("{}\n", node);
//             // out += std::format("{}: {}\n", node, g.get_node_data(node));
//             // for (auto edge : g.get_outputs(node)) {
//             //     auto [from_node, to_node, edge_data] = g.get_edge(edge);
//             //     out += std::format("    {} pointing to {}: {}\n", edge, to_node, edge_data);
//             // }
//         // }
//         return format_to(ctx.out(), "{}", ret);
//     }
// };

#endif //DIRECTED_GRAPH_H
