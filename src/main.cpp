#include <cligen/cligen.h>
#include <clixon/clixon.h>
#include <clixon/clixon_backend.h>
#include <syslog.h>
#include <unistd.h>

#include <stack>

#include "logger.hpp"

#define NAME "clixon-plug-system"

static clixon_plugin_api api;


int plugin_start([[maybe_unused]]clixon_handle h) {
    clicon_log(LOG_DEBUG, "[%s]: Start plugin", NAME);
    return 0;
}

static int plugin_exit([[maybe_unused]]clixon_handle h) {
    clicon_log(LOG_DEBUG, "[%s]: Close plugin", NAME);
    return 0;
}

int commit_done([[maybe_unused]] clicon_handle h, transaction_data td) {
    clicon_log(LOG_WARNING, "[%s]: commit_done", NAME);
    clicon_log(LOG_WARNING, "[%s]: transaction_clen = %zu", NAME, transaction_clen(td));
    clicon_log(LOG_WARNING, "[%s]: transaction_alen = %zu", NAME, transaction_alen(td));
    clicon_log(LOG_WARNING, "[%s]: transaction_dlen = %zu", NAME, transaction_dlen(td));

    cxobj **added_data = transaction_avec(td);
    size_t change_count = transaction_alen(td);
    //    cxobj **delete_data = transaction_dvec(td);
    //    size_t delete_count = transaction_dlen(td);
    //    cxobj *target_data = transaction_target(td);

    if (change_count != 0) {
        for (size_t i = 0; i < change_count; ++i) {
            std::stack<std::string> pr_node_begin;
            std::stack<std::string> pr_node_end;
            cxobj *added = added_data[i];
            cxobj *parent_node = xml_parent(added);
            pr_node_begin.push(std::string(xml_name(parent_node)));
            while (parent_node) {
                parent_node = xml_parent(parent_node);
                if (parent_node) pr_node_begin.push(std::string(xml_name(parent_node)));
            }
            clicon_log(LOG_WARNING, "[%s]: parent node OK", NAME);
            pr_node_end = pr_node_begin;
            clicon_log(LOG_WARNING, "[%s]: copy OK", NAME);
            //            clicon_log(LOG_WARNING, "[%s]: Parent name of added change %zu: %s", NAME, i, parent_name);
            auto source_buffer = cbuf_new();
            if (source_buffer == nullptr) {
            } else {
                if (clixon_xml2cbuf(source_buffer, added, 0, 0, (char *)"", -1, 0) != -1) {
                    //                    clicon_log(LOG_WARNING, "[%s]: added change %zu:\n%s", NAME, i,
                    //                    cbuf_get(source_buffer));
                }
                std::string data = cbuf_get(source_buffer);
                std::string front_node{};
                std::string back_node{};
                while (!pr_node_begin.empty()) {
                    front_node += ("<" + pr_node_begin.top() + ">");
                    pr_node_begin.pop();
                }
                clicon_log(LOG_WARNING, "[%s]: pr_node_begin OK", NAME);
                while (!pr_node_end.empty()) {
                    back_node += ("</" + pr_node_end.top() + ">");
                    pr_node_end.pop();
                }
                clicon_log(LOG_WARNING, "[%s]: pr_node_end OK", NAME);
                std::string res{};
                res.append(front_node);
                res.append(data);
                res.append(back_node);

                clicon_log(LOG_WARNING, "[%s]: config:\n%s", NAME, res.c_str());
                cbuf_free(source_buffer);
            }
        }
    }

    //    if (delete_count != 0) {
    //        for (size_t i = 0; i < delete_count; ++i) {
    //            cxobj *del = delete_data[i];
    //
    //            /* исходное состояние */
    //            auto source_buffer = cbuf_new();
    //            if (source_buffer == nullptr) {
    //                /* обработать ошибку */
    //            } else {
    //                if (clixon_xml2cbuf(source_buffer, del, 0, 1, (char *)"", -1, 0) != -1) {
    //                    clicon_log(LOG_WARNING, "[%s]: delete change %zu:\n%s", NAME, i, cbuf_get(source_buffer));
    //                }
    //                cbuf_free(source_buffer);
    //            }
    //        }
    //    }
    //
    //    auto source_buffer = cbuf_new();
    //    if (source_buffer == nullptr) {
    //        /* обработать ошибку */
    //    } else {
    //        if (clixon_xml2cbuf(source_buffer, target_data, 0, 1, (char *)"", -1, 0) != -1) {
    //            clicon_log(LOG_WARNING, "[%s]: target data\n%s", NAME, cbuf_get(source_buffer));
    //        }
    //        cbuf_free(source_buffer);
    //    }

    //    cxobj *xtop = transaction_target(td);
    //    auto buffer = cbuf_new();
    //    if (buffer == nullptr) {
    //        clicon_err(OE_PLUGIN, EINVAL, "[%s]: Fail for stage allocate memory", NAME);
    //        xml_free(xtop);
    //        return -1;
    //    }
    //
    //    /* Преобразуем в xml */
    //    if (clixon_xml2cbuf(buffer, xtop, 0, 1, (char *)"", -1, 0)) {
    //        clicon_err(OE_PLUGIN, EINVAL, "[%s]: Fail for stage convert xml in string for parsing", NAME);
    //        cbuf_free(buffer);
    //        return -1;
    //    }
    //    clicon_log(LOG_WARNING, "[%s]: config %s", NAME, cbuf_get(buffer));
    //    cbuf_free(buffer);
    clicon_log(LOG_WARNING, "[%s]: commit_done", NAME);
    return 0;
}

clixon_plugin_api *clixon_plugin_init(clicon_handle h) {
    (void)h;
    clicon_log(LOG_NOTICE, "[%s]: Load plugin ver: %d.%d.%d rev. %d", NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH,
               VERSION_TWEAK);

    strncpy(api.ca_name, NAME, strlen(NAME));
    api.ca_name[strlen(NAME)] = '\0';
    api.ca_start = plugin_start;
    api.ca_exit = plugin_exit;
    api.u.cau_backend.cb_trans_validate = commit_done;
    //    api.u.cau_backend.cb_trans_commit = commit_done;
    //    api.u.cau_backend.cb_trans_commit_done = commit_done;
    //    api.u.cau_backend.cb_trans_end = commit_done;

    return &api;
}
