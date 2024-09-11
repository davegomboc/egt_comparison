#ifndef EGT_COMPARISON_EGT_FORMAT_TYPE_HPP
#define EGT_COMPARISON_EGT_FORMAT_TYPE_HPP


#include <filesystem>
#include <string>
#include <string_view>


class egt_format_type {
public:
    egt_format_type & operator=(egt_format_type const &) = default;
    egt_format_type & operator=(egt_format_type &&) = default;
    egt_format_type(egt_format_type const &) = default;
    egt_format_type(egt_format_type &&) = default;
    virtual ~egt_format_type() = default;
    egt_format_type() = default;

    static egt_format_type * create_ptr(std::string_view const egt_format_sv);

    virtual std::filesystem::path extension() const = 0;

    virtual std::string as_string() const {
        return extension().string();
    }

    virtual bool use_requires_loading_into_memory() const {
        return true;
    }

    virtual void vtable_pinning_method() const;
};


class syzygy_wcdbl_egt_format_type : public egt_format_type {
    std::filesystem::path extension() const override {
        return "rtbw";
    }

    virtual bool use_requires_loading_into_memory() const override {
        return false;
    }

    virtual void vtable_pinning_method() const override;
};


class syzygy_dtzm_egt_format_type : public egt_format_type {
    std::filesystem::path extension() const override {
        return "rtbz";
    }

    virtual bool use_requires_loading_into_memory() const override {
        return false;
    }

    virtual void vtable_pinning_method() const override;
};


class chesspresso_binary_egt_format_type : public egt_format_type {
public:
    std::filesystem::path extension() const override {
        return "plb";
    }

    virtual void vtable_pinning_method() const override;
};


class cube_list_egt_format_type : public egt_format_type {
public:
    std::filesystem::path extension() const override {
        return "clst";
    }

    virtual void vtable_pinning_method() const override;
};


class indexed_cube_list_egt_format_type : public egt_format_type {
public:
    std::filesystem::path extension() const override {
        return "icl";
    }

    virtual void vtable_pinning_method() const override;
};


class two_level_logic_minimization_egt_format_type : public egt_format_type {
public:
    std::filesystem::path extension() const override {
        return "tllm";
    }

    virtual void vtable_pinning_method() const override;
};


class decision_dag_egt_format_type : public egt_format_type {
public:
    std::filesystem::path extension() const override {
        return "dtr";
    }

    virtual void vtable_pinning_method() const override;
};


class probe_report_nybble_egt_format_type : public egt_format_type {
    std::filesystem::path extension() const override {
        return "wpr";
    }

    virtual bool use_requires_loading_into_memory() const override {
        return false;
    }

    virtual void vtable_pinning_method() const override;
};


class multiterminal_binary_decision_diagram_egt_format_type
  : public egt_format_type {
public:
    std::filesystem::path extension() const override {
        return "mtbdd";
    }

    virtual void vtable_pinning_method() const override;
};


#endif // EGT_COMPARISON_EGT_FORMAT_TYPE_HPP
