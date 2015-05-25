inherit "/std/domain_link";

create_domain()
{
    add_preload("/std/living");
    add_preload("/cmd/wiz_cmd_apprentice");
    add_preload("/cmd/wiz_cmd_pilgrim");
    add_preload("/cmd/wiz_cmd_normal");
    add_preload("/cmd/wiz_cmd_mage");
    add_preload("/cmd/wiz_cmd_lord");
    add_preload("/cmd/wiz_cmd_arch");
    add_preload("/cmd/wiz_cmd_keeper");
    add_preload("/cmd/std/misc_cmd");
    add_preload("/cmd/std/soul_cmd");
    add_preload("/cmd/std/tracer_tool");
    add_preload("/d/Standard/login/bodies");
}
