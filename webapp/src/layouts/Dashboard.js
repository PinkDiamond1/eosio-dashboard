import React, { useState, useEffect } from 'react'
import PropTypes from 'prop-types'
import clsx from 'clsx'
import { createGlobalStyle } from 'styled-components'
import { makeStyles } from '@material-ui/styles'
import Hidden from '@material-ui/core/Hidden'
import CssBaseline from '@material-ui/core/CssBaseline'
import Box from '@material-ui/core/Box'
import Typography from '@material-ui/core/Typography'
import { isWidthUp } from '@material-ui/core/withWidth'
import { useTranslation } from 'react-i18next'
import { useLocation } from 'react-router-dom'
import Grid from '@material-ui/core/Grid'
import useMediaQuery from '@material-ui/core/useMediaQuery'

import Sidebar from '../components/Sidebar'
import NetworkSelector from '../components/NetworkSelector'
import Header from '../components/Header'
import Footer from '../components/Footer'
import PageTitle from '../components/PageTitle'
import SnackbarMessage from '../components/SnackbarMessage'
import { eosConfig, generalConfig } from '../config'
import { useSharedState } from '../context/state.context'
import routes from '../routes'

import styles from './styles'

const drawerWidth = 260
const INIT_VALUES = {
  dynamicTitle: '',
  networkTitle: '',
  pathname: null
}
const useStyles = makeStyles((theme) => styles(theme, drawerWidth))

const GlobalStyle = createGlobalStyle`
  html,
  body,
  #root {
    height: 100%;
  }

  body {
    background: ${(props) => props.theme.body.background};
  }

  .MuiCardHeader-action .MuiIconButton-root {
    padding: 4px;
    width: 28px;
    height: 28px;
  }
`

const Dashboard = ({ children, width, ual }) => {
  const [mobileOpen, setMobileOpen] = useState(false)
  const classes = useStyles()
  const { t } = useTranslation('routes')
  const location = useLocation()
  const [lacchain] = useSharedState()
  const [routeName, setRouteName] = useState(INIT_VALUES)
  const isTablet = useMediaQuery('(min-width:767px)')
  const [xsSize, setXsSize] = useState(12)

  const handleDrawerToggle = () => {
    setMobileOpen(!mobileOpen)
  }

  useEffect(() => {
    isTablet ? setXsSize(6) : setXsSize(12)
  }, [isTablet])

  useEffect(() => {
    if (routes.some((route) => route.path === location.pathname)) {
      const managementCardTitle = lacchain.dynamicTitle || ''

      setRouteName({
        dynamicTitle:
          location.pathname === '/management'
            ? managementCardTitle
            : t(`${location.pathname}>heading`),
        networkTitle: location.pathname === '/' ? eosConfig.networkLabel : '',
        pathname: location.pathname,
        pageTitle: `${location.pathname}>title`
      })
    } else {
      setRouteName(INIT_VALUES)
    }
    // eslint-disable-next-line
  }, [location.pathname, lacchain.dynamicTitle, t])

  return (
    <Box className={classes.root}>
      <CssBaseline />
      <GlobalStyle />
      <PageTitle title={t(routeName.pageTitle)} />
      <Box className={classes.drawer}>
        <Hidden mdUp implementation="js">
          <Sidebar
            PaperProps={{ style: { width: drawerWidth } }}
            variant="temporary"
            open={mobileOpen}
            onClose={handleDrawerToggle}
          />
        </Hidden>
        <Hidden smDown implementation="css">
          <Sidebar PaperProps={{ style: { width: drawerWidth } }} />
        </Hidden>
      </Box>
      <Box className={classes.appContent}>
        <Header onDrawerToggle={handleDrawerToggle} ual={ual} />
        <Box className={classes.mainContent} p={isWidthUp('lg', width) ? 6 : 4}>
          <Box className={classes.subHeader}>
            <Box className={classes.boxHeader}>
              <Grid container>
                <Grid item md={12} xs={xsSize}>
                  <Typography
                    variant="h3"
                    className={clsx(
                      classes.textAlignReadMore,
                      classes.marginBottom
                    )}
                  >
                    {routeName.pathname
                      ? `${routeName.dynamicTitle} ${routeName.networkTitle}`
                      : ''}
                  </Typography>
                </Grid>
                <Grid item md={12} xs={12}>
                  <Typography className={classes.textAlignReadMore}>
                    {t(`${location.pathname}>moreDescription`)}
                  </Typography>
                </Grid>
              </Grid>
            </Box>
            <NetworkSelector
              title={eosConfig.networkLabel}
              networkLogo={eosConfig.networkLogo}
              options={generalConfig.networkLinks}
            />
          </Box>
          {children}
        </Box>
        <Footer />
      </Box>
      <SnackbarMessage />
    </Box>
  )
}

Dashboard.propTypes = {
  children: PropTypes.node,
  width: PropTypes.any,
  ual: PropTypes.any
}

export default Dashboard
