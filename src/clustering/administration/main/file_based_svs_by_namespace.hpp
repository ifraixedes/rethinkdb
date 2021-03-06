// Copyright 2010-2014 RethinkDB, all rights reserved.
#ifndef CLUSTERING_ADMINISTRATION_MAIN_FILE_BASED_SVS_BY_NAMESPACE_HPP_
#define CLUSTERING_ADMINISTRATION_MAIN_FILE_BASED_SVS_BY_NAMESPACE_HPP_

#include <string>

#include "clustering/administration/reactor_driver.hpp"
#include "clustering/administration/issues/outdated_index.hpp"

class cache_balancer_t;
class rdb_context_t;

class file_based_svs_by_namespace_t : public svs_by_namespace_t {
public:
    file_based_svs_by_namespace_t(io_backender_t *io_backender,
                                  cache_balancer_t *balancer,
                                  const base_path_t& base_path,
                                  outdated_index_issue_client_t *_outdated_index_client)
        : io_backender_(io_backender), balancer_(balancer),
          base_path_(base_path), thread_counter_(0),
          outdated_index_client(_outdated_index_client) { }

    void get_svs(perfmon_collection_t *serializers_perfmon_collection,
                 namespace_id_t namespace_id,
                 stores_lifetimer_t *stores_out,
                 scoped_ptr_t<multistore_ptr_t> *svs_out,
                 rdb_context_t *);

    void destroy_svs(namespace_id_t namespace_id);

    serializer_filepath_t file_name_for(namespace_id_t namespace_id);

private:
    io_backender_t *io_backender_;
    cache_balancer_t *balancer_;
    const base_path_t base_path_;

    threadnum_t next_thread(int num_db_threads);
    int thread_counter_; // should only be used by `next_thread`

    outdated_index_issue_client_t *outdated_index_client;

    DISABLE_COPYING(file_based_svs_by_namespace_t);
};

#endif  // CLUSTERING_ADMINISTRATION_MAIN_FILE_BASED_SVS_BY_NAMESPACE_HPP_
