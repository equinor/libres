from cwrap import BaseCEnum


class MessageLevelEnum(BaseCEnum):
    TYPE_NAME = "message_level_enum"

    LOG_CRITICAL = None
    LOG_ERROR    = None
    LOG_WARNING  = None
    LOG_INFO     = None
    LOG_DEBUG    = None
    LOG_NOTSET   = None

    @staticmethod
    def to_enum(val):
        if val is None:
            return MessageLevelEnum.LOG_WARNING
        if val >= 50:
            return MessageLevelEnum.LOG_CRITICAL
        if val >= 40:
            return MessageLevelEnum.LOG_ERROR
        if val >= 30:
            return MessageLevelEnum.LOG_WARNING
        if val >= 20:
            return MessageLevelEnum.LOG_INFO
        if val >= 10:
            return MessageLevelEnum.LOG_DEBUG

        ## This is the legacy values:
        if val == 4:
            return MessageLevelEnum.LOG_DEBUG
        if val == 3:
            return MessageLevelEnum.LOG_INFO
        if val == 2:
            return MessageLevelEnum.LOG_WARNING
        if val == 1:
            return MessageLevelEnum.LOG_ERROR
        if val == 0:
            return MessageLevelEnum.LOG_CRITICAL
        return MessageLevelEnum.LOG_NOTSET

MessageLevelEnum.addEnum("LOG_CRITICAL", 50)
MessageLevelEnum.addEnum("LOG_ERROR",    40)
MessageLevelEnum.addEnum("LOG_WARNING",  30)
MessageLevelEnum.addEnum("LOG_INFO",     20)
MessageLevelEnum.addEnum("LOG_DEBUG",    10)
MessageLevelEnum.addEnum("LOG_NOTSET",    0)
